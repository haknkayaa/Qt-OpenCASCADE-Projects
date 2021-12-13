//
// Created by hakan on 13.12.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    mainItem_geometry = new QTreeWidgetItem();
    mainItem_geometry->setText(0, "Geometry");

   ui->projectManagerMainTreeWidget->addTopLevelItem(mainItem_geometry);

   mainWidget = ui->projectManagerMainTreeWidget;
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::expandAll() {
   mainWidget->expandAll();
}
