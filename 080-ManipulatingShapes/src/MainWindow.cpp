//
// Created by sufuk on 2.02.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <BRepBuilderAPI_Copy.hxx>
#include <TDF_AttributeIterator.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TDocStd_Application.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DataStructs.h"
#include "XSDRAW.hxx"
#include "TDF_ChildIterator.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "gp_Trsf.hxx"
#include "ViewerBox.h"
#include <QtGlobal>
#include <gp_Quaternion.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

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

//    ViewerBox *viewerBox = new ViewerBox(ui->myViewerWidget);
//    viewerBox->setParent(myViewerWidget);
//    viewerBox->show();
//    viewerBox->setPalette(Qt::transparent);
//    viewerBox->setAttribute( Qt::WA_TranslucentBackground, true );
//    viewerBox->setAttribute( Qt::WA_OpaquePaintEvent, true );
//    viewerBox->setAutoFillBackground(false);
//    viewerBox->setStyleSheet("QWidget{background-color: transparent;}");
//    viewerBox->setAttribute(Qt::WA_NoSystemBackground);
//    viewerBox->setWindowOpacity(0.4);

    connect(ui->actionImport, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open", QDir::homePath());
        myStepProcessor->loadSTEPFile(fileName);
    });

    ui->myViewerWidget->showTrihedronCube(true);
    ui->myViewerWidget->show3DGrid(true);
    ui->myViewerWidget->showPerformanceStats(true);

    connect(ui->actionCube, &QAction::triggered, this, &MainWindow::slot_createBox);
    connect(ui->actionSphere, &QAction::triggered, this, &MainWindow::slot_createSphere);
    connect(ui->actionCylinder, &QAction::triggered, this, &MainWindow::slot_createCylinder);

    connect(ui->mergeButton, &QPushButton::clicked, this, &MainWindow::slot_merge);
    connect(ui->cutButton, &QPushButton::clicked, this, &MainWindow::slot_cut);
    connect(ui->boundBoxButton, &QPushButton::clicked, this, &MainWindow::slot_boundBox);


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
    connect(ui->actionExportStep, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), "*.step");

        myStepProcessor->writeStepFile(fileName);
    });

    connect(ui->actionCreate_new_Geometry, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), "*.step");


        myStepProcessor->slot_createEmptyStep(fileName);

    });
    connect(ui->myViewerWidget, &Viewer::mouseReleasedShape, this, &MainWindow::slot_viewerMouseReleased);
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

    connect(ui->myViewerWidget, &Viewer::deletePart, this, &MainWindow::slot_deletePart);

    connect(ui->modelTreeWidget, &QTreeWidget::itemClicked, this,
            &MainWindow::slot_treeWidgetItemClicked);

    connect(ui->modelTreeWidget, &QTreeWidget::currentItemChanged, this,
            &MainWindow::slot_treeWidgetItemClicked);


    mainItem_geometry = new QTreeWidgetItem(projectManagerMainTreeWidget->invisibleRootItem());
    mainItem_geometry->setIcon(0, QIcon(":/icons/open-folder.png"));
    mainItem_geometry->setText(0, "Geometry");
    mainItem_geometry->setText(1, "Geometry");

//    connect(ui->xSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
//            &MainWindow::slot_spinboxValueChanged);
    connect(ui->xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_spinboxValueChanged);
    connect(ui->ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_spinboxValueChanged);
    connect(ui->zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_spinboxValueChanged);
