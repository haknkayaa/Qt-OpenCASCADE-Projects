/******************************************************************************
 *    STEPProcessor.cpp 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 6/9/21.
 *     description :
 ******************************************************************************/


#include <AIS_TexturedShape.hxx>
#include <Image_AlienPixMap.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include "STEPProcessor.h"

STEPProcessor::STEPProcessor(QWidget *parent) {
    qDebug() << "STEPProcessor... v0.2";

    showTexturedShape();
}

STEPProcessor::~STEPProcessor() noexcept {
    delete this;
}

// First read for STEP file
void STEPProcessor::importFile() {
    QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                                  QStandardPaths::LocateDirectory);

    QString supportedFileType = "STEP Files (*.step *.stp *.STEP *.STP)";

    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    homeLocation,
                                                    supportedFileType);

    if (!fileName.isNull()) {
        qDebug() << "--> Opening file:" << fileName;
        readSTEP(fileName);
    } else {
        qDebug() << "--> Opening file: NULL";
    }
}

void STEPProcessor::readSTEP(const QString arg_filepath) {
    STEPCAFControl_Reader reader;

    reader.SetColorMode(true);
    reader.SetNameMode(true);
    reader.SetMatMode(true);

    // dosyanın başarılı bir şekilde açılıp açılmadığı kontrolü
    if (reader.ReadFile(arg_filepath.toUtf8().constData()) != IFSelect_RetDone) {
        qDebug() << "Hata! STEP dosyası açılamadı";
    }

    //
    Handle(TDocStd_Document) readerDoc = new TDocStd_Document("StepReader");

    if (!reader.Transfer(readerDoc)) {
        qDebug() << "Hata! Dosya aktarılamadı.";
    }

    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(readerDoc->Main());

    TDF_LabelSequence shapes;
    shapeTool->GetShapes(shapes);

    TDF_LabelSequence freeShapes;
    shapeTool->GetFreeShapes(freeShapes);

    qDebug() << "GetShapes():" << shapes.Length() << "GetFreeShapes():" << freeShapes.Length();


    TDF_Label rootLabel = freeShapes.Value(1);
    TopoDS_Shape shape = shapeTool->GetShape(rootLabel);
    TopoDS_Iterator iterator(shape, true, true);

    qDebug() << "Root boundry box ....";
    Bnd_Box box;
    box.SetGap(0);
    BRepBndLib::Add(shape, box);
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    qDebug() << "Bnd box-> Min" << xmin << ymin << zmin;
    qDebug() << "Bnd box-> Max" << xmax << ymax << zmax;

    for (iterator; iterator.More(); iterator.Next()) {
        qDebug() << "Shape";

        TopoDS_Shape topo_shape = iterator.Value();
        emit readyShape(new AIS_Shape(topo_shape));

        //        qDebug() <<
        Bnd_Box box;
        BRepBndLib::Add(topo_shape, box);
        Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
        box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

        qDebug() << "Bnd box-> Min" << xmin << ymin << zmin;
        qDebug() << "Bnd box-> Max" << xmax << ymax << zmax;
    }


}


void STEPProcessor::showTexturedShape() {


    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(30.0, 40.0, 50.0);
    Handle(AIS_TexturedShape) textured = new AIS_TexturedShape(aTopoBox);

    TCollection_AsciiString aFile("sample_640×426.bmp");

    if (!aFile.IsIntegerValue())
    {
        TCollection_AsciiString aTmp(aFile);
        aFile = aFile;
    }

    textured->SetTextureFileName(aFile);

//    Handle(Image_AlienPixMap) pixMap = new Image_AlienPixMap;
//    pixMap->Load("sample_640×426.bmp");
//    pixMap->Save("test.png");
//    textured->SetTexturePixMap(pixMap);
    textured->SetTextureMapOn();
    textured->SetTextureRepeat(true, 1.0, 1.0);
    textured->DisableTextureModulate();
    textured->SetDisplayMode(3);

    MainWindow::myViewer->getContext()->Display(textured, true);
    MainWindow::myViewer->fitAll();

    textured->UpdateAttributes();
}