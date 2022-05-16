
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
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprintdialog.h>
#include <qnumeric.h>
#include <qwt_color_map.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>

#include <QLabel>
#include <QFile>
#include <QtWidgets>
#include <iostream>

#include "MainWindow.h"
#include "ui_MainWindow.h"



MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto *spectrometerPlot = new QwtSpectrogramPlot("../3d_data.txt");
    ui->horizontalLayout->addWidget(spectrometerPlot->getAxisPlot(X_AXIS));
    ui->horizontalLayout->addWidget(spectrometerPlot->getAxisPlot(Y_AXIS));
    ui->horizontalLayout->addWidget(spectrometerPlot->getAxisPlot(Z_AXIS));
}


MainWindow::~MainWindow() {
    delete ui;
}
