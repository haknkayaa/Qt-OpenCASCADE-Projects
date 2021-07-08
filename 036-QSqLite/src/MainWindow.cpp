//
// Created by hakan on 8.07.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DBManager.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    materialDialog = new MaterialDialog();

    connect(ui->openMaterialDialog, &QPushButton::clicked, [this]{
        materialDialog->show();
    });

    connect(ui->resetDB, &QPushButton::clicked, [this]{

    });

    connect(ui->importDB, &QPushButton::clicked, [this]{

    });

    connect(ui->exportDB, &QPushButton::clicked, [this]{

    });

}

MainWindow::~MainWindow() {
    delete ui;
}

