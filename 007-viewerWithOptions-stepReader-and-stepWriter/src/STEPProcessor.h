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
#include <Message_ProgressIndicator.hxx>
#include <Transfer_TransientProcess.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>
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
#include <AIS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <Message_ProgressIndicator.hxx>
#include <XSControl_WorkSession.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>


// Qt Libraries
#include <QtWidgets>

// User Libraries

using namespace std;


class NodeInteractive : public XCAFPrs_AISObject {
public:
    NodeInteractive(const TDF_Label &theLabel) : XCAFPrs_AISObject(theLabel) {}

    NodeInteractive(const TDF_Label &theLabel, QTreeWidgetItem *treeWidgetItem) : XCAFPrs_AISObject(theLabel),
                                                                                  treeWidgetItem(treeWidgetItem) {}

    QTreeWidgetItem *getTreeWidgetItem() const { return treeWidgetItem; }

    void
    setTreeWidgetItem(QTreeWidgetItem *arg_treeWidgetItem) { NodeInteractive::treeWidgetItem = arg_treeWidgetItem; }

private:
    QTreeWidgetItem *treeWidgetItem;
};

class OCCData {
public:
    OCCData() = default;

    OCCData(const OCCData &occData) {
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


class STEPProcessor : public QWidget {
Q_OBJECT
public:
    explicit STEPProcessor(QWidget *parent);

    void loadSTEPFile(const QString &arg_filePath);

    static Handle_TDocStd_Document reader(const QString &arg_filename);

    QTreeWidgetItem *getRoot(const Handle_TDocStd_Document &doc);

    QTreeWidgetItem *getChildren(QTreeWidgetItem *arg_node, const TopoDS_Shape &arg_shape);

    QList<TopoDS_Shape> shapeList;
    QList<TDF_Label> shapeLabelList;
    QList<TopoDS_Shape> getAllShape();
    QList<TDF_Label> getAllShapeLabel();

    void displayShapes(QTreeWidgetItem *arg_node);

    QTreeWidgetItem *stepFileItem;
    QTreeWidgetItem *hierarchyItem;
    QTreeWidgetItem *propertiesItem;
    QTreeWidgetItem *boundryItem;
    QTreeWidgetItem *beamRadiusItem;

    const Bnd_Box &addBoundryBox(TopoDS_Shape shape) const;

    const Bnd_Box &getBoundryBox() const;

    bool setWorldBox(double multiplier);

    bool setBeamRadius(double multiplier);

    QString nameControl(QString);

    static string toString(const TCollection_ExtendedString &extstr);

    QTreeWidgetItem *modelTree;
    Handle(XCAFDoc_ShapeTool) shapeTool;
    Handle(XCAFDoc_ColorTool) colorTool;

    inline static QProgressDialog *myProgressDialog;
    inline static Handle_Message_ProgressIndicator myProgressIndicator;

    QList<QString> addedShapeNameList;

    unsigned int shapeCount;
    unsigned int progressIndicator;

    Bnd_Box boundryBox; /* Tüm şekillerin toplanmış boundry box'ı */
    gp_Pnt boundryBoxCenterPoint; /* Boundry box'ın merkezi koordinatı */

    TopoDS_Shape worldBox_shape;
    AIS_Shape *worldBox_aisShape; /* Tüm boundry box kullanılarak oluşturulan World Box şekli */
    double worldBox_lenght; /* Boundry box'ın tek eksen kenarı (kendisi küp) */
    gp_Pnt worldBox_centerPoint;
    double worldBox_multiplier = 3;

    TopoDS_Shape beamRadius_shape;
    AIS_Shape *beamRadius_aisShape; /* World Box kullanılarak oluşturulan Beam Radius şekli */
    double beamRadius_lenght;
    gp_Pnt beamRadius_centerPoint;
    double beamRadius_multiplier = 1;

    bool has2dShape = false;
private:
    QTreeWidget* modelTreeWidget;

Q_SIGNALS:
    void readFinished();
    void readyShapeForDisplay(AIS_Shape *shape);
    void readyTreeWidgetItem(QTreeWidgetItem* item);
    void updatedSourceSphereDiameter(int value);
    void updatedWorldBoxLenght(int value);
};


#endif //STEPPROCESSOR_H