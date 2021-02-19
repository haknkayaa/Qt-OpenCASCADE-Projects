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

    contextMenuAction_moveTo = new QAction("Move To", this);
    connect(contextMenuAction_moveTo, SIGNAL(triggered()), this, SLOT(slot_moveTo()));

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
    //*******************************************************************************************
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

    addToolBar(toolBar);
    //**********************************************************************************************

    //**********************************************************************************************
    QToolBar *toolbar_settings = new QToolBar("Settings", this);
    //shapeBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // QToolButton in Menu
    QToolButton *mouseModeButton = new QToolButton();
    mouseModeButton->setText("Mouse Mode");
    mouseModeButton->setPopupMode(QToolButton::MenuButtonPopup);

    QAction *SELECT_FULLBODY = new QAction("Fullbody Select", this);
    SELECT_FULLBODY->setIcon(QIcon(":/icons/fullbody.svg"));
    connect(SELECT_FULLBODY, &QAction::triggered, this, &MainWindow::chooseFullBody);

    QAction *SELECT_FACE = new QAction("Face Select", this);
    SELECT_FACE->setIcon(QIcon(":/icons/face.svg"));
    connect(SELECT_FACE, &QAction::triggered, this, &MainWindow::chooseFace);

    QAction *SELECT_EDGE = new QAction("Edge Select", this);
    SELECT_EDGE->setIcon(QIcon(":/icons/edge.svg"));
    connect(SELECT_EDGE, &QAction::triggered, this, &MainWindow::chooseEdge);

    QAction *SELECT_VERTEX = new QAction("Vertex Select", this);
    SELECT_VERTEX->setIcon(QIcon(":/icons/vertex.svg"));
    connect(SELECT_VERTEX, &QAction::triggered, this, &MainWindow::chooseVertex);

    QMenu *mouseSelectButtonMenu = new QMenu;
    mouseSelectButtonMenu->addAction(SELECT_FULLBODY);
    mouseSelectButtonMenu->addAction(SELECT_FACE);
    mouseSelectButtonMenu->addAction(SELECT_EDGE);
    mouseSelectButtonMenu->addAction(SELECT_VERTEX);

    mouseModeButton->setMenu(mouseSelectButtonMenu);
    mouseModeButton->setDefaultAction(SELECT_FULLBODY);
    mouseModeButton->setIcon(QIcon(":/icons/mouse.svg"));
    toolbar_settings->addWidget(mouseModeButton);
    // End QToolButton in Menu

    QAction *showGrid = new QAction("Show Grid", this);
    showGrid->setIcon(QIcon(":/icons/grid.svg"));
    connect(showGrid, &QAction::triggered, this, &MainWindow::changeGrid);
    toolbar_settings->addAction(showGrid);

    QAction *measureDistance = new QAction("Measure Distance", this);
    measureDistance->setIcon(QIcon(":/icons/View-measurement.svg"));
    connect(measureDistance, &QAction::triggered, this, &MainWindow::measureDistance);
    toolbar_settings->addAction(measureDistance);

    QAction *clearScene = new QAction("Clear Scene", this);
    connect(clearScene, &QAction::triggered, this, &MainWindow::clearScene);
    toolbar_settings->addAction(clearScene);

    QAction *merge = new QAction("Merge", this);
    connect(merge, &QAction::triggered, this, &MainWindow::merge);
    toolbar_settings->addAction(merge);

    addToolBar(toolbar_settings);
    //**********************************************************************************************

    //**********************************************************************************************
    QToolBar *toolbar_create3d = new QToolBar("Create 3D Shape", this);

    //!todo: iconlar güncellenecek
    QAction *cube = new QAction("Create Cube", this);
    cube->setIcon(QIcon(":/icons/cube.svg"));
    connect(cube, &QAction::triggered, this, &MainWindow::cube);
    toolbar_create3d->addAction(cube);

    QAction *cylinder = new QAction("Create Cylinder", this);
    cylinder->setIcon(QIcon(":/icons/cylinder.svg"));
    connect(cylinder, &QAction::triggered, this, &MainWindow::cylinder);
    toolbar_create3d->addAction(cylinder);

    QAction *sphere = new QAction("Create Sphere", this);
    sphere->setIcon(QIcon(":/icons/sphere.svg"));
    connect(sphere, &QAction::triggered, this, &MainWindow::sphere);
    toolbar_create3d->addAction(sphere);

    addToolBar(toolbar_create3d);
    //**********************************************************************************************

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
void MainWindow::cube() {
    bool ok;
    QList<double> list = InputDialog::getFloats(this, &ok, 3);
    if (list.size() == 3) {
        myViewerWidget->cube(QString("Cube %1").arg(numberOfShapes[0]), list[0], list[1], list[2]);
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString("Cube %1").arg(numberOfShapes[0]));
        modelTreeWidget->insertTopLevelItem(0, item);
        numberOfShapes[0]++;
    }
}

