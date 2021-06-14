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
    setWindowTitle(QString());
    resize(700, 500);

    createMenuBar();
    createStatusBar();
    createMiddleWidget();
    retranslate();
}

// Middle Widget
void MainWindow::createMiddleWidget() {
    QHBoxLayout *mainLayout = new QHBoxLayout();

    auto *myViewerWidget = new Viewer(this);

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

    subMenu = new QMenu(QString(), this);
    menuBar->addMenu(subMenu);

    importFileAction = new QAction(QString(), this);
    connect(importFileAction, &QAction::triggered, this, &MainWindow::importFile);
    importFileAction->setStatusTip(QString());
    subMenu->addAction(importFileAction);

    langMenu = new QMenu(QString(), this);
    menuBar->addMenu(langMenu);

    turkishLangAction = new QAction(QString(), this);
    connect(turkishLangAction, &QAction::triggered, this, &MainWindow::turkishLang);
    turkishLangAction->setStatusTip(QString());
    langMenu->addAction(turkishLangAction);

    englishLangAction = new QAction(QString(), this);
    connect(englishLangAction, &QAction::triggered, this, &MainWindow::englishLang);
    englishLangAction->setStatusTip(QString());
    langMenu->addAction(englishLangAction);

    setMenuBar(menuBar);
}

// Status Bar yaratan fonksiyon
void MainWindow::createStatusBar() {
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->showMessage(QString());
    openedFolderLabel = new QLabel(QString(), this);

    mousePositionLabel = new QLabel("X: Y:", this);

    statusBar->addPermanentWidget(openedFolderLabel);
    statusBar->addPermanentWidget(mousePositionLabel);

    setStatusBar(statusBar);
}

/*
 *  Action
 *  Functions
 */

// dosya yüklemek için çalışan fonksiyon
void MainWindow::importFile() {

    QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory);

    QString supportedFileType = "STEP Files (*.step *.stp *.STEP *.STP)";

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    homeLocation,
                                                    supportedFileType);

    openedFolderLabel->setText(fileName);
}

void MainWindow::turkishLang() {
    qApp->removeTranslator(&qtLanguageTranslator);
    qtLanguageTranslator.load("turkish");
    qApp->installTranslator(&qtLanguageTranslator);
}

void MainWindow::englishLang() {
    qApp->removeTranslator(&qtLanguageTranslator);
   // qtLanguageTranslator.load("turkish");
   // qApp->installTranslator(&qtLanguageTranslator);
}

void MainWindow::retranslate() {

    setWindowTitle(tr("OpenCASCADE Window"));
    subMenu->setTitle(tr("File"));
    importFileAction->setText(tr("Import"));
    importFileAction->setStatusTip(tr("Add new file"));

    langMenu->setTitle(tr("Language"));

    turkishLangAction->setText(tr("Turkish"));
    turkishLangAction->setStatusTip(tr("Turkish"));

    englishLangAction->setText(tr("English"));
    englishLangAction->setStatusTip(tr("English"));



}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange)
        retranslate();
    QWidget::changeEvent(event);
}
