#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include <qwt_scale_div.h>
#include <qwt_series_data.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_interval_symbol.h>
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
#include <QLabel>
#include <QMenu>
#include <QAction>

class QwtPlotCurve;
class QwtPlotIntervalCurve;

class Plot: public QwtPlot
{
Q_OBJECT

public:
    enum Mode
    {
        Linear,
        Logarithmic
    };

    Plot( QWidget * = NULL );

public Q_SLOTS:
    void setMode( int );
    void exportPlot();

public slots:
    void popUpMenu(const QPoint& pos);


private:
    void insertCurve( const QString &title,
                      const QVector<QPointF> &, const QColor & );

    void insertErrorBars( const QString &title,
                          const QVector<QwtIntervalSample> &,
                          const QColor &color );

    QwtPlotCurve *data_plot;
    QwtPlotIntervalCurve *range_plot;
    QwtPlotLegendItem *legendItem;
    QwtScaleDiv yearScaleDiv() const;
    QwtPlotMagnifier *magnifier;
    QwtPlotRescaler *d_rescaler;
    QwtPlotIntervalCurve *d_intervalCurve;
    QwtPlotCurve *d_curve;
    double min_x;
    double min_y;
    double max_x;
    double max_y;
};

#endif

