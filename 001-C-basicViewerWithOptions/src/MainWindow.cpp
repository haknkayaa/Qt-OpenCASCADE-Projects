#include "MainWindow.h"
#include <QtWidgets>

#include <V3d_View.hxx>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto myViewer = new Viewer(this);
    ui->viewerlayout->addWidget(myViewer);

    ui->widget->setMaximumWidth(200);

    connect(ui->check_show3DGrid, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_show3DGrid(int)));
    connect(ui->check_showTrihedronCube, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_showTrihedronCube(int)));
    connect(ui->check_showPerformanceStats, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_showPerformanceStats(int)));

    // actual 3d position [inline defined]
    connect(myViewer, &Viewer::mousePosChanged, [this, myViewer](const QPoint currPos){

        gp_Pnt pos3d = myViewer->getCursor3DPosition(currPos);

        ui->label_xPos->setText(QString::number(pos3d.X(), 'f', 3));
        ui->label_yPos->setText(QString::number(pos3d.Y(), 'f', 3));
        ui->label_zPos->setText(QString::number(pos3d.Z(), 'f', 3));
    });


}

MainWindow::~MainWindow() {
    delete ui;
}

