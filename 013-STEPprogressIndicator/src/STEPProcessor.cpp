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


    modelTree = getRoot(readerDoc);

    addTreeWidget(modelTree);

    countShapes(modelTree);

    displayShapes(modelTree);

    if(!myProgressIndicator.IsNull()){
        myProgressIndicator->EndScope();
    }
}

/**
 *
 * @param doc
 * @return
 */
vector<AssemblyNode> STEPProcessor::getRoot(Handle_TDocStd_Document doc) {
    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);

    vector<AssemblyNode> roots;

    for (int i = 1; i <= rootLabels.Length(); ++i) {
        qDebug() << i << ". şekil inceleniyor.";

        shared_ptr<AssemblyNode> root = make_shared<AssemblyNode>();

        // label çekme
        TDF_Label rootLabel = rootLabels.Value(i);
        root->Label = rootLabel;


        // isim çekme
        Handle(TDataStd_Name) nameAttr;
        if (rootLabel.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            root->Name = toString(nameAttr->Get()).c_str();
        }else{
            root->Name = "Unknown";
        }


        root->Parent = root;

        root->treeWidgetItem = new QTreeWidgetItem();
        root->treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

        root->Index = QString::number(i);
        QString name = root->Name + " (" + root->Index + ") ";
        root->treeWidgetItem->setText(0, name);

        root->transparency = 1.0;


        TopLoc_Location location = shapeTool->GetLocation(rootLabel);
        root->Location = location;

        TopoDS_Shape shape = shapeTool->GetShape(rootLabel);
        root->shape = new AIS_Shape(shape);
        root->topoShape = shape;

        // alt üyelerini bul
        if(shapeTool->IsAssembly(rootLabel)){
            qDebug()<< "Bu şekil bir montaj. Alt şekilleri incelenecek.";
            root->Children = getChildren(root);
        }

        roots.push_back(*root);
    }

    return roots;
}

/**
 *
 * @param parent
 * @return
 */
vector<AssemblyNode> STEPProcessor::getChildren(const std::shared_ptr<AssemblyNode> &parent) {

    vector<AssemblyNode> children;

    int iteratorIndex = 1;

    TopoDS_Iterator iterator(parent->topoShape, true, true);

    for (iterator; iterator.More(); iterator.Next()) {
        qDebug() << "Alt şekil bulundu. Ve ziyaret ediliyor.";

        auto child = make_shared<AssemblyNode>();


        TDF_Label subShapelabel = shapeTool->FindShape(iterator.Value());
        child->Label = subShapelabel;

        // bu şekilde olursa location bilgisi kayboluyor
        //TopoDS_Shape shape = shapeTool->GetShape(subShapelabel);

        const TopoDS_Shape& subShape = iterator.Value();


        child->shape = new AIS_Shape(subShape);

        Quantity_Color col;
        bool result;
        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorGen, col);
        if (result) {
            child->color = col;
        }
        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorSurf, col);
        if (result) {
            child->color = col;
        }
        result = colorTool->GetColor(subShapelabel, XCAFDoc_ColorCurv, col);
        if (result) {
            child->color = col;
        }

        for(TopExp_Explorer aExpShape(iterator.Value(), TopAbs_SHAPE); aExpShape.More(); aExpShape.Next()){
            for(TopExp_Explorer aExpSolid(aExpShape.Current(),TopAbs_FACE); aExpSolid.More(); aExpSolid.Next()){

                TopoDS_Face aFace = TopoDS::Face(aExpSolid.Current());

                result = colorTool->GetColor(aFace, XCAFDoc_ColorGen, col);
                if (result) {
                    child->color = col;
                }
                result = colorTool->GetColor(aFace, XCAFDoc_ColorCurv, col);
                if (result) {
                    child->color = col;
                }
                result = colorTool->GetColor(aFace, XCAFDoc_ColorSurf, col);
                if (result) {
                    child->color = col;
                }
            }
        }

        for(TopExp_Explorer aExpShape(iterator.Value(), TopAbs_SHELL); aExpShape.More(); aExpShape.Next()){
            for(TopExp_Explorer aExpSolid(aExpShape.Current(),TopAbs_FACE); aExpSolid.More(); aExpSolid.Next()){

                TopoDS_Face aFace = TopoDS::Face(aExpSolid.Current());

                result = colorTool->GetColor(aFace, XCAFDoc_ColorGen, col);
                if (result) {
                    child->color = col;
                }
                result = colorTool->GetColor(aFace, XCAFDoc_ColorCurv, col);
                if (result) {
                    child->color = col;
                }
                result = colorTool->GetColor(aFace, XCAFDoc_ColorSurf, col);
                if (result) {
                    child->color = col;
                }
            }
        }


        child->shape->SetColor(col);


        Handle(TDataStd_Name) shapeNameAttr;
        if (subShapelabel.FindAttribute(TDataStd_Name::GetID(), shapeNameAttr)) {
//            qDebug() << "Obje ismi " << toString(shapeNameAttr->Get()).c_str();
            child->Name = toString(shapeNameAttr->Get()).c_str();
        } else {
            child->Name = "Unknown sub-shape";
        }


        child->topoShape = iterator.Value();

        child->Index = parent->Index + ":" + QString::number(iteratorIndex);

        //treewidget
        child->treeWidgetItem = new QTreeWidgetItem();
        child->treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));
        child->treeWidgetItem->setText(0, child->Name + " (" + child->Index + ")");



        child->transparency = 1.0;

        child->Parent = parent;


        if(shapeTool->IsAssembly(child->Label)){
            qDebug() << "Sub shape bir montaj. Alt şekilleri incelenecek";
            child->Children = getChildren(child);
        }

        //child->Children = GetChildren(child, shapeTool, colorTool, shapeLabel);
        children.push_back(*child);
        iteratorIndex++;

    }

    return children;
}


