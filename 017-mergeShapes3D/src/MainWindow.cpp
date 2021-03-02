#include "MainWindow.h"
#include "Viewer.h"
#include <QDialogButtonBox>
#include <QFormLayout>
// OpenCASCADE
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


#include <QtWidgets>
#include <QFont>


class QLineEdit;
class QLabel;
QTextBrowser *MainWindow::text = 0;

/** qDebug() ifadesiyle terminale basılan mesajların program içerisindeki textbrowser'a yönlendirilmesi
 *
 * @param type: handle edilecek mesaj tipi QtDebugMsg, QtInfoMsg,
 * @param context: mesajın kaynak bilgileri
 * @param msg: mesajın kendisi
 */
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {

    if (MainWindow::text == 0) {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
            case QtDebugMsg:
                fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtInfoMsg:
                fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtWarningMsg:
                fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtCriticalMsg:
                fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtFatalMsg:
                fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                abort();
        }
    } else {
        switch (type) {
            case QtDebugMsg:
            case QtWarningMsg:
            case QtCriticalMsg:
                if (MainWindow::text != 0) {
                    QFont myfont("Monospace", 10);
                    myfont.setPixelSize(12);
                    MainWindow::text->setStyleSheet("color: rgb(174,50,160)");
                    MainWindow::text->setFont(myfont);
                    //"Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    //        context.function;
                    QString output = "Debug : " + msg;
                    MainWindow::text->append(output);
                }

                break;
            case QtFatalMsg:
                abort();
        }
    }
}


// Kurucu fonksiyon
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    initialSettings();

    createActions();
    createMenuBar();
    createStatusBar();
    createToolbars();

    createMiddleWidget();

    qDebug() << "Program başlatılıyor.";
}

/** Programın çalışması için
 * ilk ayarların set edildiği fonksiyondur.
 */
void MainWindow::initialSettings() {
    qInstallMessageHandler(myMessageOutput);

    setWindowTitle("OpenCASCADE Window");
    resize(1000, 800);


    QFile styleFile(":/qss/style.qss");
    styleFile.open(QFile::ReadOnly);
    // Apply the loaded stylesheet
    QString style(styleFile.readAll());
    qApp->setStyleSheet(style);
}

