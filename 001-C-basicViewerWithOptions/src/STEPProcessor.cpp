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

    if(!fileName.isNull()) {
        qDebug() << "--> Opening file:" << fileName;
        readSTEP(fileName);
    }

    else {
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

    for (iterator; iterator.More(); iterator.Next()) {
        qDebug() << "Shape";

        TopoDS_Shape topo_shape = iterator.Value();

        emit readyShape(new AIS_Shape(topo_shape));
    }

//    for (int i = 1; i <= shapes.Length(); i++) {
//
//        qDebug() << "shapes" << i << endl
//                 << "shapes components ";
//
//        TopoDS_Shape shape = shapeTool->GetShape(shapes.Value(i));
//        Handle(AIS_Shape) ais_shape = new AIS_Shape(shape);
//
//        //get a label of shape.
//        TDF_Label aLabel;
//        aLabel = shapeTool->FindShape(shape);
//
//        Handle(XCAFDoc_ColorTool) myColors = XCAFDoc_DocumentTool::ColorTool(readerDoc->Main());
//        TDF_LabelSequence ColLabels;
//        myColors->GetColors(ColLabels);
//
//        Quantity_Color col;
//        bool result;
//        result = myColors->GetColor(aLabel, XCAFDoc_ColorGen, col);
//        if (result) {
//            ais_shape->SetColor(col);
//        }
//        result = myColors->GetColor(aLabel, XCAFDoc_ColorSurf, col);
//        if (result) {
//            ais_shape->SetColor(col);
//        }
//        result = myColors->GetColor(aLabel, XCAFDoc_ColorCurv, col);
//        if (result) {
//            ais_shape->SetColor(col);
//        }
//
//        // display
//        emit readyShape(ais_shape);
//    }



}