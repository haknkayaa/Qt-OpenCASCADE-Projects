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
    auto layout = new QVBoxLayout(widget);
    widget->setLayout(layout);

    layout->addWidget(new QLabel("Structure Shape:"));
    shapeNameLineEdit = new QLineEdit(widget);
    layout->addWidget(shapeNameLineEdit);

    layout->addWidget(new QLabel("Structure Material:"));
    materialLineEdit = new QLineEdit(widget);
    layout->addWidget(materialLineEdit);

    editStructureButton = new QPushButton("Save");
    layout->addWidget(editStructureButton);
    connect(editStructureButton, &QPushButton::clicked, this, &GDMLReader::editButtonClicked);

    gdmlStructureTree = new QTreeWidget(widget);
    gdmlStructureTree->setHeaderHidden(true);
    layout->addWidget(gdmlStructureTree);
    connect(gdmlStructureTree, &QTreeWidget::itemClicked, this, &GDMLReader::itemClicked);

    rootItem = new QTreeWidgetItem();
    rootItem->setText(0, "GDML File :") ;

    gdmlStructureTree->addTopLevelItem(rootItem);

    widget->show();
}

GDMLReader::~GDMLReader() {

}

void GDMLReader::itemClicked() {
    qDebug() << "Item clicked. ";
    qDebug() << "Current item category: " << gdmlStructureTree->currentItem()->text(1);
    qDebug() << "Current item name    : " << gdmlStructureTree->currentItem()->text(0);
    qDebug() << "Current item material: " << gdmlStructureTree->currentItem()->text(2);

    shapeNameLineEdit->setText(gdmlStructureTree->currentItem()->text(0));
    materialLineEdit->setText(gdmlStructureTree->currentItem()->text(2));
}

void GDMLReader::editButtonClicked() {
    qDebug() << "Structure saving...";

    QString volumeName      = shapeNameLineEdit->text();
    QString materialName    = "";
    QString newVolumeName   = "";
    QString newMaterialName = materialLineEdit->text();

    auto root = document.documentElement();

    auto rootSubList = getSubTag(root);

    for(auto rootSub: rootSubList){
        if(rootSub.tagName() == "structure"){
            auto structureSubList = getSubTag(rootSub);

            for(auto structureSub : structureSubList){
                if(structureSub.tagName() == "volume" &&
                structureSub.attribute("name") != "World"
                && structureSub.attribute("name") == shapeNameLineEdit->text()){
                    qDebug() << "Node bulundu. Değiştiriliyor.";

                    auto aVolumeTag = getSubTag(structureSub);

                    for(auto volSub : aVolumeTag){
                        if(volSub.tagName() == "materialref"){
                            QDomElement newSub = document.createElement(QString("materialref"));
                            newSub.setAttribute("ref", materialLineEdit->text());

                            structureSub.replaceChild(newSub, volSub);
                        }
                    }
                }
            }
        }
    }

    QFile outFile("xmlout.xml");
    outFile.open(QIODevice::WriteOnly);
    outFile.resize(0);
    QTextStream stream;

    stream.setDevice(&outFile);
    document.save(stream, 4);
    outFile.close();

    emit editedGDMLFile();
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
                        vol_mat.first  = iterator.attribute("name");
                        vol_mat.second = temp.attribute("ref");

                        shape_material_list.append(vol_mat);
                    }
                }
            }
        }

        // gui
        structuresItem = new QTreeWidgetItem();
        structuresItem->setText(0, "Structure");

        QTreeWidgetItem *worldItem = new QTreeWidgetItem();
        worldItem->setText(0, "World - VACUUM");
        structuresItem->addChild(worldItem);

        for (auto it: shape_material_list) {
            if(it.first != "World"){
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, it.first);
                item->setText(1, "STRUCTURE");
                item->setText(2, it.second);
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