// Middle Widget
void MainWindow::createMiddleWidget() {

    QHBoxLayout *mainLayout = new QHBoxLayout();

    // Dock Widgets

    {   // Model Tree Dock Widget
        QDockWidget *dockWidget_modelTree = new QDockWidget("Model Tree", this);

        modelTreeWidget = new QTreeWidget(dockWidget_modelTree);
        connect(modelTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem * , int)), this, SLOT(modelTreeItemClicked(QTreeWidgetItem * )));

        modelTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(modelTreeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::contextMenuForRightClick);

        connect(modelTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(currentItemChanged()));

        QStringList headerLabels = {"Part Name"};
        modelTreeWidget->setHeaderLabels(headerLabels);

        dockWidget_modelTree->setWidget(modelTreeWidget);
        addDockWidget(Qt::LeftDockWidgetArea, dockWidget_modelTree);
        tabifiedDockWidgets(dockWidget_modelTree);
    }   // MODEL TREE DOCK WİDGET

    {   // Information Dock Widget
        QDockWidget *dockWidget_information = new QDockWidget("Information", this);
        QWidget *informationWidget = new QWidget();

        QGridLayout *informationLayout = new QGridLayout();
        informationLayout->setAlignment(Qt::AlignTop);

        informationLayout->addWidget(new QLabel("Part Name"), 0, 0);
        information_partName = new QLineEdit(" ");
        information_partName->setReadOnly(true);
        informationLayout->addWidget(information_partName, 0, 1);

        informationLayout->addWidget(new QLabel("Parent"), 1, 0);
        information_parentName = new QLineEdit(" ");
        information_parentName->setReadOnly(true);
        informationLayout->addWidget(information_parentName, 1, 1);

        informationLayout->addWidget(new QLabel("Index"), 2, 0);
        information_index = new QLineEdit(" ");
        information_index->setReadOnly(true);
        informationLayout->addWidget(information_index, 2, 1);

        informationLayout->addWidget(new QLabel("Transparency"), 3, 0);
        information_transparencySlider = new QSlider(Qt::Horizontal);
        informationLayout->addWidget(information_transparencySlider, 3, 1);
        connect(information_transparencySlider, SIGNAL(valueChanged(int)), this, SLOT(slot_informationTransparenctValueChanged()));

        informationLayout->addWidget(new QLabel("Color"), 4, 0);
        information_colorButton = new QPushButton("...");
        connect(information_colorButton, SIGNAL(clicked(bool)), this, SLOT(slot_informationColorDialog()));
        informationLayout->addWidget(information_colorButton, 4, 1);

        informationLayout->addWidget(new QLabel("Material"), 5, 0);
        information_materialSelect = new QComboBox();
        information_materialSelect->addItems(QStringList() << "ALUMINIUM" << "GOLD");
        informationLayout->addWidget(information_materialSelect, 5, 1);

        // herhangi bir nesne seçilmediğinde disabled et
        bool IsDisabled = true;
        information_partName->setDisabled(IsDisabled);
        information_parentName->setDisabled(IsDisabled);
        information_index->setDisabled(IsDisabled);
        information_colorButton->setDisabled(IsDisabled);
        information_transparencySlider->setDisabled(IsDisabled);
        information_materialSelect->setDisabled(IsDisabled);

        informationWidget->setLayout(informationLayout);
        dockWidget_information->setWidget(informationWidget);
        addDockWidget(Qt::LeftDockWidgetArea, dockWidget_information);
        tabifiedDockWidgets(dockWidget_information);
    }   // Information Dock Widget


    QDockWidget *dockWidget_console = new QDockWidget("Console", this);
    { // CONSOLE

        text = new QTextBrowser(dockWidget_console);
        dockWidget_console->setWidget(text);

        addDockWidget(Qt::BottomDockWidgetArea, dockWidget_console);
        //tabifiedDockWidgets(dockWidget_console);
    } // CONSOLE

    QDockWidget *dockWidget_terminal = new QDockWidget("Terminal", this);
    { // CONSOLE


        QTextBrowser *text2 = new QTextBrowser(dockWidget_terminal);
        dockWidget_terminal->setWidget(text2);

        addDockWidget(Qt::BottomDockWidgetArea, dockWidget_terminal);
        //tabifiedDockWidgets(dockWidget_terminal);

    } // CONSOLE

    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::West);
    tabifyDockWidget(dockWidget_console, dockWidget_terminal);
    dockWidget_console->raise();

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    // 3D Viewer
    myViewerWidget = new Viewer(this);
    mainLayout->addWidget(myViewerWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::createActions() {

    // context qtreewidget menu
    contextMenuAction_showAllParts = new QAction("Show All Parts", this);
    connect(contextMenuAction_showAllParts, SIGNAL(triggered()), this, SLOT(slot_showAllParts()));

    contextMenuAction_showOnlySelectedPart = new QAction("Show Only", this);
    connect(contextMenuAction_showOnlySelectedPart, SIGNAL(triggered()), this, SLOT(slot_showOnlySelectedPart()));

    contextMenuAction_setVisible = new QAction("Visible / Unvisible", this);
    contextMenuAction_setVisible->setIcon(QIcon::fromTheme("view-fullscreen"));
    connect(contextMenuAction_setVisible, SIGNAL(triggered()), this, SLOT(slot_setVisible()));

    contextMenuAction_fitAll = new QAction("Fit All", this);
    connect(contextMenuAction_fitAll, SIGNAL(triggered()), this, SLOT(slot_fitAll()));

}

// Menü yaratan fonksiyon
void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);

    // File Menü
    QMenu *subMenu = new QMenu("File", this);
    menuBar->addMenu(subMenu);

    // File submenus
    // Import
    QAction *importFileAction = new QAction("Import", this);
    connect(importFileAction, &QAction::triggered, this, &MainWindow::importFile);
    importFileAction->setStatusTip("Yeni bir dosya ekle");
    subMenu->addAction(importFileAction);

    // Edit Menü
    QMenu *editMenu = new QMenu("Edit", this);
    menuBar->addMenu(editMenu);

    QMenu *viewModeMenu = new QMenu("View Mode", this);
    editMenu->addMenu(viewModeMenu);

    QAction *viewMode1 = new QAction("V3d_PERSPECTIVE", this);
    viewMode1->setCheckable(true);
    connect(viewMode1, &QAction::triggered, this, &MainWindow::changeViewProjectionMode);
    viewModeMenu->addAction(viewMode1);

    QAction *viewMode2 = new QAction("V3d_ORTHOGRAPHIC", this);
    viewMode2->setCheckable(true);
    connect(viewMode2, &QAction::triggered, this, &MainWindow::changeViewProjectionMode);
    viewModeMenu->addAction(viewMode2);

    setMenuBar(menuBar);
}

