// Open Cascade Libraries
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
#include <iostream>
#include <QSet>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepBndLib.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <AppStd_Application.hxx>
#include <XSDRAW.hxx>

#include <TDF_ChildIterator.hxx>
#include <BinXCAFDrivers.hxx>


// User definitions
#include "STEPProcessor.h"
#include "MainWindow.h"
#include "MRADSIMHelperFunctions.h"

/// Custom Progress Indicator class
class MyProgressIndicator : public Message_ProgressIndicator {


public:
    Standard_Boolean Show(const Standard_Boolean theForce = Standard_True) override {
        const Standard_Real currentPos = this->GetPosition(); // Always within [0,1]
        const int val = static_cast<int>(1 + currentPos * (100 - 1));

        STEPProcessor::myProgressDialog->setValue(val);

        Handle(TCollection_HAsciiString) aName = GetScope(1).GetName(); //current step
        if (!aName.IsNull())
            STEPProcessor::myProgressDialog->setLabelText(aName->ToCString());

        QApplication::processEvents();

        if (val > m_val) {
            m_val = val;
        }

        return Standard_True;
    }

    Standard_Boolean UserBreak() override {
        return Standard_False;
    }

private:
    int m_val = 0;
};


/// Constructor for STEPProcessor prints version of class.
STEPProcessor::STEPProcessor(QWidget *parent) : QWidget(parent) {

    // print StepProcessor version
    QString version = "0.0.2";
    qDebug() << "STEPProcessor version:" << version.toLocal8Bit();

    myProgressDialog = new QProgressDialog("Importing...", "Cancel", 0, 100);
    myProgressDialog->setWindowTitle("STEP Reader");
    myProgressDialog->setWindowModality(Qt::WindowModal);
    myProgressDialog->setCancelButton(nullptr);
    myProgressDialog->setAttribute(Qt::WA_DeleteOnClose);
    myProgressDialog->setValue(0);
    myProgressDialog->setValue(1);
    QApplication::processEvents();

    myProgressIndicator = new MyProgressIndicator();
    myProgressDialog->show();
    connect(myProgressDialog, &QProgressDialog::finished, myProgressDialog, &QProgressDialog::close);

}

/// Starts the reading given STEP file.
/// \param arg_filePath location of STEP file.
void STEPProcessor::loadSTEPFile(const QString &arg_filePath) {
    myProgressDialog->setValue(0);

    reader(arg_filePath);
//    topLevelTrees = getRoot(readerDoc);
    getRoot(readerDoc);
    qDebug() << "toplevel " << topLevelTrees.size();
    for (QTreeWidgetItem *item : topLevelTrees) {
        qDebug() << "Name: " << item->text(0);
    }
//    MainWindow::mainItem_geometry->addChildren(topLevelTrees);
    myProgressDialog->close();

    auto *messageBox = new QMessageBox(this->parentWidget());
    messageBox->setText(tr("Import finished successfully!"));
    messageBox->show();
}

/// Loads the step file, this is the first primitive method for reading.
/// \param arg_filename location of STEP file.
Handle_TDocStd_Document STEPProcessor::reader(const QString &arg_filename) {

    qDebug() << "File opening... " << arg_filename;

    STEPCAFControl_Reader reader;
//    Handle_TDocStd_Document readerDoc;
    Handle_XSControl_WorkSession myWorkSession = reader.Reader().WS();


    reader.SetColorMode(true);
    reader.SetNameMode(true);
    reader.SetMatMode(true);

    if (!myProgressIndicator.IsNull()) {
        myProgressIndicator->NewScope(30, "Loading file");
    }

    // dosyanın başarılı bir şekilde açılıp açılmadığı kontrolü
    if (reader.ReadFile(arg_filename.toUtf8().constData()) != IFSelect_RetDone) {
        qDebug() << "ERROR! STEP file cannot opened";
    }

    QCoreApplication::processEvents();

    readerDoc = new TDocStd_Document("StepReader");

    if (!myProgressIndicator.IsNull()) {
        myProgressIndicator->EndScope();
        myWorkSession->MapReader()->SetProgress(myProgressIndicator);
        myProgressIndicator->NewScope(60, "Inspecting file...");
    }

    if (!reader.Transfer(readerDoc)) {
        qDebug() << "ERROR! File did not transferred.";
    }

    if (!myProgressIndicator.IsNull()) {
        myProgressIndicator->EndScope();

        myWorkSession->MapReader()->SetProgress(nullptr);
        myProgressIndicator->NewScope(10, "Displaying shapes...");
    }

    return readerDoc;
}

