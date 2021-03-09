//
// Created by sufuk on 8.03.2021.
//

#include "ProjectCreator.h"

ProjectCreator::ProjectCreator(QString projectDirPath, QString stepFile, std::vector<QString> macroFiles, std::vector<QString> beamFiles){
    QDir projectDir;
    projectDir.mkdir(projectDirPath);
    projectDir.setPath(projectDirPath);
    projectDir.mkdir("macroFiles");
    projectDir.mkdir("stepFiles");
    projectDir.mkdir("beamFiles");

    QFile file = projectDir.path() + "/" + projectDir.dirName() + ".mrad";

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument(); // Document Start
    xmlWriter.writeStartElement("project"); // Root Tag Start

    xmlWriter.writeStartElement("geometry"); //Step Files Start
    xmlWriter.writeStartElement("stepFile");
    xmlWriter.writeAttribute("file", stepFile);
    xmlWriter.writeEndElement();
    for(const QString& itr : beamFiles){
        xmlWriter.writeStartElement("beamFile");
        xmlWriter.writeAttribute("file", itr);
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); //Step Files End

    xmlWriter.writeStartElement("macroFiles"); //Macro Files Start
    for(const QString& itr: macroFiles){
        xmlWriter.writeStartElement("macroFile");
        xmlWriter.writeAttribute("file", itr);
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); //Macro Files End

    xmlWriter.writeEndElement(); // Root Tag End
    xmlWriter.writeEndDocument(); // Document End
    file.close();
}