//    connect(ui->ySpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
//            &MainWindow::slot_spinboxValueChanged);
//    connect(ui->zSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
//            &MainWindow::slot_spinboxValueChanged);
    connect(ui->angleBox_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_rotatePart);
    connect(ui->angleBox_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_rotatePart);
    connect(ui->angleBox_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_rotatePart);

    connect(ui->scaleBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::slot_scalePart);


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
        if (ui->modelTreeWidget->selectedItems().size() <= 1) {

            gp_Trsf trsf = getNodeData(currentSelectedShape)->getObject()->Transformation();

            // Translation Part
            double x, y, z;
            trsf.TranslationPart().Coord(x, y, z);

            ui->xSpinBox->blockSignals(true);
            ui->ySpinBox->blockSignals(true);
            ui->zSpinBox->blockSignals(true);

            ui->xSpinBox->setValue(x);
            ui->ySpinBox->setValue(y);
            ui->zSpinBox->setValue(z);

            ui->xSpinBox->blockSignals(false);
            ui->ySpinBox->blockSignals(false);
            ui->zSpinBox->blockSignals(false);

            // Rotation Part
            double alpha, beta, gamma;
            trsf.GetRotation().GetEulerAngles(gp_Intrinsic_XYZ, alpha, beta, gamma);

            alpha = (alpha / M_PI) * 180;
            beta = (beta / M_PI) * 180;
            gamma = (gamma / M_PI) * 180;

            ui->angleBox_x->blockSignals(true);
            ui->angleBox_y->blockSignals(true);
            ui->angleBox_z->blockSignals(true);

            ui->angleBox_x->setValue(alpha);
            ui->angleBox_y->setValue(beta);
            ui->angleBox_z->setValue(gamma);

            ui->angleBox_x->blockSignals(false);
            ui->angleBox_y->blockSignals(false);
            ui->angleBox_z->blockSignals(false);

            // Scale part
            double scaleFactor = trsf.ScaleFactor();
            ui->scaleBox->blockSignals(true);
            ui->scaleBox->setValue(scaleFactor);
            ui->scaleBox->blockSignals(false);

            cout << "\n**************\n";
            trsf.DumpJson(cout);
            cout << "\n\n";

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

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        qDebug() << "CTRL pressed";

        projectManagerMainTreeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        qDebug() << "ModelTree Selection Mode: Multiselection";
    }
    QWidget::keyPressEvent(event);

}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Control) {
        qDebug() << "CTRL released";
        projectManagerMainTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        qDebug() << "ModelTree Selection Mode: Single Selection";
    }
    QWidget::keyReleaseEvent(event);

}

void MainWindow::slot_createBox() {

    if (currentSelectedShape != nullptr) {

        bool IsTopLevel = myStepProcessor->shapeTool->IsTopLevel(getNodeData(currentSelectedShape)->getLabel());
        bool IsFree = myStepProcessor->shapeTool->IsFree(getNodeData(currentSelectedShape)->getLabel());
        bool IsShape = myStepProcessor->shapeTool->IsShape(getNodeData(currentSelectedShape)->getLabel());
        bool IsSimpleShape = myStepProcessor->shapeTool->IsSimpleShape(getNodeData(currentSelectedShape)->getLabel());
        bool IsReference = myStepProcessor->shapeTool->IsReference(getNodeData(currentSelectedShape)->getLabel());

        bool IsAssembly = myStepProcessor->shapeTool->IsAssembly(getNodeData(currentSelectedShape)->getLabel());
        bool IsComponent = myStepProcessor->shapeTool->IsComponent(getNodeData(currentSelectedShape)->getLabel());
        bool IsCompound = myStepProcessor->shapeTool->IsCompound(getNodeData(currentSelectedShape)->getLabel());
        bool IsSubShape = myStepProcessor->shapeTool->IsSubShape(getNodeData(currentSelectedShape)->getLabel());


        QCoreApplication::processEvents();
        qDebug() << "CurrentSelectedShape : " << currentSelectedShape->text(0);

        qDebug() << "IsTopLevel : " << IsTopLevel;
        qDebug() << "IsFree : " << IsFree;
        qDebug() << "IsShape : " << IsShape;
        qDebug() << "IsSimpleShape : " << IsSimpleShape;
        qDebug() << "IsReference : " << IsReference;

        qDebug() << "IsAssembly : " << IsAssembly;
        qDebug() << "IsComponent : " << IsComponent;
        qDebug() << "IsCompound : " << IsCompound;
        qDebug() << "IsSubShape : " << IsSubShape;
        QCoreApplication::processEvents();

        if (!IsFree || !IsAssembly) {
            QMessageBox::warning(this, "Warning", "You can only add new components to the assemblies!");
        } else {
            QCoreApplication::processEvents();
            // Create item base
            QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem();
            treeWidgetItem->setCheckState(0, Qt::Checked);
            treeWidgetItem->setData(1, Qt::UserRole, QVariant(Qt::Checked));
            treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
            treeWidgetItem->setText(1, "Geometry");
            currentSelectedShape->addChild(treeWidgetItem);

            // Create data for item
            OCCData *occData = new OCCData();
            QVariant variant;
            variant.setValue(occData);
            treeWidgetItem->setData(0, Qt::UserRole, variant);

            // Construct a box & Set the location
            TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(10, 10, 10).Solid();
            aTopoBox.Location(getNodeData(currentSelectedShape)->getTopoShape().Location());

            // Add Box to the selected item and get Created label
            TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                    getNodeData(currentSelectedShape)->getLabel(), aTopoBox);
            occData->setLabel(newLabel);


            // This is necessary after Adding/Removing
            myStepProcessor->shapeTool->UpdateAssemblies();

            // Get the name of label
            Handle(TDataStd_Name) nameAttr;
            if (occData->getLabel().FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
                occData->setName(QString::fromStdString(STEPProcessor::toString(nameAttr->Get())));
            } else {
                occData->setName("Unknown");
            }

            // Check if name is already taken if it is change the name
            occData->setName(myStepProcessor->nameControl(occData->getName()));

            // Set index respective to the parent
            occData->setIndex(getNodeData(currentSelectedShape)->getIndex() + ":" +
                              QString::number(currentSelectedShape->childCount() + 1));

            // Set text of item respective to its name attribute
            treeWidgetItem->setText(0, occData->getName() + " (" + occData->getIndex() + ")");

            // Set transparency to 1.0 (Default value)
            occData->setTransparency(1.0);

            // Set material to ALUMINIUM (Default material)
            occData->setMaterial("ALUMINIUM");

            // Save topoShape to do data
            occData->setTopoShape(aTopoBox);

            // Save location to the data
            occData->setLocation(occData->getTopoShape().Location());

            // Save the volume of component
            GProp_GProps propGProps;
            BRepGProp::VolumeProperties(aTopoBox, propGProps);
            occData->setVolume(propGProps.Mass());

            myStepProcessor->colorTool->SetColor(newLabel, Quantity_NOC_FIREBRICK, XCAFDoc_ColorGen);

            // Create a NodeInteractive for actual display shape
            auto *nodeInteractive = new NodeInteractive(newLabel, treeWidgetItem);
            occData->setShape(nodeInteractive);
            occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());
            occData->setObject(nodeInteractive);


            myViewerWidget->getContext()->Display(nodeInteractive, true);

        }

    } else {
        QMessageBox::warning(this, "Warning", "Please select an parent assembly first!");

    }

}

