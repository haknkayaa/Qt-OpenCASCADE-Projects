//
// Created by sufuk on 3.09.2021.
//

#ifndef  MRADSIM_GUI_NODEINTERACTIVE_H
#define  MRADSIM_GUI_NODEINTERACTIVE_H
// Open Cascade Libraries
#include  <TopTools_HSequenceOfShape.hxx>
#include  <AIS_Shape.hxx>
#include  <STEPControl_Reader.hxx>
#include  <AIS_InteractiveObject.hxx>
#include  <AIS_DisplayMode.hxx>
#include  <AIS_ListOfInteractive.hxx>
#include  <TopoDS_Shape.hxx>
#include  <TopExp_Explorer.hxx>
#include  <TopTools_HSequenceOfShape.hxx>
#include  <STEPCAFControl_Reader.hxx>
#include  <TDocStd_Document.hxx>
#include  <XCAFDoc_DocumentTool.hxx>
#include  <XCAFDoc_ShapeTool.hxx>
#include  <XCAFDoc_ColorTool.hxx>
#include  <XCAFDoc_MaterialTool.hxx>
#include  <TDF_LabelSequence.hxx>
#include  <TDataStd_Name.hxx>
#include  <Interface_Static.hxx>
#include  <Message_ProgressIndicator.hxx>
#include  <Transfer_TransientProcess.hxx>
#include  <XSControl_WorkSession.hxx>
#include  <XSControl_TransferReader.hxx>
#include  <AIS_Shape.hxx>
#include  <AIS_InteractiveObject.hxx>
#include  <TopoDS_Shape.hxx>
#include  <TopExp_Explorer.hxx>
#include  <STEPCAFControl_Reader.hxx>
#include  <TDocStd_Document.hxx>
#include  <XCAFDoc_DocumentTool.hxx>
#include  <TDF_LabelSequence.hxx>
#include  <TDataStd_Name.hxx>
#include  <Message_ProgressIndicator.hxx>
#include  <XSControl_WorkSession.hxx>
#include  <XCAFPrs_AISObject.hxx>
#include  <BRepPrimAPI_MakeSphere.hxx>
#include  "NodeInteractive.h"
// Qt Libraries
#include  <QtWidgets>

class NodeInteractive : public XCAFPrs_AISObject {


public:

    NodeInteractive(const TDF_Label &theLabel) : XCAFPrs_AISObject(theLabel) {

    }

    NodeInteractive(const TDF_Label &theLabel, QTreeWidgetItem *treeWidgetItem) : XCAFPrs_AISObject(theLabel),
                                                                                  treeWidgetItem(treeWidgetItem) {

    }

    QTreeWidgetItem *getTreeWidgetItem() const {
        return treeWidgetItem;
    }

    void setTreeWidgetItem(QTreeWidgetItem *arg_treeWidgetItem) {
        NodeInteractive::treeWidgetItem = arg_treeWidgetItem;
    }

private:
    QTreeWidgetItem *treeWidgetItem;
};


#endif //MRADSIM_GUI_NODEINTERACTIVE_H
