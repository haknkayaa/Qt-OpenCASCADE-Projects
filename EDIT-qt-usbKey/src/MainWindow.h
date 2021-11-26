#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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

    void simulationRun();



public:
    inline static QLabel* eAbs;
    inline static QLabel* eGab;
    inline static QLabel* lAbs;
    inline static QLabel* lGap;
    inline static QProgressBar* myBar;


private:
    Ui::MainWindow *ui;

    QString fileName;
};

#endif // MAINWINDOW_H
