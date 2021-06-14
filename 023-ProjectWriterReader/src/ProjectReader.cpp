//
// Created by sufuk on 8.03.2021.
//

#include "ProjectReader.h"

ProjectReader::ProjectReader(QString projectPath) {
    QString stepFile;
    vector<QString> beamFiles;
    vector<QString> macroFiles;

    QFile file(projectPath);
    if(file.exists()){
        file.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        while(!xmlReader.isEndDocument()){
            xmlReader.readNext();
            if(xmlReader.name() == "geometry"){
                xmlReader.readNext();
                while (xmlReader.name() != "geometry"){
                    if((xmlReader.tokenType() == QXmlStreamReader::StartElement) && (xmlReader.name() == "stepFile")){
                        for(const QXmlStreamAttribute &attr : xmlReader.attributes()){
                            stepFile = attr.value().toString();
                        }
                    }
                    if((xmlReader.tokenType() == QXmlStreamReader::StartElement) && (xmlReader.name() == "beamFile")){
                        for(const QXmlStreamAttribute &attr : xmlReader.attributes()){
                            beamFiles.push_back(attr.value().toString());
                        }
                    }
                    xmlReader.readNext();
                }
            }

            if(xmlReader.name() == "macroFiles"){
                xmlReader.readNext();
                while (xmlReader.name() != "macroFiles"){
                    if(xmlReader.tokenType() == QXmlStreamReader::StartElement){
                        for(const QXmlStreamAttribute &attr : xmlReader.attributes()){
                            macroFiles.push_back(attr.value().toString());
                        }
                    }
                    xmlReader.readNext();
                }
            }
        }



    }
    //print the Step File
    qDebug() << "Step file :";
    qDebug() << stepFile;

    // Print the macro files
    qDebug() << "Macro files :";
    for(const QString& itr : macroFiles){qDebug() << itr;}
    // Print Beam files
    qDebug() << "Beam files :";
    for(const QString& itr : beamFiles){qDebug() << itr;}

    file.close();
}
