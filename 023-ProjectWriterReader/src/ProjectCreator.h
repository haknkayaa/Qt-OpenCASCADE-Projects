//
// Created by sufuk on 8.03.2021.
//
/*!
 * Used for creating Projects
 * Project also creates it own project folder
 * Prototype of ProjectCreator Class
 */
#ifndef OPENCASCADEPROJECT_PROJECTCREATOR_H
#define OPENCASCADEPROJECT_PROJECTCREATOR_H

#include <QtWidgets>
#include <QDomDocument>
class ProjectCreator {


public:

    /** Constructor */
    explicit ProjectCreator();

    /** Start Project Handling Methods */
    void initProject(const QString& projectLocation, const QString& projectName);
    void writeProject();
    void resetCache();
    /** End Project Handling Methods */

    /** Start DOM Handling Methods */
    void writeElement(const QString& elementName, const QString& parentElementName = NULL);
    void writeAttribute(const QString& attr, const QString& value, const QString& elementName = NULL);
    /** End DOM Handling Methods */

private:

    /** Start XML Dom Global Components */
    QDomDocument document;
    QDomElement root;
    /** End XML Dom Global Components */

    /** Start File Handling Global Components */
    QString projectFile;
    /** End File Handling Global Components */

};


#endif //OPENCASCADEPROJECT_PROJECTCREATOR_H
