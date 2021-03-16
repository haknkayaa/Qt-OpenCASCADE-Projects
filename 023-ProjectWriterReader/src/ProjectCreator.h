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

    explicit ProjectCreator();

    void initProject(const QString& projectLocation, const QString& projectName);

    QDomDocument document;

    QDomElement root;

    void writeElement(const QString& elementName, const QString& parentElementName = NULL);
    void writeAttribute(const QString& attr, const QString& elementName);

private:

};


#endif //OPENCASCADEPROJECT_PROJECTCREATOR_H
