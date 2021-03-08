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

// Library needed for processing XML documents
#include <QtXml>
// Library needed for processing files
#include <QFile>
#include <QDebug>
#include <QtGlobal>
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

    QMenu *subMenu = new QMenu("File", this);
    menuBar->addMenu(subMenu);

    QAction *importFileAction = new QAction("Import", this);
    connect(importFileAction, &QAction::triggered, this, &MainWindow::importFile);
    importFileAction->setStatusTip("Yeni bir dosya ekle");

    QAction *importProjectAction= new QAction("Import Project", this);
    connect(importProjectAction, &QAction::triggered, this, &MainWindow::importProject);

    QAction *saveProjectAction= new QAction("Save Project", this);
    connect(saveProjectAction, &QAction::triggered, this, &MainWindow::saveProject);

    subMenu->addAction(importFileAction);
    subMenu->addAction(importProjectAction);
    subMenu->addAction(saveProjectAction);


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

/*!
 * Used for importing .mrad projects by choosing file
 */
void MainWindow::importProject() {
    QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory);
    QString supportedFileType = "MRADSIM Files (*.mrad *.xml)";

    QFile file(QFileDialog::getOpenFileName(this, "Open File",
                                            homeLocation,
                                            supportedFileType));
    if(file.exists()){
        file.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        while(!xmlReader.isEndDocument()){
            xmlReader.readNext();
            if(xmlReader.name() == "geometry"){
                xmlReader.readNext();
                while (xmlReader.name() != "geometry"){
                    if((xmlReader.tokenType() == QXmlStreamReader::StartElement) && (xmlReader.name() == "stepFile")){
                        for(const QXmlStreamAttribute &attr : xmlReader.attributes()){
                            stepFile = attr.value().toString();
                        }
                    }
                    if((xmlReader.tokenType() == QXmlStreamReader::StartElement) && (xmlReader.name() == "beamFile")){
                        for(const QXmlStreamAttribute &attr : xmlReader.attributes()){
                            beamFiles.push_back(attr.value().toString());
                        }
                    }
                    xmlReader.readNext();
                }
            }

            if(xmlReader.name() == "macroFiles"){
                xmlReader.readNext();
                while (xmlReader.name() != "macroFiles"){
                    if(xmlReader.tokenType() == QXmlStreamReader::StartElement){
                        for(const QXmlStreamAttribute &attr : xmlReader.attributes()){
                            macroFiles.push_back(attr.value().toString());
                        }
                    }
                    xmlReader.readNext();
                }
            }
        }



    }
     //print the Step File
     qDebug() << "Step file :";
     qDebug() << stepFile;

     // Print the macro files
     qDebug() << "Macro files :";
     for(const QString& itr : macroFiles){qDebug() << itr;}
     // Print Beam files
     qDebug() << "Beam files :";
     for(const QString& itr : beamFiles){qDebug() << itr;}

    file.close();
 }

/*!
 * Used for saving/creating Projects by selecting a folder
 * Project also creates it own project folder
 * Prototype of ProjectCreator Class
 */
 void MainWindow::saveProject() {
    // Get the project folder location and name it to test
     QString projectPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         QDir::homePath(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks) + "/test";

     QDir projectDir;
     projectDir.mkdir(projectPath);
     projectDir.setPath(projectPath);
     projectDir.mkdir("macroFiles");
     projectDir.mkdir("stepFiles");

     QFile file = projectDir.path() + "/test.mrad";

     file.open(QIODevice::WriteOnly);
     QXmlStreamWriter xmlWriter(&file);
     xmlWriter.setAutoFormatting(true);
     xmlWriter.writeStartDocument(); // Document Start
     xmlWriter.writeStartElement("project"); // Root Tag Start

     xmlWriter.writeStartElement("geometry"); //Step Files Start
     xmlWriter.writeStartElement("stepFile");
     xmlWriter.writeAttribute("file", "test1.step");
     xmlWriter.writeEndElement();
     xmlWriter.writeStartElement("beamFile");
     xmlWriter.writeAttribute("file", "beam1.step");
     xmlWriter.writeEndElement();
     xmlWriter.writeEndElement(); //Step Files End

     xmlWriter.writeStartElement("macroFiles"); //Macro Files Start
     xmlWriter.writeStartElement("macroFile");
     xmlWriter.writeAttribute("file", "vis.mac");
     xmlWriter.writeEndElement();
     xmlWriter.writeStartElement("macroFile");
     xmlWriter.writeAttribute("file", "run.mac");
     xmlWriter.writeEndElement();
     xmlWriter.writeStartElement("macroFile");
     xmlWriter.writeAttribute("file", "config.mac");
     xmlWriter.writeEndElement();
     xmlWriter.writeEndElement(); //Macro Files End

     xmlWriter.writeEndElement(); // Root Tag End
     xmlWriter.writeEndDocument(); // Document End
     file.close();
 }

