//
// Created by sufuk on 15.09.2021.
//

#ifndef OPENCASCADEPROJECT_MAINWINDOW_H
#define OPENCASCADEPROJECT_MAINWINDOW_H

#include <QMainWindow>
//#include "ErrorBarPlotCurve.h"

#include <qwt_plot_intervalcurve.h>
#include <qwt_interval_symbol.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "QwtSpectrogramPlot.h"
class QwtSpectrogramPlot;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QwtPlot *plot;
};


#endif //OPENCASCADEPROJECT_MAINWINDOW_H
