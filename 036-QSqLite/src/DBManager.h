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
    bool updateMaterial(QString tableName, table_material material, QString previousName);

    int getRowCount(QString tableName);

    void printAnEntireColumn(QString tableName, QString columnName) const;
    QList<QString> returnAnEntireColumn(QString tableName, QString columnName);
    QList<table_material> returnMaterialList(QString tableName);

    table_material findMaterial(QString tableName, QString column, QString value);

private:
    QSqlDatabase m_db;
};

#endif //PROJECT_DBManager_H