/// This method returns the OCCData vector,
/// if  given doc has only one part returns it directly,
/// else searches its children recursively via getChildren()
/// \param doc : Document produced in reader()
/// \return : Actual ModelTree extracted from document.
QTreeWidgetItem *STEPProcessor::getRoot(const Handle_TDocStd_Document &doc) {
    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());//doc->Main() is the main TDF_Label
    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);

    // Get shape count
    TDF_LabelSequence tempSequence;
    shapeTool->GetShapes(tempSequence);
    shapeCount = tempSequence.Length();
    //
    progressIndicator = 1;
    QTreeWidgetItem *rootNode;

    for (int i = 1; i <= rootLabels.Length(); ++i) {

        // Create and local occData
        OCCData *occData = new OCCData();
        QVariant variant;
        variant.setValue(occData);

        // Construct Treenode with OCCData above
        rootNode = new QTreeWidgetItem();
        rootNode->setData(0, Qt::UserRole, variant);

        QApplication::processEvents();
        // Initialize the rootNode label
        //rootNode.getValue().Label = rootLabels.Value(i);
        occData->setLabel(rootLabels.Value(i));

        Handle(TDataStd_Name) nameAttr;
        if (occData->getLabel().FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            occData->setName(QString::fromStdString(toString(nameAttr->Get())));
        } else {
            occData->setName("Unknown");
        }

        occData->setName(nameControl(occData->getName()));

        occData->setIndex(QString::number(i));

//        rootNode.getValue().treeWidgetItem = new QTreeWidgetItem();
        rootNode->setIcon(0, QIcon(":/icons/part.png"));

        rootNode->setText(0, occData->getName() + " (" + occData->getIndex() + ") ");
        rootNode->setText(1, "Geometry");
        rootNode->setCheckState(0, Qt::Checked);
        rootNode->setData(1, Qt::UserRole, QVariant(Qt::Checked));

        occData->setTransparency(1.0);

        occData->setMaterial("-");

        occData->setTopoShape(shapeTool->GetShape(occData->getLabel()));

        occData->setLocation(occData->getTopoShape().Location());

        NodeInteractive *nodeInteractive = new NodeInteractive(occData->getLabel(), rootNode);
        occData->setShape(nodeInteractive);
//        occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());

        occData->setObject(nodeInteractive);

        GProp_GProps gprops;
        BRepGProp::VolumeProperties(occData->getTopoShape(), gprops);
        occData->setVolume(gprops.Mass());

        MainWindow::mainItem_geometry->addChild(rootNode);
        if (shapeTool->IsAssembly(getNodeData(rootNode)->getLabel())) {
            QApplication::processEvents();
            TopoDS_Iterator iterator(getNodeData(rootNode)->getTopoShape(), true,
                                     true);
            for (iterator; iterator.More(); iterator.Next()) {

                rootNode->addChild(getChildren(rootNode, iterator.Value()));
            }
        } else {
            MainWindow::myViewerWidget->getContext()->Display(
                    getNodeData(rootNode)->getShape(), false);
            MainWindow::myViewerWidget->getContext()->SetLocation(getNodeData(rootNode)->getShape(), getNodeData(rootNode)->getTopoShape().Location());

        }
        topLevelTrees << rootNode;

    }

    MainWindow::projectManagerMainTreeWidget->expandAll();
    MainWindow::myViewerWidget->fitAll();
    return rootNode;
}

/// Searches children through the given OCCData recursively.
/// \param parent pointer(shared_ptr) for parent OCCData.
/// \return Children vector of OCCData
QTreeWidgetItem *
STEPProcessor::getChildren(QTreeWidgetItem *arg_node, const TopoDS_Shape &arg_shape) {
    QApplication::processEvents();

    // Create an empty OCCData struct
    OCCData *occData = new OCCData();
    QVariant variant;
    variant.setValue(occData);
    // Construct Treenode with OCCData above
    QTreeWidgetItem *childNode = new QTreeWidgetItem();
    childNode->setData(0, Qt::UserRole, variant);

    GProp_GProps gprops;
    BRepGProp::VolumeProperties(arg_shape, gprops);
    double volume = gprops.Mass();
    if (volume <= 0) {
        has2dShape = true;
        return nullptr;
    }
    occData->setVolume(volume);
    occData->setLabel(shapeTool->FindShape(arg_shape));

    // Create an local TDataStd_Name to store name attribute from label
    Handle(TDataStd_Name) nameAttr;
    // Get the name attribute from label and copy it to nameAttr (Created Above)
    occData->getLabel().FindAttribute(TDataStd_Name::GetID(), nameAttr);
    // Since OCCData struct "Name" member is QString convert nameAttr to QString and store it
    occData->setName(QString::fromStdString(toString(nameAttr->Get())));

    occData->setName(nameControl(occData->getName()));
    occData->setIndex(getNodeData(arg_node)->getIndex() + ":" +
                      QString::number(arg_node->childCount() + 1));

    // Create an treeWidgetItem for our assembly
    childNode->setIcon(0, QIcon(":/icons/part.png"));

    // Set Text of treeWidgetItem to Name member of childNode
    childNode->setText(0, occData->getName() + " (" + occData->getIndex() + ")");
    childNode->setText(1, "Geometry");
    childNode->setCheckState(0, Qt::Checked);
    childNode->setData(1, Qt::UserRole, QVariant(Qt::Checked));

    // Add assembly treeWidgetItem to its parent treeWidgetItem
    arg_node->addChild(childNode);

    occData->setTransparency(1.0);
    occData->setMaterial("ALUMINIUM");

    occData->setTopoShape(arg_shape);

    occData->setLocation(occData->getTopoShape().Location());

    NodeInteractive *nodeInteractive = new NodeInteractive(occData->getLabel(), childNode);
    occData->setShape(nodeInteractive);
//    occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());

    occData->setObject(nodeInteractive);

    if (shapeTool->IsAssembly(occData->getLabel())) {
        occData->setMaterial("-");
        TopoDS_Iterator iterator(occData->getTopoShape(), true, true);
        for (iterator; iterator.More(); iterator.Next()) {
            childNode->addChild(getChildren(childNode, iterator.Value()));
        }
    } else {
        MainWindow::myViewerWidget->getContext()->Display(occData->getShape(), false);
        MainWindow::myViewerWidget->getContext()->SetLocation(occData->getShape(), occData->getTopoShape().Location());

    }
    progressIndicator++;
    if (progressIndicator >= double(shapeCount) / 10) {
        myProgressDialog->setValue(myProgressDialog->value() + 1);
        progressIndicator = 0;
    }
    return childNode;
}

