//
// Created by sufuk on 2.02.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <BRepBuilderAPI_Copy.hxx>
#include <TDF_AttributeIterator.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TDocStd_Application.hxx>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DataStructs.h"
#include "XSDRAW.hxx"
#include "TDF_ChildIterator.hxx"

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

    connect(ui->actionCube, &QAction::triggered, [this]() {

        TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(10, 30, 15).Shape();
        TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                getNodeData(mainItem_geometry->child(0))->getLabel(), aTopoBox);
        myStepProcessor->shapeTool->UpdateAssemblies();
//        qDebug() << "tag "<< newLabel.Tag();
//        TDF_Label newLabel = myStepProcessor->readerDoc->Main().FindChild(getNodeData(mainItem_geometry->child(0))->getLabel().Tag()).NewChild();
//        bool isCreated = myStepProcessor->shapeTool->AddSubShape(getNodeData(mainItem_geometry->child(0))->getLabel(), aTopoBox, newLabel);
//        qDebug() << "isCreated " << isCreated;
        //        myStepProcessor->readerDoc->Main().
        //        TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(myStepProcessor->shapeTool->FindShape(getNodeData(mainItem_geometry->child(0))->getTopoShape(), false),aTopoBox);
//                FindChild(getNodeData(mainItem_geometry->child(0))->getLabel().Father().Father().Tag(), false)
//                .FindChild(getNodeData(mainItem_geometry->child(0))->getLabel().Father().Tag(), false).NewChild();
//        TDF_Label newLabel = myStepProcessor->readerDoc->Main().FindChild(1, false).FindChild(1, false).FindChild(1, false).NewChild();
        qDebug() << "tag " << newLabel.Tag() << "depth " << newLabel.Depth();


        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(mainItem_geometry->child(0), {"Item-" + QString::number(
                mainItem_geometry->child(0)->childCount())});
        treeWidgetItem->setCheckState(0, Qt::Checked);

//        myStepProcessor->shapeTool->NewShape()
        myStepProcessor->shapeTool->SetShape(newLabel, aTopoBox);
        NodeInteractive *nodeInteractive = new NodeInteractive(newLabel, treeWidgetItem);
        nodeInteractive->SetColor(Quantity_NOC_FIREBRICK);

        OCCData *occData = new OCCData();
        QVariant variant;
        variant.setValue(occData);
        treeWidgetItem->setData(0, Qt::UserRole, variant);

//        GProp_GProps gprops;
//        BRepGProp::VolumeProperties(arg_shape, gprops);
//        double volume = gprops.Mass();
//        if (volume <= 0) {
//            has2dShape = true;
//            return nullptr;
//        }
        occData->setLabel(newLabel);
        occData->setTopoShape(aTopoBox);
        occData->setShape(nodeInteractive);
        occData->setObject(nodeInteractive);

        myViewerWidget->getContext()->Display(nodeInteractive, true);

    });

    connect(ui->actionSphere, &QAction::triggered, [this]() {

    });


    connect(ui->myViewerWidget, &Viewer::mouseSelectedShape, [this]() {
        if (!myViewerWidget->getContext()->DetectedOwner().IsNull() && mainItem_geometry->childCount() > 0) {
            auto viewCubeOwner = opencascade::handle<AIS_ViewCubeOwner>::DownCast(
                    myViewerWidget->getContext()->DetectedOwner());

            auto manipulator = opencascade::handle<AIS_Manipulator>::DownCast(
                    myViewerWidget->getContext()->DetectedInteractive());

            if (!viewCubeOwner && !manipulator) {
                currentSelectedShape = dynamic_cast<NodeInteractive *>(myViewerWidget->getContext()->DetectedInteractive().operator->())->getTreeWidgetItem();
                projectManagerMainTreeWidget->setCurrentItem(currentSelectedShape);
                qDebug() << "tag " << getNodeData(currentSelectedShape)->getLabel().Tag() << "depth "
                         << getNodeData(currentSelectedShape)->getLabel().Depth();
                qDebug() << "tag " << getNodeData(currentSelectedShape)->getLabel().Tag() << "depth "
                         << getNodeData(currentSelectedShape)->getLabel().Depth();
                myViewerWidget->getAManipulator()->Detach();
                myViewerWidget->getContext()->Erase(myViewerWidget->getAManipulator(), true);
                myViewerWidget->getAManipulator()->Attach(
                        dynamic_cast<NodeInteractive *>(myViewerWidget->getContext()->DetectedInteractive().operator->()));
                myViewerWidget->getContext()->UpdateCurrentViewer();
                myViewerWidget->getAManipulator()->EnableMode(AIS_MM_Translation);

            }

        }
    });
    connect(ui->actionExportStep, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), "*.step");

        myStepProcessor->writeStepFile(fileName);
    });
    connect(ui->myViewerWidget, &Viewer::mouseReleasedShape, [this] {


        gp_Trsf originalTransformation, newTransformation, bufferTransformation, testTransformation;
//
        originalTransformation = getNodeData(currentSelectedShape)->getTopoShape().Location().Transformation();
        newTransformation = getNodeData(currentSelectedShape)->getObject()->Transformation();
//
        TopoDS_Shape topoDsShape = getNodeData(currentSelectedShape)->getTopoShape();
        topoDsShape.Location(newTransformation);
        TDF_Label label = getNodeData(currentSelectedShape)->getLabel();
        myStepProcessor->shapeTool->RemoveComponent(label);

        TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                getNodeData(currentSelectedShape->parent())->getLabel(), topoDsShape);
        getNodeData(currentSelectedShape)->setLabel(newLabel);
        getNodeData(currentSelectedShape)->setTopoShape(topoDsShape);
        myStepProcessor->shapeTool->UpdateAssemblies();

        //        myStepProcessor->shapeTool->UpdateAssemblies();
//
//        TDF_Label label = getNodeData(currentSelectedShape)->getLabel();

//        myStepProcessor->shapeTool->
//        TDF_Label rootLabel = getNodeData(mainItem_geometry->child(0))->getLabel();
//
//
//        TDF_Label newLabel = myStepProcessor->readerDoc->Main().FindChild(getNodeData(mainItem_geometry->child(0))->getLabel().Tag()).NewChild();
//
//        myStepProcessor->shapeTool->SetShape(label, topoDsShape);
//
//
//        qDebug() << "NbChild " << rootLabel.NbChildren();


//        TDF_AttributeIterator iterator(label, true);
//
//        for (int i = 0; iterator.More(); iterator.Next()) {
//            iterator.Value();
////            newLabel.AddAttribute(iterator.Value(), true);
//        }

//        bool isRemoved = myStepProcessor->shapeTool->RemoveShape(label, true);

//        qDebug() << "NbChild after remove " << isRemoved << " " << rootLabel.NbChildren();
//
////        rootLabel.Fi
////        myStepProcessor->shapeTool->AddSubShape(label, getNodeData(currentSelectedShape)->getTopoShape(), rootLabel);
////        myStepProcessor->shapeTool->AddSubShape(label, topoDsShape);
////        myStepProcessor->shapeTool->AddComponent(rootLabel, label, newTransformation);
////        myStepProcessor->shapeTool->Add(rootLabel, label, newTransformation);
//
//        getNodeData(currentSelectedShape)->setLabel(label);
//        getNodeData(mainItem_geometry->child(0))->setLabel(rootLabel);
//
//
//        bufferTransformation = topoDsShape.Location().Transformation();
//        testTransformation = myStepProcessor->shapeTool->GetShape(getNodeData(currentSelectedShape)->getLabel()).Location().Transformation();
//        cout << "*************************\n";
//        originalTransformation.DumpJson(cout);
//        cout << "\n";
//        newTransformation.DumpJson(cout);
//        cout << "\n";
//        bufferTransformation.DumpJson(cout);
//        cout << "\n";
//        testTransformation.DumpJson(cout);
//        cout << "\n";
        myStepProcessor->shapeTool->UpdateAssemblies();
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



