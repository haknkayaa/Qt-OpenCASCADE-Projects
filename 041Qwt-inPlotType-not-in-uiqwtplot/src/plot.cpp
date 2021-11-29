#include "plot.h"
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
//#include <qprintdialog.h>
#include <qfileinfo.h>
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
#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_curve_fitter.h>
#include <qwt_symbol.h>

class Grid: public QwtPlotGrid
{
public:
    Grid()
    {
        enableXMin( true );
        setMajorPen( QPen( Qt::white, 0, Qt::DotLine ) );
        setMinorPen( QPen( Qt::gray, 0 , Qt::DotLine ) );
    }

    virtual void updateScaleDiv( const QwtScaleDiv &xMap,
                                 const QwtScaleDiv &yMap )
    {
        QList<double> ticks[QwtScaleDiv::NTickTypes];

        ticks[QwtScaleDiv::MajorTick] =
                xMap.ticks( QwtScaleDiv::MajorTick );
        ticks[QwtScaleDiv::MinorTick] =
                xMap.ticks( QwtScaleDiv::MinorTick );
        ticks[QwtScaleDiv::MediumTick] =
                xMap.ticks( QwtScaleDiv::MediumTick );


        QwtPlotGrid::updateScaleDiv(
                QwtScaleDiv( xMap.lowerBound(), xMap.upperBound(), ticks ),
                yMap );
    }
};


Plot::Plot( QWidget *parent ):
        QwtPlot( parent )
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &  )),
            this,SLOT(popUpMenu(const QPoint &)));

    setObjectName( "QwtPlot" );
    setTitle( "TITLE" );

    setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
    setAxisAutoScale(QwtPlot::yLeft, true);
    setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine(1));
    setAxisAutoScale(QwtPlot::xBottom, true);
    setCanvasBackground(QColor(50, 65, 75));
    setAxisTitle(0, " ALPER");
    setAxisTitle(2, " DOGAN");

    // grid
    QwtPlotGrid *grid = new Grid;
    grid->attach( this );
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(true);


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

    //range_plot = new QwtPlotIntervalCurve("range");
    QVector<QwtIntervalSample> rangeData(xdata.size());
    for (int i = 0; i < (int) yerrors.size(); i++) {
        rangeData[i] = QwtIntervalSample(xdata.at(i), ydata.at(i) - yerrors.at(i), ydata.at(i) + yerrors.at(i));
    }

    insertErrorBars( "Range", rangeData, Qt::black );

/*    range_plot->setSamples(rangeData);
    QwtIntervalSymbol *errorbar = new QwtIntervalSymbol(QwtIntervalSymbol::Bar);
    errorbar->setPen(QPen(Qt::white, 1));
    range_plot->setSymbol(errorbar);
    range_plot->setStyle(QwtPlotIntervalCurve::NoCurve);*/

    data_plot->attach(this);
    //range_plot->attach(this);

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    //QwtPlotZoomer* zoomer = new QwtPlotZoomer( canvas() );
    //zoomer->setRubberBandPen( QColor( Qt::black ) );
    //zoomer->setTrackerPen( QColor( Qt::black ) );
    //zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
    //                         Qt::RightButton, Qt::ControlModifier );
    //zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
    //                         Qt::RightButton );

    QwtPlotPanner *panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::LeftButton );

    magnifier = new QwtPlotMagnifier(this->canvas());
    magnifier->setMouseButton(Qt::RightButton, Qt::KeyboardModifierMask);
    this->setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
    this->setAxisAutoScale(QwtPlot::yLeft, true);

    canvas()->setPalette( Qt::darkGray );
    //canvas()->setBorderRadius( 10 );

    legendItem = new QwtPlotLegendItem();
    legendItem->attach(this);

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

    d_intervalCurve->setStyle( QwtPlotIntervalCurve::NoCurve );

    QColor c( d_intervalCurve->brush().color().rgb() ); // skip alpha

    QwtIntervalSymbol *errorBar =
            new QwtIntervalSymbol( QwtIntervalSymbol::Bar );
    errorBar->setWidth( 8 ); // should be something even
    errorBar->setPen( c );

    d_intervalCurve->setSymbol( errorBar );
    d_intervalCurve->setRenderHint( QwtPlotItem::RenderAntialiased, false );

    this->setAxisMaxMinor(xBottom, 0);

}

void Plot::insertCurve( const QString& title,
                        const QVector<QPointF>& samples, const QColor &color )
{
    d_curve = new QwtPlotCurve( title );
    d_curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve->setStyle( QwtPlotCurve::NoCurve );
    d_curve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol );

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::XCross );
    symbol->setSize( 4 );
    symbol->setPen( QPen( color ) );
    d_curve->setSymbol( symbol );

    d_curve->setSamples( samples );
    d_curve->attach( this );
}

void Plot::insertErrorBars(
        const QString &title,
        const QVector<QwtIntervalSample>& samples,
        const QColor &color )
{
    d_intervalCurve = new QwtPlotIntervalCurve( title );
    d_intervalCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_intervalCurve->setPen( QPen( Qt::white ) );

    QColor bg( color );
    bg.setAlpha( 150 );
    d_intervalCurve->setBrush( QBrush( bg ) );
    d_intervalCurve->setStyle( QwtPlotIntervalCurve::Tube );

    d_intervalCurve->setSamples( samples );
    d_intervalCurve->attach( this );
}

void Plot::setMode( int style )
{
    if ( style == Logarithmic )
    {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
        setAxisAutoScale(QwtPlot::yLeft, true);
        setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine(1));
        setAxisAutoScale(QwtPlot::xBottom, true);
    }
    else
    {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine(1));
        setAxisAutoScale(QwtPlot::yLeft, true);
        setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine(1));
        setAxisAutoScale(QwtPlot::xBottom, true);
    }

    replot();
}

void Plot::popUpMenu(const QPoint &pos)
{
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

    menu.popup(globalPos);
    menu.exec(globalPos);
    QAction* selected_action = menu.exec(globalPos);
    if(selected_action) {
        if(selected_action == action_firstAction)
        {
            qDebug() << "Clicked Fit All";

            QwtPlotZoomer* zoomer = new QwtPlotZoomer( canvas() );
            //zoomer->setZoomStack(zoomer->zoomStack(), 0);
            setAxisAutoScale(yLeft);
            setAxisAutoScale(xBottom);
            zoomer->setZoomBase();
            zoomer->setEnabled(false);
        }

        else if(selected_action == action_secondAction)
        {
            qDebug() << "Clicked Extract as PNG";
            QwtPlotRenderer *renderer = new QwtPlotRenderer();
            renderer->renderDocument(this, "test.png", QSizeF(150, 100));
        }
        else if(selected_action == action_thirdAction)
        {
            this->exportPlot();
        }
    }
}

void Plot::exportPlot()
{
#ifndef QT_NO_PRINTER
    QString fileName = "deneme.pdf";
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
        QwtPlotRenderer renderer;
        renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground, false );

        renderer.renderDocument( this, fileName, QSizeF( 300, 200 ), 85 );
    }
}
