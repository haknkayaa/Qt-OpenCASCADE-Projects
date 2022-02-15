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
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DataStructs.h"
#include "XSDRAW.hxx"
#include "TDF_ChildIterator.hxx"
#include "BRepAlgoAPI_Cut.hxx"

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

    connect(ui->actionCube, &QAction::triggered, this, &MainWindow::slot_createBox);
    connect(ui->actionSphere, &QAction::triggered, this, &MainWindow::slot_createSphere);
    connect(ui->actionCylinder, &QAction::triggered, this, &MainWindow::slot_createCylinder);

    connect(ui->mergeButton, &QPushButton::clicked, this, &MainWindow::slot_merge);
    connect(ui->cutButton, &QPushButton::clicked, this, &MainWindow::slot_cut);


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

        if (currentSelectedShape->childCount() == 0) {
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

        if (currentSelectedShape->childCount() == 0) {
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

        if (currentSelectedShape->childCount() == 0) {
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

        if (msgBox->clickedButton() == subtractModeButton){
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

    gp_Trsf originalTransformation, newTransformation;

    originalTransformation = getNodeData(currentSelectedShape)->getTopoShape().Location().Transformation();
    newTransformation = getNodeData(currentSelectedShape)->getObject()->Transformation();

    TopoDS_Shape topoDsShape = getNodeData(currentSelectedShape)->getTopoShape();
    topoDsShape.Location(newTransformation);

    TDF_Label referredLabel;
    myStepProcessor->shapeTool->GetReferredShape(getNodeData(currentSelectedShape)->getObject()->GetLabel(),
                                                 referredLabel);

    myStepProcessor->shapeTool->RemoveComponent(getNodeData(currentSelectedShape)->getObject()->GetLabel());
    if (!referredLabel.IsNull()) {
        myStepProcessor->shapeTool->RemoveComponent(referredLabel);
    }
    myStepProcessor->shapeTool->UpdateAssemblies();

    TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
            getNodeData(currentSelectedShape->parent())->getLabel(), topoDsShape);
    myStepProcessor->shapeTool->UpdateAssemblies();

    getNodeData(currentSelectedShape)->setLabel(newLabel);
    getNodeData(currentSelectedShape)->setTopoShape(topoDsShape);
    getNodeData(currentSelectedShape)->setLocation(topoDsShape.Location());
    getNodeData(currentSelectedShape)->setLocation(topoDsShape.Location());

    cout << "*************************\n";
    originalTransformation.DumpJson(cout);
    cout << "\n";
    newTransformation.DumpJson(cout);
    cout << "\n";

    myStepProcessor->shapeTool->UpdateAssemblies();
}

void MainWindow::slot_cut() {

    QList<QTreeWidgetItem*> list = ui->modelTreeWidget->selectedItems();

    if (list.size() == 2){

        TopoDS_Shape topoDsShape = getNodeData(list.at(0))->getTopoShape();

        TopoDS_Shape toolShape = getNodeData(list.at(1))->getTopoShape();

        BRepAlgoAPI_Cut cutMaker(topoDsShape, toolShape);

        TopoDS_Shape newShape = cutMaker.Shape();

        GProp_GProps volumeProperties;
        BRepGProp::VolumeProperties(newShape, volumeProperties);

        TDF_Label referredLabel;
        myStepProcessor->shapeTool->GetReferredShape(getNodeData(list.at(0))->getLabel(), referredLabel);

        myStepProcessor->shapeTool->RemoveComponent(getNodeData(list.at(0))->getLabel());

        if (!referredLabel.IsNull()) {
            myStepProcessor->shapeTool->RemoveComponent(referredLabel);
        }
        myStepProcessor->shapeTool->UpdateAssemblies();

        TDF_Label newLabel = myStepProcessor->shapeTool->AddComponent(
                getNodeData(list.at(0)->parent())->getLabel(), newShape);

        myStepProcessor->shapeTool->UpdateAssemblies();

        getNodeData(list.at(0))->setLabel(newLabel);
        getNodeData(list.at(0))->setTopoShape(newShape);

        myViewerWidget->getContext()->Erase(getNodeData(list.at(0))->getObject(), true);

        NodeInteractive *newNode = new NodeInteractive(newLabel, list.at(0));

        getNodeData(list.at(0))->setLocation(newShape.Location());
        getNodeData(list.at(0))->setShape(newNode);
        getNodeData(list.at(0))->setObject(newNode);

        myViewerWidget->getContext()->Display(newNode, true);
//        myViewerWidget->getContext()->Display(new AIS_Shape(toolShape), true);

        myStepProcessor->shapeTool->UpdateAssemblies();

    }
    else{
        QMessageBox::warning(this, "Warning", "Please select two nodes");
    }



}

