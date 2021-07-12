//
// Created by hakan on 8.07.2021.
//

#include "DBManager.h"

#include <QDebug>

///
/// \param path
DBManager::DBManager(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName(path);


    if (!m_db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
    }
}

///
DBManager::~DBManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

///
/// \return
bool DBManager::isOpen() const {
    return m_db.isOpen();
}


///
/// \param tableName
/// \return
bool DBManager::createMaterialTable(const QString &tableName) {
    bool success = true;

    QSqlQuery query;
    query.prepare("CREATE TABLE " + tableName +
                  "(materialIndex INTEGER PRIMARY KEY, "
                  + "materialName TEXT, "
                  + "materialFormula TEXT, "
                  + "materialDensityValue TEXT, "
                  + "materialDensityUnit TEXT, "
                  + "materialSubElements TEXT);");

    if (!query.exec()) {
        qDebug() << "Ayarlar tablosu zaten oluşturulmuş durumda.";
        success = false;
    }else{
        qDebug() << "Tablo oluşturuldu.";
    }

    return success;
}

///
/// \param tableName
/// \param material
/// \return
bool DBManager::insertMaterial(const QString tableName, table_material material) {
    bool success = false;

    if (!material.Name.isEmpty()) {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO " + tableName +
                         " (materialName, materialFormula, materialDensityValue, materialDensityUnit, materialSubElements) VALUES (:name, :formula, :density, :densityUnit, :subElements)");
        queryAdd.bindValue(":name", material.Name);
        queryAdd.bindValue(":formula", material.Formula);
        queryAdd.bindValue(":density", material.DensityValue);
        queryAdd.bindValue(":densityUnit", material.DensityUnit);
        queryAdd.bindValue(":subElements", material.SubElements);

        if (queryAdd.exec()) {
            success = true;
            qDebug() << "Material database'e eklendi.";
        } else {
            qDebug() << "Material eklenemedi : " << queryAdd.lastError();
        }
    }

    return success;
}

///
/// \param tableName
/// \param material
/// \return
bool DBManager::removeMaterial(QString tableName, table_material material) {
    bool success = false;

    if (!material.Name.isEmpty()) {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM " + tableName + " WHERE materialName =" + "'" + material.Name + "'");
        success = queryDelete.exec();

        if (!success) {
            qDebug() << "Material silinemedi: " << queryDelete.lastError();
        }else{
            qDebug() << "Material silindi.";
        }
    }

    return success;
}

///
/// \param tableName
/// \param material
/// \return
bool DBManager::updateMaterial(const QString tableName, table_material material) {
    bool success = false;

    QString query_str = "UPDATE " + tableName + " SET ";
    if (!material.Name.isEmpty()) query_str.append(" materialName=:name, ");
    if (!material.Formula.isEmpty()) query_str.append(" materialFormula=:formula, ");
    if (!material.DensityValue.isEmpty()) query_str.append(" materialDensityValue=:density, ");
    if (!material.DensityUnit.isEmpty()) query_str.append(" materialDensityUnit=:densityUnit, ");
    if (!material.SubElements.isEmpty()) query_str.append(" materialSubElements=:subElements ");
    query_str.append(" WHERE materialName= '" + material.Name + "'");

    qDebug() << query_str;

    if (!material.Name.isEmpty()) {

        QSqlQuery queryAdd;
        queryAdd.prepare(query_str);
        if (!material.Name.isEmpty()) queryAdd.bindValue(":name", material.Name);
        if (!material.Formula.isEmpty()) queryAdd.bindValue(":formula", material.Formula);
        if (!material.DensityValue.isEmpty()) queryAdd.bindValue(":density", material.DensityValue);
        if (!material.DensityUnit.isEmpty()) queryAdd.bindValue(":densityUnit", material.DensityUnit);
        if (!material.SubElements.isEmpty()) queryAdd.bindValue(":subElements", material.SubElements);

        if (queryAdd.exec()) {
            success = true;
            qDebug() << "Material güncellendi";
        } else {
            qDebug() << "Material güncellenemedi : " << queryAdd.lastError();
        }
    }

    return success;
}

///
/// \param tableName
/// \param columnName
/// \param valueName
/// \return
bool DBManager::removeData(const QString tableName, const QString columnName, QString valueName) {
    bool success = false;

    if (valueExists(QString(), QString(), valueName)) {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM " + tableName + " WHERE " + columnName + " = (:name)");
        queryDelete.bindValue(":name", columnName);
        success = queryDelete.exec();

        if (!success) {
            qDebug() << "remove person failed: " << queryDelete.lastError();
        }
    } else {
        qDebug() << "Remove material failed: material doesnt exist";
    }

    return success;
}

///
/// \param tableName
/// \return
bool DBManager::deleteTable(QString tableName) {
    bool success = false;

    QSqlQuery queryDelete;
    queryDelete.prepare("DROP TABLE " + tableName + " ");
    success = queryDelete.exec();

    if (!success) {
        qDebug() << "Deleting failed: " << queryDelete.lastError();
        return false;
    } else {
        return true;
    }

}

/// herhangi bi sutün adını istelinen table içerisinde arar ve tüm
/// sonuçları döndürür.
/// \param tableName
/// \param columnName
void DBManager::printAnEntireColumn(QString tableName, QString columnName) const {
    qDebug() << columnName << " in database:";
    QSqlQuery query("SELECT * FROM " + tableName);
    int idName = query.record().indexOf(columnName);
    while (query.next()) {
        QString value = query.value(idName).toString();
        qDebug() << "===" << value;
    }
}

///
/// \param tableName
/// \param columnName
/// \return
QList<QString> DBManager::returnAnEntireColumn(QString tableName, QString columnName) {
    QList<QString> valueList;
    QSqlQuery query("SELECT * FROM " + tableName);
    int idName = query.record().indexOf(columnName);
    while (query.next()) {
        QString value = query.value(idName).toString();
        valueList.push_back(value);
    }
    return valueList;
}

///
/// \param tableName
/// \return
QList<table_material> DBManager::returnMaterialList(QString tableName) {
    QList<table_material> list;

    QSqlQuery query;
    query.exec ("SELECT * FROM materials");

    int i=0;
    while (query.next()) {
        table_material temp;
        temp.Index = query.value(0).toString();
        temp.Name = query.value(1).toString();
        temp.Formula = query.value(2).toString();
        temp.DensityValue = query.value(3).toString();
        temp.DensityUnit = query.value(4).toString();
        temp.SubElements = query.value(5).toString();

        list.push_back(temp);
    }
    return list;
}

///
/// \param tableName
/// \param column
/// \param value
/// \return
bool DBManager::valueExists(const QString tableName, const QString column, const QString value) const {
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT " + column + " FROM " + tableName + " WHERE " + column + " = (:value)");
    checkQuery.bindValue(":value", value);

    if (checkQuery.exec()) {
        if (checkQuery.next()) {
            exists = true;
        }
    } else {
        qDebug() << "Value exists failed: " << checkQuery.lastError();
    }

    return exists;
}

///
/// \param tableName
/// \return
int DBManager::getRowCount(QString tableName) {
    int row = 0;
    QSqlQuery query;
    query.prepare("SELECT COUNT(1) FROM " + tableName);

    if (!query.exec()) {
        qDebug() << "Satır sayısı elde edilemedi.";
    }else{
        query.first();
        row = query.value(0).toInt();
        qDebug() << "Satır sayısı : " << QString::number(row);
    }

    return row;
}

