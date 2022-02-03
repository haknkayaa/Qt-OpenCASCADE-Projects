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
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepGProp.hxx>

// Qt Libraries
#include <QtWidgets>
#include <iostream>
#include <QSet>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

// User definitions
#include "STEPProcessor.h"

int indexOfLineStartingWith(const QStringList &list, const QString &textToFind) {
    return list.indexOf(QRegExp("^" + QRegExp::escape(textToFind) + ".+"));
}

OCCData *getNodeData(QTreeWidgetItem *arg_item) {
    return qvariant_cast<OCCData *>(arg_item->data(0, Qt::UserRole));
}

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
    QString version = "0.3.0";
    qDebug() << "STEPProcessor version:" << version.toLocal8Bit();


    boundryBox = Bnd_Box();
}

/// Starts the reading given STEP file.
/// \param arg_filePath location of STEP file.
void STEPProcessor::loadSTEPFile(const QString &arg_filePath) {
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
    myProgressDialog->setValue(0);
    connect(myProgressDialog, &QProgressDialog::finished, myProgressDialog, &QProgressDialog::close);

    // Item = Geometry -> STEP
    stepFileItem = new QTreeWidgetItem();
    stepFileItem->setIcon(0, QIcon(":/icons/stepfile.png"));
    stepFileItem->setText(0, "STEP File: " + arg_filePath);
//    ProjectWindow::mainItem_geometry->addChild(stepFileItem);

    // Item = Geometry -> STEP -> Properties
    propertiesItem = new QTreeWidgetItem();
    propertiesItem->setIcon(0, QIcon(":/icons/scroll.png"));
    propertiesItem->setText(0, "Properties");
    propertiesItem->setText(1, "Properties");
    stepFileItem->addChild(propertiesItem);

    // Item = Geometry -> STEP -> Properties -> World Box Size
    boundryItem = new QTreeWidgetItem();
    boundryItem->setIcon(0, QIcon(":/icons/worldbox.png"));
    boundryItem->setText(0, "World Box");
    boundryItem->setText(1, "WorldBox");
    boundryItem->setCheckState(0, Qt::Checked);
    propertiesItem->addChild(boundryItem);

    // Item = Geometry -> STEP -> Properties -> Beam Radius
    beamRadiusItem = new QTreeWidgetItem();
    beamRadiusItem->setIcon(0, QIcon(":/icons/worldbox.png"));
    beamRadiusItem->setText(0, "Source Radius");
    beamRadiusItem->setText(1, "BeamRadius");
    beamRadiusItem->setCheckState(0, Qt::Checked);
    propertiesItem->addChild(beamRadiusItem);

    // Item = Geometry -> STEP -> Hierarchy
    hierarchyItem = new QTreeWidgetItem();
    hierarchyItem->setIcon(0, QIcon(":/icons/scroll.png"));
    hierarchyItem->setText(0, "Hierarchy");
    hierarchyItem->setText(1, "Hierarchy");
    stepFileItem->addChild(hierarchyItem);

    propertiesItem->setExpanded(true);


    Handle(TDocStd_Document) readerDoc = reader(arg_filePath);
    modelTree = getRoot(readerDoc);
    hierarchyItem->addChild(modelTree);

    myProgressDialog->close();

    auto *messageBox = new QMessageBox(this->parentWidget());
    messageBox->setText(tr("Import finished successfully!"));
    messageBox->show();

    emit readyTreeWidgetItem(stepFileItem);

//    setWorldBox(0);
//    setBeamRadius(0);
}

