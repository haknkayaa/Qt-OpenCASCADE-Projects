//
// Created by hakan on 8.07.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MaterialDialog.h" resolved

#include "MaterialDialog.h"
#include "ui_MaterialDialog.h"

#include "DBManager.h"

MaterialDialog::MaterialDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::MaterialDialog) {
    ui->setupUi(this);


    static const QString path = "database.db";
    DBManager db(path);

    if (db.isOpen()){
        //if not exist create the database tables.
//        db.createMusicListTable(db.tableNamesMusicList);
//        db.createSettingsTable(db.tableNamesSettings);
        db.printAllPersons();
    }
    else
    {
        qDebug() << "Database could not be opened!";
    }

}

MaterialDialog::~MaterialDialog() {
    delete ui;
}

