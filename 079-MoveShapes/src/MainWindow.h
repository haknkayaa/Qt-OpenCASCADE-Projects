//
// Created by sufuk on 2.02.2022.
//

#ifndef OPENCASCADEPROJECT_MAINWINDOW_H
#define OPENCASCADEPROJECT_MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "Viewer.h"
#include "STEPProcessor.h"
#include "MRADSIMHelperFunctions.h"
class STEPProcessor;
class Viewer;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    inline static QTreeWidgetItem *mainItem_geometry;
    inline static Viewer *myViewerWidget;
    inline static QTreeWidget *projectManagerMainTreeWidget;
    inline static STEPProcessor *myStepProcessor;
    inline static QTextBrowser *consoleWidget;

private:
    Ui::MainWindow *ui;
    QTreeWidgetItem *currentSelectedShape;
    static void changeVisibility(QTreeWidgetItem *shape, bool setVisible);
    static void selectedShapeView(QTreeWidgetItem *arg_node);


public slots:
    void showOnlySelectedPart(QTreeWidgetItem *shape);
    void slot_treeWidgetItemClicked(QTreeWidgetItem *arg_item);
};


#endif //OPENCASCADEPROJECT_MAINWINDOW_H
