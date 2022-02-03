//
// Created by HAKAN on 2.02.2022.
//

#include "STEPWriter.h"



STEPWriter::STEPWriter(QWidget *parent): QWidget(parent) {
    qDebug() << "STEP Writer version 0.1";
}

bool STEPWriter::saveFile(QString arg_FileName, QList<TopoDS_Shape> arg_shapeList) {
    STEPControl_Writer writer;

    for (int i = 0; i < arg_shapeList.size(); ++i) {
        writer.Transfer(arg_shapeList.at(i), STEPControl_AsIs);
    }
    writer.Write(arg_FileName.toStdString().c_str());

    return true;
}

bool STEPWriter::saveFileFromLabel(QString arg_FileName, QList<TDF_Label> arg_shapeList) {
    STEPCAFControl_Writer writer;
    writer.SetNameMode(true);
    writer.SetColorMode(true);
    for (int i = 0; i < arg_shapeList.size(); ++i) {
        writer.Transfer(arg_shapeList.at(i), STEPControl_AsIs);
    }
    writer.Write(arg_FileName.toStdString().c_str());

    return true;
}