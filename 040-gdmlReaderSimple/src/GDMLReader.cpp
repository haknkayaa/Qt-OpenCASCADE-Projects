//
// Created by hakan on 22.09.2021.
//

#include "GDMLReader.h"

QString domElementToRawXML(const QDomElement& elem)
{
    QString head = "<"+elem.tagName();
    QDomNamedNodeMap attrs = elem.attributes();
    for(int i = 0; i<attrs.size(); ++i)
    {
        QDomAttr attr = attrs.item(i).toAttr();
        head +=
                QString::fromLatin1(" %0=\"%1\"")
                        .arg(attr.name())
                        .arg(attr.value());
    }
    head += ">";
    return head + elem.text() + "</"+elem.tagName()+">";
}

GDMLReader::GDMLReader() {
    auto widget = new QWidget();
    auto layout = new QHBoxLayout(widget);
    widget->setLayout(layout);

    gdmlStructureTree = new QTreeWidget(widget);
    gdmlStructureTree->setHeaderHidden(true);
    layout->addWidget(gdmlStructureTree);

    rootItem = new QTreeWidgetItem();
    rootItem->setText(0, "GDML File :") ;

    gdmlStructureTree->addTopLevelItem(rootItem);

    widget->show();
}

GDMLReader::~GDMLReader() {

}

///
/// \param filepath
/// \return
bool GDMLReader::readFile(QString filepath) {
    qDebug() << "Reading file : " << filepath;

    file = new QFile(filepath);

    // gui
    rootItem->setText(0, "GDML File: " + filepath);

    if (file->open(QIODevice::ReadOnly)) {
        qDebug() << "Transfering file...";

        document.setContent(file);
        file->close();

        qDebug() << "Transfer done.";
    } else {
        qDebug() << "Error! File could not be opened.";

        return false;
    }


    qDebug() << "Root " << getRootTag().tagName();

    for (auto temp: getSubTag(getRootTag())) {
        qDebug() << "Sub tag : " << temp.tagName();

        if (temp.tagName() == "materials") {
            getMaterialsAnalysis(temp);
        }
        if (temp.tagName() == "structure") {
            getStructureAnalysis(temp);
        }
//        for (auto subTemp : getSubTag(temp)) {
//            qDebug() << "Sub tag : " << subTemp.tagName();
//
//        }
    }


    printDump();

    gdmlStructureTree->expandAll();

    return true;
}

bool GDMLReader::printDump() {

    qDebug() << "Shape - Material condition list;";


    for(auto itr: shape_material_list){
        qDebug() << "\t" << itr.first << " - " << itr.second;
    }
    return 0;
}
///
/// \return
QDomElement GDMLReader::getRootTag() {
    return document.documentElement();
}

///
/// \return
QList<QDomElement> GDMLReader::getSubTag(QDomElement rootTag) {
    QList<QDomElement> subTagList;

    QDomElement subTag = rootTag.firstChild().toElement();

    while (!subTag.isNull()) {
        subTagList.append(subTag);
        subTag = subTag.nextSibling().toElement();
    }

    return subTagList;
}

///
/// \param materialsElement
/// \return
bool GDMLReader::getMaterialsAnalysis(QDomElement materialsElement) {
    qDebug() << "Materials analyzing...";

    if (materialsElement.tagName() == "materials") {
        QList<QDomElement> materialsSubTagList;

        QDomElement subTag = materialsElement.firstChild().toElement();

        // Get sub tag list
        while (!subTag.isNull()) {
            materialsSubTagList.append(subTag);
            subTag = subTag.nextSibling().toElement();
        }

        for (auto iterator: materialsSubTagList) {
            if (iterator.tagName() == "element") {
                qDebug() << "element found.";
            }
            else if(iterator.tagName() == "material"){
                qDebug() << "material found";

                materialList.append(iterator.attribute("name"));
            }
            else{

            }
        }

        // gui
        materialsItem = new QTreeWidgetItem();
        materialsItem->setText(0, "Materials");

        for (auto it: materialList) {

                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, it);
                materialsItem->addChild(item);

        }

        rootItem->addChild(materialsItem);

        return true;
    } else {
        qDebug() << "The tag is not material tag.";
        return false;
    }
}

///
/// \param structureElement
/// \return
bool GDMLReader::getStructureAnalysis(QDomElement structureElement) {
    qDebug() << "Structure analyzing...";

    // Structure tag control
    if (structureElement.tagName() == "structure") {

        // get sub tag
        QList<QDomElement> structureSubTagList = getSubTag(structureElement);

        for (auto iterator: structureSubTagList) {

            if (iterator.tagName() == "volume" && iterator.attribute("name") == "World") {
                QList<QDomElement> worldSubTagList = getSubTag(iterator);

                QPair<QString, QString> world_mat;

                for (auto temp : worldSubTagList) {
                    if(temp.tagName() == "materialref"){
                        world_mat.first = "World";
                        world_mat.second = temp.attribute("ref");

                        shape_material_list.insert(0, world_mat);
                    }
                }
            }
            else{
                QPair<QString, QString> vol_mat;

                auto volSubList = getSubTag(iterator);
                for (auto temp: volSubList) {
                    if(temp.tagName() == "materialref"){
                        vol_mat.first   = iterator.attribute("name");
                        vol_mat.second = temp.attribute("ref");

                        shape_material_list.append(vol_mat);
                    }
                }
            }
        }

        // gui
        structuresItem = new QTreeWidgetItem();
        structuresItem->setText(0, "Structure");

        // gui
        QTreeWidgetItem *worldItem = new QTreeWidgetItem();
        worldItem->setText(0, "World - VACUUM");
        structuresItem->addChild(worldItem);

        for (auto it: shape_material_list) {
            if(it.first != "World"){
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, it.first.split("V-").last());
                worldItem->addChild(item);
            }
        }

        rootItem->addChild(structuresItem);

        return true;
    } else {
        qDebug() << "The tag is not structure tag.";
        return false;
    }
}

bool GDMLReader::getDefineAnalysis(QDomElement) {

}

bool GDMLReader::getSolidAnalysis(QDomElement) {

}

bool GDMLReader::getSetupAnalysis(QDomElement) {

}