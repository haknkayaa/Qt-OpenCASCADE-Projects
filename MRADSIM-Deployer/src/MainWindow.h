//
// Created by sufuk on 4.10.2021.
//

#ifndef MRADSIM_DEPLOYER_MAINWINDOW_H
#define MRADSIM_DEPLOYER_MAINWINDOW_H

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

    inline static QTextBrowser *consoleWidget;
private:
    Ui::MainWindow *ui;
};


#endif //MRADSIM_DEPLOYER_MAINWINDOW_H
