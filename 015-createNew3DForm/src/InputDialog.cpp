//
// Created by emir on 5.02.2021.
//

#include "InputDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

/**
 *
 * @param parent
 * @param input (şekil için verilmesi gereken girdi sayısı)
 */
InputDialog::InputDialog(QWidget *parent , int input) : QDialog(parent)
{
    this->input = input;
    QFormLayout *lytMain = new QFormLayout(this);
    QValidator *validator = new QDoubleValidator(0, 100, 3, this);
    //CUBE
    if(input == 3){
        for (int i = 0; i < 3; ++i) {
            QLabel *xLabel = new QLabel(QString("X"), this);
            if (i == 1)
                xLabel->setText("Y");
            else if (i == 2)
                xLabel->setText("Z");
            QLineEdit *xLine = new QLineEdit(this);
            xLine->setValidator(validator);
            lytMain->addRow(xLabel, xLine);
            fields << xLine;
        }
    }
    //CUBE

    //CYLINDER
    else if(input == 2){
        for (int i = 0; i < 2; ++i) {
            QLabel *xLabel = new QLabel(QString("R"), this);
            if (i == 1)
                xLabel->setText("H");
            QLineEdit *xLine = new QLineEdit(this);
            xLine->setValidator(validator);
            lytMain->addRow(xLabel, xLine);
            fields << xLine;
        }
    }
    //CYLINDER

    //SPHERE
    else{
        QLabel *xLabel = new QLabel(QString("R"), this);
        QLineEdit *xLine = new QLineEdit(this);
        xLine->setValidator(validator);
        lytMain->addRow(xLabel, xLine);
        fields << xLine;
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
QList<float> InputDialog::getFloats(QWidget *parent, bool *ok, int input)
{
    InputDialog *dialog = new InputDialog(parent, input);
    dialog->setWindowTitle("Değerleri giriniz.");
    QList<float> list;

    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        foreach (auto field, dialog->fields) {
            if(!field->text().isEmpty())
                list << field->text().toFloat();
        }
    }
    dialog->deleteLater();
    return list;
}