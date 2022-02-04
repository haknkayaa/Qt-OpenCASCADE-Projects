//
// Created by sufuk on 2.02.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DataStructs.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (MainWindow::consoleWidget == nullptr) {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
            case QtDebugMsg:
                fprintf(stderr, "Debug: %s \t (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
//                fprintf(stderr, "Debug: %s \n", localMsg.constData());
                break;
            case QtInfoMsg:
                fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtWarningMsg:
                fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtCriticalMsg:
                fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtFatalMsg:
                fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                abort();
        }
    } else {
        switch (type) {
            case QtDebugMsg:
            case QtWarningMsg:
            case QtCriticalMsg:
                if (MainWindow::consoleWidget != nullptr) {
                    QScrollBar *sb = MainWindow::consoleWidget->verticalScrollBar();
                    sb->setValue(sb->maximum());

                    QFont myFont("Monospace", 10);
                    myFont.setPixelSize(12);
                    MainWindow::consoleWidget->setStyleSheet("color: rgb(174,50,160)");
                    MainWindow::consoleWidget->setFont(myFont);
                    //"Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    //        context.function;

                    QDateTime current = QDateTime::currentDateTime();
                    QString output = "[" + current.time().toString() + "]" + " - Debug : " + msg;
                    MainWindow::consoleWidget->append(output);
                }

                break;
            case QtFatalMsg:
                abort();
            case QtInfoMsg:
                break;
        }
    }
}


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    qInstallMessageHandler(myMessageOutput);

    myViewerWidget = ui->myViewerWidget;
    projectManagerMainTreeWidget = ui->modelTreeWidget;
    myStepProcessor = new STEPProcessor(nullptr);
    consoleWidget = ui->consoleWidget;

    connect(ui->actionImport, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open", QDir::homePath());
        myStepProcessor->loadSTEPFile(fileName);
    });

    ui->myViewerWidget->showTrihedronCube(true);
    ui->myViewerWidget->show3DGrid(true);
    ui->myViewerWidget->showPerformanceStats(true);


    connect(ui->myViewerWidget, &Viewer::mouseSelectedShape, [this]() {
        if (!myViewerWidget->getContext()->DetectedOwner().IsNull() && mainItem_geometry->childCount() > 0) {
            auto viewCubeOwner = opencascade::handle<AIS_ViewCubeOwner>::DownCast(
                    myViewerWidget->getContext()->DetectedOwner());

            auto manipulator = opencascade::handle<AIS_Manipulator>::DownCast(
                    myViewerWidget->getContext()->DetectedInteractive());

            if (!viewCubeOwner && !manipulator) {
                currentSelectedShape = dynamic_cast<NodeInteractive *>(myViewerWidget->getContext()->DetectedInteractive().operator->())->getTreeWidgetItem();
                projectManagerMainTreeWidget->setCurrentItem(currentSelectedShape);

                myViewerWidget->getAManipulator()->Detach();
                myViewerWidget->getContext()->Erase(myViewerWidget->getAManipulator(), true);
                myViewerWidget->getAManipulator()->Attach(
                        dynamic_cast<NodeInteractive *>(myViewerWidget->getContext()->DetectedInteractive().operator->()));
                myViewerWidget->getContext()->UpdateCurrentViewer();
                myViewerWidget->getAManipulator()->EnableMode(AIS_MM_Translation);

            }

        }
    });
    connect(ui->myViewerWidget, &Viewer::mouseSelectedVoid, [] {
        projectManagerMainTreeWidget->clearSelection();
        myViewerWidget->getAManipulator()->Detach();
        myViewerWidget->getContext()->CurrentViewer()->Redraw();

    });

    // 3D Viewer Context Menu Actions
    connect(ui->myViewerWidget, &Viewer::showAllParts, [this]() {

        ui->myViewerWidget->getContext()->DisplayAll(false);
        ui->myViewerWidget->getContext()->UpdateCurrentViewer();

        QTreeWidgetItemIterator it(projectManagerMainTreeWidget);
        while (*it) {
            if (((*it)->text(1) == "Geometry") && (*it != mainItem_geometry)) {
                (*it)->setIcon(0, QIcon(":/icons/part.png"));
                (*it)->setCheckState(0, Qt::Checked);
                (*it)->setData(1, Qt::UserRole, QVariant(Qt::Checked));
            } else if ((*it)->text(1) == "WorldBox" || (*it)->text(1) == "BeamRadius") {
                (*it)->setCheckState(0, Qt::Checked);
            }
            ++it;
        }

    });
    connect(ui->myViewerWidget, &Viewer::showOnly, [this]() {

        myViewerWidget->getContext()->EraseAll(true);
        showOnlySelectedPart(currentSelectedShape);


    });
    connect(ui->myViewerWidget, &Viewer::visibleOrInvisible, [this]() {
        if (currentSelectedShape != nullptr) {
            if (!getNodeData(currentSelectedShape)->getName().isNull()) {

                // alt üyelerini etkileyen özyinemeli fonk.
                changeVisibility(currentSelectedShape, !myViewerWidget->getContext()->IsDisplayed(
                        getNodeData(currentSelectedShape)->getObject()));
            }
        }

    });

    connect(ui->modelTreeWidget, &QTreeWidget::itemClicked, this,
            &MainWindow::slot_treeWidgetItemClicked);

    connect(ui->modelTreeWidget, &QTreeWidget::currentItemChanged, this,
            &MainWindow::slot_treeWidgetItemClicked);


    mainItem_geometry = new QTreeWidgetItem(projectManagerMainTreeWidget->invisibleRootItem());
    mainItem_geometry->setIcon(0, QIcon(":/icons/open-folder.png"));
    mainItem_geometry->setText(0, "Geometry");
    mainItem_geometry->setText(1, "Geometry");

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showOnlySelectedPart(QTreeWidgetItem *shape) {
    qDebug() << "showOnlySelectedPart";

    QTreeWidgetItemIterator iterator(projectManagerMainTreeWidget);
    while (*iterator) {
        if (((*iterator)->text(1) == "Geometry") && (*iterator != currentSelectedShape) &&
            (*iterator != mainItem_geometry)) {
            (*iterator)->setIcon(0, QIcon(":/icons/part-disable.png"));
            (*iterator)->setCheckState(0, Qt::Unchecked);
            (*iterator)->setData(1, Qt::UserRole, QVariant(Qt::Unchecked));
        } else if ((*iterator)->text(1) == "WorldBox" || (*iterator)->text(1) == "BeamRadius") {
            (*iterator)->setCheckState(0, Qt::Unchecked);
        }
        ++iterator;
    }

    if (shape->childCount() > 0) {
        QTreeWidgetItemIterator it(shape->child(0));

        while (*it) {
            if (((*it)->parent() == shape->parent() || (*it)->text(1) != "Geometry") ||
                (!(*it)->data(0, Qt::UserRole).canConvert<OCCData *>()) ||
                (!getNodeData((*it))->getIndex().startsWith(getNodeData(shape)->getIndex()))) {
                break;
            } else {

                (*it)->setIcon(0, QIcon(":/icons/part.png"));
                (*it)->setCheckState(0, Qt::Checked);
                (*it)->setData(1, Qt::UserRole, QVariant(Qt::Checked));
            }
            if ((*it)->childCount() == 0) {
                myViewerWidget->getContext()->Display(
                        getNodeData((*it))->getObject(), false);
            }
            ++it;
        }
    } else {

        shape->setIcon(0, QIcon(":/icons/part.png"));
        shape->setCheckState(0, Qt::Checked);
        shape->setData(1, Qt::UserRole, QVariant(Qt::Checked));
        myViewerWidget->getContext()->Display(getNodeData(shape)->getShape(),
                                              false);
    }

    myViewerWidget->getContext()->UpdateCurrentViewer();
    myViewerWidget->fitAll();

}

