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
#include <qwt_plot_rescaler.h>
#include <qwt_plot_layout.h>
#include <qwt_series_data.h>
#include <qwt_point_data.h>
#include <QLabel>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMenu>
#include <QAction>
#include <qwt_series_store.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_legend.h>
#include <qwt_interval_symbol.h>
#include <qwt_symbol.h>
#include <qwt_series_data.h>
#include <qwt_text.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_renderer.h>
#include <qdatetime.h>
#include <qfiledialog.h>
#include <qimagewriter.h>


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


    data_plot = new QwtPlotCurve("data");
    data_plot->setSamples(xdata, ydata);
    data_plot->setSymbol(new QwtSymbol(QwtSymbol::Star2, Qt::NoBrush, QPen(Qt::red), QSize(8, 8)));
    data_plot->setStyle(QwtPlotCurve::NoCurve);
    data_plot->setRenderHint(QwtPlotItem::RenderAntialiased);

    range_plot = new QwtPlotIntervalCurve("range");
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
    magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
    magnifier->setMouseButton(Qt::RightButton, Qt::KeyboardModifierMask);
    ui->qwtPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
//    QList<double> interval;
//    interval.append(0.1);
 //   const QwtScaleDiv Interval =  QwtScaleDiv(0 ,10 );
 //   ui->qwtPlot->setAxisScaleDiv( QwtPlot::yLeft, yScaleDiv );
    ui->qwtPlot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine(0.1));
    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
//    const QwtScaleEngine& xStepSize = ui->qwtPlot->axisStepSize(QwtPlot::xBottom);
//    ui->qwtPlot->setAxisScaleDiv( QwtPlot::xBottom, xScaleDiv );
    ui->qwtPlot->setCanvasBackground(QColor(50, 65, 75));
    grid = new QwtPlotGrid;
//    grid->setYDiv(Interval);
//    grid->setXDiv(Interval);
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);
    grid->setPen(QPen(Qt::black, 0.5 , Qt::DashLine));
  //  QwtScaleDiv *xDiv = new QwtScaleDiv(0.0, 50);
  //  grid->setXDiv(*xDiv);
    //grid->setYDiv(*yDiv);
    grid->attach(ui->qwtPlot);
    ui->qwtPlot->setAxisTitle(0, " SUFUK");
    ui->qwtPlot->setAxisTitle(2, " GULER");
    ui->qwtPlot->setTitle("TITLE");


//    QwtLegend *customLegend = new QwtLegend(this);
//    ui->qwtPlot->insertLegend(customLegend, QwtPlot::RightLegend);
//    customLegend->setGeometry(QRect(70,30,120,100));

    legendItem = new QwtPlotLegendItem();
    legendItem->attach(ui->qwtPlot);

    legendItem->setRenderHint(QwtPlotItem::RenderAntialiased);
    legendItem->setMaxColumns(1);
    legendItem->setAlignment(Qt::AlignRight);

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
    /*const QwtInterval xIntv =
            ui->qwtPlot->axisInterval(QwtPlot::xBottom);
    const QwtInterval yIntv =
            ui->qwtPlot->axisInterval(QwtPlot::yLeft);
    min_x = xIntv.minValue();
    min_y = yIntv.minValue();
    max_x = xIntv.maxValue();
    max_y = yIntv.maxValue();
    d_rescaler = new QwtPlotRescaler( ui->qwtPlot->canvas() );
    d_rescaler->setReferenceAxis( QwtPlot::xBottom );
    d_rescaler->setRescalePolicy( QwtPlotRescaler::Fitting );*/


}
void MainWindow::popUpMenu(const QPoint &pos)
{
    //QwtMagnifier
    QMenu menu;
    QPoint globalPos = mapToGlobal(pos);
    QAction* action_firstAction = menu.addAction(
            QIcon(),
            QString("Fit all")
    );
    menu.addSeparator();
    QAction* action_secondAction = menu.addAction(
            QIcon(),
            QString("Extract as PNG")
    );
    menu.addSeparator();
    QAction* action_thirdAction = menu.addAction(
            QIcon(),
            QString("Extract as PDF")
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
            QwtPlotZoomer* zoomer = new QwtPlotZoomer( ui->qwtPlot );
            zoomer->setRubberBandPen( QColor( Qt::black ) );
            zoomer->setTrackerPen( QColor( Qt::black ) );
            zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
                                     Qt::RightButton, Qt::ControlModifier );
            zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
                                     Qt::RightButton );
