#include "MainWindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto myViewer = new Viewer(this);
    ui->viewerlayout->addWidget(myViewer);

    ui->widget->setMaximumWidth(200);

}

MainWindow::~MainWindow() {
    delete ui;
}