void MainWindow::changeVisibility(QTreeWidgetItem *shape, bool setVisible) {
    if (setVisible) {
        shape->setCheckState(0, Qt::Checked);
        shape->setIcon(0, QIcon(":/icons/part.png"));
        shape->setData(1, Qt::UserRole, QVariant(Qt::Checked));

        if (shape->childCount() > 0) {
            QTreeWidgetItemIterator it(shape->child(0));
            QApplication::processEvents();

            while (*it) {
                if (((*it)->parent() == shape->parent() || (*it)->text(1) != "Geometry") ||
                    (!(*it)->data(0, Qt::UserRole).canConvert<OCCData *>()) ||
                    (!getNodeData((*it))->getIndex().startsWith(getNodeData(shape)->getIndex()))) {
                    break;
                } else {
                    (*it)->setCheckState(0, Qt::Checked);
                    (*it)->setIcon(0, QIcon(":/icons/part.png"));
                    (*it)->setData(1, Qt::UserRole, QVariant(Qt::Checked));
                }
                if ((*it)->childCount() == 0 && !myViewerWidget->getContext()->IsDisplayed(
                        getNodeData((*it))->getObject())) {
                    myViewerWidget->getContext()->Display(
                            getNodeData((*it))->getObject(), false);
                }
                ++it;
            }
        } else {
            myViewerWidget->getContext()->Display(getNodeData(shape)->getObject(),
                                                  false);
        }

    } else {
        shape->setCheckState(0, Qt::Unchecked);
        shape->setIcon(0, QIcon(":/icons/part-disable.png"));
        shape->setData(1, Qt::UserRole, QVariant(Qt::Unchecked));


        if (shape->childCount() > 0) {
            QTreeWidgetItemIterator it(shape->child(0));

            while (*it) {
                if (((*it)->parent() == shape->parent() || (*it)->text(1) != "Geometry") ||
                    (!(*it)->data(0, Qt::UserRole).canConvert<OCCData *>()) ||
                    (!getNodeData((*it))->getIndex().simplified().startsWith(getNodeData(shape)->getIndex()))) {
                    break;
                } else {
                    (*it)->setCheckState(0, Qt::Unchecked);
                    (*it)->setIcon(0, QIcon(":/icons/part-disable.png"));
                    (*it)->setData(1, Qt::UserRole, QVariant(Qt::Unchecked));

                }
                if ((*it)->childCount() == 0 && myViewerWidget->getContext()->IsDisplayed(
                        getNodeData((*it))->getObject())) {
                    myViewerWidget->getContext()->Erase(
                            getNodeData((*it))->getObject(),
                            false);
                }
                ++it;
            }
        } else {
            myViewerWidget->getContext()->Erase(getNodeData(shape)->getObject(),
                                                true);
        }
    }
}

