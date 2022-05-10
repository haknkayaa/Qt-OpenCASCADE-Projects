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

class QwtSpectrogramPlot : public QWidget{
Q_OBJECT
public:
    QwtSpectrogramPlot(QWidget *parent = nullptr);

};


#endif //PROJECT_QWTSPECTROGRAMPLOT_H
