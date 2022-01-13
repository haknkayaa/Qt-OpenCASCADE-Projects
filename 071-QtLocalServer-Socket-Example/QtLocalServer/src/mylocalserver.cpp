#include "mylocalserver.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QTime>

MyLocalServer::MyLocalServer(QString serverName, QWidget *parent)
        : QWidget(parent) ,serverName(serverName){


    localServer = new QLocalServer(this);

    connect(localServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

MyLocalServer::~MyLocalServer() {
    QLocalServer::removeServer(serverName);
    delete localServer;
}


void MyLocalServer::start(){
    while (!localServer->listen(serverName)) {
        QMessageBox::critical(nullptr, "Server error",
                              "Unable to start server: " + localServer->errorString());
        localServer->close();

        QLocalServer::removeServer(serverName);
    }
}

void MyLocalServer::slotNewConnection() {
    QLocalSocket *localSocket = localServer->nextPendingConnection();
//    localSocket->setSocketDescriptor()
    clientLists.append(localSocket);

    connect(localSocket, SIGNAL(disconnected()), localSocket, SLOT(deleteLater()));
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    sendToClient(localSocket, "Server response: Connected!");

    emit newClientJoined("Client " + QString::number(localSocket->socketDescriptor()));
}


QString MyLocalServer::slotReadClient() {
    QLocalSocket *localSocket = (QLocalSocket *) sender();

    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);

    quint16 nextBlockSize  = 0;

    for (;;) {
        if (!nextBlockSize) {
            if (localSocket->bytesAvailable() < (int) sizeof(quint16))
                break;

            in >> nextBlockSize;
        }

        QTime time;
        QString string;
        in >> time >> string;

        QString message = "(" + time.toString() + ") " + "Client has sent - " + string;

        nextBlockSize = 0;

//        sendToClient(localSocket, "Server response: received \"" + string + "\"");

        emit messageReceived(message);
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


void MyLocalServer::sendMessage(QString msg) {
    QByteArray array;

    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << msg;

    out.device()->seek(0);
    out << quint16(array.size() - sizeof(quint16));

    clientLists.at(0)->write(array);
}

