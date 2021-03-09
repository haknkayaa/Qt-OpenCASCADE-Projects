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
class ProjectCreator {


public:

    explicit ProjectCreator(QString projectDirPath, QString stepFile, std::vector<QString> macroFiles, std::vector<QString> beamFiles);

private:

};


#endif //OPENCASCADEPROJECT_PROJECTCREATOR_H