// Status Bar yaratan fonksiyon
void MainWindow::createStatusBar() {
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->showMessage("Hazır");

    openedFolderLabel = new QLabel("Not Selected File", this);

    statusBar->addPermanentWidget(openedFolderLabel);

    setStatusBar(statusBar);
}

// Toolbar yaratan fonksiyon
void MainWindow::createToolbars() {
    QToolBar *toolBar = new QToolBar("Toolbar 1", this);

    // Üst bakışa
    QAction *viewTop = new QAction("View Top", this);
    viewTop->setIcon(QIcon(":/icons/view-top.svg"));
    connect(viewTop, &QAction::triggered, this, &MainWindow::viewTop);
    toolBar->addAction(viewTop);

    QAction *viewBottom = new QAction("View Bottom", this);
    viewBottom->setIcon(QIcon(":/icons/view-bottom.svg"));
    connect(viewBottom, &QAction::triggered, this, &MainWindow::viewBottom);
    toolBar->addAction(viewBottom);

    QAction *viewLeft = new QAction("View Left", this);
    viewLeft->setIcon(QIcon(":/icons/view-left.svg"));
    connect(viewLeft, &QAction::triggered, this, &MainWindow::viewLeft);
    toolBar->addAction(viewLeft);

    QAction *viewRight = new QAction("View Right", this);
    viewRight->setIcon(QIcon(":/icons/view-right.svg"));
    connect(viewRight, &QAction::triggered, this, &MainWindow::viewRight);
    toolBar->addAction(viewRight);

    QToolBar *shapeBar = new QToolBar("Shapes", this);

    //!todo: iconlar güncellenecek
    QAction *cube = new QAction("Cube", this);
    cube->setIcon(QIcon(":/icons/view-left.svg"));
    connect(cube, &QAction::triggered, this, &MainWindow::createCube);
    shapeBar->addAction(cube);

    QAction *cylinder = new QAction("Cylinder", this);
    cylinder->setIcon(QIcon(":/icons/view-left.svg"));
    connect(cylinder, &QAction::triggered, this, &MainWindow::createCylinder);
    shapeBar->addAction(cylinder);

    QAction *sphere = new QAction("Sphere", this);
    sphere->setIcon(QIcon(":/icons/view-left.svg"));
    connect(sphere, &QAction::triggered, this, &MainWindow::createSphere);
    shapeBar->addAction(sphere);

    QAction *clearScene = new QAction("Clear Scene", this);
    connect(clearScene, &QAction::triggered, this, &MainWindow::clearScene);
    shapeBar->addAction(clearScene);

    QAction *merge = new QAction("Merge", this);
    connect(merge, &QAction::triggered, this, &MainWindow::merge);
    shapeBar->addAction(merge);

    addToolBar(toolBar);
    addToolBar(shapeBar);
}

/*
 *  Action Functions
 */

// dosya yüklemek için çalışan fonksiyon
void MainWindow::importFile() {

    QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),
                                                  QStandardPaths::LocateDirectory);

    QString supportedFileType = "STEP Files (*.step *.stp *.STEP *.STP)";

    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    homeLocation,
                                                    supportedFileType);

    openedFolderLabel->setText("Yükleniyor...");

    if (!fileName.isEmpty()) {
        mySTEPProcessor = new STEPProcessor(fileName);
        openedFolderLabel->setText(fileName);
    }

}

void MainWindow::changeViewProjectionMode() {
    myViewerWidget->changeViewProjectionType();
}

void MainWindow::viewTop() {
    myViewerWidget->viewTop();
}

void MainWindow::viewBottom() {
    myViewerWidget->viewBottom();
}

void MainWindow::viewRight() {
    myViewerWidget->viewRight();
}