void MainWindow::slot_createCylinder() {
    if (currentSelectedShape != nullptr) {

        bool IsTopLevel = myStepProcessor->shapeTool->IsTopLevel(getNodeData(currentSelectedShape)->getLabel());
        bool IsFree = myStepProcessor->shapeTool->IsFree(getNodeData(currentSelectedShape)->getLabel());
        bool IsShape = myStepProcessor->shapeTool->IsShape(getNodeData(currentSelectedShape)->getLabel());
        bool IsSimpleShape = myStepProcessor->shapeTool->IsSimpleShape(getNodeData(currentSelectedShape)->getLabel());
        bool IsReference = myStepProcessor->shapeTool->IsReference(getNodeData(currentSelectedShape)->getLabel());

        bool IsAssembly = myStepProcessor->shapeTool->IsAssembly(getNodeData(currentSelectedShape)->getLabel());
        bool IsComponent = myStepProcessor->shapeTool->IsComponent(getNodeData(currentSelectedShape)->getLabel());
        bool IsCompound = myStepProcessor->shapeTool->IsCompound(getNodeData(currentSelectedShape)->getLabel());
        bool IsSubShape = myStepProcessor->shapeTool->IsSubShape(getNodeData(currentSelectedShape)->getLabel());


        QCoreApplication::processEvents();
        qDebug() << "CurrentSelectedShape : " << currentSelectedShape->text(0);

        qDebug() << "IsTopLevel : " << IsTopLevel;
        qDebug() << "IsFree : " << IsFree;
        qDebug() << "IsShape : " << IsShape;
        qDebug() << "IsSimpleShape : " << IsSimpleShape;
        qDebug() << "IsReference : " << IsReference;

        qDebug() << "IsAssembly : " << IsAssembly;
        qDebug() << "IsComponent : " << IsComponent;
        qDebug() << "IsCompound : " << IsCompound;
        qDebug() << "IsSubShape : " << IsSubShape;
        QCoreApplication::processEvents();

        if (!IsFree || !IsAssembly) {
            QMessageBox::warning(this, "Warning", "You can only add new components to the assemblies!");
        } else {
            // Create item base
            QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem();
            treeWidgetItem->setCheckState(0, Qt::Checked);
            treeWidgetItem->setData(1, Qt::UserRole, QVariant(Qt::Checked));
            treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
            treeWidgetItem->setText(1, "Geometry");
            currentSelectedShape->addChild(treeWidgetItem);

            // Create data for item
            OCCData *occData = new OCCData();
            QVariant variant;
            variant.setValue(occData);
            treeWidgetItem->setData(0, Qt::UserRole, variant);

            // Construct a box & Set the location
            TopoDS_Shape topoDsShape = BRepPrimAPI_MakeCylinder(10, 10).Solid();
            topoDsShape.Location(getNodeData(currentSelectedShape)->getTopoShape().Location());

            // Add Box to the selected item and get Created label
            TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                    getNodeData(currentSelectedShape)->getLabel(), topoDsShape);
            occData->setLabel(newLabel);


            // This is necessary after Adding/Removing
            myStepProcessor->shapeTool->UpdateAssemblies();

            // Get the name of label
            Handle(TDataStd_Name) nameAttr;
            if (occData->getLabel().FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
                occData->setName(QString::fromStdString(STEPProcessor::toString(nameAttr->Get())));
            } else {
                occData->setName("Unknown");
            }

            // Check if name is already taken if it is change the name
            occData->setName(myStepProcessor->nameControl(occData->getName()));

            // Set index respective to the parent
            occData->setIndex(getNodeData(currentSelectedShape)->getIndex() + ":" +
                              QString::number(currentSelectedShape->childCount() + 1));

            // Set text of item respective to its name attribute
            treeWidgetItem->setText(0, occData->getName() + " (" + occData->getIndex() + ")");

            // Set transparency to 1.0 (Default value)
            occData->setTransparency(1.0);

            // Set material to ALUMINIUM (Default material)
            occData->setMaterial("ALUMINIUM");

            // Save topoShape to do data
            occData->setTopoShape(topoDsShape);

            // Save location to the data
            occData->setLocation(occData->getTopoShape().Location());

            // Save the volume of component
            GProp_GProps propGProps;
            BRepGProp::VolumeProperties(topoDsShape, propGProps);
            occData->setVolume(propGProps.Mass());

            myStepProcessor->colorTool->SetColor(newLabel, Quantity_NOC_FIREBRICK, XCAFDoc_ColorGen);

            // Create a NodeInteractive for actual display shape
            auto *nodeInteractive = new NodeInteractive(newLabel, treeWidgetItem);
            occData->setShape(nodeInteractive);
            occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());
            occData->setObject(nodeInteractive);


            myViewerWidget->getContext()->Display(nodeInteractive, true);

        }

    } else {
        QMessageBox::warning(this, "Warning", "Please select an parent assembly first!");

    }

}

