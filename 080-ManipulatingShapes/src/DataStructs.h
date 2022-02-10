#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QString>

#include <Quantity_Color.hxx>
#include <Standard_Real.hxx>
#include <gp_Pnt.hxx>
#include <string>
#include <iostream>
#include <memory>
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
#include <Message_ProgressIndicator.hxx>
#include <Transfer_TransientProcess.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>
#include "NodeInteractive.h"



class AIS_InteractiveObject;

class QListWidgetItem;

class OCCData {
public:
    OCCData() = default;
    OCCData(const OCCData& occData) {
        this->Name = occData.Name;
        this->Label = occData.Label;
        this->Index = occData.Index;
        this->object = occData.object;
        this->shape = occData.shape;
        this->topoDsShape = occData.topoDsShape;
        this->Location = occData.Location;
        this->color = occData.color;
        this->Material = occData.Material;
        this->transparency = occData.transparency;
        this->volume = occData.volume;
        this->boundaryBox = occData.boundaryBox;

    }

    virtual ~OCCData() = default;

    const QString &getName() const {
        return Name;
    }

    void setName(const QString &name) {
        Name = name;
    }

    const TDF_Label &getLabel() const {
        return Label;
    }

    void setLabel(const TDF_Label &label) {
        Label = label;
    }

    const QString &getIndex() const {
        return Index;
    }

    void setIndex(const QString &index) {
        Index = index;
    }

    NodeInteractive *getObject() const {
        return object;
    }

    void setObject(NodeInteractive *arg_object) {
        OCCData::object = arg_object;
    }

    AIS_Shape *getShape() const {
        return shape;
    }

    void setShape(AIS_Shape *arg_shape) {
        OCCData::shape = arg_shape;
    }

    const TopoDS_Shape &getTopoShape() const {
        return topoDsShape;
    }

    void setTopoShape(const TopoDS_Shape &arg_topoShape) {
        OCCData::topoDsShape = arg_topoShape;
    }

    const TopLoc_Location &getLocation() const {
        return Location;
    }

    void setLocation(const TopLoc_Location &location) {
        Location = location;
    }

    const Quantity_Color &getColor() const {
        return color;
    }

    void setColor(const Quantity_Color &arg_color) {
        OCCData::color = arg_color;
    }

    const QString &getMaterial() const {
        return Material;
    }

    void setMaterial(const QString &material) {
        Material = material;
    }

    double getTransparency() const {
        return transparency;
    }

    void setTransparency(double arg_transparency) {
        OCCData::transparency = arg_transparency;
    }

    double getVolume() const {
        return volume;
    }

    void setVolume(double arg_volume) {
        OCCData::volume = arg_volume;
    }

    const Bnd_Box &getBoundaryBox() const {
        return boundaryBox;
    }

    void setBoundaryBox(const Bnd_Box &arg_boundaryBox) {
        OCCData::boundaryBox = arg_boundaryBox;
    }

private:
    QString Name;
    TDF_Label Label;
    QString Index;
    NodeInteractive *object;
    AIS_Shape *shape;
    TopoDS_Shape topoDsShape;
    TopLoc_Location Location;
    Quantity_Color color;
    QString Material;
    double transparency;
    double volume;
    Bnd_Box boundaryBox;
};
Q_DECLARE_METATYPE(OCCData*)


#endif //DATASTRUCTS_H
