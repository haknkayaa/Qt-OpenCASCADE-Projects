
#include "MainWindow.h"
#include "./ui_mainwindow.h"


#include "Simulator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    geantSimulator = new Simulator();


    eAbs = ui->label_3;
    eGab = ui->label_2;
    lAbs = ui->label;
    lGap = ui->label_4;
    myBar = ui->progressBar;


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCount()));
    timer->start(100);

    connect(ui->runButton, &QPushButton::clicked, [this]{
        QApplication::processEvents();
        geantSimulator->run();
        QApplication::processEvents();
    });

    connect(ui->chooseFile, &QPushButton::clicked, [this]{
        fileName = QFileDialog::getOpenFileName(this,
                                                tr("Open File"), "/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/EDIT-geant4-Qt-QProgressBar", tr("All Files (*.*)"));


        geantSimulator->setMacro(fileName.toUtf8().data());
    });


}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::updateCount(){
    QString path = "/home/hakan/Desktop/progress/"; // assume it is some path
    QDir dir( path );
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );

    int total_files = dir.count();
    qDebug() << objectName() << " File count: " << total_files;
}



