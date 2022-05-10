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
#include <QFile>
#include <QtWidgets>
#include <iostream>
#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QString dataFileName = "3d_data.txt";

    double x[50] = {0}, y[50] = {0}, z[50] = {0};
    QVector<double> allX, allY, allZ, allData;
    QVector<double> xData, yData, zData;

    QFile file(dataFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains("#"))
            continue;

        QStringList list = line.split(",");
        allX.append(list.at(0).toDouble());
        allY.append(list.at(1).toDouble());
        allZ.append(list.at(2).toDouble());
        allData.append(list.at(3).toDouble());

        x[list.at(0).toInt()] += list[3].toDouble();
        y[list.at(1).toInt()] += list[3].toDouble();
        z[list.at(2).toInt()] += list[3].toDouble();
    }
    file.close();

    double dataMax = *std::max_element(allData.begin(), allData.end());
    double dataMin = *std::min_element(allData.begin(), allData.end());
    double xBinningMax = *std::max_element(allX.begin(), allX.end());
    double yBinningMax = *std::max_element(allY.begin(), allY.end());
    double zBinningMax = *std::max_element(allZ.begin(), allZ.end());

    for(int i = 0; i < 50; i++) {
        xData.append(x[i]);
    }

    double xMin = *std::min_element(xData.begin(), xData.end());
    double xMax = *std::max_element(xData.begin(), xData.end());

    for(int i = 0; i < 50; i++) {
        yData.append(y[i]);
    }

    double yMin = *std::min_element(yData.begin(), yData.end());
    double yMax = *std::max_element(yData.begin(), yData.end());

    for(int i = 0; i < 50; i++) {
        zData.append(z[i]);
    }

    double zMin = *std::min_element(zData.begin(), zData.end());
    double zMax = *std::max_element(zData.begin(), zData.end());


    std::cout << "xMin: " << xMin << " xMax: " << xMax << std::endl;
    std::cout << "yMin: " << yMin << " yMax: " << yMax << std::endl;
    std::cout << "zMin: " << zMin << " zMax: " << zMax << std::endl;

    for (int i = 0; i < 50; i++) {
        qDebug() << x[i] << " " << y[i] << " " << z[i];
    }


//    QwtPlot *plot = new QwtPlot(this);
    plot = new QwtPlot(this);
    plot->installEventFilter(this);

    QwtPlotCurve *data_plot = new QwtPlotCurve("data");
    data_plot->setPen(Qt::blue, 1);

    QVector<double> counter;
    for(int i = 0; i < 50; i++) {
        counter.append(i);
    }

    data_plot->setSamples(counter, QVector<double>(std::begin(y), std::end(y)));
    plot->setAxisScale(QwtPlot::yLeft, yMin, yMax, (yMax - yMin) / 10);
    plot->setAxisScale(QwtPlot::xBottom, 0,50,5);
    data_plot->setStyle(QwtPlotCurve::Steps);
    data_plot->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    data_plot->attach(plot);

    QwtPlotPanner *panner = new QwtPlotPanner(plot->canvas());
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(plot->canvas());

    plot->setAxisTitle(0, " Value (Gy)");
    plot->setAxisTitle(2, " Binning Size ");
    plot->setTitle("3D Score");
    plot->replot();

    ui->horizontalLayout->addWidget(plot);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == plot && event->type() == QMouseEvent::Wheel) {
        qDebug() << "Resize";
        plot->setAxisScale(QwtPlot::yLeft, 4.85767e-05, 0.00953064, (0.00953064 - 4.85767e-05) / 10);
        plot->replot();
    }
    return QMainWindow::eventFilter(watched, event);
}

MainWindow::~MainWindow() {
    delete ui;
}
