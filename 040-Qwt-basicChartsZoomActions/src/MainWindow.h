//
// Created by sufuk on 15.09.2021.
//

#ifndef OPENCASCADEPROJECT_MAINWINDOW_H
#define OPENCASCADEPROJECT_MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include <qwt_plot_intervalcurve.h>
#include <qwt_interval_symbol.h>
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
#include <qwt_plot_zoomer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

};


#endif //OPENCASCADEPROJECT_MAINWINDOW_H