/** Model ağacını print eden fonksiyon
 *
 * @param arg_modelTree
 */
void STEPProcessor::dumpModelTree(vector<AssemblyNode> arg_modelTree) {
    for (int i = 0; i < arg_modelTree.size(); ++i) {
        qDebug() << "** " << arg_modelTree[i].Name;

        if (arg_modelTree.begin() != arg_modelTree.end()) {
            dumpModelTree(arg_modelTree[i].Children);
        }
    }
}

/** Vektör dizisindeki QTreeWidgetItem'leri liste halinde yinelemeli şekilde elde eder.
 *
 * @param arg_modelTree : model tre
 * @return QList<QTreeWidgetItem *> : QTreeWidgetItem'lerinden oluşan QList döndürür
 */
QList<QTreeWidgetItem *> getTreeWidgetItemList(vector<AssemblyNode> arg_modelTree) {
    QList<QTreeWidgetItem *> items;

    // array boyunca iterasyon
    for (int i = 0; i < arg_modelTree.size(); ++i) {

        // şuan andaki itemi al
        auto item = arg_modelTree[i].treeWidgetItem;

        // eğer itemin alt üyeleri var ise onları da child olarak ekle
        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
            auto child = getTreeWidgetItemList(arg_modelTree[i].Children);

            for (int j = 0; j < child.size(); ++j) {
                item->addChild(child[j]);
            }
        }

        // itemi listeyi ekle
        items.append(item);
    }
    return items;
}

/** Parametre olarak verilen vektör dizisini iterasyona sokarak
 *  QTreeWidget'a item olarak ekler.
 *
 * @param arg_modelTree : işlenecek vektör dizisi
 */
void STEPProcessor::addTreeWidget(vector<AssemblyNode> arg_modelTree) {
    QList<QTreeWidgetItem *> items;

    auto list = getTreeWidgetItemList(arg_modelTree);

    for (int i = 0; i < list.size(); ++i) {
        items.append(list[i]);
    }

    MainWindow::modelTreeWidget->insertTopLevelItems(0, items);
}

/**
 *
 * @param arg_modelTree
 */
void STEPProcessor::displayShapes(vector<AssemblyNode> arg_modelTree) {
    for (int i = 0; i < arg_modelTree.size(); ++i) {


        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
            displayShapes(arg_modelTree[i].Children);
        }else{
            MainWindow::myViewerWidget->getContext()->Display(arg_modelTree[i].shape, 0);
            MainWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
            MainWindow::myViewerWidget->fitAll();

            ProgressOfDisplay++;
            if(ProgressOfDisplay >= shapeCounter/10){
                myProgressDialog->setValue(myProgressDialog->value() + 1);
                ProgressOfDisplay = 0;
            }
        }
    }
}

/**
 *
 * @param arg_modelTree
 */
void STEPProcessor::countShapes(vector<AssemblyNode> arg_modelTree) {

    for (int i = 0; i < arg_modelTree.size(); ++i) {

        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
            countShapes(arg_modelTree[i].Children);
        }else{
            shapeCounter++;
        }
    }
}