void MainWindow::viewLeft() {
    myViewerWidget->viewLeft();
}

//!TODO: Treewidgetta shapelere tıklanınca program çöküyor!
// Herhangi bir step dosyası eklenip tıklanınca çökmüyor!
void MainWindow::createCube() {

    QDialog *cubeDialog = new QDialog();
    cubeDialog->setWindowTitle("Create Cube");

    QFormLayout *lytMain = new QFormLayout(this);

    QLabel *xLabel = new QLabel("X");
    QDoubleSpinBox *xSpinBox = new QDoubleSpinBox(this);
    lytMain->addRow(xLabel, xSpinBox);

    QLabel *yLabel = new QLabel("Y");
    QDoubleSpinBox *ySpinBox = new QDoubleSpinBox(this);
    lytMain->addRow(yLabel, ySpinBox);

    QLabel *zLabel = new QLabel("Z");
    QDoubleSpinBox *zSpinBox = new QDoubleSpinBox(this);
    lytMain->addRow(zLabel, zSpinBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, cubeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, cubeDialog, &QDialog::accept);
    cubeDialog->setLayout(lytMain);

    if(cubeDialog->exec() == QDialog::Accepted) {
        //Create a box
        TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(xSpinBox->value(), ySpinBox->value(), zSpinBox->value()).Shape();
        auto *anAisBox = new AIS_Shape(aTopoBox);
        QString nameOfShape = QString("Cube %1").arg(numberOfShapes[0]);
        anAisBox->SetColor(Quantity_NOC_AZURE);

        //Shapes vektörüne eklemek için yeni bir Shape oluşturur.
        Shape newShape;
        newShape.name = nameOfShape;
        newShape.shape = anAisBox;
        shapes.push_back(newShape);

        //ModelTree ye eklenir.
        auto *item = new QTreeWidgetItem();
        item->setText(0, nameOfShape);
        modelTreeWidget->insertTopLevelItem(0, item);
        numberOfShapes[0]++;

        myViewerWidget->getContext()->Display(anAisBox, Standard_True);
        myViewerWidget->fitAll();
    }
}

void MainWindow::createCylinder() {

    QDialog *cylinderDialog = new QDialog();
    cylinderDialog->setWindowTitle("Create Cylinder");

    QFormLayout *lytMain = new QFormLayout(this);

    QLabel *rLabel = new QLabel("R");
    QDoubleSpinBox *rSpinBox = new QDoubleSpinBox(this);
    lytMain->addRow(rLabel, rSpinBox);

    QLabel *hLabel = new QLabel("H");
    QDoubleSpinBox *hSpinBox = new QDoubleSpinBox(this);
    lytMain->addRow(hLabel, hSpinBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, cylinderDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, cylinderDialog, &QDialog::accept);
    cylinderDialog->setLayout(lytMain);

    if (cylinderDialog->exec() == QDialog::Accepted) {
        //myViewerWidget->cylinder(QString("Cylinder %1").arg(numberOfShapes[1]), list[0], list[1]);

        TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder (rSpinBox->value(), hSpinBox->value()).Shape();
        auto *anAisCylinder = new AIS_Shape(aTopoCylinder);
        QString nameOfShape = QString("Cylinder %1").arg(numberOfShapes[1]);
        anAisCylinder->SetColor(Quantity_NOC_AZURE);

        //Shapes vektörüne eklemek için yeni bir Shape oluşturur.
        Shape newShape;
        newShape.name = nameOfShape;
        newShape.shape = anAisCylinder;
        shapes.push_back(newShape);

        //ModelTree ye eklenir.
        auto *item = new QTreeWidgetItem();
        item->setText(0, nameOfShape);
        modelTreeWidget->insertTopLevelItem(0, item);
        numberOfShapes[1]++;
        myViewerWidget->getContext()->Display(anAisCylinder, Standard_True);
        myViewerWidget->fitAll();
    }
}

