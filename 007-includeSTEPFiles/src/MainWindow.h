#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// User Libraries
#include "Viewer.h"
#include "STEPProcessor.h"
#include "VariableTypes.h"


class STEPProcessor;


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);

    // global public variables
    static QTextBrowser *text;
    inline static Viewer *myViewerWidget;
    inline static QTreeWidget *modelTreeWidget;

    QLineEdit *information_partName;
    QLineEdit *information_parentName;
    QSlider *information_transparencySlider;
    QPushButton *information_colorButton;
    QComboBox *information_materialSelect;

private:
    STEPProcessor *mySTEPProcessor;

    // functions
    void initialSettings();
    void createActions();
    void createMenuBar();
    void createStatusBar();
    void createToolbars();
    void createMiddleWidget();

    QLabel *openedFolderLabel;

    // QTreeWidget Context Menu
    QAction *contextMenuAction_showAllParts;
    QAction *contextMenuAction_showOnlySelectedPart;
    QAction *contextMenuAction_setVisible;
    QAction *contextMenuAction_fitAll;



private slots:

    void importFile();
    void changeViewProjectionMode();
    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();

    void modelTreeItemClicked(QTreeWidgetItem *);
    void findItemFromUploadedObjects(QTreeWidgetItem*, vector<AssemblyNode>);

    void contextMenuForRightClick(const QPoint &pos);


    void slot_showAllParts();
    void slot_showOnlySelectedPart();
    void slot_setVisible();
    void slot_fitAll();
};


#endif //MAINWINDOW_H
