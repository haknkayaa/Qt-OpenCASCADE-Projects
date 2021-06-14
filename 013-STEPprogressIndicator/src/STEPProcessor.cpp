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

int countTree(lstdTreeNode<AssemblyNode> node){

    int i = 0;
    for (lstdTreeNode<AssemblyNode> child : node.getChildren()) {
        i++;
    }
    return i;
}
void dumpTree(lstdTreeNode<AssemblyNode> node){
    for (lstdTreeNode<AssemblyNode> child : node.getChildren()) {

        if (countTree(child) > 0){

            dumpTree(child);

        }
        else{

            qDebug() << child.getValue().Name;
            MainWindow::myViewerWidget->getContext()->Display(child.getValue().shape, true);

        }
    }
}
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

    modelTree = getRoot(readerDoc);
//    modelTree.addChild(modelTree);
//
//    modelTree = getRoot(readerDoc);
//
//    addTreeWidget(modelTree);
//
//    countShapes(modelTree);
//
//    displayShapes(modelTree);

    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->EndScope();
    }
}

lstdTreeNode<AssemblyNode> STEPProcessor::getRoot(opencascade::handle<TDocStd_Document> doc) {

    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);


    AssemblyNode root;
    QApplication::processEvents();

    for (int i = 1; i <= rootLabels.Length(); ++i) {
        QApplication::processEvents();

        TDF_Label rootLabel = rootLabels.Value(i);
        root.Label = rootLabel;

        Handle(TDataStd_Name) nameAttr;
        if (rootLabel.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            root.Name = toString(nameAttr->Get()).c_str();
        }else{
            root.Name = "Unknown";
        }

        root.treeWidgetItem = new QTreeWidgetItem();
        root.treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

        root.Index = QString::number(i);
        QString name = root.Name + " (" + root.Index + ") ";
        root.treeWidgetItem->setText(0, name);

        root.transparency = 1.0;

        TopLoc_Location location = shapeTool->GetLocation(rootLabel);
        root.Location = location;

        TopoDS_Shape shape = shapeTool->GetShape(rootLabel);
        root.shape = new AIS_Shape(shape);
        root.topoShape = shape;


        lstdTreeNode<AssemblyNode> rootNode(root);

//        MainWindow::myViewerWidget->getContext()->Display(root->shape, true);
        MainWindow::modelTreeWidget->addTopLevelItem(rootNode.getValue().treeWidgetItem);

        qDebug() << rootNode.getValue().treeWidgetItem->text(0);
        if(shapeTool->IsAssembly(rootLabel)){
            QApplication::processEvents();

            deepBuildAssemblyTree(rootNode, rootLabel);
        }
        const int mal = 0;
//        dumpTree(rootNode);
    }

    return lstdTreeNode<AssemblyNode>();
}

