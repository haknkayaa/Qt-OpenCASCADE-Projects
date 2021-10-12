
#include <src/GDML/GDMLViewer.hh>
#include "MainWindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->widget_right->setMaximumWidth(350);

    // Viewer
    myViewer = ui->stepViewer;



    connect(ui->tabWidget, &QTabWidget::currentChanged, [this]{
        ui->stackedWidget->setCurrentIndex(ui->tabWidget->currentIndex());
    });

    ui->tabWidget->setCurrentIndex(1);

    STEPProcessor *mySTEPProcessor = new STEPProcessor();
//    GDMLViewer *myGDMLViewer = new GDMLViewer();

    QObject::connect(ui->importButton, &QPushButton::clicked, [this, mySTEPProcessor] {
        QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                                      QStandardPaths::LocateDirectory);

        QString supportedFileType = "All Files (*.*) ;;"
                                    "STEP Files (*.step *.stp *.STEP *.STP) ;;"
                                    "GDML Files (*.gdml *.GDML)";

        QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                        homeLocation,
                                                        supportedFileType);

        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix() == "step" || fileInfo.suffix() == "stp") {
            qDebug() << "--> Opening STEP file: " << fileName;
            ui->tabWidget->setCurrentIndex(0);
            mySTEPProcessor->readSTEP(fileName);
        }

        else if(fileInfo.suffix() == "gdml"){
            qDebug() << "--> Opening GDML file: " << fileName;
            ui->gdmlViewer->readGDML(fileName.toUtf8().constData());
            ui->tabWidget->setCurrentIndex(1);
        }
    });
//    QObject::connect(mySTEPProcessor, &STEPProcessor::readyShape, ui->tabWidget, &Viewer::slot_showShape);

    // on/off settings for viewer
    connect(ui->check_show3DGrid, &QCheckBox::stateChanged, [this] {
        ui->stepViewer->show3DGrid(!ui->stepViewer->isEnabled3DGrid());
    });
    connect(ui->check_showTrihedronCube, &QCheckBox::stateChanged, [this] {
        ui->stepViewer->showTrihedronCube(!ui->stepViewer->isEnabledTrihedronCube());
    });
    connect(ui->check_showPerformanceStats, &QCheckBox::stateChanged, [this] {
        ui->stepViewer->showPerformanceStats(!ui->stepViewer->isEnabledPerformanceStats());
    });

    // projection mode
    connect(ui->viewMode, &QComboBox::currentTextChanged, ui->stepViewer, &Viewer::changeProjectionMode);

    // explode
    connect(ui->check_explode, &QCheckBox::stateChanged, [this] {
        ui->slider_explode->setEnabled(ui->check_explode->isChecked());
//        myViewer->slot_explode(ui->check_explode->isChecked(), ui->slider_explode->value());
    });
    connect(ui->slider_explode, &QSlider::valueChanged, [this] {
//        myViewer->slot_explode(ui->check_explode->isChecked(), ui->slider_explode->value());
    });

    // clip plane initial settings and signal/slots
    ui->button_XInvert->setEnabled(false);
    ui->slider_XPlane->setEnabled(false);

    connect(ui->check_XPlane, &QCheckBox::stateChanged, [this] {
        ui->button_XInvert->setEnabled(ui->check_XPlane->isChecked());
        ui->slider_XPlane->setEnabled(ui->check_XPlane->isChecked());
    });

    ui->button_YInvert->setEnabled(false);
    ui->slider_YPlane->setEnabled(false);

    connect(ui->check_YPlane, &QCheckBox::stateChanged, [this] {
        ui->button_YInvert->setEnabled(ui->check_YPlane->isChecked());
        ui->slider_YPlane->setEnabled(ui->check_YPlane->isChecked());
    });

    ui->button_ZInvert->setEnabled(false);
    ui->slider_ZPlane->setEnabled(false);

    connect(ui->check_ZPlane, &QCheckBox::stateChanged, [this] {
        ui->button_ZInvert->setEnabled(ui->check_ZPlane->isChecked());
        ui->slider_ZPlane->setEnabled(ui->check_ZPlane->isChecked());
    });

    // view projection buttons
    connect(ui->xpos_button, &QPushButton::clicked, ui->stepViewer, [this] { ui->stepViewer->slot_changeProjectionAxis(1); });
    connect(ui->xneg_button, &QPushButton::clicked, ui->stepViewer, [this] { ui->stepViewer->slot_changeProjectionAxis(2); });
    connect(ui->ypos_button, &QPushButton::clicked, ui->stepViewer, [this] { ui->stepViewer->slot_changeProjectionAxis(3); });
    connect(ui->yneg_button, &QPushButton::clicked, ui->stepViewer, [this] { ui->stepViewer->slot_changeProjectionAxis(4); });
    connect(ui->zpos_button, &QPushButton::clicked, ui->stepViewer, [this] { ui->stepViewer->slot_changeProjectionAxis(5); });
    connect(ui->zneg_button, &QPushButton::clicked, ui->stepViewer, [this] { ui->stepViewer->slot_changeProjectionAxis(6); });

    // actual 3d position [inline defined]
    connect(ui->stepViewer, &Viewer::mousePosChanged, [this](const QPoint currPos) {
        gp_Pnt pos3d = ui->stepViewer->getCursor3DPosition(currPos);

        ui->label_xPos->setText(QString::number(pos3d.X(), 'f', 3));
        ui->label_yPos->setText(QString::number(pos3d.Y(), 'f', 3));
        ui->label_zPos->setText(QString::number(pos3d.Z(), 'f', 3));
    });


    connect(ui->importGDMLButton, &QPushButton::clicked, [this]{
        QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                                      QStandardPaths::LocateDirectory);

        QString supportedFileType = "All Files (*.*) ;;"
                                    "STEP Files (*.step *.stp *.STEP *.STP) ;;"
                                    "GDML Files (*.gdml *.GDML)";

        QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                        homeLocation,
                                                        supportedFileType);

        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix() == "step" || fileInfo.suffix() == "stp") {
            qDebug() << "--> Opening STEP file: " << fileName;
            ui->tabWidget->setCurrentIndex(0);
//            mySTEPProcessor->readSTEP(fileName);
        }

        else if(fileInfo.suffix() == "gdml"){
            qDebug() << "--> Opening GDML file: " << fileName;
            ui->gdmlViewer->readGDML(fileName.toUtf8().constData());
            ui->tabWidget->setCurrentIndex(1);
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}



