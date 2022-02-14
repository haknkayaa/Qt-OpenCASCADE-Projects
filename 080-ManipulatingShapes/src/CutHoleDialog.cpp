//
// Created by sufuk on 14.02.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CutHoleDialog.h" resolved

#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include "CutHoleDialog.h"
#include "ui_CutHoleDialog.h"
#include "MRADSIMHelperFunctions.h"
#include "MainWindow.h"

CutHoleDialog::CutHoleDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::CutHoleDialog) {
    ui->setupUi(this);
    connect(ui->cutButton, &QPushButton::clicked, this, &CutHoleDialog::cutHoleClicked);
}

CutHoleDialog::~CutHoleDialog() {
    delete ui;
}

NodeInteractive *CutHoleDialog::getNodeInteractive() const {
    return nodeInteractive;
}

void CutHoleDialog::setNodeInteractive(NodeInteractive *nodeInteractive) {
    CutHoleDialog::nodeInteractive = nodeInteractive;
    getNodeData(nodeInteractive->getTreeWidgetItem());
    double xMin, xMax, yMin, yMax, zMin, zMax;

    Bnd_Box box;
    BRepBndLib::Add(getNodeData(nodeInteractive->getTreeWidgetItem())->getTopoShape(), box);
    box.Get(xMin, yMin, zMin, xMax, yMax, zMax);
    gp_Pnt pnt;
//    pnt.SetCoord(xMax - xMin, yMax - yMin, zMax - zMin);

    ui->currentShapeBoundBox_X->setValue(qAbs(xMax - xMin));
    ui->currentShapeBoundBox_Y->setValue(qAbs(yMax - yMin));
    ui->currentShapeBoundBox_Z->setValue(qAbs(zMax - zMin));

}

void CutHoleDialog::cutHoleClicked() {


    TopoDS_Shape topoDsShape = getNodeData(MainWindow::currentSelectedShape)->getTopoShape();

    BRepPrimAPI_MakeCylinder cylinderMaker(ui->radiusBox->value(), ui->heightBox->value());

    TopoDS_Shape cylinder = cylinderMaker.Shape();
    cylinder.Location(getNodeData(MainWindow::currentSelectedShape)->getObject()->Transformation());

    BRepAlgoAPI_Cut cutMaker(topoDsShape, cylinder);

    TopoDS_Shape newShape = cutMaker.Shape();

    GProp_GProps volumeProperties;
    BRepGProp::VolumeProperties(newShape, volumeProperties);

    TDF_Label referredLabel;
    MainWindow::myStepProcessor->shapeTool->GetReferredShape(getNodeData(MainWindow::currentSelectedShape)->getLabel(), referredLabel);
    MainWindow::myStepProcessor->shapeTool->RemoveComponent(getNodeData(MainWindow::currentSelectedShape)->getLabel());
    if (!referredLabel.IsNull()) {
        MainWindow::myStepProcessor->shapeTool->RemoveComponent(referredLabel);
    }
    MainWindow::myStepProcessor->shapeTool->UpdateAssemblies();

    TDF_Label newLabel = MainWindow::myStepProcessor->shapeTool->AddComponent(
            getNodeData(MainWindow::currentSelectedShape->parent())->getLabel(), newShape);
    MainWindow::myStepProcessor->shapeTool->UpdateAssemblies();


    getNodeData(MainWindow::currentSelectedShape)->setLabel(newLabel);
    getNodeData(MainWindow::currentSelectedShape)->setTopoShape(newShape);
//

    MainWindow::myViewerWidget->getContext()->Erase(nodeInteractive, true);

    NodeInteractive *newNode = new NodeInteractive(newLabel, MainWindow::currentSelectedShape);

    getNodeData(MainWindow::currentSelectedShape)->setLocation(newShape.Location());
    getNodeData(MainWindow::currentSelectedShape)->setShape(newNode);
    getNodeData(MainWindow::currentSelectedShape)->setObject(newNode);



    MainWindow::myViewerWidget->getContext()->Display(newNode, true);
    MainWindow::myViewerWidget->getContext()->Display(new AIS_Shape(cylinder), true);

    MainWindow::myStepProcessor->shapeTool->UpdateAssemblies();

//    gp_Pnt lowerLeftCornerOfBox(-50.0,-50.0,0.0);
//    BRepPrimAPI_MakeBox boxMaker(lowerLeftCornerOfBox,100,100,50);
//    TopoDS_Shape box = boxMaker.Shape();
//    //Create a cylinder with a radius 25.0 and height 50.0, centered at the origin
//    BRepPrimAPI_MakeCylinder cylinderMaker(25.0,50.0);
//    TopoDS_Shape cylinder = cylinderMaker.Shape();
//
//    //Cut the cylinder out from the box
//    BRepAlgoAPI_Cut cutMaker(box,cylinder);
//    TopoDS_Shape boxWithHole = cutMaker.Shape();
//
//    GProp_GProps volumeProperties;
//    BRepGProp::VolumeProperties(boxWithHole,volumeProperties);
//
//    AIS_Shape *shape = new AIS_Shape(boxWithHole);
//    MainWindow::myViewerWidget->getContext()->Display(shape, true);
}

