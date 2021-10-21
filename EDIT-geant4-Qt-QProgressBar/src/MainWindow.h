#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Simulator.h"

class Simulator;

// OpenCASCADE Libraries
#include <V3d_View.hxx>

// Qt Libraries
#include <QMainWindow>
#include <QtWidgets>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:
    void updateCount();

public:
    inline static QLabel* eAbs;
    inline static QLabel* eGab;
    inline static QLabel* lAbs;
    inline static QLabel* lGap;
    inline static QProgressBar* myBar;

signals:
    void progressUpdate(int value);

private:
    Ui::MainWindow *ui;

    QString fileName;

    Simulator *geantSimulator;
    QTimer *timer;
    QProgressDialog *myProgressDialog;
};

#endif // MAINWINDOW_H
