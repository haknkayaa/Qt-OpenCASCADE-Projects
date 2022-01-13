#include "mylocalserver.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QTime>

MyLocalServer::MyLocalServer(QString serverName, QWidget *parent)
        : QWidget(parent), nextBlockSize(0) ,serverName(serverName){


    localServer = new QLocalServer(this);
    if (!localServer->listen(serverName)) {
        QMessageBox::critical(nullptr, "Server error",
                              "Unable to start server: " + localServer->errorString());
        localServer->close();

        QLocalServer::removeServer(serverName);
        return;
    }


    connect(localServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));


//    textEdit = new QTextEdit;
//    textEdit->setReadOnly(true);
//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->addWidget(new QLabel(serverName));
//    layout->addWidget(textEdit);
//    setLayout(layout);
}

MyLocalServer::~MyLocalServer() {
    QLocalServer::removeServer(serverName);
}


void MyLocalServer::start(){

}

void MyLocalServer::slotNewConnection() {
    QLocalSocket *localSocket = localServer->nextPendingConnection();

    connect(localSocket, SIGNAL(disconnected()), localSocket, SLOT(deleteLater()));
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    sendToClient(localSocket, "Server response: Connected!");
}


QString MyLocalServer::slotReadClient() {
    QLocalSocket *localSocket = (QLocalSocket *) sender();

    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);

    for (;;) {
        if (!nextBlockSize) {
            if (localSocket->bytesAvailable() < (int) sizeof(quint16))
                break;

            in >> nextBlockSize;
        }

        QTime time;
        QString string;
        in >> time >> string;

        QString message = time.toString() + " " + "Client has sent - " + string;
//        textEdit->append(message);

        nextBlockSize = 0;

        sendToClient(localSocket, "Server response: received \"" + string + "\"");

        return message;
    }
}


void MyLocalServer::sendToClient(QLocalSocket *localSocket, const QString &string) {
    QByteArray array;

    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << string;

    out.device()->seek(0);
    out << quint16(array.size() - sizeof(quint16));

    localSocket->write(array);
}