void MainWindow::createSphere(){

    QDialog *sphereDialog = new QDialog();
    sphereDialog->setWindowTitle("Create Sphere");

    QFormLayout *lytMain = new QFormLayout(this);

    QLabel *rLabel = new QLabel("R");
    QDoubleSpinBox *rSpinBox = new QDoubleSpinBox(this);
    lytMain->addRow(rLabel, rSpinBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, sphereDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, sphereDialog, &QDialog::accept);
    sphereDialog->setLayout(lytMain);

    if (sphereDialog->exec() == QDialog::Accepted) {
        //myViewerWidget->sphere(QString("Sphere %1").arg(numberOfShapes[2]), list[0]);

        TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(rSpinBox->value()).Shape();
        auto *anAisSphere = new AIS_Shape(aTopoSphere);
        QString nameOfShape = QString("Sphere %1").arg(numberOfShapes[2]);
        anAisSphere->SetColor(Quantity_NOC_AZURE);

        //Shapes vektörüne eklemek için yeni bir Shape oluşturur
        Shape newShape;
        newShape.name = nameOfShape;
        newShape.shape = anAisSphere;
        shapes.push_back(newShape);

        //ModelTree ye eklenir.
        auto *item = new QTreeWidgetItem();
        item->setText(0, nameOfShape);
        modelTreeWidget->insertTopLevelItem(0, item);
        numberOfShapes[2]++;

        myViewerWidget->getContext()->Display(anAisSphere, Standard_True);
        myViewerWidget->fitAll();
    }
}

//Clears all objects
void MainWindow::clearScene() {
    myViewerWidget->getContext()->RemoveAll();
    modelTreeWidget->clear();
    int size = sizeof(numberOfShapes)/sizeof(numberOfShapes[0]);
    for (int i = 0; i < size; ++i) {
        numberOfShapes[i] = 1;
    }
}


/** Renk seçim dialog penceresini başlatan butonun fonksiyonu
 *
 */
void MainWindow::slot_informationColorDialog() {
    qDebug() << "Renk seçimi";

    Quantity_Color color;
    QColor initialColor = QColor::fromRgbF(color.Red(), color.Green(), color.Blue());

    QColor selectedColor = QColorDialog::getColor(initialColor, this, "Select Color");

    if (selectedColor.isValid()) {
        QPixmap pixmap(QSize(30, 30));
        pixmap.fill(QColor::fromRgbF(selectedColor.redF(), selectedColor.greenF(), selectedColor.blueF()));
        information_colorButton->setIcon(QIcon(pixmap));
        information_colorButton->setText("");


        Quantity_Color quantityColor(selectedColor.redF(), selectedColor.greenF(), selectedColor.blueF(),
                                     Quantity_TOC_RGB);
        currentSelectedShape.color = quantityColor;

        qDebug() << "CurrentSelectedShape rengi updated.";


    }
}

void MainWindow::slot_informationTransparenctValueChanged() {
    qDebug() << "Transparency değeri değiştir. Değer " << information_transparencySlider->value();


    double transp = 1.0 - ((double)information_transparencySlider->value() /
                           (double)information_transparencySlider->maximum());



    myViewerWidget->getContext()->SetTransparency(currentSelectedShape.shape, transp, true);

    currentSelectedShape.transparency = 1.0 - transp;
    findUpdatedItemFromUploadedObjects(currentSelectedShape, mySTEPProcessor->modelTree);
}

/** QTreeWidget üzerinde herhangi bir item'e tıklanıldığında çalışacak
 * fonksiyondur.
 *!TODO: Shapelere tıklanması için düzenleme yapılacak
 * @param arg_item : tıklanılan item
 */