/// Displays shapes of given model tree,
/// Search for its children recursively.
/// \param arg_modelTree vector of OCCData.
void STEPProcessor::displayShapes(QTreeWidgetItem *arg_node) {

    if (arg_node->childCount() > 0) {
        for (int i = 0; i < arg_node->childCount(); i++) {
            displayShapes(arg_node->child(i));
        }
    } else {
        MainWindow::myViewerWidget->getContext()->Display(
                getNodeData(arg_node)->getShape(), false);

    }

}

/// This method checks the given name of parts for equality.
/// \param arg_name : Part name for changing.
/// \return : If given name is used more than once returns it with index.
QString STEPProcessor::nameControl(QString arg_name) {
//        qDebug() << "Activate: only same mode";
    int nameSuffix = 1;
//    arg_name.simplified();
    arg_name.replace(" ", "");
    arg_name.replace("(", "_");
    arg_name.replace(")", "_");
    arg_name.replace("ö", "o");
    arg_name.replace("ç", "c");
    arg_name.replace("ş", "s");
    arg_name.replace("ğ", "g");
    arg_name.replace("ü", "u");
    arg_name.replace("ı", "i");

    QString tempName = arg_name;
    for (auto &i: addedShapeNameList) {
        if (arg_name == i) {

            for (auto &j: addedShapeNameList) {
                if (arg_name == j) {
                    nameSuffix++;
                    break;
                }
            }
            arg_name = tempName + "-" + QString::number(nameSuffix);
        }
    }

    addedShapeNameList.push_back(arg_name);
    return arg_name;
}

/// Converter between TCollection_ExtendedString to std::string
/// \param extstr (TCollection_ExtendedString)
/// \return text (std::string)
string STEPProcessor::toString(const TCollection_ExtendedString &extstr) {
    char *str = new char[extstr.LengthOfCString() + 1];
    extstr.ToUTF8CString(str);
    std::string text(str);
    delete[] str;
    return text;
}

void STEPProcessor::writeStepFile(const QString& fileName) {


    STEPCAFControl_Writer m_writer;
    m_writer.SetNameMode(true);
    m_writer.SetColorMode(true);

    m_writer.Transfer(readerDoc, STEPControl_AsIs);

    m_writer.Write(fileName.toStdString().c_str());
}

void STEPProcessor::slot_createEmptyStep(QString fileName) {

    STEPCAFControl_Writer m_writer;
    m_writer.SetNameMode(true);
    m_writer.SetColorMode(true);

    Handle(TDocStd_Application) app = new TDocStd_Application;

    BinXCAFDrivers::DefineFormat(app);
    Handle(TDocStd_Document) doc;
    app->NewDocument("MDTV-XCAF", doc);
    doc->AddComment("Hello from iradets");

    Handle(XCAFDoc_ShapeTool) shape_tool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());//doc->Main() is the main TDF_Label

    TopoDS_Compound aCompound;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aCompound);


    TDF_Label newLabel = shape_tool->NewShape();
    shape_tool->UpdateAssemblies();

    TDF_Label childLabel = shape_tool->AddComponent(newLabel, aCompound, true);
    shape_tool->UpdateAssemblies();

    TDataStd_Name::Set(childLabel, "TESTNAME1");
    shape_tool->UpdateAssemblies();

    TDataStd_Name::Set(newLabel, "TESTNAME2");


//    shape_tool->AddComponent(shape_tool->Label() ,aCompound, true);

    shape_tool->UpdateAssemblies();

    m_writer.Transfer(doc, STEPControl_AsIs);

    m_writer.Write(fileName.toStdString().c_str());


    this->loadSTEPFile(fileName);
}