void MainWindow::slot_createSphere() {
    if (currentSelectedShape != nullptr) {

        bool IsTopLevel = myStepProcessor->shapeTool->IsTopLevel(getNodeData(currentSelectedShape)->getLabel());
        bool IsFree = myStepProcessor->shapeTool->IsFree(getNodeData(currentSelectedShape)->getLabel());
        bool IsShape = myStepProcessor->shapeTool->IsShape(getNodeData(currentSelectedShape)->getLabel());
        bool IsSimpleShape = myStepProcessor->shapeTool->IsSimpleShape(getNodeData(currentSelectedShape)->getLabel());
        bool IsReference = myStepProcessor->shapeTool->IsReference(getNodeData(currentSelectedShape)->getLabel());

        bool IsAssembly = myStepProcessor->shapeTool->IsAssembly(getNodeData(currentSelectedShape)->getLabel());
        bool IsComponent = myStepProcessor->shapeTool->IsComponent(getNodeData(currentSelectedShape)->getLabel());
        bool IsCompound = myStepProcessor->shapeTool->IsCompound(getNodeData(currentSelectedShape)->getLabel());
        bool IsSubShape = myStepProcessor->shapeTool->IsSubShape(getNodeData(currentSelectedShape)->getLabel());


        QCoreApplication::processEvents();
        qDebug() << "CurrentSelectedShape : " << currentSelectedShape->text(0);

        qDebug() << "IsTopLevel : " << IsTopLevel;
        qDebug() << "IsFree : " << IsFree;
        qDebug() << "IsShape : " << IsShape;
        qDebug() << "IsSimpleShape : " << IsSimpleShape;
        qDebug() << "IsReference : " << IsReference;

        qDebug() << "IsAssembly : " << IsAssembly;
        qDebug() << "IsComponent : " << IsComponent;
        qDebug() << "IsCompound : " << IsCompound;
        qDebug() << "IsSubShape : " << IsSubShape;
        QCoreApplication::processEvents();

        if (!IsFree || !IsAssembly) {
            QMessageBox::warning(this, "Warning", "You can only add new components to the assemblies!");
        } else {
            // Create item base
            QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem();
            treeWidgetItem->setCheckState(0, Qt::Checked);
            treeWidgetItem->setData(1, Qt::UserRole, QVariant(Qt::Checked));
            treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
            treeWidgetItem->setText(1, "Geometry");
            currentSelectedShape->addChild(treeWidgetItem);

            // Create data for item
            OCCData *occData = new OCCData();
            QVariant variant;
            variant.setValue(occData);
            treeWidgetItem->setData(0, Qt::UserRole, variant);

            // Construct a box & Set the location
            TopoDS_Shape topoDsShape = BRepPrimAPI_MakeSphere(10).Solid();
            topoDsShape.Location(getNodeData(currentSelectedShape)->getTopoShape().Location());

            // Add Box to the selected item and get Created label
            TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                    getNodeData(currentSelectedShape)->getLabel(), topoDsShape);
            occData->setLabel(newLabel);


            // This is necessary after Adding/Removing
            myStepProcessor->shapeTool->UpdateAssemblies();

            // Get the name of label
            Handle(TDataStd_Name) nameAttr;
            if (occData->getLabel().FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
                occData->setName(QString::fromStdString(STEPProcessor::toString(nameAttr->Get())));
            } else {
                occData->setName("Unknown");
            }

            // Check if name is already taken if it is change the name
            occData->setName(myStepProcessor->nameControl(occData->getName()));

            // Set index respective to the parent
            occData->setIndex(getNodeData(currentSelectedShape)->getIndex() + ":" +
                              QString::number(currentSelectedShape->childCount() + 1));

            // Set text of item respective to its name attribute
            treeWidgetItem->setText(0, occData->getName() + " (" + occData->getIndex() + ")");

            // Set transparency to 1.0 (Default value)
            occData->setTransparency(1.0);

            // Set material to ALUMINIUM (Default material)
            occData->setMaterial("ALUMINIUM");

            // Save topoShape to do data
            occData->setTopoShape(topoDsShape);

            // Save location to the data
            occData->setLocation(occData->getTopoShape().Location());

            // Save the volume of component
            GProp_GProps propGProps;
            BRepGProp::VolumeProperties(topoDsShape, propGProps);
            occData->setVolume(propGProps.Mass());

            myStepProcessor->colorTool->SetColor(newLabel, Quantity_NOC_FIREBRICK, XCAFDoc_ColorGen);
            // Create a NodeInteractive for actual display shape
            auto *nodeInteractive = new NodeInteractive(newLabel, treeWidgetItem);
            occData->setShape(nodeInteractive);
            occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());
            occData->setObject(nodeInteractive);

            myViewerWidget->getContext()->Display(nodeInteractive, true);

        }

    } else {
        QMessageBox::warning(this, "Warning", "Please select an parent assembly first!");

    }

}

