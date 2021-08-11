/******************************************************************************
 *    STEPProcessor.cpp 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 6/9/21.
 *     description :
 ******************************************************************************/


#include "STEPProcessor.h"

STEPProcessor::STEPProcessor(QWidget *parent) {
    qDebug() << "STEPProcessor... v0.2";
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