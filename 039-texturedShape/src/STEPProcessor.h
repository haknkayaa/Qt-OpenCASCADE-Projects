/******************************************************************************
 *    STEPProcessor.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 6/9/21.
 *     description :
 ******************************************************************************/

#ifndef PROJECT_STEPPROCESSOR_H
#define PROJECT_STEPPROCESSOR_H

// Qt Libraries
#include <QtWidgets>

// OpenCASCADE Libraries
// Open Cascade Libraries
#include <TopTools_HSequenceOfShape.hxx>
#include <AIS_Shape.hxx>
#include <STEPControl_Reader.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_DisplayMode.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_MaterialTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <Interface_Static.hxx>
#include <BRepBndLib.hxx>


#include "MainWindow.h"

class STEPProcessor : public QWidget {
Q_OBJECT
public:
    explicit STEPProcessor(QWidget *parent = nullptr);

    ~STEPProcessor() override;

    /* Public functions */
    void importFile();

    void readSTEP(const QString);

    void showTexturedShape();

signals:

    void readyShape(Handle_AIS_Shape shape);

    /* Public variables */
};


#endif //PROJECT_STEPPROCESSOR_H