void MainWindow::cylinder() {
    bool ok;
    QList<double> list = InputDialog::getFloats(this, &ok, 2);
    if (list.size() == 2) {
        myViewerWidget->cylinder(QString("Cylinder %1").arg(numberOfShapes[1]), list[0], list[1]);
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString("Cylinder %1").arg(numberOfShapes[1]));
        modelTreeWidget->insertTopLevelItem(0, item);
        numberOfShapes[1]++;

    }
}

void MainWindow::sphere(){
    bool ok;
    QList<double> list = InputDialog::getFloats(this, &ok, 1);
    if (list.size() == 1) {
        myViewerWidget->sphere(QString("Sphere %1").arg(numberOfShapes[2]), list[0]);
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString("Sphere %1").arg(numberOfShapes[2]));
        modelTreeWidget->insertTopLevelItem(0, item);
        numberOfShapes[2]++;

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


        Quantity_Color quantityColor(selectedColor.redF(), selectedColor.greenF(), selectedColor.blueF(),Quantity_TOC_RGB);
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
//    if(currentTreeWidgetItem->text(0).operator!=("Cube"))
//        findSelectedItemFromUploadedObjects(currentTreeWidgetItem, mySTEPProcessor->modelTree);

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
        contextMenu.addSeparator();
        contextMenu.addAction(contextMenuAction_moveTo);

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


void MainWindow::slot_moveTo(){
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
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, moveDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, moveDialog, &QDialog::reject);

    moveDialog->setLayout(lytMain);

    QString shapeName = modelTreeWidget->currentItem()->text(0);
    qDebug() << "Secilen shape:" << shapeName;

    int index = -1;
    for (int i = 0; i < myViewerWidget->shapes.size(); ++i) {
        if(!QString::compare(shapeName, myViewerWidget->shapes[i].name, Qt::CaseInsensitive))
            index = i;
    }

    if(moveDialog->exec() == QDialog::Accepted)
    {
        //TODO: Childi olan objeler için çalışmıyor! düzeltilmeli
        if (index != -1)
            myViewerWidget->moveTo(myViewerWidget->shapes[index].shape, xSlider->value(), ySlider->value(), zSlider->value());
        else
            myViewerWidget->moveTo(currentSelectedShape.shape, xSlider->value(), ySlider->value(), zSlider->value());
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
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lyt->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, mergeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, mergeDialog, &QDialog::reject);

    mergeDialog->setLayout(lyt);

    if(mergeDialog->exec() == QDialog::Accepted) {
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
            item->setText(0, QString("Birlesitirilen Obje"));
            modelTreeWidget->insertTopLevelItem(0, item);
        }
    }
}

void MainWindow::chooseFullBody(){
    qDebug() << "Full Body seçme seçeneği etkinleştirildi.";
    myViewerWidget->selectionMode( 0);

    myMouseMode = MOUSE_SELECT_FULLBODY;
}
void MainWindow::chooseVertex(){
    qDebug() << "Vertex seçme özelliği etkinleştirildi";
    myViewerWidget->selectionMode(1);

    myMouseMode = MOUSE_SELECT_VERTEX;
}
void MainWindow::chooseEdge(){
    qDebug() << "Kenar seçme seçeneği etkinleştirildi.";
    myViewerWidget->selectionMode(2);

    myMouseMode = MOUSE_SELECT_EDGE;
}
void MainWindow::chooseFace() {
    qDebug() << "Face seçme özelliği etkinleştirildi";
    myViewerWidget->selectionMode(4);

    myMouseMode = MOUSE_SELECT_FACE;
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


void MainWindow::changeGrid() {
    myViewerWidget->toggleGrid();
}