void MainWindow::slot_merge() {

    QList<QTreeWidgetItem *> selectedItems = projectManagerMainTreeWidget->selectedItems();

    if (selectedItems.size() >= 2) {

        QMessageBox *msgBox = new QMessageBox(this);

        msgBox->setText("Please select the merge type you want!");

        QAbstractButton *defaultModeButton = msgBox->addButton("Default", QMessageBox::YesRole);
        QAbstractButton *subtractModeButton = msgBox->addButton("Subtract", QMessageBox::NoRole);

        msgBox->exec();

        if (msgBox->clickedButton() == subtractModeButton) {
            slot_cut();
        }

        // Create a compound and a builder
        TopoDS_Compound aCompound;
        BRep_Builder aBuilder;
        aBuilder.MakeCompound(aCompound);

        for (QTreeWidgetItem *item: selectedItems) {
            aBuilder.Add(aCompound, getNodeData(item)->getTopoShape());
            myViewerWidget->getContext()->Remove(getNodeData(item)->getObject(), true);
            mainItem_geometry->removeChild(item);
            delete item;
        }

        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem();
        treeWidgetItem->setCheckState(0, Qt::Checked);
        treeWidgetItem->setData(1, Qt::UserRole, QVariant(Qt::Checked));
        treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
        treeWidgetItem->setText(1, "Geometry");
        //TODO FIND A METHOD TO SELECTING PARENT FOR COMPOUND
        mainItem_geometry->child(0)->addChild(treeWidgetItem);

        // Create data for item
        OCCData *occData = new OCCData();
        QVariant variant;
        variant.setValue(occData);
        treeWidgetItem->setData(0, Qt::UserRole, variant);

        // Set the location
        aCompound.Location(getNodeData(treeWidgetItem->parent())->getTopoShape().Location());

        // Add Compound to the selected item and get Created label
        TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                getNodeData(treeWidgetItem->parent())->getLabel(), aCompound);
        occData->setLabel(newLabel);

        // This is necessary after Adding/Removing
        myStepProcessor->shapeTool->UpdateAssemblies();

        // Get the name of label
        Handle(TDataStd_Name) nameAttr;
        if (occData->getLabel().FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            occData->setName(QString::fromStdString(STEPProcessor::toString(nameAttr->Get())));
        } else {
            occData->setName("Unknown");
        }

        // Check if name is already taken if it is change the name
        occData->setName(myStepProcessor->nameControl(occData->getName()));

        // Set index respective to the parent
        occData->setIndex(getNodeData(treeWidgetItem->parent())->getIndex() + ":" +
                          QString::number(treeWidgetItem->parent()->childCount() + 1));

        // Set text of item respective to its name attribute
        treeWidgetItem->setText(0, occData->getName() + " (" + occData->getIndex() + ")");

        // Set transparency to 1.0 (Default value)
        occData->setTransparency(1.0);

        // Set material to ALUMINIUM (Default material)
        occData->setMaterial("ALUMINIUM");

        // Save topoShape to do data
        occData->setTopoShape(aCompound);

        // Save location to the data
        occData->setLocation(occData->getTopoShape().Location());

        // Save the volume of component
        GProp_GProps propGProps;
        BRepGProp::VolumeProperties(aCompound, propGProps);
        occData->setVolume(propGProps.Mass());

        // Create a NodeInteractive for actual display shape
        auto *nodeInteractive = new NodeInteractive(newLabel, treeWidgetItem);
        occData->setShape(nodeInteractive);
        occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());
        occData->setObject(nodeInteractive);

        // TODO DOESNT WORK INVESTIGATE
        nodeInteractive->SetColor(Quantity_NOC_FIREBRICK);

        myViewerWidget->getContext()->Display(nodeInteractive, true);

    }

}

