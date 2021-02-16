//
// Created by emir on 5.02.2021.
//

#include "InputDialog.h"

#include <QLabel>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>

/**
 *
 * @param parent
 * @param input (şekil için verilmesi gereken girdi sayısı)
 */
InputDialog::InputDialog(QWidget *parent , int input) : QDialog(parent)
{
    QFormLayout *lytMain = new QFormLayout(this);

    //CUBE
    if(input == 3){
        for (int i = 0; i < 3; ++i) {
            QLabel *label = new QLabel(QString("X"), this);
            if (i == 1)
                label->setText("Y");
            else if (i == 2)
                label->setText("Z");
            QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
            lytMain->addRow(label, spinBox);
            fields << spinBox;
        }
    }
    //CUBE

    //CYLINDER
    else if(input == 2){
        for (int i = 0; i < 2; ++i) {
            QLabel *label = new QLabel(QString("R"), this);
            if (i == 1)
                label->setText("H");
            QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
            lytMain->addRow(label, spinBox);
            fields << spinBox;
        }
    }
    //CYLINDER

    //SPHERE
    else{
        QLabel *label = new QLabel(QString("R"), this);
        QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
        lytMain->addRow(label, spinBox);
        fields << spinBox;
    }
    //SPHERE

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted,this, &InputDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,this, &InputDialog::reject);

    setLayout(lytMain);
}

/**
 *
 * @param parent
 * @param ok
 * @param input (şekil için verilmesi gereken girdi sayısı)
 * @return
 */
QList<double> InputDialog::getFloats(QWidget *parent, bool *ok, int input)
{
    InputDialog *dialog = new InputDialog(parent, input);
    dialog->setWindowTitle("Değerleri giriniz.");
    QList<double> list;

    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        foreach (auto field, dialog->fields) {
            if(field->value() != 0)
                list << field->value();
        }
    }
    dialog->deleteLater();
    return list;
}