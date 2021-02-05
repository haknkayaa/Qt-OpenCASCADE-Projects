//
// Created by emir on 4.02.2021.
//

#include "STEPProcessor.h"

STEPProcessor::STEPProcessor(QString arg_filename) {
    QString version = "0.0.1";

    qDebug() << "STEPProcessor version:" << version.toLocal8Bit();

    loadSTEPFile(arg_filename);
}

void STEPProcessor::loadSTEPFile(QString arg_filename) {

    qDebug() << "Dosya açılıyor... " << arg_filename;

    STEPCAFControl_Reader reader;

    reader.SetColorMode(true);
    reader.SetNameMode(true);
    reader.SetMatMode(true);

    // dosyanın başarılı bir şekilde açılıp açılmadığı kontrolü
    if (reader.ReadFile(arg_filename.toUtf8().constData()) != IFSelect_RetDone) {
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


    for (int i = 1; i <= shapes.Length(); i++) {

        qDebug() << "shapes" << i << endl
                 << "shapes components ";

        TopoDS_Shape shape = shapeTool->GetShape(shapes.Value(i));
        Handle(AIS_Shape) ais_shape = new AIS_Shape(shape);

        //get a label of shape.
        TDF_Label aLabel;
        aLabel = shapeTool->FindShape(shape);

        // display
        MainWindow::myViewerWidget->getContext()->Display(ais_shape, 0);
        MainWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
    }
}