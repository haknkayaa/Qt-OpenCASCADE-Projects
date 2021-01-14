#include "STEPProcessor.h"

QString nameMethod;

string toString(const TCollection_ExtendedString &extstr) {
    char *str = new char[extstr.LengthOfCString() + 1];
    extstr.ToUTF8CString(str);
    std::string text(str);
    delete[] str;
    return text;
}


/** STEPProcessor sınıfının kurucu fonksiyonu
 */
STEPProcessor::STEPProcessor(QString arg_filename) {
    QString version = "0.0.1";

    qDebug() << "STEPProcessor version:" << version.toLocal8Bit();

    loadSTEPFile(arg_filename);
}

/** STEP formatındaki dosyasının ilk defa yüklenmesi
 *
 * @param arg_filename: Yüklenecek step dosyasının path tanımı.
 * Örnek: "/home/hakan/Desktop/STEPS/test.step"
 */
void STEPProcessor::loadSTEPFile(QString arg_filename) {

    QString filename;


    qDebug() << "Dosya açılıyor... " << arg_filename;

    STEPCAFControl_Reader reader;

    reader.SetColorMode(true);
    reader.SetNameMode(true);
    reader.SetMatMode(true);

    // dosyanın başarılı bir şekilde açılıp açılmadığı kontrolü
    if (reader.ReadFile(arg_filename.toUtf8().constData()) != IFSelect_RetDone) {
        qDebug() << "Hata! STEP dosyası açılamadı";
    }

    //
    Handle(TDocStd_Document) readerDoc = new TDocStd_Document("StepReader");

    if (!reader.Transfer(readerDoc)) {
        qDebug() << "Hata! Dosya aktarılamadı.";
    }

//    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(readerDoc->Main());
//
//    TDF_LabelSequence shapes;
//    shapeTool->GetShapes(shapes);
//
//    TDF_LabelSequence freeShapes;
//    shapeTool->GetFreeShapes(freeShapes);
//
//    qDebug() << "GetShapes():" << shapes.Length() << "GetFreeShapes():" << freeShapes.Length();
//
//
//    for (int i = 1; i <= shapes.Length(); i++) {
//
//        qDebug() << "shapes" << i << endl
//                 << "shapes components ";
//
//        TopoDS_Shape shape = shapeTool->GetShape(shapes.Value(i));
//        Handle(AIS_Shape) ais_shape = new AIS_Shape(shape);
//
//        //get a label of shape.
//        TDF_Label aLabel;
//        aLabel = shapeTool->FindShape(shape);
//
//        Handle(XCAFDoc_ColorTool) myColors = XCAFDoc_DocumentTool::ColorTool(readerDoc->Main());
//        TDF_LabelSequence ColLabels;
//        myColors->GetColors(ColLabels);
//
//        Quantity_Color col;
//        bool result;
//        result = myColors->GetColor(aLabel, XCAFDoc_ColorGen, col);
//        if (result) {
//            ais_shape->SetColor(col);
//        }
//        result = myColors->GetColor(aLabel, XCAFDoc_ColorSurf, col);
//        if (result) {
//            ais_shape->SetColor(col);
//        }
//        result = myColors->GetColor(aLabel, XCAFDoc_ColorCurv, col);
//        if (result) {
//            ais_shape->SetColor(col);
//        }
//
//
//
//        // display
////        MainWindow::myViewerWidget->getContext()->Display(ais_shape, 0);
////        MainWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
//    }


    // Global define
    //vector<AssemblyNode> roots;

    modelTree = GetRootsFromDocument(readerDoc);

    dumpModelTree(modelTree);

    addTreeWidget(modelTree);

    displayShapes(modelTree);

}

/** STEP file yüklendikten sonta transfer edilen DOC değişkenindeki root şekilleri yakalar
 *
 * @param doc : işlenecek doc file
 * @return :
 */
vector<AssemblyNode> STEPProcessor::GetRootsFromDocument(Handle(TDocStd_Document) doc) {

    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);


    vector<AssemblyNode> roots;

    for (int i = 1; i <= rootLabels.Length(); ++i) {
        TDF_Label rootLabel = rootLabels.Value(i);

        shared_ptr<AssemblyNode> root = make_shared<AssemblyNode>();

        Handle(TDataStd_Name) nameAttr;
        if (rootLabel.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
            //root->Name = reinterpret_cast<const wchar_t *>(nameAttr->Get().ToExtString());
            root->Name = toString(nameAttr->Get()).c_str();
        }

        root->Label = rootLabel;
        root->Parent = root;

        root->treeWidgetItem = new QTreeWidgetItem();
        root->treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

        root->Index = QString::number(i);
        QString name = root->Name + " (" + root->Index + ") ";
        root->treeWidgetItem->setText(0, name);

        root->transparency = 1.0;


        TopoDS_Shape shape = shapeTool->GetShape(rootLabel);
        root->shape = new AIS_Shape(shape);

        root->Children = GetChildren(root, shapeTool, colorTool, rootLabel);
        roots.push_back(*root);

//        for(TopoDS_Iterator it(shape); it.More(); it.Next()){
//            qDebug() << "Şekil bulundu";
//
//
//
//            TDF_Label aLabel;
//            aLabel = shapeTool->FindShape(it.Value());
//            Handle(TDataStd_Name) shapeNameAttr;
//            if (aLabel.FindAttribute(TDataStd_Name::GetID(), shapeNameAttr)) {
//                //child->Name = reinterpret_cast<const wchar_t *>(shapeNameAttr->Get().ToExtString());
//                qDebug() << toString(shapeNameAttr->Get()).c_str();
//            } else {
//                //child->Name = L"[Unnamed]";
//                //child->Name = "Unnamed";
//            }
//
//            for(TopoDS_Iterator it2(it.Value()); it2.More(); it2.Next()){
//                TDF_Label aLabel;
//                aLabel = shapeTool->FindShape(it.Value());
//                Handle(TDataStd_Name) shapeNameAttr;
//                if (aLabel.FindAttribute(TDataStd_Name::GetID(), shapeNameAttr)) {
//                    //child->Name = reinterpret_cast<const wchar_t *>(shapeNameAttr->Get().ToExtString());
//                    qDebug() << "---" <<toString(shapeNameAttr->Get()).c_str();
//                } else {
//                    //child->Name = L"[Unnamed]";
//                    //child->Name = "Unnamed";
//                }
//            }
//        }

    }

    return roots;
}

