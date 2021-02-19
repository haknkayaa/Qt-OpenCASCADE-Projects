/******************************************************************************
 *    VariableTypes.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 1/4/21.
 *     description :
 ******************************************************************************/

#ifndef VARIABLETYPES_H
#define VARIABLETYPES_H


#include <string>
#include <iostream>
#include <memory>


using namespace std;

struct AssemblyNode {
    QString Name;
    TDF_Label Label;
    QString Index;
    vector<AssemblyNode> Children;
    shared_ptr<AssemblyNode> Parent;
    QTreeWidgetItem *treeWidgetItem;
    AIS_InteractiveObject *object;
    AIS_Shape *shape;
    TopoDS_Shape topoShape;
    TopLoc_Location Location;
    Quantity_Color color;
    double transparency;
};

struct CurrentNode{
    QString itemIndex;
    AssemblyNode item;
};


#include <QString>

#include <Quantity_Color.hxx>
#include <Standard_Real.hxx>

class AIS_InteractiveObject;
class QListWidgetItem;


#endif //VARIABLETYPES_H
