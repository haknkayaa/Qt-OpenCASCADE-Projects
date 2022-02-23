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
#include "NodeInteractive.h"
// Qt Libraries
#include <QtWidgets>

// User Libraries
#include "MainWindow.h"

using namespace std;

/// main class for handling STEP files.
class STEPProcessor : public QWidget {
Q_OBJECT
public:
    explicit STEPProcessor(QWidget *parent);

    void loadSTEPFile(const QString &arg_filePath);

    void writeStepFile(QString);

    Handle_TDocStd_Document reader(const QString &arg_filename);

    QTreeWidgetItem *getRoot(const Handle(TDocStd_Document) &doc);

    QTreeWidgetItem *getChildren(QTreeWidgetItem *arg_node, const TopoDS_Shape &arg_shape);

    void displayShapes(QTreeWidgetItem *arg_node);

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


    bool has2dShape = false;

};


#endif //STEPPROCESSOR_H