void MainWindow::modelTreeItemClicked(QTreeWidgetItem *arg_item) {
    QString itemName = arg_item->data(0, Qt::EditRole).toString();
    qDebug() << "Model Tree üzerinden" << itemName << "seçildi.";
    qDebug() << itemName;

    //if statment açılıp itemName cube cylinder yada sphere değilse veya öyleyse diye kontrol edilebilir!
    ///* currentSelectedShape = */ findSelectedItemFromUploadedObjects(arg_item, mySTEPProcessor->modelTree);


    if(currentSelectedShape.Children.size() > 1){
        myViewerWidget->getContext()->ClearSelected(false);
        for (int i = 0; i < currentSelectedShape.Children.size(); ++i) {
            Handle(AIS_InteractiveObject) obj = currentSelectedShape.Children[i].shape;
            myViewerWidget->getContext()->AddOrRemoveSelected(obj, true);
        }
        myViewerWidget->getContext()->UpdateCurrentViewer();

    }else{
        myViewerWidget->getContext()->ClearSelected(false);
        Handle(AIS_InteractiveObject) obj = currentSelectedShape.shape;
        myViewerWidget->getContext()->AddOrRemoveSelected(obj, true);
        myViewerWidget->getContext()->UpdateCurrentViewer();
    }



    if (!currentSelectedShape.Name.isNull()) {
        //information kısmını aktifleştir

        bool IsDisabled = false;
        information_partName->setDisabled(IsDisabled);
        information_parentName->setDisabled(IsDisabled);
        information_index->setDisabled(IsDisabled);
        information_colorButton->setDisabled(IsDisabled);
        information_transparencySlider->setDisabled(IsDisabled);
        information_materialSelect->setDisabled(IsDisabled);

        // tıklanılan nesnenin bilgileir information bölümünde gösterilmesi
        information_partName->setText(currentSelectedShape.Name);
        information_parentName->setText(currentSelectedShape.Parent->Name);
        information_index->setText(currentSelectedShape.Index);

        qDebug() << "current transP" << double(currentSelectedShape.transparency);
        information_transparencySlider->setValue(currentSelectedShape.transparency * 100.0);

        QPixmap pixmap(QSize(30, 30));
        pixmap.fill(QColor::fromRgbF(currentSelectedShape.color.Red(), currentSelectedShape.color.Green(),
                                     currentSelectedShape.color.Blue()));
        information_colorButton->setIcon(QIcon(pixmap));
        information_colorButton->setText("");
    }

}

/** QTreeWidget üzerinde bir item seçilmiş iken başka farklı bir item seçildiğinde
 * çalışan fonksiyondur.
 * currentSelectedShape ile ilgili dataları modelTree üzerinde günceller.
 */
void MainWindow::currentItemChanged() {
    qDebug() << "Seçili obje değişti. Eski seçili nesne için veriler kaydedilecek.";

    findUpdatedItemFromUploadedObjects(currentSelectedShape, mySTEPProcessor->modelTree);
}

//! todo: return AssemblyNode
/** Seçili QTreeWidgetItem'ini yüklenen objeler arasında arar.
 * Bulunca currentSelectedShape olarak atar.
 *
 * @param arg_item : QTreeWidget üzerindeki seçilen item
 * @param arg_modelTree : Aranacak vector dizisi
 */
void MainWindow::findSelectedItemFromUploadedObjects(QTreeWidgetItem *arg_item, vector<AssemblyNode> arg_modelTree) {
    for (int i = 0; i < arg_modelTree.size(); ++i) {
        if (arg_item == arg_modelTree[i].treeWidgetItem) {
            qDebug() << "Eşleşme bulundu. ";
            qDebug() << "modelTree içindeki > " << arg_modelTree[i].Name;
            qDebug() << arg_modelTree[i].Index;

            currentSelectedShape = arg_modelTree[i];
        }
        findSelectedItemFromUploadedObjects(arg_item, arg_modelTree[i].Children);
    }
}

/** current item modeltree içinde arayip modeltree dizisinde kendisini güncellemesini ister
 *
 * @param arg_currentNode
 * @param arg_modelTree
 */
void MainWindow::findUpdatedItemFromUploadedObjects(AssemblyNode arg_currentNode, vector<AssemblyNode> arg_modelTree) {
    for (int i = 0; i < arg_modelTree.size(); ++i) {
        if (arg_currentNode.treeWidgetItem == arg_modelTree[i].treeWidgetItem) {
            qDebug() << "Eşleşme bulundu. Veri güncellenecek.";
            qDebug() << "modelTree içindeki > " << arg_modelTree[i].Name;
            qDebug() << "Current Selected Shape Index: " << arg_currentNode.Index;

            // verilen objeyi günceller
            updateCurrentSelectedItem(arg_currentNode);

            qDebug() << "modelTree içndeki datanın rengi güncellendi";
        }
        findUpdatedItemFromUploadedObjects(arg_currentNode, arg_modelTree[i].Children);
    }
}

/** Seçili item üzerindeki değişiklikleri kaydeder
 *
 * @param arg_currentSelectedItem: seçili item
 */
 //! todo: burayı çöz