lstdTreeNode<AssemblyNode> STEPProcessor::getChildren(lstdTreeNode<AssemblyNode> node) {
//
//    int iteratorIndex = 0;
//    AssemblyNode child;
//    lstdTreeNode<AssemblyNode> childNode(child);
//    TDF_ChildIterator childIterator(node.getValue().Label);
//
//    for (childIterator; childIterator.More() ; childIterator.Next()) {
//        QApplication::processEvents();
//        qDebug() << "Alt şekil bulundu. Ve ziyaret ediliyor.";
//
//        childIterator.Value().
//
//
//    }


//
//    TopoDS_Iterator iterator(node.getValue().topoShape, true, true);

//
//    for (iterator; iterator.More(); iterator.Next()) {
//        QApplication::processEvents();
//
//        qDebug() << "Alt şekil bulundu. Ve ziyaret ediliyor.";
//
//        TDF_Label subShapelabel = shapeTool->FindShape(iterator.Value());
//        childNode.getValue().Label = subShapelabel;
//        const TopoDS_Shape& subShape = iterator.Value();
//
//        childNode.getValue().shape = new AIS_Shape(subShape);
//        Quantity_Color col;
//        bool result;
//        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorGen, col);
//        if (result) {
//            childNode.getValue().color = col;
//        }
//        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorSurf, col);
//        if (result) {
//            childNode.getValue().color = col;
//        }
//        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorCurv, col);
//        if (result) {
//            childNode.getValue().color = col;
//        }
//
//        childNode.getValue().shape->SetColor(col);
//        Handle(TDataStd_Name) shapeNameAttr;
//        if (subShapelabel.FindAttribute(TDataStd_Name::GetID(), shapeNameAttr)) {
////            qDebug() << "Obje ismi " << toString(shapeNameAttr->Get()).c_str();
//            childNode.getValue().Name = toString(shapeNameAttr->Get()).c_str();
//        } else {
//            childNode.getValue().Name = "Unknown sub-shape";
//        }
//
//        childNode.getValue().topoShape = iterator.Value();
//
//        childNode.getValue().Index = node.getValue().Index + ":" + QString::number(iteratorIndex);
//
//        //treewidget
//        childNode.getValue().treeWidgetItem = new QTreeWidgetItem();
//        childNode.getValue().treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
//        childNode.getValue().treeWidgetItem->setText(0, childNode.getValue().Name + " (" + childNode.getValue().Index + ")");
//
//        childNode.getValue().transparency = 1.0;
//
//        node.getValue().treeWidgetItem->addChild(childNode.getValue().treeWidgetItem);
//        qDebug() << childNode.getValue().Name;
//
//        if(shapeTool->IsAssembly(childNode.getValue().Label)){
//            qDebug() << "Sub shape bir montaj. Alt şekilleri incelenecek";
//            QApplication::processEvents();
//
//            childNode.addChild(getChildren(childNode));
//        } else{
//            MainWindow::myViewerWidget->getContext()->Display(childNode.getValue().shape, true);
//        }
//
//    }
//    return childNode;
//
}

lstdTreeNode<AssemblyNode> STEPProcessor::deepBuildAssemblyTree(lstdTreeNode<AssemblyNode> node, const TDF_Label &label) {
    QApplication::processEvents();

    Handle(TDataStd_Name) nameAttr;
    label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
    qDebug() << "sufuk1 " << QString::fromStdString(toString(nameAttr->Get()).c_str());

    AssemblyNode assemblyNode;
    assemblyNode.Label = label;
    assemblyNode.Name = QString::fromStdString(toString(nameAttr->Get()).c_str());
    assemblyNode.treeWidgetItem = new QTreeWidgetItem();
    assemblyNode.treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
    QString name = assemblyNode.Name;
    assemblyNode.treeWidgetItem->setText(0, name);

    node.getValue().treeWidgetItem->addChild(assemblyNode.treeWidgetItem);
    lstdTreeNode<AssemblyNode> childNode(assemblyNode);

    TDF_LabelSequence seq;
    shapeTool->GetComponents(label, seq);

    for (const TDF_Label& child : seq ) {
        QApplication::processEvents();
        Handle(TDataStd_Name) nameAttr;
        child.FindAttribute(TDataStd_Name::GetID(), nameAttr);
        if (shapeTool->IsAssembly(child)){
            // TODO buraya girmiyor

            qDebug() << "sufuk Assembly:: " << QString::fromStdString(toString(nameAttr->Get()).c_str());
            childNode.addChild(deepBuildAssemblyTree(childNode, child));
        }
        else if (shapeTool->IsReference(child)) {
            TDF_Label referred;
            shapeTool->GetReferredShape(child, referred);
            qDebug() << "sufuk Reference:: " << QString::fromStdString(toString(nameAttr->Get()).c_str());
            TopLoc_Location location = shapeTool->GetLocation(referred);
            TopoDS_Shape shape = shapeTool->GetShape(referred);
            childNode.getValue().shape = new AIS_Shape(shape);
            qDebug() << "Displaying " << QString::fromStdString(toString(nameAttr->Get()).c_str());
            MainWindow::myViewerWidget->getContext()->Display(childNode.getValue().shape, false);

            childNode.addChild(deepBuildAssemblyTree(childNode, referred));
        }
        else{
            // TODO buraya girmiyor
            qDebug() << "sufuk part:: " << QString::fromStdString(toString(nameAttr->Get()).c_str());
//            MainWindow::myViewerWidget->getContext()->Display(childNode.getValue().shape, false);

        }
//        TopoDS_Shape shape = shapeTool->GetShape(child);
//        childNode.getValue().shape = new AIS_Shape(shape);
//        MainWindow::myViewerWidget->getContext()->Display(childNode.getValue().shape, true);
//        childNode.getValue().shape.Is
    }
    return childNode;
}

