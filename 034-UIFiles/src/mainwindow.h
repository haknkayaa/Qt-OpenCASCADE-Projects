#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <AIS_Shape.hxx>
#include "Viewer.h"
#include "VariableTypes.h"
#include "STEPProcessor.h"
#include "libtree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class STEPProcessor;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    STEPProcessor *myStepprocessor;
    static QTextBrowser *text;

public slots:
    void slot_test();
    void slot_addTopLevelModelTree(QTreeWidgetItem*);
    void slot_displayShape(AIS_Shape*);
    void slot_treeWidgetItemClicked(QTreeWidgetItem*);
private:
    void findSelectedItemFromUploadedObjects(QTreeWidgetItem *arg_item, TreeNode<OCCData> *arg_modelTree);
    void selectedShapeView(TreeNode<OCCData> *shapes);
    Ui::MainWindow *ui;
    TreeNode<OCCData> *currentSelectedNode;
};
#endif // MAINWINDOW_H
