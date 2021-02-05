//
// Created by emir on 5.02.2021.
//

#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QDoubleValidator>
class QLineEdit;
class QLabel;

class InputDialog : public QDialog
{
Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr, int input = 3);

    static QList<float> getFloats(QWidget *parent, bool *ok = nullptr, int input = 2);

private:
    QList<QLineEdit*> fields;
    int input;
};

#endif // INPUTDIALOG_H