void STEPProcessor::dumpAssemblyTree(const TDF_Label& label) {
    Handle(TDataStd_Name) nameAttr;
    label.FindAttribute(TDataStd_Name::GetID(), nameAttr);
    qDebug() << "sufuk1 " << QString::fromStdString(toString(nameAttr->Get()).c_str());

    AssemblyNode assemblyNode;
    assemblyNode.Label = label;
    assemblyNode.Name = QString::fromStdString(toString(nameAttr->Get()).c_str());
    assemblyNode.treeWidgetItem = new QTreeWidgetItem();
    assemblyNode.treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
    QString name = assemblyNode.Name;
    assemblyNode.treeWidgetItem->setText(0, name);
    QApplication::processEvents();
    TDF_LabelSequence seq;
    shapeTool->GetComponents(label, seq);
    for (const TDF_Label& child : seq ) {
        QApplication::processEvents();

//           node.addChild(assemblyNode);
        Handle(TDataStd_Name) nameAttr;
        child.FindAttribute(TDataStd_Name::GetID(), nameAttr);
        if (shapeTool->IsAssembly(child)){
            qDebug() << "sufuk Assembly:: " << QString::fromStdString(toString(nameAttr->Get()).c_str());
            dumpAssemblyTree(child);
        }
        else if (shapeTool->IsReference(child)) {
            TDF_Label referred;
            shapeTool->GetReferredShape(child, referred);
//            qDebug() << "sufuk Reference:: " << QString::fromStdString(toString(nameAttr->Get()).c_str());
            dumpAssemblyTree(referred);

        }
        else{
            qDebug() << "sufuk part:: " << QString::fromStdString(toString(nameAttr->Get()).c_str());

        }
    }
}



/**
 *
 * @param doc
 * @return
 */
//vector<AssemblyNode> STEPProcessor::getRoot(Handle_TDocStd_Document doc) {
//    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
//    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());
//
//    TDF_LabelSequence rootLabels;
//    shapeTool->GetFreeShapes(rootLabels);
//
//    vector<AssemblyNode> roots;
//
//    for (int i = 1; i <= rootLabels.Length(); ++i) {
//        qDebug() << i << ". şekil inceleniyor.";
//
//        shared_ptr<AssemblyNode> root = make_shared<AssemblyNode>();
//
//        // label çekme
//        TDF_Label rootLabel = rootLabels.Value(i);
//        root->Label = rootLabel;
//
//
//        // isim çekme
//        Handle(TDataStd_Name) nameAttr;
//        if (rootLabel.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
//            root->Name = toString(nameAttr->Get()).c_str();
//        }else{
//            root->Name = "Unknown";
//        }
//
//
//        root->Parent = root;
//
//        root->treeWidgetItem = new QTreeWidgetItem();
//        root->treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
//
//        root->Index = QString::number(i);
//        QString name = root->Name + " (" + root->Index + ") ";
//        root->treeWidgetItem->setText(0, name);
//
//        root->transparency = 1.0;
//
//
//        TopLoc_Location location = shapeTool->GetLocation(rootLabel);
//        root->Location = location;
//
//        TopoDS_Shape shape = shapeTool->GetShape(rootLabel);
//        root->shape = new AIS_Shape(shape);
//        root->topoShape = shape;
//
//        // alt üyelerini bul
//        if(shapeTool->IsAssembly(rootLabel)){
//            qDebug()<< "Bu şekil bir montaj. Alt şekilleri incelenecek.";
//            root->Children = getChildren(root);
//        }
//
//        roots.push_back(*root);
//    }
//
//    return roots;
//}

