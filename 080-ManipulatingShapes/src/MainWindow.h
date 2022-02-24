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
    inline static QTreeWidgetItem *currentSelectedShape;

private:
    Ui::MainWindow *ui;
    static void changeVisibility(QTreeWidgetItem *shape, bool setVisible);
    static void selectedShapeView(QTreeWidgetItem *arg_node);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

public slots:

    void showOnlySelectedPart(QTreeWidgetItem *shape);
    void slot_treeWidgetItemClicked(QTreeWidgetItem *arg_item);
    void slot_createBox();
    void slot_createCylinder();
    void slot_createSphere();
    void slot_viewerMouseReleased();
    void slot_merge();
    void slot_cut();
    void slot_deletePart();
    void slot_spinboxValueChanged();
    void slot_rotatePart();
    void slot_scalePart();

};


#endif //OPENCASCADEPROJECT_MAINWINDOW_H