void MainWindow::slot_viewerMouseReleased() {

    gp_Trsf newTransformation;

    newTransformation = getNodeData(currentSelectedShape)->getObject()->Transformation();

    TopoDS_Shape topoDsShape = getNodeData(currentSelectedShape)->getTopoShape();
    topoDsShape.Location(newTransformation);

    myStepProcessor->shapeTool->SetShape(getNodeData(currentSelectedShape)->getLabel(), topoDsShape);

    myStepProcessor->shapeTool->UpdateAssemblies();

    getNodeData(currentSelectedShape)->setTopoShape(topoDsShape);
    getNodeData(currentSelectedShape)->setLocation(topoDsShape.Location());

    myStepProcessor->shapeTool->UpdateAssemblies();

    slot_treeWidgetItemClicked(currentSelectedShape);
}

void MainWindow::slot_cut() {

    if (ui->modelTreeWidget->selectedItems().size() == 2) {

        // Get Selected Items
        QTreeWidgetItem *baseItem = ui->modelTreeWidget->selectedItems().at(0);
        QTreeWidgetItem *toolItem = ui->modelTreeWidget->selectedItems().at(1);

        // Get shapes of selected items
        TopoDS_Shape baseShape = getNodeData(baseItem)->getTopoShape();
        TopoDS_Shape toolShape = getNodeData(toolItem)->getTopoShape();

        // Create the cutter (automatically applies the cut)
        BRepAlgoAPI_Cut cutMaker(baseShape, toolShape);

        // Get the cut shape from cutter
        TopoDS_Shape newShape = cutMaker.Shape();

        // Change the shape from XCaf_Doc
        myStepProcessor->shapeTool->SetShape(getNodeData(baseItem)->getLabel(), newShape);

        // Remove the old object from viewer
        myViewerWidget->getContext()->Remove(getNodeData(baseItem)->getObject(), true);

        // Create a new node
        NodeInteractive *newNode = new NodeInteractive(getNodeData(baseItem)->getLabel(), baseItem);
        getNodeData(baseItem)->setLocation(newShape.Location());
        getNodeData(baseItem)->setShape(newNode);
        getNodeData(baseItem)->setObject(newNode);

        myViewerWidget->getContext()->Display(newNode, true);

        myStepProcessor->shapeTool->UpdateAssemblies();

    } else {
        QMessageBox::warning(this, "Warning", "Please select two nodes");
    }


}

