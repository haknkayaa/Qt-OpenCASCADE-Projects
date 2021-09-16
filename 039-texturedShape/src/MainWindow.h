#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// User Libraries
#include "Viewer.h"
#include "STEPProcessor.h"


// OpenCASCADE Libraries
#include <V3d_View.hxx>

// Qt Libraries
#include <QMainWindow>
#include <QtWidgets>

class Viewer;
class STEPProcessor;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    inline static STEPProcessor *mySTEPProcessor;
    inline static Viewer *myViewer;

public slots:


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
