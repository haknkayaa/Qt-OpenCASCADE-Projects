#include "MainWindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    Viewer *myViewer = new Viewer(this);
    ui->viewerlayout->addWidget(myViewer);

    // on/off settings for viewer
    connect(ui->check_show3DGrid, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_show3DGrid(int)));
    connect(ui->check_showTrihedronCube, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_showTrihedronCube(int)));
    connect(ui->check_showPerformanceStats, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_showPerformanceStats(int)));

    // actual 3d position [inline defined]
    connect(myViewer, &Viewer::mousePosChanged, [this, myViewer](const QPoint currPos) {
        gp_Pnt pos3d = myViewer->getCursor3DPosition(currPos);

        ui->label_xPos->setText(QString::number(pos3d.X(), 'f', 3));
        ui->label_yPos->setText(QString::number(pos3d.Y(), 'f', 3));
        ui->label_zPos->setText(QString::number(pos3d.Z(), 'f', 3));
    });

    // view projection
    connect(ui->xpos_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(1); });
    connect(ui->xneg_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(2); });
    connect(ui->ypos_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(3); });
    connect(ui->yneg_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(4); });
    connect(ui->zpos_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(5); });
    connect(ui->zneg_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(6); });

    // clip plane initial settings and signal/slots
    ui->button_XInvert->setEnabled(false);
    ui->slider_XPlane->setEnabled(false);
    QObject::connect(ui->check_XPlane, &QCheckBox::stateChanged, [this] {
        ui->button_XInvert->setEnabled(ui->check_XPlane->isChecked());
        ui->slider_XPlane->setEnabled(ui->check_XPlane->isChecked());
    });

    ui->button_YInvert->setEnabled(false);
    ui->slider_YPlane->setEnabled(false);
    QObject::connect(ui->check_YPlane, &QCheckBox::stateChanged, [this] {
        ui->button_YInvert->setEnabled(ui->check_YPlane->isChecked());
        ui->slider_YPlane->setEnabled(ui->check_YPlane->isChecked());
    });

    ui->button_ZInvert->setEnabled(false);
    ui->slider_ZPlane->setEnabled(false);
    QObject::connect(ui->check_ZPlane, &QCheckBox::stateChanged, [this] {
        ui->button_ZInvert->setEnabled(ui->check_ZPlane->isChecked());
        ui->slider_ZPlane->setEnabled(ui->check_ZPlane->isChecked());
    });

    // STEP reader
    STEPProcessor *mySTEPProcessor = new STEPProcessor();
    QObject::connect(ui->importButton, &QPushButton::clicked, mySTEPProcessor, &STEPProcessor::importFile);
    QObject::connect(mySTEPProcessor, &STEPProcessor::readyShape, myViewer, &Viewer::slot_showShape);

}

MainWindow::~MainWindow() {
    delete ui;
}