/**
 *
 * @param parent
 * @return
 */
//vector<AssemblyNode> STEPProcessor::getChildren(const std::shared_ptr<AssemblyNode> &parent) {
//
//    vector<AssemblyNode> children;
//
//    int iteratorIndex = 1;
//
//    TopoDS_Iterator iterator(parent->topoShape, true, true);
//
//    for (iterator; iterator.More(); iterator.Next()) {
//        qDebug() << "Alt şekil bulundu. Ve ziyaret ediliyor.";
//
//        auto child = make_shared<AssemblyNode>();
//
//
//        TDF_Label subShapelabel = shapeTool->FindShape(iterator.Value());
//        child->Label = subShapelabel;
//
//        // bu şekilde olursa location bilgisi kayboluyor
//        //TopoDS_Shape shape = shapeTool->GetShape(subShapelabel);
//
//        const TopoDS_Shape& subShape = iterator.Value();
//
//
//        child->shape = new AIS_Shape(subShape);
//
//        Quantity_Color col;
//        bool result;
//        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorGen, col);
//        if (result) {
//            child->color = col;
//        }
//        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorSurf, col);
//        if (result) {
//            child->color = col;
//        }
//        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorCurv, col);
//        if (result) {
//            child->color = col;
//        }
//
//        for(TopExp_Explorer aExpShape(iterator.Value(), TopAbs_SHAPE); aExpShape.More(); aExpShape.Next()){
//            for(TopExp_Explorer aExpSolid(aExpShape.Current(),TopAbs_FACE); aExpSolid.More(); aExpSolid.Next()){
//
//                TopoDS_Face aFace = TopoDS::Face(aExpSolid.Current());
//
//                result = colorTool->GetColor(aFace, XCAFDoc_ColorGen, col);
//                if (result) {
//                    child->color = col;
//                }
//                result = colorTool->GetColor(aFace, XCAFDoc_ColorCurv, col);
//                if (result) {
//                    child->color = col;
//                }
//                result = colorTool->GetColor(aFace, XCAFDoc_ColorSurf, col);
//                if (result) {
//                    child->color = col;
//                }
//            }
//        }
//
//        for(TopExp_Explorer aExpShape(iterator.Value(), TopAbs_SHELL); aExpShape.More(); aExpShape.Next()){
//            for(TopExp_Explorer aExpSolid(aExpShape.Current(),TopAbs_FACE); aExpSolid.More(); aExpSolid.Next()){
//
//                TopoDS_Face aFace = TopoDS::Face(aExpSolid.Current());
//
//                result = colorTool->GetColor(aFace, XCAFDoc_ColorGen, col);
//                if (result) {
//                    child->color = col;
//                }
//                result = colorTool->GetColor(aFace, XCAFDoc_ColorCurv, col);
//                if (result) {
//                    child->color = col;
//                }
//                result = colorTool->GetColor(aFace, XCAFDoc_ColorSurf, col);
//                if (result) {
//                    child->color = col;
//                }
//            }
//        }
//
//
//        child->shape->SetColor(col);
//
//
//        Handle(TDataStd_Name) shapeNameAttr;
//        if (subShapelabel.FindAttribute(TDataStd_Name::GetID(), shapeNameAttr)) {
////            qDebug() << "Obje ismi " << toString(shapeNameAttr->Get()).c_str();
//            child->Name = toString(shapeNameAttr->Get()).c_str();
//        } else {
//            child->Name = "Unknown sub-shape";
//        }
//
//
//        child->topoShape = iterator.Value();
//
//        child->Index = parent->Index + ":" + QString::number(iteratorIndex);
//
//        //treewidget
//        child->treeWidgetItem = new QTreeWidgetItem();
//        child->treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
//        child->treeWidgetItem->setText(0, child->Name + " (" + child->Index + ")");
//
//
//
//        child->transparency = 1.0;
//
//        child->Parent = parent;
//
//
//        if(shapeTool->IsAssembly(child->Label)){
//            qDebug() << "Sub shape bir montaj. Alt şekilleri incelenecek";
//            child->Children = getChildren(child);
//        }
//
//        //child->Children = GetChildren(child, shapeTool, colorTool, shapeLabel);
//        children.push_back(*child);
//        iteratorIndex++;
//
//    }
//
//    return children;
//}


