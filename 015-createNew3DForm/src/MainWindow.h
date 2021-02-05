#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// User Libraries
#include "Viewer.h"
#include "STEPProcessor.h"
#include "VariableTypes.h"
#include "InputDialog.h"
#include "QDialog"

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

    QLineEdit *information_partName{};
    QLineEdit *information_parentName{};
    QLineEdit *information_index;
    QSlider *information_transparencySlider{};
    QPushButton *information_colorButton{};
    QComboBox *information_materialSelect{};

private:
    STEPProcessor *mySTEPProcessor{};

    QList<QLineEdit*> fields;
    // functions
    void initialSettings();
    void createActions();
    void createMenuBar(); // This function Creates Menu Bar
    void createStatusBar();
    void createToolbars();
    void createMiddleWidget();

    QLabel *openedFolderLabel{};

    // QTreeWidget Context Menu
    QAction *contextMenuAction_showAllParts{};
    QAction *contextMenuAction_showOnlySelectedPart{};
    QAction *contextMenuAction_setVisible{};
    QAction *contextMenuAction_fitAll{};

    AssemblyNode currentSelectedShape;

private slots:
    // menu action
    void importFile();

    void changeViewProjectionMode();

    // toolbar action
    void cube();
    void cylinder();
    void sphere();
    void clear();
    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();
    void slot_informationColorDialog();
    void slot_informationTransparenctValueChanged();

    void modelTreeItemClicked(QTreeWidgetItem *);
    void currentItemChanged();
    void findSelectedItemFromUploadedObjects(QTreeWidgetItem*, vector<AssemblyNode>);
    void findUpdatedItemFromUploadedObjects(AssemblyNode arg_currentNode, vector<AssemblyNode> arg_modelTree);
    void updateCurrentSelectedItem(AssemblyNode);

    void contextMenuForRightClick(const QPoint &pos);

    void slot_showAllParts();
    void slot_showOnlySelectedPart();
    void slot_setVisible();
    void slot_fitAll();
};


#endif //MAINWINDOW_H
