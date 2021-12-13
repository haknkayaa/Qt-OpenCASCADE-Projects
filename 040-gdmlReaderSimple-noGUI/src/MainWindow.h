//
// Created by hakan on 13.12.2021.
//

#ifndef GDMLREADER_MAINWINDOW_H
#define GDMLREADER_MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    inline static QTreeWidget* mainWidget;
    inline static QTreeWidgetItem *mainItem_geometry;

    inline static void expandAll();

private:
    Ui::MainWindow *ui;


};


#endif //GDMLREADER_MAINWINDOW_H
