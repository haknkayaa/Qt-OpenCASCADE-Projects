#ifndef STEPPROCESSOR_H
#define STEPPROCESSOR_H

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

// Qt Libraries
#include <QtWidgets>

// User Libraries
#include "MainWindow.h"
#include "VariableTypes.h"




class STEPProcessor {
public:
    explicit STEPProcessor(QString arg_filename);

    inline static vector<AssemblyNode> modelTree;

    void loadSTEPFile(QString);

    vector<AssemblyNode> GetRootsFromDocument(Handle(TDocStd_Document) doc);

    vector<AssemblyNode> GetChildren(const std::shared_ptr<AssemblyNode> &parent,
                                     const Handle(XCAFDoc_ShapeTool) &shapeTool,
                                     const Handle(XCAFDoc_ColorTool) &colorTool,
                                     const TDF_Label &parentLabel);

    void dumpModelTree(vector<AssemblyNode> arg_modelTree);

    void addTreeWidget(vector<AssemblyNode> arg_modelTree);

private:
};


#endif //STEPPROCESSOR_H
