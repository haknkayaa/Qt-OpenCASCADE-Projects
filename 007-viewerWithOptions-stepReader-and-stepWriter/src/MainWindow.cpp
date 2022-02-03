#include "MainWindow.h"
#include "./ui_mainwindow.h"

#define GETNODE(item) qvariant_cast<OCCData *>(item->data(0, Qt::UserRole))


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->widget_right->setMaximumWidth(350);


    // Viewer
    Viewer *myViewer = new Viewer(this);
    ui->viewerlayout->addWidget(myViewer);

    // STEP reader
    STEPProcessor *mySTEPProcessor = new STEPProcessor(this);
    STEPWriter *mySTEPWriter       = new STEPWriter(this);

    connect(ui->importButton, &QPushButton::clicked, [this, mySTEPProcessor]{
        QString homeLocation = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(),QStandardPaths::LocateDirectory);
        QString supportedFileType = "STEP Files (*.step *.stp *.STEP *.STP)";
        QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                        homeLocation,
                                                        supportedFileType);

        if(!fileName.isNull()) {
            qDebug() << "--> Opening file:" << fileName;
            mySTEPProcessor->loadSTEPFile(fileName);
        }

        else {
            qDebug() << "--> Opening file: NULL";
        }
    });
    connect(ui->exportButton, &QPushButton::clicked, [this, mySTEPWriter, mySTEPProcessor]{
       mySTEPWriter->saveFileFromLabel("test.step", mySTEPProcessor->getAllShapeLabel());
    });

    connect(mySTEPProcessor, &STEPProcessor::readyShapeForDisplay, myViewer, &Viewer::slot_showShape);
    connect(mySTEPProcessor, &STEPProcessor::readyTreeWidgetItem, [this](QTreeWidgetItem* item){
       ui->treeWidget->addTopLevelItem(item);
       ui->treeWidget->expandAll();
    });

    connect(ui->treeWidget, &QTreeWidget::itemClicked, [this, myViewer](QTreeWidgetItem* item){
        myViewer->getContext()->ClearSelected(true);

        qDebug() << "Item clicked...";

        if (item->childCount() > 0) {
            for (int i = 0; i < item->childCount (); i++){
                QTreeWidgetItem *child = item->child (i);
                myViewer->getContext()->AddOrRemoveSelected(GETNODE(child)->getObject(), true);
            }
        }
        else {
            myViewer->getContext()->AddOrRemoveSelected(GETNODE(item)->getObject(), true);
        }
        myViewer->getContext()->UpdateCurrentViewer();
    });

    // on/off settings for viewer
    connect(ui->check_show3DGrid, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_show3DGrid(int)));
    connect(ui->check_showTrihedronCube, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_showTrihedronCube(int)));
    connect(ui->check_showPerformanceStats, SIGNAL(stateChanged(int)), myViewer, SLOT(slot_showPerformanceStats(int)));

    // projection mode
    connect(ui->viewMode, &QComboBox::currentTextChanged, myViewer, &Viewer::slot_changeProjectionMode);

    // explode
    connect(ui->check_explode, &QCheckBox::stateChanged, [this, myViewer]{
        ui->slider_explode->setEnabled(ui->check_explode->isChecked());
        myViewer->slot_explode(ui->check_explode->isChecked(), ui->slider_explode->value());
    });
    connect(ui->slider_explode, &QSlider::valueChanged, [this, myViewer]{
        myViewer->slot_explode(ui->check_explode->isChecked(), ui->slider_explode->value());
    });

    // clip plane initial settings and signal/slots
    ui->button_XInvert->setEnabled(false);
    ui->slider_XPlane->setEnabled(false);
    QObject::connect(ui->check_XPlane, &QCheckBox::stateChanged, [this] {
        ui->button_XInvert->setEnabled(ui->check_XPlane->isChecked());
        ui->slider_XPlane->setEnabled(ui->check_XPlane->isChecked());
    });

    ui->button_YInvert->setEnabled(false);
    ui->slider_YPlane->setEnabled(false);
    QObject::connect(ui->check_YPlane, &QCheckBox::stateChanged, [this] {
        ui->button_YInvert->setEnabled(ui->check_YPlane->isChecked());
        ui->slider_YPlane->setEnabled(ui->check_YPlane->isChecked());
    });

    ui->button_ZInvert->setEnabled(false);
    ui->slider_ZPlane->setEnabled(false);
    QObject::connect(ui->check_ZPlane, &QCheckBox::stateChanged, [this] {
        ui->button_ZInvert->setEnabled(ui->check_ZPlane->isChecked());
        ui->slider_ZPlane->setEnabled(ui->check_ZPlane->isChecked());
    });

    // view projection buttons
    connect(ui->xpos_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(1); });
    connect(ui->xneg_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(2); });
    connect(ui->ypos_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(3); });
    connect(ui->yneg_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(4); });
    connect(ui->zpos_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(5); });
    connect(ui->zneg_button, &QPushButton::clicked, myViewer, [myViewer] { myViewer->slot_changeProjectionAxis(6); });

    // actual 3d position [inline defined]
    connect(myViewer, &Viewer::mousePosChanged, [this, myViewer](const QPoint currPos) {
        gp_Pnt pos3d = myViewer->getCursor3DPosition(currPos);

        ui->label_xPos->setText(QString::number(pos3d.X(), 'f', 3));
        ui->label_yPos->setText(QString::number(pos3d.Y(), 'f', 3));
        ui->label_zPos->setText(QString::number(pos3d.Z(), 'f', 3));
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            break;

        case Qt::Key_S:
            break;

        case Qt::Key_A:
            break;

        case Qt::Key_D:
            break;

    }
}
