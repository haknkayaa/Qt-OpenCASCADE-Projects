/******************************************************************************
 *    CADActions.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/19/21.
 *     description :
 ******************************************************************************/

#ifndef CADACTIONS_H
#define CADACTIONS_H

// OpenCASCADE Libraries
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <AIS_Shape.hxx>

// Qt Libraries
#include <QtWidgets>

void MainWindow::chooseFullBody() {
    qDebug() << "Full Body seçme seçeneği etkinleştirildi.";
    myViewerWidget->changeMouseSelectingMode(0);

    myMouseMode = MOUSE_SELECT_FULLBODY;
}

void MainWindow::chooseVertex() {
    qDebug() << "Vertex seçme özelliği etkinleştirildi";
    myViewerWidget->changeMouseSelectingMode(1);

    myMouseMode = MOUSE_SELECT_VERTEX;
}

void MainWindow::chooseEdge() {
    qDebug() << "Kenar seçme seçeneği etkinleştirildi.";
    myViewerWidget->changeMouseSelectingMode(2);

    myMouseMode = MOUSE_SELECT_EDGE;
}

void MainWindow::chooseFace() {
    qDebug() << "Face seçme özelliği etkinleştirildi";
    myViewerWidget->changeMouseSelectingMode(4);

    myMouseMode = MOUSE_SELECT_FACE;
}

void MainWindow::changeGrid() {
    myViewerWidget->toggleGrid();
}

void MainWindow::measureDistance() {
    //chooseVertex();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    float distance = 0;
    TopoDS_Vertex vertex;
    TopoDS_Vertex vertex2;
    int check = 0;
    for (myViewerWidget->getContext()->InitSelected(); myViewerWidget->getContext()->MoreSelected(); myViewerWidget->getContext()->NextSelected()) {
        // Handle(AIS_InteractiveObject) anIO = myContext->Current();
        TopoDS_Shape shape = myViewerWidget->getContext()->SelectedShape();
        if (myMouseMode == MOUSE_SELECT_VERTEX) {
            vertex = TopoDS::Vertex(shape);
            //TopoDS_Vertex* retVal = new TopoDS_Vertex(vtx);
            gp_Pnt pnt = BRep_Tool::Pnt(vertex);
            myViewerWidget->getContext()->NextSelected();
            TopoDS_Shape shape2 = myViewerWidget->getContext()->SelectedShape();
            vertex2 = TopoDS::Vertex(shape2);
            //TopoDS_Vertex* retVal = new TopoDS_Vertex(vtx);
            gp_Pnt pnt2 = BRep_Tool::Pnt(vertex2);
            distance = pnt.Distance(pnt2);
            qDebug() << "Distance:" << distance << "MM";
            check = 1;
        }
        break;
    }
    if (check == 1) {
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(vertex, vertex2);
        AIS_Shape *anAisedge = new AIS_Shape(edge);
        myViewerWidget->getContext()->Display(anAisedge);
        myViewerWidget->getContext()->UpdateCurrentViewer();
    }
}

