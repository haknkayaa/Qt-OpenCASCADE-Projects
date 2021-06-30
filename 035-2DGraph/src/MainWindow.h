//
// Created by hakan on 29.06.2021.
//

#ifndef PROJECT_MAINWINDOW_H
#define PROJECT_MAINWINDOW_H

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
    void openDialog();
    void setupPlot();
    void fileParser();

private:
    Ui::MainWindow *ui;
};


#endif //PROJECT_MAINWINDOW_H
