//
// Created by hakan on 8.07.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MaterialDialog.h" resolved

#include "MaterialDialog.h"
#include "ui_MaterialDialog.h"


MaterialDialog::MaterialDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::MaterialDialog) {
    ui->setupUi(this);

    // UI init (for tableWidget)
    ui->materialsTableWidget->setColumnCount(3);
    ui->materialsTableWidget->setRowCount(0);
    ui->materialsTableWidget->setHorizontalHeaderLabels({"Name", "Formula", "Density"});
    ui->materialsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->materialsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->materialsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    static const QString databaseName = "database.db";
    db = new DBManager(databaseName);

    updateMaterialTableWidget();

    // clear button
    connect(ui->clear, &QPushButton::clicked, [this] {
        ui->materialName->clear();
        ui->materialDensity->clear();
        ui->nValue->clear();
    });

    // add button
    connect(ui->addMaterial, &QPushButton::clicked, [this] {

        table_material temp;
        temp.Name = ui->materialName->text();
        temp.Formula = ui->materialName->text();
        temp.DensityValue = ui->materialDensity->text();
        temp.DensityUnit = ui->densityUnit->currentText();
        temp.SubElements = "skdf";

        db->insertMaterial("materials", temp);

        updateMaterialTableWidget();

    });

    // edit button
    connect(ui->editMaterial, &QPushButton::clicked, [this] {

        table_material temp;
        temp.Name = ui->materialName->text();
        temp.Formula = ui->materialFormula->text();
        temp.DensityValue = ui->materialDensity->text();
        temp.DensityUnit = ui->densityUnit->currentText();
        temp.SubElements = "skdf";

        db->updateMaterial("materials", temp);

        updateMaterialTableWidget();
    });

    // delete button
    connect(ui->deleteMaterial, &QPushButton::clicked, [this] {

        table_material temp;
        temp.Name = ui->materialName->text();
        temp.Formula = ui->materialName->text();
        temp.DensityValue = ui->materialDensity->text();
        temp.DensityUnit = ui->densityUnit->currentText();
        temp.SubElements = "skdf";

        db->removeMaterial("materials", temp);

        updateMaterialTableWidget();
    });

    // Add fraction

    connect(ui->addSubElement, &QPushButton::clicked, [this] {
        t_subElement tempSubElement;

        if (ui->fractionCompositeSelect->currentText() == "Fraction") {
            tempSubElement.type = "Fraction";
            tempSubElement.elementName = ui->fractionElement->currentText();
            tempSubElement.elementRate = ui->nValue->text();

//            currentMaterial.SubElements.append(tempSubElement);
        } else if (ui->fractionCompositeSelect->currentText() == "Composite") {
            tempSubElement.type = "Composite";
            tempSubElement.elementName = ui->fractionElement->currentText();
            tempSubElement.elementRate = ui->nValue->text();

//            currentMaterial.SubElements.append(tempSubElement);
        }

        // Fraction / Composite ekleme çıkarma kısmı
        QString subLabelText = tempSubElement.type + " => " + tempSubElement.elementName + " : " + tempSubElement.elementRate;
        QLabel *subLabel = new QLabel("");
        subLabel->setText(subLabelText);
        QPushButton *subElementRemoveButton = new QPushButton("-" + QString::number(i));

        QWidget *subElementWidget = new QWidget();
        QGridLayout *subElementWidgetLayout = new QGridLayout();
        subElementWidgetLayout->setMargin(0);
        subElementWidget->setLayout(subElementWidgetLayout);
        subElementWidgetLayout->addWidget(subLabel, 0, 0, 1, 3);
        subElementWidgetLayout->addWidget(subElementRemoveButton, 0, 3, 1, 1);

        ui->gridLayout->addWidget(subElementWidget, ui->gridLayout->rowCount() , 0, 1,4);

        i++;
        connect(subElementRemoveButton, &QPushButton::clicked, subElementRemoveButton, &QPushButton::deleteLater);
        connect(subElementRemoveButton, &QPushButton::destroyed, [this, subElementRemoveButton, subElementWidget]{
            qDebug() << subElementRemoveButton->text() << "numaralı button silindi.";
            subElementWidget->deleteLater();
        });


    });


    // tableWidget selected row
    connect(ui->materialsTableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, [this] {
        QModelIndexList selection = ui->materialsTableWidget->selectionModel()->selectedRows();

        if(!selection.isEmpty()){
            table_material test = db->findMaterial("materials", "materialName",
                                                   ui->materialsTableWidget->item(selection[0].row(), 0)->text());

            if (!test.Name.isEmpty()) {
                qDebug() << "test material databaseden bulundu" << test.DensityValue;

                // gerekli bölgelere bilgilerin doldurulması
                ui->materialName->setText(test.Name);
                ui->materialDensity->setText(test.DensityValue);
                ui->densityUnit->setCurrentText(test.DensityUnit);
                ui->materialFormula->setText(test.Formula);

            } else {
                qDebug() << "test material bulunamadı";
            }
        }
    });
}

MaterialDialog::~MaterialDialog() {
    delete ui;
}

bool MaterialDialog::updateMaterialTableWidget() {
    if (db->isOpen()) {

        qDebug() << "Material updated";

        // element listesini databaseden çekerek yükle
        ui->fractionElement->addItems(db->returnAnEntireColumn("elements", "element"));

        // kaç material olduğunu label'a bas
        ui->materialsCountLabel->setText(QString::number(db->getRowCount("materials")) + " materials found.");

        // material listesini tableWidget'a yerleştir
        int i = 0;
        ui->materialsTableWidget->clearContents();
        ui->materialsTableWidget->setRowCount(0);

        for (auto it : db->returnMaterialList("materials")) {
            ui->materialsTableWidget->insertRow(i);
            ui->materialsTableWidget->setItem(i, 0, new QTableWidgetItem(it.Name));
            ui->materialsTableWidget->setItem(i, 1, new QTableWidgetItem(it.Formula));
            ui->materialsTableWidget->setItem(i, 2, new QTableWidgetItem(it.DensityValue + " " + it.DensityUnit));
            i++;
        }
    } else {
        qDebug() << "Database could not be opened!";
    }
}




