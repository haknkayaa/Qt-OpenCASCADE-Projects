//
// Created by hakan on 13.01.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mylocalserver.h"

mainwindow::mainwindow(QWidget *parent) :
        QDialog(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);

    this->setWindowTitle("Server");

    ui->label->setText("Device Name: Server");
    ui->label_2->setText("Status: Started");
    ui->listWidget->addItem("Server (This machine)");

    connect(ui->pushButton, &QPushButton::clicked, this, &mainwindow::sendButton);

    // qlocalserver
    server = new MyLocalServer("MyLocalServer2");
    connect(server, &MyLocalServer::messageReceived, this, &mainwindow::printMessage);
    connect(server, &MyLocalServer::newClientJoined, this, &mainwindow::addClient);
    server->start();
}

mainwindow::~mainwindow() {
    delete ui;
}


void mainwindow::sendButton() {
    server->sendMessage("Server:" + ui->lineEdit->text());
    ui->lineEdit->clear();
}

void mainwindow::printMessage(QString msg) {
    qDebug() << "Mesaj alındı.";

    ui->plainTextEdit->appendPlainText(msg);
}

void mainwindow::addClient(const QString &client) {
    qDebug() << "Yeni bir client katıldı.";

    ui->listWidget->addItem(client);
}