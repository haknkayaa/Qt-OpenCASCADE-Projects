//
// Created by hakan on 22.09.2021.
//

#ifndef GDMLREADER_GDMLREADER_H
#define GDMLREADER_GDMLREADER_H

// Qt Libraries
#include <QtWidgets>
#include <QDomDocument>

struct MATERIAL{
    QString name;
    QString atomNumber;
};

// User Libraries
struct WORLD{
    QString world;
    QString materialref;
    QString solidref;
    QString physvol;
};

class GDMLReader : public QObject{
Q_OBJECT
public:
    explicit GDMLReader();
    ~GDMLReader();

    bool               readFile(QString);
    bool               printDump();
    QDomElement        getRootTag();
    QList<QDomElement> getSubTag(QDomElement);

    bool getMaterialsAnalysis(QDomElement);
    bool getStructureAnalysis(QDomElement);
    bool getDefineAnalysis(QDomElement);
    bool getSolidAnalysis(QDomElement);
    bool getSetupAnalysis(QDomElement);

private:
    QFile *file;
    QDomDocument document;

    QList<QString> shapeList;
    QList<QString> materialList;

    QList<QPair<QString, QString>> shape_material_list;

    // GUI components
    QTreeWidget *gdmlStructureTree;

    QTreeWidgetItem *rootItem;
    QTreeWidgetItem *materialsItem;
    QTreeWidgetItem *structuresItem;
};


#endif //GDMLREADER_GDMLREADER_H
