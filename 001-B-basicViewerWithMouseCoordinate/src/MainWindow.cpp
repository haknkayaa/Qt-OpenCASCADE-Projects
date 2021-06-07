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
#include <Geom_CartesianPoint.hxx>
#include <AIS_Line.hxx>

#include <Prs3d_Arrow.hxx>
#include <Geom_CartesianPoint.hxx>
#include <AIS_Line.hxx>
#include <AIS_Axis.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ArrowAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Geom_Axis1Placement.hxx>

// Kurucu fonksiyon
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OpenCASCADE Window");
    resize(700, 500);

    createMenuBar();
    createStatusBar();


    createMiddleWidget();
}

// Middle Widget
void MainWindow::createMiddleWidget() {
    QHBoxLayout *mainLayout = new QHBoxLayout();

    myViewerWidget = new Viewer(this);
    connect(myViewerWidget, SIGNAL(mousePosChanged(int, int)), this, SLOT(updateMousePos(int, int)));


    mainLayout->addWidget(myViewerWidget);

    // Make Box
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();
    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);

    anAisBox->SetColor(Quantity_NOC_AZURE);
    // Start


//    Handle(Prs3d_Drawer) myDrawer = anAisBox->Attributes()->Link();
//    myDrawer->SetLineArrowDraw(true);
//    myDrawer->ArrowAspect()->SetLength(20);

    // End
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

    QMenu *subMenu = new QMenu("File", this);
    menuBar->addMenu(subMenu);

    QAction *importFileAction = new QAction("Import", this);
    connect(importFileAction, &QAction::triggered, this, &MainWindow::importFile);
    importFileAction->setStatusTip("Yeni bir dosya ekle");
    subMenu->addAction(importFileAction);

    setMenuBar(menuBar);
}

// Status Bar yaratan fonksiyon
void MainWindow::createStatusBar() {
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->showMessage("Hazır");

    openedFolderLabel = new QLabel("Not Selected File", this);

    mousePositionLabel = new QLabel("X: Y:", this);

    statusBar->addPermanentWidget(openedFolderLabel);
    statusBar->addPermanentWidget(mousePositionLabel);

    setStatusBar(statusBar);
}

void MainWindow::updateMousePos(int arg_x, int arg_y) {
    qDebug() << "Mouse coordinate dğeişimi tetiklendi";

    QString string;
    string += "X: ";
    string += QString::number(arg_x);
    string += ", ";
    string += "Y: ";
    string += QString::number(arg_y);

    mousePositionLabel->setText(string);
}

// dosya yüklemek için çalışan fonksiyon
void MainWindow::importFile() {

    QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                                  QStandardPaths::LocateDirectory);

    QString supportedFileType = "STEP Files (*.step *.stp *.STEP *.STP)";

    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    homeLocation,
                                                    supportedFileType);

    openedFolderLabel->setText(fileName);
}

