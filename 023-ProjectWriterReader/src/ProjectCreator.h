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

    explicit ProjectCreator(QString projectLocation);
    void loadList(const QList<QString>& inputList, int listIndex);

    QXmlStreamWriter xmlWriter;
    QDir projectDir;
    void writeXml();
    // File Components Start
    QString projectDirPath;
    QString stepFile;
    QList<QString> macroFiles;
    QList<QString> beamFiles;
    // File Components End
private:
    void writeMacroFiles();
    void writeStepFiles();
};


#endif //OPENCASCADEPROJECT_PROJECTCREATOR_H
