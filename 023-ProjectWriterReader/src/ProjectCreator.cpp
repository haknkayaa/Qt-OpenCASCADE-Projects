//
// Created by sufuk on 8.03.2021.
//

#include "ProjectCreator.h"

ProjectCreator::ProjectCreator(QString projectDirPath) {
    QDir projectDir;
    projectDir.mkdir(projectDirPath);
    projectDir.setPath(projectDirPath);
    projectDir.mkdir("macroFiles");
    projectDir.mkdir("stepFiles");

    QFile file = projectDir.path() + "/" + projectDir.dirName();

    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument(); // Document Start
    xmlWriter.writeStartElement("project"); // Root Tag Start

    xmlWriter.writeStartElement("geometry"); //Step Files Start
    xmlWriter.writeStartElement("stepFile");
    xmlWriter.writeAttribute("file", "test1.step");
    xmlWriter.writeEndElement();
    xmlWriter.writeStartElement("beamFile");
    xmlWriter.writeAttribute("file", "beam1.step");
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement(); //Step Files End

    xmlWriter.writeStartElement("macroFiles"); //Macro Files Start
    xmlWriter.writeStartElement("macroFile");
    xmlWriter.writeAttribute("file", "vis.mac");
    xmlWriter.writeEndElement();
    xmlWriter.writeStartElement("macroFile");
    xmlWriter.writeAttribute("file", "run.mac");
    xmlWriter.writeEndElement();
    xmlWriter.writeStartElement("macroFile");
    xmlWriter.writeAttribute("file", "config.mac");
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement(); //Macro Files End

    xmlWriter.writeEndElement(); // Root Tag End
    xmlWriter.writeEndDocument(); // Document End
    file.close();
}