void MainWindow::updateCurrentSelectedItem(AssemblyNode arg_currentSelectedItem) {
    QStringList it = arg_currentSelectedItem.Index.split(":");

    if (it.size() == 1) {
        mySTEPProcessor->modelTree[it[0].toInt() - 1] = arg_currentSelectedItem;
    } else if (it.size() == 2) {
        mySTEPProcessor->modelTree[it[0].toInt() - 1].Children[it[1].toInt() - 1] = arg_currentSelectedItem;
    } else if (it.size() == 3) {
        mySTEPProcessor->modelTree[it[0].toInt() - 1].Children[it[1].toInt() - 1].Children[it[2].toInt() -
                                                                                           1] = arg_currentSelectedItem;
    } else if (it.size() == 4) {
        mySTEPProcessor->modelTree[it[0].toInt() - 1].Children[it[1].toInt() - 1].Children[it[2].toInt() - 1].Children[
                it[3].toInt() - 1] = arg_currentSelectedItem;
    } else if (it.size() == 5) {
        mySTEPProcessor->modelTree[it[0].toInt() - 1].Children[it[1].toInt() - 1].Children[it[2].toInt() - 1].Children[
                it[3].toInt() - 1].Children[it[4].toInt() - 1] = arg_currentSelectedItem;
    }
}


/** QTreeWidget üzerinde sağ click yapıldığında açılacak
 * olan menüyü yaratan fonksiyondur.
 *
 * @param arg_pos: tıklamanın bulunduğu posizyon
 */
void MainWindow::contextMenuForRightClick(const QPoint &arg_pos) {

    QTreeWidget *currentTreeWidget = modelTreeWidget;
    QTreeWidgetItem *currentTreeWidgetItem = currentTreeWidget->itemAt(arg_pos);
    //qDebug() << arg_pos;
    //qDebug() << currentTreeWidgetItem->text(0);
    if(currentTreeWidgetItem->text(0).operator!=("Cube"))
        findSelectedItemFromUploadedObjects(currentTreeWidgetItem, mySTEPProcessor->modelTree);

    if (currentTreeWidgetItem) {
        qDebug() << currentTreeWidgetItem->data(0, Qt::EditRole).toString() << "selected.";
        qDebug() << arg_pos << currentTreeWidgetItem->text(0);
        qDebug() << "Item index : " << modelTreeWidget->currentIndex().row();

        QMenu contextMenu(tr("Context menu"), this);

        contextMenu.addAction(contextMenuAction_showAllParts);
        contextMenu.addAction(contextMenuAction_showOnlySelectedPart);
        contextMenu.addSeparator();
        contextMenu.addAction(contextMenuAction_setVisible);
        contextMenu.addSeparator();
        contextMenu.addAction(contextMenuAction_fitAll);

        QPoint pt(arg_pos);
        contextMenu.exec(currentTreeWidget->mapToGlobal(arg_pos));
    }

}

/** QTreeWidget üzerinded sağ click yapıldığında
 * "Show ALl" ile tüm nesneleri gösterir.
 */
void MainWindow::slot_showAllParts() {
    qDebug() << "Show All Parts";

    myViewerWidget->getContext()->EraseAll(true);
    mySTEPProcessor->displayShapes(mySTEPProcessor->modelTree);
}

/** QTreeWidget üzerinded sağ click yapıldığında
 * "Show Only" ile sadece seçili nesneyi gösterir.
 */
void MainWindow::slot_showOnlySelectedPart() {
    qDebug() << "Show Only Selected Part";

    /* Var olan tüm gösterimleri sil */
    myViewerWidget->getContext()->EraseAll(false);
    myViewerWidget->getContext()->UpdateCurrentViewer();

    /* Eğer alt üyeleri var ise kendisini değil
     * alt üyelerinin tamamını göstermesi için kontrol */
    if (currentSelectedShape.Children.size() > 1){
        for (int i = 0; i < currentSelectedShape.Children.size(); ++i) {
            myViewerWidget->getContext()->Display(currentSelectedShape.Children[i].shape, false);
            myViewerWidget->getContext()->UpdateCurrentViewer();
            myViewerWidget->fitAll();
        }
    }else{ /* Eğer alt üyeleri yok ise kendisini göstersin */
        myViewerWidget->getContext()->Display(currentSelectedShape.shape, false);
        myViewerWidget->getContext()->UpdateCurrentViewer();
        myViewerWidget->fitAll();
    }
}

