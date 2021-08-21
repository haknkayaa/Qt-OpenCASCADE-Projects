#include <ShapeFix_Shape.hxx>
#include <Prs3d_PointAspect.hxx>
#include <XCAFPrs_Style.hxx>
#include <TopoDS.hxx>

#include <Message_ProgressIndicator.hxx>
#include <Transfer_TransientProcess.hxx>

#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>


#include "STEPProcessor.h"
#include <XCAFPrs_AISObject.hxx>
#include <TDF_ChildIterator.hxx>


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
STEPProcessor::STEPProcessor(QWidget *parent) {
    QString version = "0.0.1";
    emit signal_mal();
    qDebug() << "STEPProcessor version:" << version.toLocal8Bit();

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
        myWorkSession->MapReader()->SetProgress(nullptr);
        myProgressIndicator->NewScope(10, "Displaying shapes...");
    }

    modelTree = getRoot(readerDoc); // TREENODE

    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->EndScope();
    }
}

TreeNode<OCCData> STEPProcessor::getRoot(opencascade::handle<TDocStd_Document> doc) {

    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());//doc->Main() is the main TDF_Label
    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);

    // Create and local occData
    OCCData occData;
    // Construct Treenode with OCCData above
    TreeNode<OCCData> rootNode(occData);
    for (int i = 1; i <= rootLabels.Length(); ++i) {
        QApplication::processEvents();

        // Initialize the rootNode label
        rootNode.getValue().Label = rootLabels.Value(i);

        Handle(TDataStd_Name) nameAttr;
        if (rootNode.getValue().Label.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            rootNode.getValue().Name = QString::fromStdString(toString(nameAttr->Get()));
        }else{
            rootNode.getValue().Name = "Unknown";
        }

        rootNode.getValue().Index = QString::number(i);

        rootNode.getValue().treeWidgetItem = new QTreeWidgetItem();
        rootNode.getValue().treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
        rootNode.getValue().treeWidgetItem->setText(0, rootNode.getValue().Name + " (" + rootNode.getValue().Index + ") ");
        rootNode.getValue().treeWidgetItem->setText(1, "Geometry");

        rootNode.getValue().transparency = 1.0;


        rootNode.getValue().Location = shapeTool->GetLocation(rootNode.getValue().Label);

        rootNode.getValue().topoShape = shapeTool->GetShape(rootNode.getValue().Label).Located(rootNode.getValue().Location);

        rootNode.getValue().shape = new XCAFPrs_AISObject(rootNode.getValue().Label);
        rootNode.getValue().shape->SetLocalTransformation(rootNode.getValue().topoShape.Location().Transformation());

        rootNode.getValue().object = rootNode.getValue().shape;

//        MainWindow::getModelTree()->insertTopLevelItem(0, rootNode.getValue().treeWidgetItem);
        emit signal_treeItemAdd(rootNode.getValue().treeWidgetItem);

        if (shapeTool->IsAssembly(rootNode.getValue().Label)) {
            QApplication::processEvents();
            TopoDS_Iterator iterator(rootNode.getValue().topoShape, true, true);
            for(iterator; iterator.More(); iterator.Next()){

                rootNode.addChild(getChildren(rootNode, iterator.Value()));
            }
        }

    }
    return rootNode;
}
TreeNode<OCCData> STEPProcessor::getChildren(TreeNode<OCCData> arg_node, TopoDS_Shape arg_shape) {
    QApplication::processEvents();

    // Create an empty OCCData struct
    OCCData occData;
    // Construct a arg_node with empty OCCData struct above
    TreeNode<OCCData> childNode(occData, &arg_node);

    childNode.getValue().Label = shapeTool->FindShape(arg_shape);

    // Create an local TDataStd_Name to store name attribute from label
    Handle(TDataStd_Name) nameAttr;
    // Get the name attribute from label and copy it to nameAttr (Created Above)
    childNode.getValue().Label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
    // Since OCCData struct "Name" member is QString convert nameAttr to QString and store it
    childNode.getValue().Name = QString::fromStdString(toString(nameAttr->Get()));


    // TODO HANDLE INDEX
    childNode.getValue().Index = arg_node.getValue().Index + ":" + QString::number(arg_node.getChildren().size() + 1);

    // Create an treeWidgetItem for our assembly
    childNode.getValue().treeWidgetItem = new QTreeWidgetItem();
    childNode.getValue().treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

    // Set Text of treeWidgetItem to Name member of childNode
    childNode.getValue().treeWidgetItem->setText(0, childNode.getValue().Name + " (" + childNode.getValue().Index + ")");
    childNode.getValue().treeWidgetItem->setText(1, "Geometry");
    // Add assembly treeWidgetItem to its parent treeWidgetItem
    arg_node.getValue().treeWidgetItem->addChild(childNode.getValue().treeWidgetItem);

    childNode.getValue().transparency = 1.0;

    childNode.getValue().Location = shapeTool->GetLocation(childNode.getValue().Label);
    childNode.getValue().topoShape = arg_shape;
    childNode.getValue().shape = new XCAFPrs_AISObject(childNode.getValue().Label);
    childNode.getValue().shape->SetLocalTransformation(childNode.getValue().topoShape.Location().Transformation());
    childNode.getValue().object = childNode.getValue().shape;

    if (shapeTool->IsAssembly(childNode.getValue().Label)){
        TopoDS_Iterator iterator(childNode.getValue().topoShape, true, true);
        for (iterator ; iterator.More() ; iterator.Next()) {
            childNode.addChild(getChildren(childNode, iterator.Value()));
        }
    } else{

        emit signal_displayShape(childNode.getValue().shape);
//        MainWindow::getViewerWidget()->getContext()->Display(childNode.getValue().shape, false);

    }
    return childNode;
}