void MainWindow::merge() {
    QDialog *mergeDialog = new QDialog();
    mergeDialog->setWindowTitle("Choose Shapes");

    QFormLayout *lyt = new QFormLayout();

    QLabel *firstLabel = new QLabel();
    QComboBox *firstShape = new QComboBox();
    lyt->addRow(firstLabel, firstShape);

    QLabel *secondLabel = new QLabel();
    QComboBox *secondShape = new QComboBox();
    lyt->addRow(secondLabel, secondShape);

    //int size = numberOfShapes[0] + numberOfShapes[1] + numberOfShapes[2];

    for (int i = 0; i < modelTreeWidget->topLevelItemCount(); ++i) {
        firstShape->addItem(modelTreeWidget->topLevelItem(i)->text(0));
        secondShape->addItem(modelTreeWidget->topLevelItem(i)->text(0));
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
             Qt::Horizontal, this);
    lyt->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, mergeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, mergeDialog, &QDialog::reject);

    mergeDialog->setLayout(lyt);

    if (mergeDialog->exec() == QDialog::Accepted) {
        //eğer input olarak aynı cisimler verilmediyse işlemlere devam etsin.
        if (QString::compare(firstShape->currentText(), secondShape->currentText(), Qt::CaseInsensitive)) {

            myViewerWidget->merge(firstShape->currentText(), secondShape->currentText());

            //modelTreeWidgettan birleştirilen objelerin silinmesi
            //Obje textine göre aranır bulunursa x e 0 atanır ve silme işlemi yapılır
            for (int i = 0; i < modelTreeWidget->topLevelItemCount(); ++i) {
                int x = QString::compare(modelTreeWidget->topLevelItem(i)->text(0),
                                         firstShape->currentText(), Qt::CaseInsensitive);
                if (!x) {
                    delete modelTreeWidget->takeTopLevelItem(i);
                    break;
                }
            }

            //ikinci shape için treewidgettan silme işlemi
            for (int i = 0; i < modelTreeWidget->topLevelItemCount(); ++i) {
                int x = QString::compare(modelTreeWidget->topLevelItem(i)->text(0),
                                         secondShape->currentText(), Qt::CaseInsensitive);
                if (!x) {
                    delete modelTreeWidget->takeTopLevelItem(i);
                    break;
                }
            }

            //treewidgeta yeni objenin eklenmesi
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, QString("Birlestirilen Obje"));
            modelTreeWidget->insertTopLevelItem(0, item);
        }
    }
}

//!TODO: Treewidgetta shapelere tıklanınca program çöküyor!
// Herhangi bir step dosyası eklenip tıklanınca çökmüyor!
void MainWindow::createCube() {

    QDialog *cubeDialog = new QDialog();
    cubeDialog->setWindowTitle("Choose Shapes");

    QFormLayout *lyt = new QFormLayout();

    QLabel *label = new QLabel(QString("X"), this);
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    lyt->addRow(label, spinBox);

    QLabel *label2 = new QLabel(QString("X"), this);
    QDoubleSpinBox *spinBox2 = new QDoubleSpinBox(this);
    lyt->addRow(label2, spinBox2);

    QLabel *label3 = new QLabel(QString("X"), this);
    QDoubleSpinBox *spinBox3 = new QDoubleSpinBox(this);
    lyt->addRow(label3, spinBox3);

    QDialogButtonBox *buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
             Qt::Horizontal, this);
    lyt->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, cubeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, cubeDialog, &QDialog::reject);

    cubeDialog->setLayout(lyt);

    if (cubeDialog->exec() == QDialog::Accepted) {
        qDebug() << "Cube yaratıldı";

        // Make Box
        TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(spinBox->value(), spinBox2->value(), spinBox3->value()).Shape();
        AIS_Shape *anAisBox = new AIS_Shape(aTopoBox);
        anAisBox->SetColor(Quantity_NOC_AZURE);

        Shape newShape;
        newShape.name = "Cube ";
        newShape.shape = anAisBox;
        //shapes.push_back(newShape);

        myViewerWidget->getContext()->Display(anAisBox, Standard_True);
        myViewerWidget->fitAll();
    }
}

void MainWindow::createCylinder() {

    QDialog *cubeDialog = new QDialog();
    cubeDialog->setWindowTitle("Choose Shapes");

    QFormLayout *lyt = new QFormLayout();

    QLabel *label = new QLabel(QString("R"), this);
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    lyt->addRow(label, spinBox);

    QLabel *label2 = new QLabel(QString("H"), this);
    QDoubleSpinBox *spinBox2 = new QDoubleSpinBox(this);
    lyt->addRow(label2, spinBox2);

    QDialogButtonBox *buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                        Qt::Horizontal, this);
    lyt->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, cubeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, cubeDialog, &QDialog::reject);

    cubeDialog->setLayout(lyt);

    if (cubeDialog->exec() == QDialog::Accepted) {
        qDebug() << "Cylinder yaratıldı";

        // Make Box
        TopoDS_Shape aTopoBox = BRepPrimAPI_MakeCylinder(spinBox->value(), spinBox2->value()).Shape();
        AIS_Shape *anAisBox = new AIS_Shape(aTopoBox);
        anAisBox->SetColor(Quantity_NOC_AZURE);

        Shape newShape;
        newShape.name = "Cylinder ";
        newShape.shape = anAisBox;
        //shapes.push_back(newShape);

        myViewerWidget->getContext()->Display(anAisBox, Standard_True);
        myViewerWidget->fitAll();
    }

}

