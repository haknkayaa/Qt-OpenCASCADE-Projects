
#include "MainWindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

//    ui->label   = lAbs;
//    ui->label_2 = eGab;
//    ui->label_3 = eAbs;
//    ui->label_4 = lGap;
    eAbs = ui->label_3;
    eGab = ui->label_2;
    lAbs = ui->label;
    lGap = ui->label_4;
    myBar = ui->progressBar;

    connect(ui->runButton, &QPushButton::clicked, [this]{
        this->simulationRun();
    });

    connect(ui->chooseFile, &QPushButton::clicked, [this]{
        fileName = QFileDialog::getOpenFileName(this,
                                                tr("Open File"), qApp->applicationDirPath(), tr("All Files (*.*)"));
    });


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::simulationRun() {


}





