//
// Created by sufuk on 15.09.2021.
//

/// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_legenditem.h>

#include <QLabel>
#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_wantToSleep = true;
    count = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCount()));
    timer->start(100);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateCount()
{
    ++count;
//    qDebug() << objectName() << " count: " << count;

    QString path = "/home/hakan/Desktop/progress/"; // assume it is some path
    QDir dir( path );
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );

    int total_files = dir.count();
    qDebug() << objectName() << " File count: " << total_files;
    ui->progressBar->setValue(total_files/10);

    if (m_wantToSleep)
        qDebug() << "Sleep.";
}