void MainWindow::slot_deletePart() {

    // Remove part from viewer for good
    myViewerWidget->getContext()->Remove(getNodeData(currentSelectedShape)->getObject(), true);

    // Remove part from XCafDoc
    myStepProcessor->shapeTool->RemoveShape(getNodeData(currentSelectedShape)->getLabel(), true);
    myStepProcessor->shapeTool->UpdateAssemblies();

    // Remove part from tree widget
    currentSelectedShape->parent()->removeChild(currentSelectedShape);
    projectManagerMainTreeWidget->clearSelection();
    projectManagerMainTreeWidget->setCurrentItem(mainItem_geometry->child(0));


}

void MainWindow::slot_spinboxValueChanged() {

    if (currentSelectedShape != nullptr && currentSelectedShape->childCount() == 0) {

        // Get Object
        NodeInteractive *nodeInteractive = getNodeData(currentSelectedShape)->getObject();

        //Get current transformation and apply new translation to it
        gp_Trsf transformation = nodeInteractive->Transformation();
        transformation.SetTranslationPart(gp_Vec(ui->xSpinBox->value(), ui->ySpinBox->value(), ui->zSpinBox->value()));

        // Redisplay the object with new transformation
        myViewerWidget->getContext()->SetLocation(nodeInteractive,TopLoc_Location(transformation));
        myViewerWidget->getContext()->UpdateCurrentViewer();
        myViewerWidget->getContext()->CurrentViewer()->Redraw();

        slot_viewerMouseReleased();

    }
}

void MainWindow::slot_rotatePart() {

    if (currentSelectedShape != nullptr && currentSelectedShape->childCount() == 0) {

        // Get Object
        NodeInteractive *nodeInteractive = getNodeData(currentSelectedShape)->getObject();

        // Create temporary transformations to apply rotations
        gp_Trsf rotX, rotY, rotZ;

        // Apply rotations
        rotX.SetRotation(gp_Ax1({0, 0, 0}, {1, 0, 0}), M_PI * (ui->angleBox_x->value() / 180));
        rotY.SetRotation(gp_Ax1({0, 0, 0}, {0, 1, 0}), M_PI * (ui->angleBox_y->value() / 180));
        rotZ.SetRotation(gp_Ax1({0, 0, 0}, {0, 0, 1}), M_PI * (ui->angleBox_z->value() / 180));

        // Make them a single transformation
        TopLoc_Location rotAll(rotX * rotY * rotZ);

        // Get current translation (Actual Location)
        double x, y, z;
        nodeInteractive->Transformation().TranslationPart().Coord(x, y, z);

        // Create a new transformation equals to the rotated transformation
        gp_Trsf trsf = rotAll.Transformation();

        // Change tge translation of transformation
        trsf.SetTranslationPart(gp_Vec(x, y, z));

        // Get the copy of object's TopoDS_Shape and apply new transformation to it
        TopoDS_Shape topoDsShape = nodeInteractive->Shape();
        topoDsShape.Location(trsf);

        // Change the respected label's shape.
        myStepProcessor->shapeTool->SetShape(nodeInteractive->GetLabel(), topoDsShape);
        //Also change the object's shape
        nodeInteractive->SetShape(topoDsShape);
        myStepProcessor->shapeTool->UpdateAssemblies();

        // Make necessary modifications for model tree.
        getNodeData(currentSelectedShape)->setTopoShape(topoDsShape);
        getNodeData(currentSelectedShape)->setLocation(topoDsShape.Location());

        // Redisplay the object with new transformation
        myViewerWidget->getContext()->SetLocation(nodeInteractive, topoDsShape.Location());
        myViewerWidget->getContext()->UpdateCurrentViewer();
        myViewerWidget->getContext()->CurrentViewer()->Redraw();

        slot_treeWidgetItemClicked(currentSelectedShape);
    }
}

