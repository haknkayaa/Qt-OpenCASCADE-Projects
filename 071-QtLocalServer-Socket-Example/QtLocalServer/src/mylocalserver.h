#ifndef MYLOCALSERVER_H
#define MYLOCALSERVER_H

#include <QWidget>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTextEdit>

class MyLocalServer : public QWidget {
Q_OBJECT

public:

    explicit MyLocalServer(QString serverName, QWidget *parent = 0);

    ~MyLocalServer() override;

    void start();

    void sendMessage(QString msg);

    QLocalServer *localServer;
    QString serverName;


    QList<QLocalSocket*> clientLists;

    void sendToClient(QLocalSocket *localSocket, const QString &string);

public slots:

    virtual void slotNewConnection();


    QString slotReadClient();

signals:
    void messageReceived(QString msg);
    void newClientJoined(QString client);
};

#endif 