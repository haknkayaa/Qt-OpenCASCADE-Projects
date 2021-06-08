#include "MainWindow.h"
#include <QtWidgets>

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
}

MainWindow::~MainWindow() {
    delete ui;
}

