//
// Created by hakan on 29.06.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <fstream>
#include <iostream>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Buttons
    connect(ui->addFile, &QPushButton::clicked, this, &MainWindow::openDialog);
    connect(ui->saveImageButton, &QPushButton::clicked, this, &MainWindow::saveImageGraph);

    connect(ui->fitAllButton, &QPushButton::clicked, [this]{
       ui->customPlot->replot();
    });

    connect(ui->savePDFButton, &QPushButton::clicked, this, &MainWindow::savePDFGraph);


    // List Widget
    connect(ui->listWidget, &QListWidget::currentRowChanged,
            [this] { drawScatter(ui->listWidget->currentItem()->text(), "\t"); });


}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::openDialog() {
    qDebug() << "Choose file...";

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), "/home/hakan/Downloads/",
                                                    tr("Data Files (*.dat) ;;"
                                                       "Text Files (*.txt *.TXT)"));

    if (!fileName.isEmpty()) {
        ui->listWidget->addItem(fileName);
//        drawScatter(fileName, "\t");
//        ui->customPlot->update();
    }
}


void MainWindow::setupPlot() {

}

void MainWindow::drawScatter(const QString &file, QString splitter) {
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setAdaptiveSampling(true);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssPlus);

    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));


    QCPErrorBars *errorBars = new QCPErrorBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    errorBars->removeFromLegend();
    errorBars->setAntialiased(false);
    errorBars->setDataPlottable(ui->customPlot->graph(0));
    errorBars->setPen(QPen(QColor(180, 180, 180)));
    ui->customPlot->graph(0)->setName("Error Bars");

//    QFile file("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/035-2DGraph/raw_data/example.dat");
//    QFile file("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/035-2DGraph/raw_data/exampleWithYError.dat");
    QFile *fileObject = new QFile(file);
    QTextStream in(fileObject);

    QVector<double> x(300), y(300), yError(300);


    if (!fileObject->open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int count = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList variablesList = line.split(splitter);

//        qDebug() << "Line :" << line;
        qDebug() << count << "1:" << variablesList.at(0).toDouble() << " 2:" << variablesList.at(1).toDouble()
                 << " 3:" << variablesList.at(2).toDouble();

        if (variablesList.at(0).toDouble() + variablesList.at(1).toDouble() > 0) {
            x.append(variablesList.at(0).toDouble(NULL));
            y.append(variablesList.at(1).toDouble(NULL));
            yError.append(variablesList.at(2).toDouble(NULL));

        }
        count++;
        if (count == 500) break;

    }

    ui->customPlot->graph(0)->setData(x, y);
    errorBars->setData(yError);

    ui->customPlot->xAxis->setLabel("X raw data");
    ui->customPlot->yAxis->setLabel("Y raw data");

    ui->customPlot->xAxis->setRange(0, 7);
    ui->customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);


    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));


    ui->customPlot->graph(0)->rescaleAxes();

    ui->customPlot->xAxis->setTickLabelRotation(30);
    ui->customPlot->xAxis->ticker()->setTickCount(9);
    ui->customPlot->xAxis->setNumberFormat("ebc");
    ui->customPlot->xAxis->setNumberPrecision(1);

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems);

    ui->customPlot->replot();
}

void MainWindow::saveImageGraph() {
    ui->customPlot->saveJpg("/home/hakan/Desktop/graph.jpg", 0, 0, 1.0, -1);
    qDebug() << "Saved image";
}

void MainWindow::savePDFGraph(){
    ui->customPlot->savePdf("/home/hakan/Desktop/graph.pdf",0,0,QCP::epAllowCosmetic);
}