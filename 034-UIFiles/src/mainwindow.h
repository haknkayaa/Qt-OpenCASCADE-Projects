#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "Viewer.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    inline static Viewer *myViewerWidget;
    QTreeWidget *modelTreeWidget;
    static QTextBrowser *text;

public slots:
    void slot_test();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
