//
// Created by HAKAN on 2.02.2022.
//

#ifndef PROJECT_STEPWRITER_H
#define PROJECT_STEPWRITER_H

#include <QtWidgets>

// OpenCASCADE Libs
#include <NCollection_Vector.hxx>
#include <STEPControl_Controller.hxx>
#include <Transfer_FinderProcess.hxx>
#include <TopoDS_Shape.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPCAFControl_Writer.hxx>

class STEPWriter : public QWidget{
Q_OBJECT
public:
    explicit STEPWriter(QWidget *parent = nullptr);

    bool saveFile(QString arg_FileName, QList<TopoDS_Shape> arg_shapeList);
    bool saveFileFromLabel(QString arg_FileName, QList<TDF_Label>    arg_shapeList);
};


#endif //PROJECT_STEPWRITER_H
