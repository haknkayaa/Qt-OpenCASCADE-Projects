//
// Created by sufuk on 8.03.2021.
//

#include "ProjectCreator.h"

ProjectCreator::ProjectCreator(QString projectLocation) {
    projectDirPath = projectLocation;
    projectDir.setPath(projectDirPath);
}




void ProjectCreator::writeXml() {

    projectDir.mkdir(projectDirPath);
    projectDir.setPath(projectDirPath);
    QFile file = projectDir.path() + "/" + projectDir.dirName() + ".mrad";


    file.open(QIODevice::WriteOnly);
    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument(); // Document Start
    xmlWriter.writeStartElement("project"); // Root Tag Start
    writeStepFiles();
    writeMacroFiles();
    xmlWriter.writeEndElement(); // Root Tag End
    xmlWriter.writeEndDocument(); // Document End
    file.close();
}

void ProjectCreator::writeMacroFiles() { // Private
    {//Macro Files Start
        xmlWriter.writeStartElement("macroFiles");
        for (const QString &itr: macroFiles) {
            QFileInfo macroFileInfo(itr);
            xmlWriter.writeStartElement("macroFile");
            xmlWriter.writeAttribute("file", macroFileInfo.absoluteFilePath());
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }//Macro Files End

}

void ProjectCreator::writeStepFiles() {
    {//Step Files Start
        xmlWriter.writeStartElement("geometry");
        xmlWriter.writeStartElement("stepFile");
        QFileInfo stepFileInfo(stepFile);
        xmlWriter.writeAttribute("file", stepFileInfo.absoluteFilePath());
        xmlWriter.writeEndElement();

        for (const QString &itr : beamFiles) {
            QFileInfo beamFileInfo(itr);
            xmlWriter.writeStartElement("beamFile");
            xmlWriter.writeAttribute("file", beamFileInfo.absoluteFilePath());
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }//Step Files End

}

void ProjectCreator::loadList(const QList<QString> &inputList, int listIndex) {
    if(listIndex == 0){
        for(const QString& itr : inputList){
            macroFiles.push_back(itr);
        }
    }
    else if(listIndex == 1){
        for(const QString& itr : inputList){
            beamFiles.push_back(itr);
        }
    }
    else{
        qDebug() << "Please use only 0 or 1 as index";
    }
}