/** Model ağacını print eden fonksiyon
 *
 * @param arg_modelTree
 */
//void STEPProcessor::dumpModelTree(vector<AssemblyNode> arg_modelTree) {
//    for (int i = 0; i < arg_modelTree.size(); ++i) {
//        qDebug() << "** " << arg_modelTree[i].Name;
//
//        if (arg_modelTree.begin() != arg_modelTree.end()) {
//            dumpModelTree(arg_modelTree[i].Children);
//        }
//    }
//}

/** Vektör dizisindeki QTreeWidgetItem'leri liste halinde yinelemeli şekilde elde eder.
 *
 * @param arg_modelTree : model tre
 * @return QList<QTreeWidgetItem *> : QTreeWidgetItem'lerinden oluşan QList döndürür
 */
//QList<QTreeWidgetItem *> getTreeWidgetItemList(vector<AssemblyNode> arg_modelTree) {
//    QList<QTreeWidgetItem *> items;
//
//    // array boyunca iterasyon
//    for (int i = 0; i < arg_modelTree.size(); ++i) {
//
//        // şuan andaki itemi al
//        auto item = arg_modelTree[i].treeWidgetItem;
//
//        // eğer itemin alt üyeleri var ise onları da child olarak ekle
//        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
//            auto child = getTreeWidgetItemList(arg_modelTree[i].Children);
//
//            for (int j = 0; j < child.size(); ++j) {
//                item->addChild(child[j]);
//            }
//        }
//
//        // itemi listeyi ekle
//        items.append(item);
//    }
//    return items;
//}

/** Parametre olarak verilen vektör dizisini iterasyona sokarak
 *  QTreeWidget'a item olarak ekler.
 *
 * @param arg_modelTree : işlenecek vektör dizisi
 */
//void STEPProcessor::addTreeWidget(vector<AssemblyNode> arg_modelTree) {
//    QList<QTreeWidgetItem *> items;
//
//    auto list = getTreeWidgetItemList(arg_modelTree);
//
//    for (int i = 0; i < list.size(); ++i) {
//        items.append(list[i]);
//    }
//
//    MainWindow::modelTreeWidget->insertTopLevelItems(0, items);
//}

/**
 *
 * @param arg_modelTree
 */
//void STEPProcessor::displayShapes(vector<AssemblyNode> arg_modelTree) {
//    for (int i = 0; i < arg_modelTree.size(); ++i) {
//
//
//        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
//            displayShapes(arg_modelTree[i].Children);
//        }else{
//            MainWindow::myViewerWidget->getContext()->Display(arg_modelTree[i].shape, 0);
//            MainWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
//            MainWindow::myViewerWidget->fitAll();
//
//            ProgressOfDisplay++;
//            if(ProgressOfDisplay >= shapeCounter/10){
//                myProgressDialog->setValue(myProgressDialog->value() + 1);
//                ProgressOfDisplay = 0;
//            }
//        }
//    }
//}

/**
 *
 * @param arg_modelTree
 */
//void STEPProcessor::countShapes(vector<AssemblyNode> arg_modelTree) {
//
//    for (int i = 0; i < arg_modelTree.size(); ++i) {
//
//        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
//            countShapes(arg_modelTree[i].Children);
//        }else{
//            shapeCounter++;
//        }
//    }
//}
