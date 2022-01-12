//
// Created by hakan on 28.10.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MRADSIMCommander.h" resolved

#include "MRADSIMCommander.h"
#include "ui_MRADSIMCommander.h"

#include "MainWindow.h"

MRADSIMCommander::MRADSIMCommander(QWidget *parent) :
        QWidget(parent), ui(new Ui::MRADSIMCommander) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MRADSIMCommander::sendCommand);

    connect(ui->addCommandInQueue, &QToolButton::clicked, this, &MRADSIMCommander::addCommand);

    connect(ui->runButton, &QPushButton::clicked, this, &MRADSIMCommander::runCommandList);
    connect(ui->runAllButton, &QPushButton::clicked, this, &MRADSIMCommander::runAllCommandList);
    connect(ui->clearButton, &QPushButton::clicked, this, &MRADSIMCommander::clearCommandList);
    connect(ui->stepButton, &QPushButton::clicked, this, &MRADSIMCommander::stepCommandList);
    connect(ui->stopButton, &QPushButton::clicked, this, &MRADSIMCommander::stopCommandList);

    connect(ui->tableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, [this]{
        QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();

        qDebug() << "Selection changed: " <<  ui->tableWidget->item(selection[0].row(), 0)->text();
    });

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("List"));
    QHeaderView* header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

MRADSIMCommander::~MRADSIMCommander() {
    delete ui;
}


void MRADSIMCommander::sendCommand() {
    qDebug() << "Sending -> " << ui->lineEdit->text();

    ui->lineEdit->clear();
}

/// TableWidget'a lineEdit üzerindeki komutu ekler
void MRADSIMCommander::addCommand() {
    qDebug() << "Adding ->" << ui->lineEdit->text();

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0 , new QTableWidgetItem(ui->lineEdit->text()));

    ui->lineEdit->clear();
}

/// Tüm komutları işler
void MRADSIMCommander::runCommandList() {

    QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();

    QString command = ui->tableWidget->item(selection[0].row(), 0)->text();
    qDebug() << "Run: " <<  command;

//    MainWindow::UImanager->ApplyCommand(command.toUtf8().constData());
}

void MRADSIMCommander::runAllCommandList() {
    int totalCommandCount = ui->tableWidget->rowCount();

    for (int i = 0; i < totalCommandCount; i++) {
        QString command = ui->tableWidget->item(i, 0)->text();
        qDebug() << "Run: " <<  command;

//        MainWindow::UImanager->ApplyCommand(command.toUtf8().constData());
    }
}

/// Tüm komutları temizler
void MRADSIMCommander::clearCommandList() {
    ui->tableWidget->setRowCount(0);
}

/// Tüm komutları tek tek işler
void MRADSIMCommander::stepCommandList() {
   // todo: burayı tamamla
}

/// Çalıştırmayı durdurur
void MRADSIMCommander::stopCommandList() {
    // todo: burayı tamamla
}

