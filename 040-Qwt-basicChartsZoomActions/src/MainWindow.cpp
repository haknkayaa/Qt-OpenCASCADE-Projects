//
// Created by sufuk on 15.09.2021.
//



#include <QLabel>
#include <iostream>
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
    for (int i = 0; i < (int) yerrors.size(); i++) {
        range[i] = QwtIntervalSample(xdata.at(i), ydata.at(i) - yerrors.at(i), ydata.at(i) + yerrors.at(i));
    }
    range_plot->setSamples(range);

    QwtIntervalSymbol *errorbar = new QwtIntervalSymbol(QwtIntervalSymbol::Bar);
    errorbar->setPen(QPen(Qt::black, 1));
    range_plot->setSymbol(errorbar);
    range_plot->setStyle(QwtPlotIntervalCurve::NoCurve);

    data_plot->attach(ui->qwtPlot);
    range_plot->attach(ui->qwtPlot);

    // mouse rectangle ile zoom
    // QwtPlotZoomer *zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );
    // zoomer->setZoomBase()

    // mouse panner
    QwtPlotPanner *panner = new QwtPlotPanner(ui->qwtPlot->canvas());

    // mouse scroll zoomer
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
//    magnifier->setWheelModifiers(Qt::ShiftModifier);
    magnifier->setAxisEnabled(QwtPlot::xBottom, true);
    magnifier->setAxisEnabled(QwtPlot::yLeft,false);

//    QwtPlotMagnifier *zoom_x = new QwtPlotMagnifier( ui->qwtPlot->canvas());
//    zoom_x->setWheelModifiers(Qt::ShiftModifier);
//    zoom_x->setAxisEnabled(QwtPlot::xBottom, true);
//    zoom_x->setAxisEnabled(QwtPlot::yLeft,false);

    QwtPlotMagnifier *zoom_y = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    zoom_y->setWheelModifiers(Qt::ControlModifier);
    zoom_y->setAxisEnabled(Qt::XAxis,true);
    zoom_y->setAxisEnabled(Qt::YAxis,true);

    ui->qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 1, 100);
    ui->qwtPlot->replot();
    ui->qwtPlot->setCanvasBackground(QColor(50,65,75));
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(true);
    grid->enableY(true);
    grid->attach(ui->qwtPlot);
    ui->qwtPlot->setAxisTitle(0, " SUFUK");
    ui->qwtPlot->setAxisTitle(2, " GULER");
    ui->qwtPlot->setTitle("TITLE");


//    QwtLegend *customLegend = new QwtLegend(this);
//    ui->qwtPlot->insertLegend(customLegend, QwtPlot::RightLegend);
//    customLegend->setGeometry(QRect(70,30,120,100));

    QwtPlotLegendItem *legendItem = new QwtPlotLegendItem();
    legendItem->attach(ui->qwtPlot);

    legendItem->setRenderHint(QwtPlotItem::RenderAntialiased);
    legendItem->setMaxColumns(1);
//    legendItem->setAlignment(Qt::AlignRight);

    QColor textColor(Qt::white);
    legendItem->setTextPen(textColor);
    legendItem->setBorderPen(textColor);

    QColor backgroundColor(Qt::gray);
    backgroundColor.setAlpha(200);
    legendItem->setBackgroundBrush(backgroundColor);

    QwtLegendData data;
    data.setValue(QwtLegendData::Role::TitleRole, QString("<table>   <tr> <th>Result </th>  <th> </th>  <th> </th> </tr>   "
                                                          "<tr> <td>Total Entries</td> <td>=</td> <td> N/A </td> </tr>   "
                                                          "<tr> <td>Mean</td> <td>=</td> <td> N/A </td> </tr>  "
                                                          "<tr> <td>Error</td> <td>=</td>     <td> N/A </td> </tr> "
                                                          "<tr> <td>Total Ionasing Dose</td>  <td>=</td> <td> N/A </td> </tr> </table>"));
    QList<QwtLegendData> list;
    list << data;
    legendItem->updateLegend(legendItem, list);

    // save to image
    QwtPlotRenderer *renderer = new QwtPlotRenderer();
    renderer->renderDocument(ui->qwtPlot, "test.png", QSizeF(150, 100));

}


MainWindow::~MainWindow() {
    delete ui;
}