void MainWindow::createSphere() {

    QDialog *cubeDialog = new QDialog();
    cubeDialog->setWindowTitle("Choose Shapes");

    QFormLayout *lyt = new QFormLayout();

    QLabel *label = new QLabel(QString("R"), this);
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    lyt->addRow(label, spinBox);


    QDialogButtonBox *buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                        Qt::Horizontal, this);
    lyt->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, cubeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, cubeDialog, &QDialog::reject);

    cubeDialog->setLayout(lyt);

    if (cubeDialog->exec() == QDialog::Accepted) {
        qDebug() << "Sphere yaratıldı";

        // Make Box
        TopoDS_Shape aTopoBox = BRepPrimAPI_MakeSphere(spinBox->value()).Shape();
        AIS_Shape *anAisBox = new AIS_Shape(aTopoBox);
        anAisBox->SetColor(Quantity_NOC_AZURE);

        Shape newShape;
        newShape.name = "Sphere ";
        newShape.shape = anAisBox;
        //shapes.push_back(newShape);

        myViewerWidget->getContext()->Display(anAisBox, Standard_True);
        myViewerWidget->fitAll();
    }
}

//Clears all objects
void MainWindow::clearScene() {
    myViewerWidget->getContext()->RemoveAll();
    modelTreeWidget->clear();
    int size = sizeof(numberOfShapes) / sizeof(numberOfShapes[0]);
    for (int i = 0; i < size; ++i) {
        numberOfShapes[i] = 1;
    }
}

void MainWindow::slot_moveTo() {
    qDebug() << "Move To";

    QDialog *moveDialog = new QDialog();
    moveDialog->setWindowTitle("Move To");

    QFormLayout *lytMain = new QFormLayout();

    QLabel *xLabel = new QLabel("X");
    QSlider *xSlider = new QSlider(Qt::Horizontal);
    xSlider->setMinimum(-100);
    xSlider->setMaximum(100);
    lytMain->addRow(xLabel, xSlider);

    QLabel *yLabel = new QLabel("Y");
    QSlider *ySlider = new QSlider(Qt::Horizontal);
    ySlider->setMinimum(-100);
    ySlider->setMaximum(100);
    lytMain->addRow(yLabel, ySlider);

    QLabel *zLabel = new QLabel("Z");
    QSlider *zSlider = new QSlider(Qt::Horizontal);
    zSlider->setMinimum(-100);
    zSlider->setMaximum(100);
    lytMain->addRow(zLabel, zSlider);

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            (QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
             Qt::Horizontal, this);
    lytMain->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, moveDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, moveDialog, &QDialog::reject);

    moveDialog->setLayout(lytMain);

    QString shapeName = modelTreeWidget->currentItem()->text(0);
    qDebug() << "Secilen shape:" << shapeName;

    int index = -1;
    for (int i = 0; i < myViewerWidget->shapes.size(); ++i) {
        if (!QString::compare(shapeName, myViewerWidget->shapes[i].name, Qt::CaseInsensitive))
            index = i;
    }

    if (moveDialog->exec() == QDialog::Accepted) {
        //TODO: Childi olan objeler için çalışmıyor! düzeltilmeli
        if (index != -1)
            myViewerWidget->moveTo(myViewerWidget->shapes[index].shape, xSlider->value(), ySlider->value(),
                                   zSlider->value());
        else
            myViewerWidget->moveTo(currentSelectedShape.shape, xSlider->value(), ySlider->value(), zSlider->value());
    }
}


#endif //CADACTIONS_H
