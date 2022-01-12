//
// Created by hakan on 13.12.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GDMLReader.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    mainItem_geometry = new QTreeWidgetItem();
    mainItem_geometry->setText(0, "Geometry");

    ui->projectManagerMainTreeWidget->addTopLevelItem(mainItem_geometry);

    mainWidget = ui->projectManagerMainTreeWidget;

    GDMLReader *reader = new GDMLReader();
//    reader->readFile("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/Example - GDML Files/cube.gdml");
    reader->readFile("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/Example - GDML Files/sabanci5_1.gdml");
//    reader->readFile("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/Example - GDML Files/ctechV05.gdml");




    connect(mainWidget, &QTreeWidget::currentItemChanged, [this, reader] {

        qDebug() << "Current item changed: " << mainWidget->currentItem()->text(0);

        if (mainWidget->currentItem()->text(1) == "GDMLItem-Volume") {
            ui->l_worldname->setText(reader->worldTag.ref);

            for(const auto& it: reader->worldTag.childShapes){
                if(it.physvol == mainWidget->currentItem()->text(0)){
                    ui->l_solidName->setText(it.solidref);
                    ui->l_physname->setText(it.physvol);
                    ui->l_volumename->setText(it.volumeref);
                }
            }
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::expandAll() {
    mainWidget->expandAll();
}
