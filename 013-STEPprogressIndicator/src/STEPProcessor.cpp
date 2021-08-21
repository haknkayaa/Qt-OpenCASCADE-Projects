#include <ShapeFix_Shape.hxx>
#include <Prs3d_PointAspect.hxx>
#include <XCAFPrs.hxx>
#include <XCAFPrs_Style.hxx>
#include <TopoDS.hxx>

#include <Message_ProgressIndicator.hxx>
#include <Transfer_TransientProcess.hxx>

#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>


#include "STEPProcessor.h"


#include <QProgressDialog>
#include <TDF_ChildIterator.hxx>


QString nameMethod;
/**
 *
 * @param extstr
 * @return
 */

string toString(const TCollection_ExtendedString &extstr) {
    char *str = new char[extstr.LengthOfCString() + 1];
    extstr.ToUTF8CString(str);
    std::string text(str);
    delete[] str;
    return text;
}
class MyProgressIndicator : public Message_ProgressIndicator{

public:
    Standard_Boolean Show(const Standard_Boolean /*force*/) override{
        const Standard_Real currentPos = this->GetPosition(); // Always within [0,1]
        const int val = static_cast<int>(1 + currentPos * (100 - 1));

        STEPProcessor::myProgressDialog->setValue(val);
        Handle(TCollection_HAsciiString) aName = GetScope(1).GetName(); //current step
        if (!aName.IsNull())
            STEPProcessor::myProgressDialog->setLabelText (aName->ToCString());

        QApplication::processEvents();

        if (val > m_val) {
            std::cout << val;
            if (val < 100)
                std::cout << "-";
            else
                std::cout << "%";
            std::cout.flush();
            m_val = val;
        }

        return Standard_True;
    }

    Standard_Boolean UserBreak() override{
        return Standard_False;
    }

private:
    int m_val = 0;
};

/** STEPProcessor sınıfının kurucu fonksiyonu
 */
STEPProcessor::STEPProcessor(const QString& arg_filename, QWidget *parent) {
    QString version = "0.0.1";

    qDebug() << "STEPProcessor version:" << version.toLocal8Bit();

    loadSTEPFile(arg_filename);
}

/** STEP formatındaki dosyasının ilk defa yüklenmesi
 *
 * @param arg_filename: Yüklenecek step dosyasının path tanımı.
 * Örnek: "/home/hakan/Desktop/Example - STEP Files/test.step"
 */
void STEPProcessor::loadSTEPFile(const QString& arg_filename) {


    // Progress Dialog
    myProgressDialog = new QProgressDialog("Importing...", "Cancel", 0, 100);
    myProgressDialog->setWindowTitle("STEP Reader");
    myProgressDialog->setValue(0);
    myProgressDialog->show();
    QApplication::processEvents();


    qDebug() << "Dosya açılıyor... " << arg_filename;


    STEPCAFControl_Reader myReader;
    Handle_XSControl_WorkSession myWorkSession = myReader.Reader().WS();
    Handle_Message_ProgressIndicator myProgressIndicator = new MyProgressIndicator();

    myReader.SetColorMode(true);
    myReader.SetNameMode(true);
    myReader.SetMatMode(true);


    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->NewScope(30, "Loading file");
    }

    // dosyanın başarılı bir şekilde açılıp açılmadığı kontrolü
    if (myReader.ReadFile(arg_filename.toUtf8().constData()) != IFSelect_RetDone) {
        qDebug() << "Hata! STEP dosyası açılamadı";
    }
    QCoreApplication::processEvents();
    //
    readerDoc = new TDocStd_Document("StepReader");


    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->EndScope();
        myWorkSession->MapReader()->SetProgress(myProgressIndicator);
        myProgressIndicator->NewScope(60, "Inspecting file...");
    }


    if (!myReader.Transfer(readerDoc)) {
        qDebug() << "Hata! Dosya aktarılamadı.";
    }

    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->EndScope();
        myWorkSession->MapReader()->SetProgress(NULL);
        myProgressIndicator->NewScope(10, "Displaying shapes...");
    }

    modelTree = getRoot(readerDoc); // TREENODE

    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->EndScope();
    }
}