/// Loads the step file, this is the first primitive method for reading.
/// \param arg_filename location of STEP file.
Handle_TDocStd_Document STEPProcessor::reader(const QString &arg_filename) {

    qDebug() << "File opening... " << arg_filename;

    STEPCAFControl_Reader reader;
    Handle_TDocStd_Document readerDoc;
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
    shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());

    TDF_LabelSequence rootLabels;
    shapeTool->GetFreeShapes(rootLabels);

    TDF_LabelSequence tempSequence;
    shapeTool->GetShapes(tempSequence);
    shapeCount = tempSequence.Length();

    progressIndicator = 1;

    OCCData *occData = new OCCData();
    QVariant variant;
    variant.setValue(occData);

    QTreeWidgetItem *rootNode = new QTreeWidgetItem();
    rootNode->setData(0, Qt::UserRole, variant);

    for (int i = 1; i <= rootLabels.Length(); ++i) {
        QApplication::processEvents();
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

        rootNode->setIcon(0, QIcon(":/icons/part.png"));
        rootNode->setText(0, occData->getName() + " (" + occData->getIndex() + ") ");
        rootNode->setText(1, "Geometry");
        rootNode->setCheckState(0, Qt::Checked);
        rootNode->setData(1, Qt::UserRole, QVariant(Qt::Checked));

        occData->setTransparency(1.0);
        occData->setMaterial("-");
        occData->setTopoShape(shapeTool->GetShape(occData->getLabel()));

        addBoundryBox(occData->getTopoShape());

        occData->setLocation(occData->getTopoShape().Location());

        NodeInteractive *nodeInteractive = new NodeInteractive(occData->getLabel(), rootNode);
        occData->setShape(nodeInteractive);
        occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());

        occData->setObject(nodeInteractive);

        GProp_GProps gprops;
        BRepGProp::VolumeProperties(occData->getTopoShape(), gprops);
        occData->setVolume(gprops.Mass());


        if (shapeTool->IsAssembly(getNodeData(rootNode)->getLabel())) {
            QApplication::processEvents();
            TopoDS_Iterator iterator(getNodeData(rootNode)->getTopoShape(), true,
                                     true);
            for (iterator; iterator.More(); iterator.Next()) {

                rootNode->addChild(getChildren(rootNode, iterator.Value()));
            }
        } else {
            shapeList.append(occData->getTopoShape());
            shapeLabelList.append(occData->getLabel());
            emit readyShapeForDisplay(getNodeData(rootNode)->getShape());
        }

