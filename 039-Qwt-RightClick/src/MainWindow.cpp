//
// Created by sufuk on 15.09.2021.
//

/// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <qwt_symbol.h>
#include <qwt_plot_picker.h>
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
#include <QMenu>
#include <QAction>



MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &  )),this,SLOT(popUpMenu(const QPoint &)));
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
    errorbar->setPen(QPen(Qt::white, 1));
    range_plot->setSymbol(errorbar);
    range_plot->setStyle(QwtPlotIntervalCurve::NoCurve);

    data_plot->attach(ui->qwtPlot);
    range_plot->attach(ui->qwtPlot);
//    QwtPlotZoomer *zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );
//
//    zoomer->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
//    zoomer->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
//    zoomer->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );
//
    QwtPlotPanner *panner = new QwtPlotPanner(ui->qwtPlot->canvas());
//
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
    magnifier->setMouseButton(Qt::RightButton, Qt::KeyboardModifierMask);
    ui->qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
//    QList<double> interval;
//    interval.append(0.1);
    const QwtScaleDiv Interval =  QwtScaleDiv(0 ,10 );
//    ui->qwtPlot->setAxisScaleDiv( QwtPlot::yLeft, yScaleDiv );
    ui->qwtPlot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine(0.1));
    //ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
//    const QwtScaleEngine& xStepSize = ui->qwtPlot->axisStepSize(QwtPlot::xBottom);
//    ui->qwtPlot->setAxisScaleDiv( QwtPlot::xBottom, xScaleDiv );
    ui->qwtPlot->setCanvasBackground(QColor(50, 65, 75));
    QwtPlotGrid *grid = new QwtPlotGrid;
//    grid->setYDiv(Interval);
//    grid->setXDiv(Interval);
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
    legendItem->setAlignment(Qt::AlignTrailing);
    //legendItem->setAlignment(Qt::AlignTop);

    QColor textColor(Qt::white);
    legendItem->setTextPen(textColor);
    legendItem->setBorderPen(textColor);

    QColor backgroundColor(Qt::gray);
    backgroundColor.setAlpha(200);
    legendItem->setBackgroundBrush(backgroundColor);

    QwtLegendData data;
    data.setValue(QwtLegendData::Role::TitleRole,
                  QString("<table>   <tr> <th>Result </th>  <th> </th>  <th> </th> </tr>   "
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
void MainWindow::popUpMenu(const QPoint &pos)
{
    //QwtMagnifier
    QMenu menu;
    QPoint globalPos = mapToGlobal(pos);
    QAction* action_firstAction = menu.addAction(
            QIcon(),
            QString("Extract as PDF")
    );
    menu.addSeparator();
    QAction* action_secondAction = menu.addAction(
            QIcon(),
            QString("Extract as PNG")
    );
    menu.addSeparator();
    QAction* action_otheroptions = menu.addAction(
            QIcon(),
            QString("Other Option")
    );

    QAction* selected_action = menu.exec(globalPos);
    if(selected_action) {
        if(selected_action == action_firstAction)
        {
            qDebug() << "Clicked Extract as PDF";
        }
        else if(selected_action == action_secondAction)
        {
            qDebug() << "Clicked Extract as PNG";
        }
        else if(selected_action == action_otheroptions)
        {
            qDebug() << "Clicked Other Options";
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
