//
// Created by hakan on 22.09.2021.
//

#ifndef GDMLREADER_GDMLREADER_H
#define GDMLREADER_GDMLREADER_H

// Qt Libraries
#include <QtWidgets>
#include <QDomDocument>

// User Libraries
#include "MainWindow.h"


enum SUB_ELEMENT_TYPE{
    FRACTION,
    COMPOSITE
};

struct t_Element{
    QString name;
    QString formula;
    QString Z;
    QString atomValue;
};

struct t_SubElement{
    SUB_ELEMENT_TYPE type;
    QString n;
    QString ref;
};

struct t_Material {
    QString name;
    QString dValue;
    QString dUnit;
    QList<t_SubElement> subElement;
};

struct t_Shape {
    QString solidref;
    QString physvol;
    QString volumeref;
};

struct t_World {
    QString ref;
    QString solidref;
    QString materialref;
    QList<t_Shape> childShapes;
};



class GDMLData {
public:
    GDMLData() {

    }
    GDMLData(const GDMLData& gdmlData) {
        this->Name = gdmlData.Name;
    }

    virtual ~GDMLData() {

    }


private:
    QString Name;

};
Q_DECLARE_METATYPE(GDMLData*);



class GDMLReader : public QObject{
Q_OBJECT
public:
    explicit GDMLReader();
    ~GDMLReader();

    bool               readFile(QString);

    QDomElement        getRootTag();
    QList<QDomElement> getSubTag(QDomElement);

    t_World getWorld();

    bool getMaterialsAnalysis(QDomElement);
    bool getStructureAnalysis(QDomElement);
    bool getDefineAnalysis(QDomElement);
    bool getSolidAnalysis(QDomElement);
    bool getSetupAnalysis(QDomElement);

    void addOnceMaterial(QList<t_Material>&, t_Material);
    void addOnceElement(QList<t_Element>&, t_Element);

public slots:


private:
    QFile *file;
    QDomDocument document;


    QList<t_Material> materialList;
    QList<t_Element> elementList;

    // GUI components
    QTreeWidgetItem *rootItem;
    QTreeWidgetItem *materialsItem;
    QTreeWidgetItem *structuresItem;


signals:
    void editedGDMLFile();
};


#endif //GDMLREADER_GDMLREADER_H