void MainWindow::slot_treeWidgetItemClicked(QTreeWidgetItem *arg_item) {
    if ((arg_item->text(1) == "Geometry") && (arg_item != mainItem_geometry)) {
        currentSelectedShape = arg_item;
        myViewerWidget->getContext()->ClearSelected(true);
        selectedShapeView(currentSelectedShape);
        myViewerWidget->getContext()->UpdateCurrentViewer();

        if (qvariant_cast<Qt::CheckState>(arg_item->data(1, Qt::UserRole)) != arg_item->checkState(0)) {
            changeVisibility(arg_item, arg_item->checkState(0));
        }

    }
}

void MainWindow::selectedShapeView(QTreeWidgetItem *arg_node) {
    if (arg_node->childCount() > 0) {
        QTreeWidgetItemIterator it(arg_node->child(0));

        while (*it) {
            if (((*it)->parent() == arg_node->parent() || (*it)->text(1) != "Geometry") ||
                (!(*it)->data(0, Qt::UserRole).canConvert<OCCData *>()) ||
                (!getNodeData((*it))->getIndex().startsWith(getNodeData(arg_node)->getIndex()))) {
                break;
            } else if ((*it)->childCount() == 0) {
                myViewerWidget->getContext()->AddOrRemoveSelected(getNodeData((*it))->getObject(), false);
            }
            ++it;
        }
    } else {
        myViewerWidget->getContext()->AddOrRemoveSelected(getNodeData(arg_node)->getObject(), false);
    }

}



