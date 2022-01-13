//
// Created by hakan on 13.01.2022.
//

#ifndef SOMELIBRARY_MAINWINDOW_H
#define SOMELIBRARY_MAINWINDOW_H

#include <QDialog>
#include "mylocalserver.h"


QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QDialog {
Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);

    ~mainwindow() override;

private:
    Ui::mainwindow *ui;

    MyLocalServer* server;

private slots:
    void sendButton();
    void printMessage(QString msg);
    void addClient(const QString& client);
};


#endif //SOMELIBRARY_MAINWINDOW_H