lstdTreeNode<OCCData> STEPProcessor::getRoot(opencascade::handle<TDocStd_Document> doc) {

    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main()); //doc->Main() is the main TDF_Label
    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);

    // Create and local occData
    OCCData occData;
    // Construct Treenode with OCCData above
    lstdTreeNode<OCCData> rootNode(occData);

    for (int i = 1; i <= rootLabels.Length(); ++i) {
        QApplication::processEvents();

        // Initialize the rootNode label
        rootNode.getValue().Label = rootLabels.Value(i);;

        Handle(TDataStd_Name) nameAttr;
        if (rootNode.getValue().Label.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            rootNode.getValue().Name = QString::fromStdString(toString(nameAttr->Get()));
        }else{
            rootNode.getValue().Name = "Unknown";
        }

        rootNode.getValue().treeWidgetItem = new QTreeWidgetItem();
        rootNode.getValue().treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

        rootNode.getValue().Index = QString::number(i);
        QString name = rootNode.getValue().Name + " (" + rootNode.getValue().Index + ") ";
        rootNode.getValue().treeWidgetItem->setText(0, name);

        rootNode.getValue().transparency = 1.0;

        TopLoc_Location location = shapeTool->GetLocation(rootNode.getValue().Label);
        rootNode.getValue().Location = location;

        TopoDS_Shape shape = shapeTool->GetShape(rootNode.getValue().Label);
        rootNode.getValue().shape = new AIS_Shape(shape);
        rootNode.getValue().topoShape = shape;

        MainWindow::modelTreeWidget->addTopLevelItem(rootNode.getValue().treeWidgetItem);

        qDebug() << rootNode.getValue().treeWidgetItem->text(0);
        if(shapeTool->IsAssembly(rootNode.getValue().Label)){
            QApplication::processEvents();
            TDF_LabelSequence sequence;
            shapeTool->GetComponents(rootNode.getValue().Label, sequence);
            for(TDF_Label childLabel : sequence){
                rootNode.addChild(getChildren(rootNode, childLabel));
            }

        }
    }
        return rootNode;


}
lstdTreeNode<OCCData> STEPProcessor::getChildren(lstdTreeNode<OCCData> arg_node, const TDF_Label &arg_label) {
    QApplication::processEvents();

    // If arg_label is assembly
    if (shapeTool->IsAssembly(arg_label)){

        // Create an empty OCCData struct
        OCCData occData;
        // Construct a arg_node with empty OCCData struct above
        lstdTreeNode<OCCData> childNode(occData);

        // Initialize childNode arg_label with given arg_label
        childNode.getValue().Label = arg_label;

        // Create an local TDataStd_Name to store name attribute from label
        Handle(TDataStd_Name) nameAttr;
        // Get the name attribute from label and copy it to nameAttr (Created Above)
        childNode.getValue().Label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
        // Since OCCData struct "Name" member is QString convert nameAttr to QString and store it
        childNode.getValue().Name = QString::fromStdString(toString(nameAttr->Get()));
        qDebug() << "Assembly: " << childNode.getValue().Name;

        // TODO HANDLE INDEX
        childNode.getValue().Index = arg_node.getValue().Index + ": 1";

        // Create an treeWidgetItem for our assembly
        childNode.getValue().treeWidgetItem = new QTreeWidgetItem();
        childNode.getValue().treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

        // Set Text of treeWidgetItem to Name member of childNode
        childNode.getValue().treeWidgetItem->setText(0, childNode.getValue().Name + " (" + childNode.getValue().Index + ")");

        // Add assembly treeWidgetItem to its parent treeWidgetItem
        arg_node.getValue().treeWidgetItem->addChild(childNode.getValue().treeWidgetItem);


        // Get location and TopoShape from arg_label and create an AIS_Shape from topoShape
        childNode.getValue().Location = shapeTool->GetLocation(childNode.getValue().Label);
        childNode.getValue().topoShape = shapeTool->GetShape(childNode.getValue().Label);
        childNode.getValue().shape = new AIS_Shape(childNode.getValue().topoShape);

        // Get color (Dont worry it will not override the childLabels colors unless want to display Assembly itself then it will call the assembly color)
        Quantity_Color col;
        bool result;
        result = colorTool->GetColor(childNode.getValue().Label, XCAFDoc_ColorGen, col);
        if (result) {
            childNode.getValue().color = col;
        }
        result = colorTool->GetColor(childNode.getValue().Label, XCAFDoc_ColorSurf, col);
        if (result) {
            childNode.getValue().color = col;
        }
        result = colorTool->GetColor(childNode.getValue().Label, XCAFDoc_ColorCurv, col);
        if (result) {
            childNode.getValue().color = col;
        }
        childNode.getValue().shape->SetColor(col);

        // Get components of an assembly to TDF_LabelSequence
        TDF_LabelSequence labelSequence;
        shapeTool->GetComponents(arg_label, labelSequence);

        // Do the same things for children of Assembly
        for (const TDF_Label& childLabel : labelSequence ) {
            childNode.addChild(getChildren(childNode, childLabel));
        }
        return childNode;
    }
    // If arg_label refers to another arg_label
    else if(shapeTool->IsReference(arg_label)){

        // Create an local TDataStd_Name to store name attribute from label
        Handle(TDataStd_Name) nameAttr;
        // Get the name attribute from label and copy it to nameAttr (Created Above)
        arg_label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
        // Convert nameAttr to QString
        qDebug() << "Found Reference Label: " <<  QString::fromStdString(toString(nameAttr->Get())) << "will look for its referredLabel label";

        // Create an local TDF_Label to store referred label
        TDF_Label referredLabel;
        // get the referred Label from arg_label
        shapeTool->GetReferredShape(arg_label, referredLabel);
        // do not initialize the childNode, pass the parent arg_node (arg_node) again with referredLabel arg_label
        return getChildren(arg_node, referredLabel);
    }
    // If arg_label is neither Assembly nor Reference (Like actual Part !! Hurrahhh)
    else{

        // Create an empty OCCData struct
        OCCData occData;
        // Construct a arg_node with empty OCCData struct above
        lstdTreeNode<OCCData> childNode(occData);

        // Initialize childNode arg_label with given arg_label
        childNode.getValue().Label = arg_label;

        // Create an local TDataStd_Name to store name attribute from label
        Handle(TDataStd_Name) nameAttr;
        // Get the name attribute from label and copy it to nameAttr (Created Above)
        childNode.getValue().Label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
        // Since OCCData struct "Name" member is QString convert nameAttr to QString and store it
        childNode.getValue().Name = QString::fromStdString(toString(nameAttr->Get()));
        qDebug() << "Part: " << childNode.getValue().Name;

        // TODO HANDLE INDEX
        childNode.getValue().Index = arg_node.getValue().Index + ": 1";

        // Create an treeWidgetItem for our part
        childNode.getValue().treeWidgetItem = new QTreeWidgetItem();
        childNode.getValue().treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
        // Set Text of treeWidgetItem to Name member of childNode
        childNode.getValue().treeWidgetItem->setText(0, childNode.getValue().Name + " (" + childNode.getValue().Index + ")");
        // Add assembly treeWidgetItem to its parent treeWidgetItem
        arg_node.getValue().treeWidgetItem->addChild(childNode.getValue().treeWidgetItem);


        // Get location and TopoShape from arg_label and create an AIS_Shape from topoShape
        childNode.getValue().Location = shapeTool->GetLocation(childNode.getValue().Label);
        childNode.getValue().topoShape = shapeTool->GetShape(childNode.getValue().Label);
        childNode.getValue().shape = new AIS_Shape(childNode.getValue().topoShape);

        // Get color
        Quantity_Color col;
        bool result;
        result = colorTool->GetColor(childNode.getValue().Label, XCAFDoc_ColorGen, col);
        if (result) {
            childNode.getValue().color = col;
        }
        result = colorTool->GetColor(childNode.getValue().Label, XCAFDoc_ColorSurf, col);
        if (result) {
            childNode.getValue().color = col;
        }
        result = colorTool->GetColor(childNode.getValue().Label, XCAFDoc_ColorCurv, col);
        if (result) {
            childNode.getValue().color = col;
        }
        childNode.getValue().shape->SetColor(col);

        // Since it is an actual part You can Display it :)
        MainWindow::myViewerWidget->getContext()->Display(childNode.getValue().shape, false);

        // Dont forget to add its treeWidgetItem to Parent treeWidgetItem
        arg_node.getValue().treeWidgetItem->addChild(childNode.getValue().treeWidgetItem);

        return childNode;

    }

}
