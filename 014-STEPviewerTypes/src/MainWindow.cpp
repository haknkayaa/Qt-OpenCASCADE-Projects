#include "MainWindow.h"
#include "Viewer.h"

// OpenCASCADE
#include <V3d_View.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Graphic3d_ClipPlane.hxx>
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
#include <Handle_AIS_InteractiveContext.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_LineAspect.hxx>
#include <StdSelect_EdgeFilter.hxx>

#include <QtWidgets>
#include <QFont>
#include <Image_AlienPixMap.hxx>
#include <Graphic3d_Texture2Dmanual.hxx>
#include <BRepBndLib.hxx>

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
        connect(modelTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem * , int)), this,
                SLOT(modelTreeItemClicked(QTreeWidgetItem * )));

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
        connect(information_transparencySlider, SIGNAL(valueChanged(int)), this,
                SLOT(slot_informationTransparenctValueChanged()));

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

    // View
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

    // Edit
    QToolBar *toolbar2 = new QToolBar("Toolbar 2", this);

    QAction *act_clipPlane = new QAction("Clip Plane", this);
    connect(act_clipPlane, &QAction::triggered, this, &MainWindow::slot_clipPlane);
    toolbar2->addAction(act_clipPlane);

    addToolBar(toolbar2);
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

void MainWindow::slot_clipPlane() {
    qDebug() << "Clip plane triggered.";

    QDialog *clipPlaneDialog = new QDialog();

    QGridLayout *clipPlaneDialogLayout = new QGridLayout();

    // Row 1: X Plane
    {
        xPlaneActived = new QCheckBox("X Plane");
        clipPlaneDialogLayout->addWidget(xPlaneActived, 0, 0);

        xPlaneValue = new QSpinBox();
        clipPlaneDialogLayout->addWidget(xPlaneValue, 0, 1);

        xPlaneSlider = new QSlider(Qt::Horizontal);
        clipPlaneDialogLayout->addWidget(xPlaneSlider, 0, 2);

        xPlaneFlip = new QPushButton("Flip");
        clipPlaneDialogLayout->addWidget(xPlaneFlip, 0, 3);

        connect(xPlaneActived, &QCheckBox::stateChanged, this, &MainWindow::slot_clipPlaneChanged);
        connect(xPlaneValue, SIGNAL(valueChanged(int)), this, SLOT(slot_clipPlaneChanged()));
        connect(xPlaneSlider, &QSlider::valueChanged, this, &MainWindow::slot_clipPlaneChanged);
        connect(xPlaneFlip, &QPushButton::clicked, this, &MainWindow::slot_clipPlaneChanged);
    }

    // Row 2: Y Plane
    {
        yPlaneActived = new QCheckBox("Y Plane");
        clipPlaneDialogLayout->addWidget(yPlaneActived, 1, 0);

        yPlaneValue = new QSpinBox();
        clipPlaneDialogLayout->addWidget(yPlaneValue, 1, 1);

        yPlaneSlider = new QSlider(Qt::Horizontal);
        clipPlaneDialogLayout->addWidget(yPlaneSlider, 1, 2);

        yPlaneFlip = new QPushButton("Flip");
        clipPlaneDialogLayout->addWidget(yPlaneFlip, 1, 3);

        connect(yPlaneActived, &QCheckBox::stateChanged, this, &MainWindow::slot_clipPlaneChanged);
        connect(yPlaneValue, SIGNAL(valueChanged(int)), this, SLOT(slot_clipPlaneChanged()));
        connect(yPlaneSlider, &QSlider::valueChanged, this, &MainWindow::slot_clipPlaneChanged);
        connect(yPlaneFlip, &QPushButton::clicked, this, &MainWindow::slot_clipPlaneChanged);
    }

    // Row 3: Z Plane
    {
        zPlaneActived = new QCheckBox("Z Plane");
        clipPlaneDialogLayout->addWidget(zPlaneActived, 2, 0);

        zPlaneValue = new QSpinBox();
        clipPlaneDialogLayout->addWidget(zPlaneValue, 2, 1);

        zPlaneSlider = new QSlider(Qt::Horizontal);
        clipPlaneDialogLayout->addWidget(zPlaneSlider, 2, 2);

        zPlaneFlip = new QPushButton("Flip");
        clipPlaneDialogLayout->addWidget(zPlaneFlip, 2, 3);

        connect(zPlaneActived, &QCheckBox::stateChanged, this, &MainWindow::slot_clipPlaneChanged);
        connect(zPlaneValue, SIGNAL(valueChanged(int)), this, SLOT(slot_clipPlaneChanged()));
        connect(zPlaneSlider, &QSlider::valueChanged, this, &MainWindow::slot_clipPlaneChanged);
        connect(zPlaneFlip, &QPushButton::clicked, this, &MainWindow::slot_clipPlaneChanged);
    }

    clipPlaneDialog->setLayout(clipPlaneDialogLayout);
    clipPlaneDialog->show();

}


