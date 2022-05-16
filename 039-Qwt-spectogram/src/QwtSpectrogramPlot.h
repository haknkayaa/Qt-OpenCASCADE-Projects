//
// Created by hakan on 10.05.2022.
//

#ifndef PROJECT_QWTSPECTROGRAMPLOT_H
#define PROJECT_QWTSPECTROGRAMPLOT_H

// Qt Libraries
#include <QtWidgets>

// QwtLibraries
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
#include <qwt_matrix_raster_data.h>

typedef enum{
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} Axis;

class QwtSpectrogramPlot{
public:
    explicit QwtSpectrogramPlot(QString dataFilePath);

    void readDataFile(QString dataFilePath);
    QWidget* getAxisPlot(Axis axis);
    void exportImage(QwtPlot* plot, QString fileName);
private:
    QString m_dataFilePath;
    QVector<double> x_AllData, y_AllData, z_AllData, raw_allData;
    double xBinningMax, yBinningMax, zBinningMax;

    QVector<double> totalXPlane;
    QVector<double> totalYPlane;
    QVector<double> totalZPlane;

    double min_totalXPlane, max_totalXPlane;
    double min_totalYPlane, max_totalYPlane;
    double min_totalZPlane, max_totalZPlane;
};


#endif //PROJECT_QWTSPECTROGRAMPLOT_H
