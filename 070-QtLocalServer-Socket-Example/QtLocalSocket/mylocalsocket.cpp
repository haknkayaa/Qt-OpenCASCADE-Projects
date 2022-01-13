#include "mylocalsocket.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMessageBox>

MyLocalSocket::MyLocalSocket(QString serverName, QWidget *parent)
        : QWidget(parent), nextBlockSize(0), revision(0) {

    localSocket = new QLocalSocket(this);


    connect(localSocket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),
            this, &MyLocalSocket::slotError);


    localSocket->setServerName(serverName);


    connect(localSocket, SIGNAL(connected()), SLOT(slotConnected()));

    connect(localSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));


    textEdit = new QTextEdit;
    sendRevision = new QPushButton("Send next revision");


    connect(sendRevision, SIGNAL(clicked()), this, SLOT(slotSendToServer()));


    textEdit->setReadOnly(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Sender revisions"));
    layout->addWidget(textEdit);
    layout->addWidget(sendRevision);
    setLayout(layout);


    localSocket->connectToServer();
}

MyLocalSocket::~MyLocalSocket() {

}


void MyLocalSocket::slotReadyRead() {

    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {
        if (!nextBlockSize) {
            if (localSocket->bytesAvailable() < (int) sizeof(quint16))
                break;
        }
        in >> nextBlockSize;

        if (localSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString string;
        in >> time >> string;

        textEdit->append(time.toString() + " " + string);
        nextBlockSize = 0;
    }
}


void MyLocalSocket::slotError(QLocalSocket::LocalSocketError error) {
    QString strError =
            "Error: " + (error == QLocalSocket::ServerNotFoundError ?
                         "The server was not found." :
                         error == QLocalSocket::PeerClosedError ?
                         "The server is closed." :
                         error == QLocalSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(localSocket->errorString()));
    textEdit->append(strError);
}


void MyLocalSocket::slotSendToServer() {

    QByteArray arrayBlock;
    QDataStream out(&arrayBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    ++revision;
    QString message = "Revision: " + QString("%1").arg(revision);
    out << quint16(0) << QTime::currentTime() << message;

    out.device()->seek(0);
    out << quint16(arrayBlock.size() - sizeof(quint16));

    localSocket->write(arrayBlock);
}


void MyLocalSocket::slotConnected() {
    textEdit->append("Received the connected() signal");
}
