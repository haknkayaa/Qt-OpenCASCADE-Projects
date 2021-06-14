//
// Created by sufuk on 8.03.2021.
//

#include "ProjectCreator.h"

ProjectCreator::ProjectCreator() {


}
/*!
 * Initializes the project folder and creates .mrad file
 * @param projectLocation
 * @param projectName
 */
void ProjectCreator::initProject(const QString &projectLocation, const QString &projectName) {

    /** Initialize the project folder */
    QDir projectDir(projectLocation + "/" + projectName);

    //TODO move this waring to main window side and pop some message box
    if (projectDir.exists()){qDebug() << "Warning There is already another project in there they will be overwritten";}

    /** Create the project dir with its own path */
    projectDir.mkdir(projectDir.path());

    /** Define projectFile to global var for later use */
    projectFile = projectDir.path() + "/" + projectName + ".xml";

    /** Initialize the root element of Dom Document */
    root = document.createElement(projectName);
    document.appendChild(root);

}

/*!
 * Writes the project to the xml file
 */
void ProjectCreator::writeProject() {
    QFile file(projectFile);
    if(!file.open(QIODevice::WriteOnly)){qDebug() << "Failed to open the file.";}
    else{
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
    }
}

/*!
 * Resets the evey Global variable for using the same ProjectCreator "object"
 * for creating another projects
 */
void ProjectCreator::resetCache() {
    document.clear();
    root.clear();
    projectFile.clear();
}

/*!
 * Writes an element under given parent,
 * if there isn't ant given parent writes the element under root.
 * @param elementName
 * @param parentElementName
 */
void ProjectCreator::writeElement(const QString &elementName, const QString& parentElementName) {
    QDomElement tempElement = document.createElement(elementName);
    if (parentElementName == NULL){
        root.appendChild(tempElement);
    }
    else{
        document.elementsByTagName(parentElementName).at(0).toElement().appendChild(tempElement);
    }
}
/*!
 * Writes an attribute to given element,
 * if there isn't any given element writes attribute to root element
 * @param attr
 * @param value
 * @param elementName
 */
void ProjectCreator::writeAttribute(const QString &attr, const QString& value, const QString &elementName) {
    if (elementName == NULL){
        root.setAttribute(attr, value);
    }
    else{
        document.elementsByTagName(elementName).at(0).toElement().setAttribute(attr, value);
    }
}
