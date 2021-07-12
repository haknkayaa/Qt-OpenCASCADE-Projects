//
// Created by hakan on 8.07.2021.
//

#ifndef PROJECT_DBManager_H
#define PROJECT_DBManager_H

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>


#include "DataStructs.h"

class DBManager{
public:
    DBManager(const QString& path);
    ~DBManager();

    bool isOpen() const;

    bool createMaterialTable(const QString &tableName);

    bool valueExists(QString tableName, QString column, const QString value) const;
    bool removeData(QString tableName, QString columnName, QString valueName);
    bool deleteTable(QString tableName);

    bool insertMaterial(QString tableName, table_material material);
    bool removeMaterial(QString tableName, table_material material);
    bool updateMaterial(QString tableName, table_material material);

    int getRowCount(QString tableName);

    void printAnEntireColumn(QString tableName, QString columnName) const;
    QList<QString> returnAnEntireColumn(QString tableName, QString columnName);
    QList<table_material> returnMaterialList(QString tableName);

    table_material findMaterial(QString tableName, QString column, QString value){

        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName + " WHERE " + column + " =  '" + value + "'");

        table_material temp;

        if (query.exec()) {
            if (query.next()) {

                temp.Index = query.value(0).toString();
                temp.Name = query.value(1).toString();
                temp.Formula = query.value(2).toString();
                temp.DensityValue = query.value(3).toString();
                temp.DensityUnit = query.value(4).toString();
                temp.SubElements = query.value(5).toString();
            }
        } else {
            qDebug() << "Value exists failed: " << query.lastError();
        }

        return temp;
    }

private:
    QSqlDatabase m_db;
};

#endif //PROJECT_DBManager_H
