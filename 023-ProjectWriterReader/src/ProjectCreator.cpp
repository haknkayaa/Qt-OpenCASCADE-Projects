//
// Created by sufuk on 8.03.2021.
//

#include "ProjectCreator.h"

ProjectCreator::ProjectCreator() {



}

void ProjectCreator::initProject(const QString &projectLocation, const QString &projectName) {

    QDir projectDir;
    projectDir.setPath(projectLocation + "/" + projectName);
    if (projectDir.exists()){qDebug() << "Warning There is already another project in there they will be overwritten";}
    projectDir.mkdir(projectDir.path());
    QFile file(projectDir.path() + "/" + projectName + ".xml");
    file.open(QIODevice::WriteOnly);


    root = document.createElement(projectName);
    document.appendChild(root);
    QDomElement stepFiles = document.createElement("stepFiles");
    root.appendChild(stepFiles);

    writeElement("BeamFiles", "stepFiles");
    writeElement("mybema", "BeamFiles");
    writeElement("mybema2", "BeamFiles");
    writeElement("GeoFiles", "stepFiles");
    writeElement("mybema3", "BeamFiles");
    writeElement("geofile", "GeoFiles");

    QTextStream stream(&file);
    stream << document.toString();
    file.close();

}

void ProjectCreator::writeElement(const QString &elementName, const QString& parentElementName) {
    QDomElement tempElement = document.createElement(elementName);
    if (parentElementName == NULL){
        root.appendChild(tempElement);
    }
    else{
        document.elementsByTagName(parentElementName).at(0).appendChild(tempElement);
    }
}

void ProjectCreator::writeAttribute(const QString &attr, const QString &elementName) {

}

