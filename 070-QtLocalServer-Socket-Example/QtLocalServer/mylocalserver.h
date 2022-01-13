#ifndef MYLOCALSERVER_H
#define MYLOCALSERVER_H

#include <QWidget>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTextEdit>

class MyLocalServer : public QWidget {
Q_OBJECT

public:
    MyLocalServer(QString serverName, QWidget *parent = 0);

    ~MyLocalServer();

private:

    QLocalServer *localServer;


    QTextEdit *textEdit;


    quint16 nextBlockSize;


    void sendToClient(QLocalSocket *localSocket, const QString &string);

public slots:

    virtual void slotNewConnection();


    void slotReadClient();
};

#endif 