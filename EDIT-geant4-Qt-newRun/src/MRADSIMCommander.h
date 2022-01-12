//
// Created by hakan on 28.10.2021.
//

#ifndef PROJECT_MRADSIMCOMMANDER_H
#define PROJECT_MRADSIMCOMMANDER_H

#include <QWidget>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MRADSIMCommander; }
QT_END_NAMESPACE

class MRADSIMCommander : public QWidget {
Q_OBJECT

public:
    explicit MRADSIMCommander(QWidget *parent = nullptr);

    ~MRADSIMCommander() override;

public slots:
    void sendCommand();

    void addCommand();

    void runCommandList();
    void runAllCommandList();
    void clearCommandList();
    void stepCommandList();
    void stopCommandList();

private:
    Ui::MRADSIMCommander *ui;
};


#endif //PROJECT_MRADSIMCOMMANDER_H