void MainWindow::slot_scalePart() {

    // Get Object and Item
    NodeInteractive *nodeInteractive = getNodeData(currentSelectedShape)->getObject();
    TopoDS_Shape topoDsShape = nodeInteractive->Shape();

    // Create a new transformation and change its scale factor
    gp_Trsf trsf = nodeInteractive->Transformation();
    trsf.SetScaleFactor(ui->scaleBox->value());
//    double x, y, z;
//    nodeInteractive->Transformation().TranslationPart().Coord(x, y, z);
//    trsf.SetTranslationPart(gp_Vec(x, y, z));

    topoDsShape.Location(trsf);

    // Change the shape from XCaf_Doc
    myStepProcessor->shapeTool->SetShape(nodeInteractive->GetLabel(), topoDsShape);
    myStepProcessor->shapeTool->UpdateAssemblies();

    // Remove the old object from viewer
    myViewerWidget->getContext()->Remove(nodeInteractive, true);

    // Create a new node
    NodeInteractive *newNode = new NodeInteractive(getNodeData(currentSelectedShape)->getLabel(), currentSelectedShape);
    newNode->SetShape(topoDsShape);
    getNodeData(currentSelectedShape)->setTopoShape(topoDsShape);
    getNodeData(currentSelectedShape)->setLocation(topoDsShape.Location());
    getNodeData(currentSelectedShape)->setShape(newNode);
    getNodeData(currentSelectedShape)->setObject(newNode);

    myViewerWidget->getContext()->Display(newNode, true);
    myViewerWidget->getContext()->SetLocation(newNode, topoDsShape.Location());
    myViewerWidget->getContext()->UpdateCurrentViewer();
    myViewerWidget->getContext()->CurrentViewer()->Redraw();

    cout << "\n--------------\n";
    trsf.DumpJson(cout);
    cout << "\n\n";
}

void MainWindow::slot_boundBox() {

    NodeInteractive *nodeInteractive = getNodeData(currentSelectedShape)->getObject();
    TopoDS_Shape topoDsShape = nodeInteractive->Shape();

    Bnd_Box boundaryBox = Bnd_Box();
    BRepBndLib::AddClose(topoDsShape, (Bnd_Box &) boundaryBox);

    double xMin, xMax, yMin, yMax, zMin, zMax;
    double centerPointX, centerPointY, centerPointZ, lenghtXAxis, lenghtYAxis, lenghtZAxis;

    // boundry box üzerinden değerler çekilir
    boundaryBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

    centerPointX = (xMin + xMax) / 2.0;
    centerPointY = (yMin + yMax) / 2.0;
    centerPointZ = (zMin + zMax) / 2.0;
    lenghtXAxis = (xMax - xMin);
    lenghtYAxis = (yMax - yMin);
    lenghtZAxis = (zMax - zMin);

    if (lenghtXAxis >= lenghtYAxis) {
        if (lenghtXAxis >= lenghtZAxis) {
            lenghtYAxis = lenghtXAxis;
            lenghtZAxis = lenghtXAxis;
        } else {
            lenghtXAxis = lenghtZAxis;
            lenghtYAxis = lenghtZAxis;
        }
    } else {
        if (lenghtYAxis >= lenghtZAxis) {
            lenghtXAxis = lenghtYAxis;
            lenghtZAxis = lenghtYAxis;
        } else {
            lenghtXAxis = lenghtZAxis;
            lenghtYAxis = lenghtZAxis;
        }
    }
    gp_Pnt centerPoint(-centerPointX, -centerPointY, -centerPointZ);
    gp_Pnt cornerPoint(centerPointX - (lenghtZAxis / 2.0), centerPointY - (lenghtZAxis / 2.0),
                       centerPointZ - (lenghtZAxis / 2.0));
    TopoDS_Shape boundShape = BRepPrimAPI_MakeBox(cornerPoint, lenghtZAxis, lenghtZAxis, lenghtZAxis).Shape();
    Handle_AIS_Shape aisShape = new AIS_Shape(boundShape);

    myViewerWidget->getContext()->Display(aisShape, false);
    myViewerWidget->getContext()->SetLocation(aisShape, boundShape.Location());
    myViewerWidget->getContext()->SetSelectionModeActive(aisShape, -1, false);
    myViewerWidget->getContext()->SetTransparency(aisShape, 0.8, false);
    myViewerWidget->getContext()->SetColor(aisShape, Quantity_NOC_LIGHTBLUE4, false);
    myViewerWidget->getContext()->UpdateCurrentViewer();

}

