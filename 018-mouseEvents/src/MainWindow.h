#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// User Libraries
#include "Viewer.h"
#include "STEPProcessor.h"
#include "VariableTypes.h"
#include "QDialog"

class STEPProcessor;

enum Mouse_Mode{
    MOUSE_SELECT_FULLBODY,
    MOUSE_SELECT_FACE,
    MOUSE_SELECT_EDGE,
    MOUSE_SELECT_VERTEX,
    MOUSE_MEASURE,
    MOUSE_MOVE,
    MOUSE_PAN,
};

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

    Mouse_Mode myMouseMode;

private:
    STEPProcessor *mySTEPProcessor{};

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
    QAction *contextMenuAction_moveTo{};

    AssemblyNode currentSelectedShape;

    //Kullanıcının oluşturduğu şekil sayısını tutar (İsim verebilmek için).
    //[0] => CUBE || [1] => CYLINDER || [2] => SPHERE
    int numberOfShapes[3] = {1, 1, 1};

    //Shapes vektörü Shape struct'ından oluşmuştur(VariableTypes'da tanımlı).
    //Sahnede oluşturulan şekilleri tutar (merge, move işlemleri için)
    //TODO: modelTree ye taşınması gerek!
    std::vector <Shape> shapes;
private slots:
    // menu action
    void importFile();

    void changeViewProjectionMode();

    // toolbar action
    void createCube();
    void createCylinder();
    void createSphere();
    void clearScene();
    void moveTo();
    void merge();
    void measureDistance();
    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();

    void chooseFullBody();
    void chooseEdge();
    void chooseFace();
    void chooseVertex();

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
