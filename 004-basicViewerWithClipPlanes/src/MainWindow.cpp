#include "MainWindow.h"
#include "Viewer.h"

// OpenCASCADE
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <AIS_Shape.hxx>

// Kurucu fonksiyon
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OpenCASCADE Window");
    resize(700, 500);

    createMenuBar();
    createStatusBar();
    createToolbars();


    createMiddleWidget();
}

// Middle Widget
void MainWindow::createMiddleWidget() {
    QHBoxLayout *mainLayout = new QHBoxLayout();

    myViewerWidget = new Viewer(this);

    mainLayout->addWidget(myViewerWidget);

    // Make Box
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();
    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);

    anAisBox->SetColor(Quantity_NOC_AZURE);

    myViewerWidget->getContext()->Display(anAisBox, Standard_True);
    myViewerWidget->fitAll();
    // Make Box


    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

// Menü yaratan fonksiyon
void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);

    // File Menü
    QMenu *subMenu = new QMenu("File", this);
    menuBar->addMenu(subMenu);

    // File submenus
    // Import
    QAction *importFileAction = new QAction("Import", this);
    connect(importFileAction, &QAction::triggered, this, &MainWindow::importFile);
    importFileAction->setStatusTip("Yeni bir dosya ekle");
    subMenu->addAction(importFileAction);

    // Edit Menü
    QMenu *editMenu = new QMenu("Edit", this);
    menuBar->addMenu(editMenu);

    QMenu *viewModeMenu = new QMenu("View Mode", this);
    editMenu->addMenu(viewModeMenu);

    QAction *viewMode1 = new QAction("V3d_PERSPECTIVE", this);
    viewMode1->setCheckable(true);
    connect(viewMode1, &QAction::triggered, this, &MainWindow::changeViewMode);
    viewModeMenu->addAction(viewMode1);

    QAction *viewMode2 = new QAction("V3d_ORTHOGRAPHIC", this);
    viewMode2->setCheckable(true);
    connect(viewMode2, &QAction::triggered, this, &MainWindow::changeViewMode);
    viewModeMenu->addAction(viewMode2);

    setMenuBar(menuBar);
}

// Status Bar yaratan fonksiyon
void MainWindow::createStatusBar() {
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->showMessage("Hazır");

    openedFolderLabel = new QLabel("Not Selected File", this);


    statusBar->addPermanentWidget(openedFolderLabel);

    setStatusBar(statusBar);
}

//
void MainWindow::createToolbars(){
    QToolBar *toolBar = new QToolBar("Toolbar 1", this);

    // Üst bakışa
    QAction *viewTop = new QAction("View Top", this);
    connect(viewTop, &QAction::triggered, this, &MainWindow::viewTop);
    toolBar->addAction(viewTop);

    QAction *viewBottom = new QAction("View Bottom", this);
    connect(viewBottom, &QAction::triggered, this, &MainWindow::viewBottom);
    toolBar->addAction(viewBottom);

    QAction *viewLeft = new QAction("View Left", this);
    connect(viewLeft, &QAction::triggered, this, &MainWindow::viewLeft);
    toolBar->addAction(viewLeft);

    QAction *viewRight = new QAction("View Right", this);
    connect(viewRight, &QAction::triggered, this, &MainWindow::viewRight);
    toolBar->addAction(viewRight);

    addToolBar(toolBar);
}
/*
 *  Action
 *  Functions
 */

// dosya yüklemek için çalışan fonksiyon
void MainWindow::importFile() {

    QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory);

    QString supportedFileType = "STEP Files (*.step *.stp *.STEP *.STP)";

    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    homeLocation,
                                                    supportedFileType);

    openedFolderLabel->setText(fileName);
}

void MainWindow::changeViewMode() {
    myViewerWidget->changeViewerMode();
}

void MainWindow::viewTop() {
    myViewerWidget->viewTop();
}

void MainWindow::viewBottom() {
    myViewerWidget->viewBottom();
}

void MainWindow::viewRight() {
    myViewerWidget->viewRight();
}

void MainWindow::viewLeft() {
    myViewerWidget->viewLeft();
}