/** DOC içindeki alt şekilleri yinemeli olarak tarar ve değişkene atar.
 *
 *  @param parent     :
 *  @param shapeTool  :
 *  @param colorTool  :
 *  @param parentLabel:
 *  @return vector<AssemblyNode> :
 */
vector<AssemblyNode> STEPProcessor::GetChildren(const std::shared_ptr<AssemblyNode> &parent,
                                                const Handle(XCAFDoc_ShapeTool) &shapeTool,
                                                const Handle(XCAFDoc_ColorTool) &colorTool,
                                                const TDF_Label &parentLabel) {

    TDF_LabelSequence components;

    vector<AssemblyNode> children;

    if (shapeTool->GetComponents(parentLabel, components, Standard_False)) {

        for (Standard_Integer compIndex = 1; compIndex <= components.Length(); ++compIndex) {
            auto child = make_shared<AssemblyNode>();

            TDF_Label componentLabel = components.Value(compIndex);

            TDF_Label shapeLabel;
            if (!shapeTool->GetReferredShape(componentLabel, shapeLabel)) {
                shapeLabel = componentLabel;
            }

            Handle(TDataStd_Name) shapeNameAttr;
            if (shapeLabel.FindAttribute(TDataStd_Name::GetID(), shapeNameAttr)) {
                //child->Name = reinterpret_cast<const wchar_t *>(shapeNameAttr->Get().ToExtString());
                child->Name = toString(shapeNameAttr->Get()).c_str();
            } else {
                //child->Name = L"[Unnamed]";
                child->Name = "Unnamed";
            }
            child->Label = componentLabel;
            child->Parent = parent;



            //treewidget
            child->treeWidgetItem = new QTreeWidgetItem();
            child->treeWidgetItem->setIcon(0, QIcon(":/icons/part.png"));

            child->Index = child->Parent->Index + ":" + QString::number(compIndex);
            QString name = child->Name + " -> (" + child->Index + ")";
            child->treeWidgetItem->setText(0, name);
            child->transparency = 1.0;
            // shape
            TopoDS_Shape shape = shapeTool->GetShape(componentLabel);
            child->shape = new AIS_Shape(shape);


            // color
            TDF_Label aLabel;
            aLabel = shapeTool->FindShape(shape);

            Quantity_Color col;
            bool result;
            result = colorTool->GetColor(aLabel, XCAFDoc_ColorGen, col);
            if (result) {
                child->color = col;
            }
            result = colorTool->GetColor(aLabel, XCAFDoc_ColorSurf, col);
            if (result) {
                child->color = col;
            }
            result = colorTool->GetColor(aLabel, XCAFDoc_ColorCurv, col);
            if (result) {
                child->color = col;
            }

            child->shape->SetColor(col);

            child->Children = GetChildren(child, shapeTool, colorTool, shapeLabel);
            children.push_back(*child);

            //
//            { // Görselleştirme
//                TopoDS_Shape shape = shapeTool->GetShape(componentLabel);
//                Handle(AIS_Shape) ais_shape = new AIS_Shape(shape);
//
//                //get a label of shape.
//                TDF_Label aLabel;
//                aLabel = shapeTool->FindShape(shape);
//
//                TDF_LabelSequence ColLabels;
//                colorTool->GetColors(ColLabels);
//
//                Quantity_Color col;
//                bool result;
//                result = colorTool->GetColor(aLabel, XCAFDoc_ColorGen, col);
//                if (result) {
//                    ais_shape->SetColor(col);
//                }
//                result = colorTool->GetColor(aLabel, XCAFDoc_ColorSurf, col);
//                if (result) {
//                    ais_shape->SetColor(col);
//                }
//                result = colorTool->GetColor(aLabel, XCAFDoc_ColorCurv, col);
//                if (result) {
//                    ais_shape->SetColor(col);
//                }
//
//                ais_shape->SetColor(col);
//
//                // display
//                MainWindow::myViewerWidget->getContext()->Display(ais_shape, 0);
//                MainWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
//
//            } // Son Görselleştirme

        }
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

void STEPProcessor::displayShapes(vector<AssemblyNode> arg_modelTree) {
    for (int i = 0; i < arg_modelTree.size(); ++i) {
//        MainWindow::myViewerWidget->getContext()->SetColor(arg_modelTree[i].shape, arg_modelTree[i].color, true);
        MainWindow::myViewerWidget->getContext()->Display(arg_modelTree[i].shape, 0);
        MainWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
        MainWindow::myViewerWidget->fitAll();

        if (arg_modelTree[i].Children.begin() != arg_modelTree[i].Children.end()) {
            displayShapes(arg_modelTree[i].Children);
        }

    }


}