void MainWindow::slot_clipPlaneChanged() {
    qDebug() << "Değişiklik oldu.....";

//    xPlaneSlider->setValue(xPlaneValue->value());
//    yPlaneSlider->setValue(yPlaneValue->value());
//    zPlaneSlider->setValue(zPlaneValue->value());

    xPlaneValue->setValue(xPlaneSlider->value());
    yPlaneValue->setValue(yPlaneSlider->value());
    zPlaneValue->setValue(zPlaneSlider->value());


    qDebug() << "X eksenine clip plane eklendi.";


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


    double transp = 1.0 - ((double) information_transparencySlider->value() /
                           (double) information_transparencySlider->maximum());


    myViewerWidget->getContext()->SetTransparency(currentSelectedShape.shape, transp, true);

    currentSelectedShape.transparency = 1.0 - transp;
    findUpdatedItemFromUploadedObjects(currentSelectedShape, mySTEPProcessor->modelTree);
}

/** QTreeWidget üzerinde herhangi bir item'e tıklanıldığında çalışacak
 * fonksiyondur.
 *
 * @param arg_item : tıklanılan item
 */
void MainWindow::modelTreeItemClicked(QTreeWidgetItem *arg_item) {
    QString itemName = arg_item->data(0, Qt::EditRole).toString();
    qDebug() << "Model Tree üzerinden" << itemName << "seçildi.";

    /* currentSelectedShape = */findSelectedItemFromUploadedObjects(arg_item, mySTEPProcessor->modelTree);


    if (currentSelectedShape.Children.size() > 1) {
        myViewerWidget->getContext()->ClearSelected(false);
        for (int i = 0; i < currentSelectedShape.Children.size(); ++i) {
            Handle(AIS_InteractiveObject) obj = currentSelectedShape.Children[i].shape;
            myViewerWidget->getContext()->AddOrRemoveSelected(obj, true);
        }
        myViewerWidget->getContext()->UpdateCurrentViewer();

    } else {
        myViewerWidget->getContext()->ClearSelected(false);
        Handle(AIS_InteractiveObject) obj = currentSelectedShape.shape;

        myViewerWidget->getContext()->AddOrRemoveSelected(obj, true);
        myViewerWidget->getContext()->UpdateCurrentViewer();

        Bnd_Box outlineBox;
        BRepBndLib::Add(currentSelectedShape.topoShape, outlineBox);
        Standard_Real xMin, xMax, yMin, yMax, zMin, zMax;
        outlineBox.Get(xMin, xMax, yMin, yMax, zMin, zMax);
        gp_Pnt minPnt(xMin, yMin, zMin), maxPnt(xMax, yMax, zMax);
        TopoDS_Shape outlineShape = BRepPrimAPI_MakeBox(minPnt,maxPnt).Shape();

        Handle(AIS_InteractiveObject) obj2 = new AIS_Shape(outlineShape);
        myViewerWidget->getContext()->Display(obj2, true);
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
    if (currentSelectedShape.Children.size() > 1) {
        for (int i = 0; i < currentSelectedShape.Children.size(); ++i) {
            myViewerWidget->getContext()->Display(currentSelectedShape.Children[i].shape, false);
            myViewerWidget->getContext()->UpdateCurrentViewer();
            myViewerWidget->fitAll();
        }
    } else { /* Eğer alt üyeleri yok ise kendisini göstersin */
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









