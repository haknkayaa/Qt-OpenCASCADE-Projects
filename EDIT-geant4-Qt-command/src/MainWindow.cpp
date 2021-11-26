
#include "MainWindow.h"
#include "./ui_mainwindow.h"


#include "simulation/B4DetectorConstruction.hh"
#include "simulation/B4aActionInitialization.hh"

#ifdef G4MULTITHREADED

#include "G4MTRunManager.hh"

#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


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
        macro = fileName.toUtf8().constData();
    });


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::simulationRun() {
    mradsimLinker = new MRADSIMLinker();
    mradsimLinker->simulationRun(macro);
}





