//
// Created by sufuk on 22.04.2021.
//

#ifndef OPENCASCADEPROJECT_TESTQSTRING_H
#define OPENCASCADEPROJECT_TESTQSTRING_H
#include <QtWidgets>
#include <QtTest/QtTest>

class TestQString: public QObject{
    Q_OBJECT


private slots:
    void toUpper_data();
    void toUpper();
};


#endif //OPENCASCADEPROJECT_TESTQSTRING_H