/*            qDebug() << "Fit all";
            ui->qwtPlot->replot();
            QwtPlotRescaler *rescaler = new QwtPlotRescaler(ui->qwtPlot);
            rescaler->setRescalePolicy(QwtPlotRescaler::Expanding);
            rescaler->setEnabled(true);
            rescaler->rescale();
            QRectF rectOfInterest;
            QwtPlotRescaler::ExpandingDirection direction = QwtPlotRescaler::ExpandUp;

            d_rescaler->setRescalePolicy( QwtPlotRescaler::Fitting );
            const QwtInterval xIntv =d_rescaler->intervalHint( QwtPlot::xBottom );
            const QwtInterval yIntv = d_rescaler->intervalHint( QwtPlot::yLeft );

            rectOfInterest = QRectF( xIntv.minValue(), yIntv.minValue(),
                                     xIntv.width(), yIntv.width() );
            direction = QwtPlotRescaler::ExpandBoth;
            ui->qwtPlot->setRectOfInterest( rectOfInterest );

            ui->qwtPlot->setAxisScale(QwtPlot::xBottom, min_x, max_x);
            ui->qwtPlot->setAxisScale(QwtPlot::yLeft, min_y, max_y);
            d_rescaler->setEnabled( true );
            for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
                d_rescaler->setExpandingDirection( direction );
            if ( true )
                d_rescaler->rescale();
            else
            ui->qwtPlot->replot();*/
        }

        else if(selected_action == action_secondAction)
        {
            qDebug() << "Clicked Extract as PNG";
            QwtPlotRenderer *renderer = new QwtPlotRenderer();
            renderer->renderDocument(ui->qwtPlot, "test.png", QSizeF(150, 100));
        }
        else if(selected_action == action_thirdAction)
        {
            connect( action_thirdAction, SIGNAL( clicked() ), ui->qwtPlot, SLOT( exportPlot() ) );
        }
        else if(selected_action == action_otheroptions)
        {
            qDebug() << "Clicked Other Options";
        }
    }
}
void MainWindow::exportPlot()
{
#ifndef QT_NO_PRINTER
    QString fileName = "friedberg.pdf";
#else
    QString fileName = "friedberg.png";
#endif

#ifndef QT_NO_FILEDIALOG
    const QList<QByteArray> imageFormats =
            QImageWriter::supportedImageFormats();

    QStringList filter;
    filter += "PDF Documents (*.pdf)";
#ifndef QWT_NO_SVG
    filter += "SVG Documents (*.svg)";
#endif
    filter += "Postscript Documents (*.ps)";

    if ( imageFormats.size() > 0 )
    {
        QString imageFilter( "Images (" );
        for ( int i = 0; i < imageFormats.size(); i++ )
        {
            if ( i > 0 )
                imageFilter += " ";
            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }
        imageFilter += ")";

        filter += imageFilter;
    }

    fileName = QFileDialog::getSaveFileName(
            this, "Export File Name", fileName,
            filter.join( ";;" ), NULL, QFileDialog::DontConfirmOverwrite );
#endif
    if ( !fileName.isEmpty() )
    {
        QwtPlotRenderer *renderer;
        renderer->setDiscardFlag( QwtPlotRenderer::DiscardBackground, false );
        //renderer.renderDocument( this, fileName, QSizeF( 300, 200 ), 85 );
    }
}
/*
void MainWindow::setRescaleMode( int mode )
{
    bool doEnable = true;
    QString info;
    QRectF rectOfInterest;
    QwtPlotRescaler::ExpandingDirection direction = QwtPlotRescaler::ExpandUp;
    d_rescaler = new QwtPlotRescaler( ui->qwtPlot->canvas() );
    d_rescaler->setReferenceAxis( QwtPlot::xBottom );

            d_rescaler->setRescalePolicy( QwtPlotRescaler::Fitting );
            const QwtInterval xIntv =
                    d_rescaler->intervalHint( QwtPlot::xBottom );
            const QwtInterval yIntv =
                    d_rescaler->intervalHint( QwtPlot::yLeft );

            rectOfInterest = QRectF( xIntv.minValue(), yIntv.minValue(),
                                     xIntv.width(), yIntv.width() );
            direction = QwtPlotRescaler::ExpandBoth;

            info = "Fitting";

    d_rescaler->setEnabled( doEnable );
    for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
        d_rescaler->setExpandingDirection( direction );

    if ( doEnable )
        d_rescaler->rescale();
    else
        ui->qwtPlot->replot();
}*/

/*void MainWindow::shiftAxis( QwtPlot* plot, int axis, double increment )
{
    QwtInterval interval = ui->qwtPlot->axisInterval(QwtPlot::yLeft );
    ui->qwtPlot->setAxisScale( QwtPlot::yLeft, interval.minValue() + 5, interval.maxValue() + 5 );
    QwtInterval interval_1 = ui->qwtPlot->axisInterval( QwtPlot::xBottom );
    ui->qwtPlot->setAxisScale( QwtPlot::xBottom, interval_1.minValue() + 5, interval_1.maxValue() + 5 );
}

void MainWindow::shiftAxis( QwtPlot* plot, int axis, int numPixels )
{
    QwtScaleMap map = ui->qwtPlot->canvasMap( QwtPlot::yLeft );
    QwtInterval interval = ui->qwtPlot->axisInterval( QwtPlot::yLeft );
    QwtScaleMap map_1 = ui->qwtPlot->canvasMap( QwtPlot::xBottom );
    QwtInterval interval_1 = ui->qwtPlot->axisInterval( QwtPlot::xBottom );

    double min = map.invTransform( map.transform( interval.minValue() ) + 1 );
    double max = map.invTransform( map.transform( interval.maxValue() ) + 1 );
    double min_1 = map_1.invTransform( map_1.transform( interval_1.minValue() ) + 1 );
    double max_1 = map_1.invTransform( map_1.transform( interval_1.maxValue() ) + 1 );

    ui->qwtPlot->setAxisScale( QwtPlot::yLeft, min, max , 0 );
    ui->qwtPlot->setAxisScale( QwtPlot::xBottom, min_1, max_1 , 0 );
}*/

MainWindow::~MainWindow() {
    delete ui;
}
