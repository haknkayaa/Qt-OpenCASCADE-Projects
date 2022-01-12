//
// Created by hakan on 22.09.2021.
//

#include "GDMLReader.h"

QString domElementToRawXML(const QDomElement &elem) {
    QString head = "<" + elem.tagName();
    QDomNamedNodeMap attrs = elem.attributes();
    for (int i = 0; i < attrs.size(); ++i) {
        QDomAttr attr = attrs.item(i).toAttr();
        head +=
                QString::fromLatin1(" %0=\"%1\"")
                        .arg(attr.name())
                        .arg(attr.value());
    }
    head += ">";
    return head + elem.text() + "</" + elem.tagName() + ">";
}

GDMLReader::GDMLReader() {

    rootItem = new QTreeWidgetItem();
    rootItem->setText(0, "GDML File :");

    MainWindow::mainItem_geometry->addChild(rootItem);
//    MainWindow::mainWidget->expandAll();
}

GDMLReader::~GDMLReader() {

}


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

    worldTag = getWorld();

    for (auto temp: getSubTag(getRootTag())) {
//        qDebug() << "Sub tag : " << temp.tagName();

        if (temp.tagName() == "materials") {
            getMaterialsAnalysis(temp);
        }
        if (temp.tagName() == "structure") {
            getStructureAnalysis(temp);
        }
    }


    return true;
}


QDomElement GDMLReader::getRootTag() {
    return document.documentElement();
}

QList<QDomElement> GDMLReader::getSubTag(QDomElement rootTag) {
    QList<QDomElement> subTagList;

    QDomElement subTag = rootTag.firstChild().toElement();

    while (!subTag.isNull()) {
        subTagList.append(subTag);
        subTag = subTag.nextSibling().toElement();
    }

    return subTagList;
}

