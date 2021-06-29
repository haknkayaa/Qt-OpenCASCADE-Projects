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
#include "mainwindow.h"
#include "VariableTypes.h"
#include "libtree.h"



class STEPProcessor : public QWidget{
Q_OBJECT
public:
    explicit STEPProcessor(const QString& arg_filename, QWidget *parent);

    TreeNode<OCCData> modelTree;

    void loadSTEPFile(const QString&);

    TreeNode<OCCData> getRoot(Handle(TDocStd_Document) doc);
    TreeNode<OCCData> getChildren(TreeNode<OCCData> arg_node, TopoDS_Shape arg_shape);


    inline static QProgressDialog *myProgressDialog;

    Handle(TDocStd_Document) readerDoc;
    Handle(XCAFDoc_ShapeTool) shapeTool;
    Handle(XCAFDoc_ColorTool) colorTool;

//private:
//
signals:
    void signal_treeItemAdd(QTreeWidgetItem*);
    void signal_displayShape(AIS_Shape*);
    void signal_mal();
};


#endif //STEPPROCESSOR_H