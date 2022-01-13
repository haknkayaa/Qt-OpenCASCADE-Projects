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


    connect(ui->pushButton, &QPushButton::clicked, this, &mainwindow::sendButton);


    MyLocalServer* server = new MyLocalServer("MyServer2");

    connect(server, &MyLocalServer::messageReceived, this, &mainwindow::printMessage);

    server->start();

}

mainwindow::~mainwindow() {
    delete ui;
}


void mainwindow::sendButton() {

}

void mainwindow::printMessage() {

}