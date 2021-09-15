//
// Created by sufuk on 15.09.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QVector<double> xdata, ydata, yerrors;
    xdata << 5.00;
    xdata << 8.00;
    xdata << 11.00;
    xdata << 14.00;
    xdata << 17.00;


    ydata << 5;
    ydata << 9;
    ydata << 5;
    ydata << 5;
    ydata << 5;

    yerrors << 1;
    yerrors << 2;
    yerrors << 1;
    yerrors << 2;
    yerrors << 2;

    QwtPlotCurve *data_plot = new QwtPlotCurve("data");
    data_plot->setSamples(xdata, ydata);
    data_plot->setSymbol(new QwtSymbol(QwtSymbol::Star2, Qt::NoBrush, QPen(Qt::red), QSize(8, 8)));
    data_plot->setStyle(QwtPlotCurve::NoCurve);
    data_plot->setRenderHint(QwtPlotItem::RenderAntialiased);

    QwtPlotIntervalCurve *range_plot = new QwtPlotIntervalCurve("range");
    QVector<QwtIntervalSample> range(xdata.size());
    for(int i = 0; i < (int)yerrors.size(); i++) {
        range[i] = QwtIntervalSample(xdata.at(i), ydata.at(i) - yerrors.at(i), ydata.at(i) + yerrors.at(i));
    }
    range_plot->setSamples(range);

    QwtIntervalSymbol *errorbar = new QwtIntervalSymbol(QwtIntervalSymbol::Bar);
    errorbar->setPen(QPen(Qt::black, 1));
    range_plot->setSymbol(errorbar);
    range_plot->setStyle(QwtPlotIntervalCurve::NoCurve);

    data_plot->attach(ui->qwtPlot);
    range_plot->attach(ui->qwtPlot);
}

MainWindow::~MainWindow() {
    delete ui;
}

