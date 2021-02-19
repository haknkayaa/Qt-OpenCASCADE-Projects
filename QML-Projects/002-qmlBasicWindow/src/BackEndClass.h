/******************************************************************************
 *    BackEndClass.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/18/21.
 *     description :
 ******************************************************************************/

#ifndef BACKENDCLASS_H
#define BACKENDCLASS_H


// Our header file
#pragma once
#include <QObject>
#include <QString>
#include <QVector>

class BackEndClass : public QObject {
    Q_OBJECT

    // Ilgili mesaj degiskenine erisim yontemi
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
public:
    explicit BackEndClass(QObject *parent = nullptr);

    // Mevcut sayac degerini don
    Q_INVOKABLE int incrementCounter();

    // Degiskenimizin "setter" metodu
    void setMessage(const QString &msg);

    // Degiskenimizin "getter" metodu
    QString message() const;

    signals:
            // Degiskenimize iliskin sinyal
            void messageChanged();

public slots:
            void resetCounter();

private:
    // Degiskenimiz
    QString mMessage {"Hello from BackEndClass!"};

    // Sayac
    int mCounter{0};
};


#endif //BACKENDCLASS_H