//        emit readyTreeWidgetItem(rootNode);
    }

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
    occData->setName(QString::fromStdString(toString(nameAttr->Get())));
    occData->setName(nameControl(occData->getName()));
    occData->setIndex(getNodeData(arg_node)->getIndex() + ":" + QString::number(arg_node->childCount() + 1));

    // Create an treeWidgetItem for our assembly
    childNode->setIcon(0, QIcon(":/icons/part.png"));
    childNode->setText(0, occData->getName() + " (" + occData->getIndex() + ")");
    childNode->setText(1, "Geometry");
    childNode->setCheckState(0, Qt::Checked);
    childNode->setData(1, Qt::UserRole, QVariant(Qt::Checked));

    // Add assembly treeWidgetItem to its parent treeWidgetItem
    arg_node->addChild(childNode);

    occData->setTransparency(1.0);
    occData->setMaterial("ALUMINIUM");

    occData->setTopoShape(arg_shape);
    addBoundryBox(occData->getTopoShape());

    occData->setLocation(occData->getTopoShape().Location());

    NodeInteractive *nodeInteractive = new NodeInteractive(occData->getLabel(), childNode);
    occData->setShape(nodeInteractive);
    occData->getShape()->SetLocalTransformation(occData->getTopoShape().Location().Transformation());

    occData->setObject(nodeInteractive);

    if (shapeTool->IsAssembly(occData->getLabel())) {
        occData->setMaterial("-");
        TopoDS_Iterator iterator(occData->getTopoShape(), true, true);
        for (iterator; iterator.More(); iterator.Next()) {
            childNode->addChild(getChildren(childNode, iterator.Value()));
        }
    } else {
        shapeList.append(occData->getTopoShape());
        shapeLabelList.append(occData->getLabel());
        emit readyShapeForDisplay(occData->getShape());
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
//        ProjectWindow::myViewerWidget->getContext()->Display(
//                getNodeData(arg_node)->getShape(), false);

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

///
/// \param shape
/// \return Bnd_Box boundery
const Bnd_Box &STEPProcessor::addBoundryBox(TopoDS_Shape shape) const {
//    if (!shape.IsNull()) { BRepBndLib::AddClose(shape, (Bnd_Box &) boundryBox); }

//    ProjectWindow::projectInfo.simulationContent.particleSourceCenterPoint = beamRadius_centerPoint;
//    ProjectWindow::projectInfo.simulationContent.particleSourceRadius = beamRadius_lenght;
//    ProjectWindow::projectInfo.simulationContent.particleSourceMultiplier = beamRadius_multiplier;
//
//    ProjectWindow::projectInfo.simulationContent.worldBoxMultiplier = worldBox_multiplier;
//    ProjectWindow::projectInfo.simulationContent.worldBoxCenterPoint = worldBox_centerPoint;
//    ProjectWindow::projectInfo.simulationContent.worldBoxLenght = worldBox_lenght;

    return boundryBox;
}

///
/// \return
const Bnd_Box &STEPProcessor::getBoundryBox() const {
    return boundryBox;
}

///
/// \param multiplier
/// \return
bool STEPProcessor::setWorldBox(double value = 0) {
    qDebug() << "Changing World Box Size : " << value;

    // Daha önce varsa
    if (!worldBox_shape.IsNull()) {
//        ProjectWindow::myViewerWidget->getContext()->Remove(worldBox_aisShape, true);
    }

    // create and show boundry box shape
    double xMin, xMax, yMin, yMax, zMin, zMax;
    double centerPointX, centerPointY, centerPointZ, lenghtXAxis, lenghtYAxis, lenghtZAxis;

    // boundry box üzerinden değerler çekilir
    boundryBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

    centerPointX = (xMin + xMax) / 2.0;
    centerPointY = (yMin + yMax) / 2.0;
    centerPointZ = (zMin + zMax) / 2.0;


    qDebug() << "Center point > " << centerPointX << ", " << centerPointY << ", " << centerPointZ;

    lenghtXAxis = (xMax - xMin) * worldBox_multiplier;
    lenghtYAxis = (yMax - yMin) * worldBox_multiplier;
    lenghtZAxis = (zMax - zMin) * worldBox_multiplier;

    // en büyük kenarı elde etmek için karşılaştırma
    if (lenghtXAxis >= lenghtYAxis) {
        if (lenghtXAxis >= lenghtZAxis) {
            lenghtYAxis = lenghtXAxis;
            lenghtZAxis = lenghtXAxis;
        } else {
            lenghtXAxis = lenghtZAxis;
            lenghtYAxis = lenghtZAxis;
        }
    } else {
        if (lenghtYAxis >= lenghtZAxis) {
            lenghtXAxis = lenghtYAxis;
            lenghtZAxis = lenghtYAxis;
        } else {
            lenghtXAxis = lenghtZAxis;
            lenghtYAxis = lenghtZAxis;
        }
    }


    if (value > 0) {
        worldBox_lenght = value;
    } else {
        worldBox_lenght = lenghtZAxis;
    }

    // başlangıç noktası yaratımı
    gp_Pnt centerPoint(-centerPointX, -centerPointY, -centerPointZ);
    gp_Pnt cornerPoint(centerPointX - (worldBox_lenght / 2.0), centerPointY - (worldBox_lenght / 2.0),
                       centerPointZ - (worldBox_lenght / 2.0));

    // box çizimi
    worldBox_shape = BRepPrimAPI_MakeBox(cornerPoint, worldBox_lenght, worldBox_lenght, worldBox_lenght).Shape();
    worldBox_aisShape = new AIS_Shape(worldBox_shape);

//    ProjectWindow::myViewerWidget->getContext()->Display(worldBox_aisShape, false);
//    ProjectWindow::myViewerWidget->getContext()->SetSelectionModeActive(worldBox_aisShape, -1, false);
//    ProjectWindow::myViewerWidget->getContext()->SetTransparency(worldBox_aisShape, 0.8, false);
//    ProjectWindow::myViewerWidget->getContext()->SetColor(worldBox_aisShape, Quantity_NOC_LIGHTBLUE4, false);
//    ProjectWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
//
//    boundryItem->setText(0, "World Box Size : " + QString::number(worldBox_lenght) + " mm");
//
//    ProjectWindow::projectInfo.simulationContent.worldBoxMultiplier = worldBox_multiplier;
//    ProjectWindow::projectInfo.simulationContent.worldBoxCenterPoint = centerPoint;
//    ProjectWindow::projectInfo.simulationContent.worldBoxLenght = worldBox_lenght;


    qDebug() << "World box center : " << centerPointX << ", " << centerPointY << ", " << centerPointZ;

    emit updatedWorldBoxLenght(worldBox_lenght);

    return true;
}

///
/// \param multiplier
/// \return
bool STEPProcessor::setBeamRadius(double value = 0) {
    qDebug() << "Beam Radius Changing : " << value;

    double centerPointX, centerPointY, centerPointZ, lenghtXAxis, lenghtYAxis, lenghtZAxis;

    // create and show boundry box shape
    Standard_Real xMin, xMax, yMin, yMax, zMin, zMax;
    // boundry box üzerinden değerler çekilir
    boundryBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

    centerPointX = (xMin + xMax) / 2;
    centerPointY = (yMin + yMax) / 2;
    centerPointZ = (zMin + zMax) / 2;
    lenghtXAxis = (xMax - xMin) * beamRadius_multiplier;
    lenghtYAxis = (yMax - yMin) * beamRadius_multiplier;
    lenghtZAxis = (zMax - zMin) * beamRadius_multiplier;

    // daha önce yaratılmışsa
    if (!beamRadius_shape.IsNull()) {
//        ProjectWindow::myViewerWidget->getContext()->Remove(beamRadius_aisShape, true);
    }

    // en büyük kenarı elde etmek için karşılaştırma
    if (lenghtXAxis >= lenghtYAxis) {
        if (lenghtXAxis >= lenghtZAxis) {
            lenghtYAxis = lenghtXAxis;
            lenghtZAxis = lenghtXAxis;
        } else {
            lenghtXAxis = lenghtZAxis;
            lenghtYAxis = lenghtZAxis;
        }
    } else {
        if (lenghtYAxis >= lenghtZAxis) {
            lenghtXAxis = lenghtYAxis;
            lenghtZAxis = lenghtYAxis;
        } else {
            lenghtXAxis = lenghtZAxis;
            lenghtYAxis = lenghtZAxis;
        }
    }

    // başlangıç noktası yaratımı
    gp_Pnt centerPoint(centerPointX, centerPointY, centerPointZ);

    if (value > 0) {
        beamRadius_lenght = value;
    } else {
        beamRadius_lenght = lenghtXAxis;
    }

    // box çizimi
    beamRadius_shape = BRepPrimAPI_MakeSphere(centerPoint, beamRadius_lenght).Shape();
    beamRadius_aisShape = new AIS_Shape(beamRadius_shape);
    beamRadius_centerPoint = gp_Pnt(0, 0, 0);


//    ProjectWindow::myViewerWidget->getContext()->Display(beamRadius_aisShape, false);
//    ProjectWindow::myViewerWidget->getContext()->SetSelectionModeActive(beamRadius_aisShape, -1, false);
//    ProjectWindow::myViewerWidget->getContext()->SetTransparency(beamRadius_aisShape, 0.8, false);
//    ProjectWindow::myViewerWidget->getContext()->SetColor(beamRadius_aisShape, Quantity_NOC_RED4, false);
//    ProjectWindow::myViewerWidget->getContext()->UpdateCurrentViewer();
//
//    beamRadiusItem->setText(0, "Beam Radius : " + QString::number(beamRadius_lenght) + " mm");
//
//    ProjectWindow::projectInfo.simulationContent.particleSourceCenterPoint = beamRadius_centerPoint;
//    ProjectWindow::projectInfo.simulationContent.particleSourceRadius      = beamRadius_lenght;
//    ProjectWindow::projectInfo.simulationContent.particleSourceMultiplier  = beamRadius_multiplier;

    emit updatedSourceSphereDiameter(beamRadius_lenght);

    return true;

}

QList<TopoDS_Shape> STEPProcessor::getAllShape() {
    return shapeList;
}

QList<TDF_Label> STEPProcessor::getAllShapeLabel() {
    return shapeLabelList;
}