/** QTreeWidget üzerinded sağ click yapıldığında
 * "Visible/Unvisible" ile görünürlüğü terslenir.
 */
void MainWindow::slot_setVisible() {
    qDebug() << "Visible / Unvisible";

//    if(myViewerWidget->getContext()->IsDisplayed(currentSelectedShape.shape)){
//        qDebug() << "Obje görünür halde. Bu";
//        myViewerWidget->getContext()->Erase(currentSelectedShape.shape, 1);
//        myViewerWidget->fitAll();
//    }
    myViewerWidget->getContext()->Display(currentSelectedShape.shape, 1);
    //myViewerWidget->getContext()->UpdateCurrentViewer();
    myViewerWidget->fitAll();
}

/** QTreeWidget üzerinded sağ click yapıldığında
 * "Fit All" ile tüm nesneleri ekrana sığdırır.
 *
 */
void MainWindow::slot_fitAll() {
    qDebug() << "Fit All";

    myViewerWidget->getContext()->UpdateCurrentViewer();
    myViewerWidget->fitAll();
}


/**
 * İki ais_shape birleştirir, modelTreeWidgettan ve shapes vektöründen önceki iki shapei siler
 * Yeni oluşan shapei shapes vectorüne ekler.
 * Birleştirme sırasına göre ve şekil sayısına göre kontrol edilmeli!.
 */
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

    //Açılan dialogdaki comboboxlara modelTreeWidgetda var olan şekilleri ekler.
    for (int i = 0; i < modelTreeWidget->topLevelItemCount(); ++i) {
        firstShape->addItem(modelTreeWidget->topLevelItem(i)->text(0));
        secondShape->addItem(modelTreeWidget->topLevelItem(i)->text(0));
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lyt->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, mergeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, mergeDialog, &QDialog::reject);

    mergeDialog->setLayout(lyt);

    if(mergeDialog->exec() == QDialog::Accepted) {
        //TODO: iki tane ayrı ayrı birleştirilmiş objeler için düzenlenmeli
        //eğer input olarak aynı cisimler verilmediyse işlemlere devam etsin.
        if (QString::compare(firstShape->currentText(), secondShape->currentText(), Qt::CaseInsensitive)) {

            //Birleştirme için builder ve compound oluşturulur
            TopoDS_Compound aCompound;
            BRep_Builder aBuilder;
            aBuilder.MakeCompound(aCompound);

            int index1, index2;
            //birlestirilcek olan shapelerin shapes vectorundeki indexini bulur
            // (compare 0 dönerse bulmuş demek).
            for (int i = 0; i < shapes.size(); ++i) {
                int x = QString::compare(shapes[i].name, firstShape->currentText(), Qt::CaseInsensitive);
                int y = QString::compare(shapes[i].name, secondShape->currentText(), Qt::CaseInsensitive);
                if (!x)
                    index1 = i;
                else if (!y)
                    index2 = i;
            }

            //Bulunan şekiller alınır ve compound üzerinde birleştirilir.
            TopoDS_Shape firstAisShape = shapes[index1].shape->Shape();
            TopoDS_Shape secondAisShape = shapes[index2].shape->Shape();

            aBuilder.Add(aCompound, firstAisShape);
            aBuilder.Add(aCompound, secondAisShape);
            auto *anAISShape = new AIS_Shape(aCompound);


            //Birlestirilen shapeleri sahneden ve shapes vectorunden siler
            myViewerWidget->getContext()->Remove(shapes[index1].shape);
            myViewerWidget->getContext()->Remove(shapes[index2].shape);
            shapes.erase(shapes.begin()+index1);
            shapes.erase(shapes.begin()+index2);

            //Oluşan shape i shapes vectorüne ekler
            Shape newShape;
            newShape.name = QString("Birlesitirilen Obje");
            newShape.shape = anAISShape;
            shapes.push_back(newShape);


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
            item->setText(0, QString("Birlesitirilen Obje"));
            modelTreeWidget->insertTopLevelItem(0, item);

            myViewerWidget->getContext()->Display(anAISShape, Standard_True);
        }
    }
}









