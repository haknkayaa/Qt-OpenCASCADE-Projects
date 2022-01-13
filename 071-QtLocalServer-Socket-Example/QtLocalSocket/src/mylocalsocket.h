#ifndef MYLOCALSOCKET_H
#define MYLOCALSOCKET_H

#include <QWidget>
#include <QLocalSocket>
#include <QTextEdit>
#include <QPushButton>

class MyLocalSocket : public QWidget {
Q_OBJECT

public:
    MyLocalSocket(QString serverName, QWidget *parent = 0);
    ~MyLocalSocket();

private:

    QLocalSocket *localSocket;
    QTextEdit *textEdit;
    QPushButton *sendRevision;
    quint16 nextBlockSize;
    int revision;

private slots:

    void slotReadyRead();
    void slotError(QLocalSocket::LocalSocketError error);
    void slotSendToServer();
    void slotConnected();
};

#endif 