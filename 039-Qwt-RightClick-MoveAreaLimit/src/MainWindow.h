//
// Created by sufuk on 15.09.2021.
//

#ifndef OPENCASCADEPROJECT_MAINWINDOW_H
#define OPENCASCADEPROJECT_MAINWINDOW_H

#include <QMainWindow>

//#include "ErrorBarPlotCurve.h"

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
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMenu>
#include <QAction>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    enum RescaleMode
    {
        KeepScales,
        Fixed,
        Expanding,
        Fitting
    };
public slots:
    void popUpMenu(const QPoint& pos);
    void exportPlot();
//    void setRescaleMode( int mode );
/*    void shiftAxis( QwtPlot* plot, int axis, double increment );
    void shiftAxis( QwtPlot* plot, int axis, int numPixels );*/
private:
    Ui::MainWindow *ui;
    QwtPlotCurve *data_plot;
    QwtPlotIntervalCurve *range_plot;
    QwtPlotMagnifier *magnifier;
    QwtPlotGrid *grid;
    QwtPlotLegendItem *legendItem;
    QwtPlotRescaler *d_rescaler;
    double min_x;
    double min_y;
    double max_x;
    double max_y;
};


#endif //OPENCASCADEPROJECT_MAINWINDOW_H
