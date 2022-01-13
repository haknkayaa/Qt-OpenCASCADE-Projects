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

private:

    QLocalServer *localServer;
    QString serverName;
    quint16 nextBlockSize;

    void sendToClient(QLocalSocket *localSocket, const QString &string);

public slots:

    virtual void slotNewConnection();


    QString slotReadClient();

signals:
    void messageReceived();
};

#endif 