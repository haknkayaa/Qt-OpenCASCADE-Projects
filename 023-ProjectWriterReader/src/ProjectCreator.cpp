//
// Created by sufuk on 8.03.2021.
//

#include "ProjectCreator.h"

ProjectCreator::ProjectCreator(QString projectDirPath, QString stepFile, std::vector<QString> macroFiles, std::vector<QString> beamFiles) {
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

    {//Step Files Start
        xmlWriter.writeStartElement("geometry");
        xmlWriter.writeStartElement("stepFile");
        QFileInfo stepFileInfo(stepFile);
        xmlWriter.writeAttribute("file", stepFileInfo.fileName());
        xmlWriter.writeEndElement();
        QFile::copy(stepFile, projectDir.path() + "/" + "stepFiles" + "/" + stepFileInfo.fileName());

         for (const QString &itr : beamFiles) {
            QFileInfo beamFileInfo(itr);
            xmlWriter.writeStartElement("beamFile");
            xmlWriter.writeAttribute("file", beamFileInfo.fileName());
            xmlWriter.writeEndElement();
            QFile::copy(itr, projectDirPath + "/" + "beamFiles" + "/" + beamFileInfo.fileName());
         }
    xmlWriter.writeEndElement();
    }//Step Files End

    {//Macro Files Start
        xmlWriter.writeStartElement("macroFiles");
        for (const QString &itr: macroFiles) {
            QFileInfo macroFileInfo(itr);
            xmlWriter.writeStartElement("macroFile");
            xmlWriter.writeAttribute("file", macroFileInfo.fileName());
            xmlWriter.writeEndElement();
            QFile::copy(itr, projectDirPath + "/" + "macroFiles" + "/" + macroFileInfo.fileName());
        }
        xmlWriter.writeEndElement();
    }//Macro Files End

    xmlWriter.writeEndElement(); // Root Tag End
    xmlWriter.writeEndDocument(); // Document End
    file.close();
}
