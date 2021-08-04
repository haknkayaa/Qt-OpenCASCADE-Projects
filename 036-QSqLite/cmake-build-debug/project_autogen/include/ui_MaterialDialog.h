/********************************************************************************
** Form generated from reading UI file 'MaterialDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALDIALOG_H
#define UI_MATERIALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MaterialDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QComboBox *fractionElement;
    QLineEdit *materialDensity;
    QLineEdit *nValue;
    QFrame *line;
    QComboBox *fractionCompositeSelect;
    QPushButton *deleteMaterial;
    QLabel *label;
    QPushButton *addSubElement;
    QPushButton *editMaterial;
    QLabel *label_2;
    QComboBox *densityUnit;
    QLabel *label_3;
    QPushButton *addMaterial;
    QPushButton *clear;
    QLineEdit *materialFormula;
    QLineEdit *materialName;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QLabel *materialsCountLabel;
    QTableWidget *materialsTableWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *importDB;
    QPushButton *exportDB;
    QPushButton *resetDB;

    void setupUi(QDialog *MaterialDialog)
    {
        if (MaterialDialog->objectName().isEmpty())
            MaterialDialog->setObjectName(QString::fromUtf8("MaterialDialog"));
        MaterialDialog->resize(590, 641);
        verticalLayout_2 = new QVBoxLayout(MaterialDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(MaterialDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        fractionElement = new QComboBox(groupBox);
        fractionElement->addItem(QString());
        fractionElement->setObjectName(QString::fromUtf8("fractionElement"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(fractionElement->sizePolicy().hasHeightForWidth());
        fractionElement->setSizePolicy(sizePolicy);

        gridLayout->addWidget(fractionElement, 4, 2, 1, 1);

        materialDensity = new QLineEdit(groupBox);
        materialDensity->setObjectName(QString::fromUtf8("materialDensity"));

        gridLayout->addWidget(materialDensity, 3, 1, 1, 2);

        nValue = new QLineEdit(groupBox);
        nValue->setObjectName(QString::fromUtf8("nValue"));

        gridLayout->addWidget(nValue, 4, 1, 1, 1);

        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 4, 4, 1);

        fractionCompositeSelect = new QComboBox(groupBox);
        fractionCompositeSelect->addItem(QString());
        fractionCompositeSelect->addItem(QString());
        fractionCompositeSelect->setObjectName(QString::fromUtf8("fractionCompositeSelect"));

        gridLayout->addWidget(fractionCompositeSelect, 4, 0, 1, 1);

        deleteMaterial = new QPushButton(groupBox);
        deleteMaterial->setObjectName(QString::fromUtf8("deleteMaterial"));

        gridLayout->addWidget(deleteMaterial, 4, 5, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        addSubElement = new QPushButton(groupBox);
        addSubElement->setObjectName(QString::fromUtf8("addSubElement"));

        gridLayout->addWidget(addSubElement, 4, 3, 1, 1);

        editMaterial = new QPushButton(groupBox);
        editMaterial->setObjectName(QString::fromUtf8("editMaterial"));

        gridLayout->addWidget(editMaterial, 3, 5, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        densityUnit = new QComboBox(groupBox);
        densityUnit->addItem(QString());
        densityUnit->addItem(QString());
        densityUnit->addItem(QString());
        densityUnit->setObjectName(QString::fromUtf8("densityUnit"));

        gridLayout->addWidget(densityUnit, 3, 3, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        addMaterial = new QPushButton(groupBox);
        addMaterial->setObjectName(QString::fromUtf8("addMaterial"));

        gridLayout->addWidget(addMaterial, 2, 5, 1, 1);

        clear = new QPushButton(groupBox);
        clear->setObjectName(QString::fromUtf8("clear"));

        gridLayout->addWidget(clear, 1, 5, 1, 1);

        materialFormula = new QLineEdit(groupBox);
        materialFormula->setObjectName(QString::fromUtf8("materialFormula"));

        gridLayout->addWidget(materialFormula, 2, 1, 1, 3);

        materialName = new QLineEdit(groupBox);
        materialName->setObjectName(QString::fromUtf8("materialName"));

        gridLayout->addWidget(materialName, 1, 1, 1, 3);


        horizontalLayout_2->addLayout(gridLayout);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MaterialDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        materialsCountLabel = new QLabel(groupBox_2);
        materialsCountLabel->setObjectName(QString::fromUtf8("materialsCountLabel"));

        verticalLayout->addWidget(materialsCountLabel);

        materialsTableWidget = new QTableWidget(groupBox_2);
        materialsTableWidget->setObjectName(QString::fromUtf8("materialsTableWidget"));

        verticalLayout->addWidget(materialsTableWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        importDB = new QPushButton(groupBox_2);
        importDB->setObjectName(QString::fromUtf8("importDB"));

        horizontalLayout->addWidget(importDB);

        exportDB = new QPushButton(groupBox_2);
        exportDB->setObjectName(QString::fromUtf8("exportDB"));

        horizontalLayout->addWidget(exportDB);

        resetDB = new QPushButton(groupBox_2);
        resetDB->setObjectName(QString::fromUtf8("resetDB"));

        horizontalLayout->addWidget(resetDB);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(groupBox_2);


        retranslateUi(MaterialDialog);

        QMetaObject::connectSlotsByName(MaterialDialog);
    } // setupUi

    void retranslateUi(QDialog *MaterialDialog)
    {
        MaterialDialog->setWindowTitle(QApplication::translate("MaterialDialog", "MaterialDialog", nullptr));
        groupBox->setTitle(QApplication::translate("MaterialDialog", "Material Properties", nullptr));
        fractionElement->setItemText(0, QApplication::translate("MaterialDialog", "Not Selected", nullptr));

        materialDensity->setText(QString());
        materialDensity->setPlaceholderText(QApplication::translate("MaterialDialog", "material density", nullptr));
        nValue->setText(QString());
        nValue->setPlaceholderText(QApplication::translate("MaterialDialog", "n value", nullptr));
        fractionCompositeSelect->setItemText(0, QApplication::translate("MaterialDialog", "Fraction", nullptr));
        fractionCompositeSelect->setItemText(1, QApplication::translate("MaterialDialog", "Composite", nullptr));

        deleteMaterial->setText(QApplication::translate("MaterialDialog", "Delete", nullptr));
        label->setText(QApplication::translate("MaterialDialog", "Material Name", nullptr));
        addSubElement->setText(QApplication::translate("MaterialDialog", "+", nullptr));
        editMaterial->setText(QApplication::translate("MaterialDialog", "Edit", nullptr));
        label_2->setText(QApplication::translate("MaterialDialog", "Density", nullptr));
        densityUnit->setItemText(0, QApplication::translate("MaterialDialog", "g/cm3", nullptr));
        densityUnit->setItemText(1, QApplication::translate("MaterialDialog", "kg/cm3", nullptr));
        densityUnit->setItemText(2, QApplication::translate("MaterialDialog", "kg/m3", nullptr));

        label_3->setText(QApplication::translate("MaterialDialog", "Material Formula", nullptr));
        addMaterial->setText(QApplication::translate("MaterialDialog", "Add", nullptr));
        clear->setText(QApplication::translate("MaterialDialog", "Clear", nullptr));
        materialFormula->setPlaceholderText(QApplication::translate("MaterialDialog", "material formula", nullptr));
        materialName->setText(QString());
        materialName->setPlaceholderText(QApplication::translate("MaterialDialog", "material name", nullptr));
        groupBox_2->setTitle(QApplication::translate("MaterialDialog", "Material List", nullptr));
        materialsCountLabel->setText(QApplication::translate("MaterialDialog", "N/A materials found.", nullptr));
        importDB->setText(QApplication::translate("MaterialDialog", "Import DB", nullptr));
        exportDB->setText(QApplication::translate("MaterialDialog", "Export DB", nullptr));
        resetDB->setText(QApplication::translate("MaterialDialog", "Reset DB", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaterialDialog: public Ui_MaterialDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALDIALOG_H