/// Parser:
/// <setup name="Default" version="1.0">
//    <world ref="World"/>
//  </setup>
///
/// \return
t_World GDMLReader::getWorld() {
    qDebug() << "Getting world...";

    t_World response;

    for (auto temp: getSubTag(getRootTag())) {
        if (temp.tagName() == "setup") {
            QList<QDomElement> setupSubTagList;

            QDomElement subTag = temp.firstChild().toElement();

            while (!subTag.isNull()) {
                setupSubTagList.append(subTag);
                subTag = subTag.nextSibling().toElement();
            }

            for (auto sub_setupTag: setupSubTagList) {
                if (sub_setupTag.tagName() == "world") {
                    if (sub_setupTag.hasAttribute("ref")) {
                        response.ref = sub_setupTag.attribute("ref");
                    }
                }
            }
        }
    }

    for (auto temp: getSubTag(getRootTag())) {
        if (temp.tagName() == "structure") {
            for (auto sub_structure: getSubTag(temp)) {
                if (sub_structure.tagName() == "volume") {
                    if (sub_structure.hasAttribute("name")) {
                        if (sub_structure.attribute("name") == response.ref) {
                            for (auto sub_world: getSubTag(sub_structure)) {
                                if (sub_world.tagName() == "materialref") {
                                    if (sub_world.hasAttribute("ref")) {
                                        response.materialref = sub_world.attribute("ref");
                                    }
                                }
                                if (sub_world.tagName() == "solidref") {
                                    if (sub_world.hasAttribute("ref")) {
                                        response.solidref = sub_world.attribute("ref");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug() << "\n"
                "World Information \n"
                "==================== \n"
                "ref        : " << response.ref << "\n"
                                                   "solidref   : " << response.solidref << "\n"
                                                                                           "materialref: "
             << response.materialref << "\n";

    return response;
}

void GDMLReader::addOnceMaterial(QList<t_Material> &list, t_Material newData) {
    bool isAdded = false;
    for (auto it: list) {
        if (newData.name == it.name) {
            isAdded = true;
        }
    }

    if (!isAdded) {
        list.append(newData);
    }
}

void GDMLReader::addOnceElement(QList<t_Element> &list, t_Element newData) {
    bool isAdded = false;
    for (auto it: list) {
        if (newData.name == it.name) {
            isAdded = true;
        }
    }

    if (!isAdded) {
        list.append(newData);
    }
}


bool GDMLReader::getMaterialsAnalysis(QDomElement materialsElement) {
    qDebug() << "Materials analyzing...";

    materialsItem = new QTreeWidgetItem();
    materialsItem->setText(0, "Materials");
    materialsItem->setExpanded(true);

    if (materialsElement.tagName() == "materials") {
        for (auto it: getSubTag(materialsElement)) {
            if (it.tagName() == "element") {
                t_Element foundItem;
                foundItem.name = it.attribute("name");
                foundItem.formula = it.attribute("formula");
                foundItem.Z = it.attribute("Z");

                for (auto sub_it: getSubTag(it)) {
                    if (sub_it.tagName() == "atom") {
                        foundItem.atomValue = sub_it.attribute("value");
                    }
                }

                addOnceElement(elementList, foundItem);
            } else if (it.tagName() == "material") {
                t_Material foundItem;
                foundItem.name = it.attribute("name");

                for (auto sub_it: getSubTag(it)) {
                    if (sub_it.tagName() == "D") {
                        foundItem.dValue = sub_it.attribute("value");
                        foundItem.dUnit = sub_it.attribute("unit");
                    } else if (sub_it.tagName() == "fraction") {
                        t_SubElement foundSubElement;
                        foundSubElement.type = FRACTION;
                        foundSubElement.n = sub_it.attribute("n");
                        foundSubElement.ref = sub_it.attribute("ref");
                        foundItem.subElement.append(foundSubElement);
                    } else if (sub_it.tagName() == "composite") {
                        t_SubElement foundSubElement;
                        foundSubElement.type = COMPOSITE;
                        foundSubElement.n = sub_it.attribute("n");
                        foundSubElement.ref = sub_it.attribute("ref");
                        foundItem.subElement.append(foundSubElement);
                    }
                }

                addOnceMaterial(materialList, foundItem);
            }
        }

        for (auto it: materialList) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, it.name);
            materialsItem->addChild(item);
        }

        rootItem->addChild(materialsItem);

        return true;
    } else {
        qDebug() << "The tag is not material tag.";
        return false;
    }
}

bool GDMLReader::getStructureAnalysis(QDomElement structureElement) {

    qDebug() << "Structure analyzing...";

    materialsItem = new QTreeWidgetItem();
    materialsItem->setText(0, "Materials");
    materialsItem->setExpanded(true);

    if (structureElement.tagName() == "structure") {
        for (auto volume: getSubTag(structureElement)) {
            if (volume.tagName() == "volume") {
                if (volume.attribute("name") == worldTag.ref) {
                    for (auto subTag_volume: getSubTag(volume)) {
                        if (subTag_volume.tagName() == "physvol") {
                            t_Shape foundShape;

                            // phys volume name elde edilmesi
                            if (subTag_volume.hasAttribute("name")) {
                                foundShape.physvol = subTag_volume.attribute("name");
                            } else {
                                for (auto subTag_physvol: getSubTag(subTag_volume)) {
                                    if (subTag_physvol.tagName() == "volumeref") {
                                        if (subTag_physvol.hasAttribute("ref")) {
                                            foundShape.physvol = subTag_physvol.attribute("ref");
                                        }
                                    }
                                }
                            }

                            // volumeref name elde edilmesi
                            for (auto phys_it: getSubTag(subTag_volume)) {
                                if (phys_it.tagName() == "volumeref") {
                                    if (phys_it.hasAttribute("ref")) {
                                        foundShape.volumeref = phys_it.attribute("ref");
                                    }
                                }
                            }

                            // solidref elde edilmesi
                            for (auto sub_volume: getSubTag(structureElement)) {
                                if (sub_volume.tagName() == "volume") {
                                    if (sub_volume.attribute("name") == foundShape.volumeref) {
                                        for (auto subTag_volume_it: getSubTag(sub_volume)) {
                                            if (subTag_volume_it.tagName() == "solidref") {
                                                if (subTag_volume_it.hasAttribute("ref")) {
                                                    foundShape.solidref = subTag_volume_it.attribute("ref");
                                                }
                                            }
                                            if(subTag_volume_it.tagName() == "materialref"){
                                                if(subTag_volume_it.hasAttribute("ref")){
                                                    foundShape.materialref = subTag_volume_it.attribute("ref");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            worldTag.childShapes.append(foundShape);
                        }
                    }
                }
            }
        }

        structuresItem = new QTreeWidgetItem();
        structuresItem->setText(0, "Structure");
        structuresItem->setExpanded(true);

        QTreeWidgetItem *worldItem = new QTreeWidgetItem();
        worldItem->setText(0, "World - VACUUM");
        worldItem->setText(1, "GDMLItem-Volume");
        worldItem->setExpanded(true);

        structuresItem->addChild(worldItem);

        for (auto it: worldTag.childShapes) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, it.physvol);
            item->setText(1, "GDMLItem-Volume");
            worldItem->addChild(item);
        }

        rootItem->addChild(structuresItem);

        return true;
    } else {
        qDebug() << "The tag is not material tag.";
        return false;
    }
}

bool GDMLReader::getDefineAnalysis(QDomElement) {

}

bool GDMLReader::getSolidAnalysis(QDomElement) {

}

bool GDMLReader::getSetupAnalysis(QDomElement) {

}

t_Shape GDMLReader::getShapeFromPhysVol(QString physvolName) {
    t_Shape response;
    response.physvol = physvolName;

    // todo find response.solidref


    // todo find response.volumeref



    return response;
}