//
// Created by sufuk on 11.06.2021.
//

#ifndef OPENCASCADEPROJECT_TREEITEM_H
#define OPENCASCADEPROJECT_TREEITEM_H
#include <QtWidgets>
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
struct myNode {
    QString Name;
    TDF_Label Label;
    QString Index;
    QTreeWidgetItem *treeWidgetItem;
    AIS_InteractiveObject *object;
    AIS_Shape *shape;
    TopoDS_Shape topoShape;
    TopLoc_Location Location;
    Quantity_Color color;
    double transparency;
};
class TreeItem
{
public:
    explicit TreeItem(const QVector<myNode> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    myNode data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QVector<TreeItem*> m_childItems;
    QVector<myNode> m_itemData;
    TreeItem *m_parentItem;
};


#endif //OPENCASCADEPROJECT_TREEITEM_H
