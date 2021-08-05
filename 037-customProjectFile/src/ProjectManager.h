//
// Created by sufuk on 18.03.2021.
//

#ifndef MRADSIM_GUI_PROJECTMANAGER_H
#define MRADSIM_GUI_PROJECTMANAGER_H

#include <QtWidgets>
#include <QDomDocument>

typedef struct{
    QString projectName;
    QString projectPath;
    QString projectDate;

} t_ProjectMetaData;

class ProjectManager {

public:

    explicit ProjectManager();
    ~ProjectManager();

    bool           createProject(const QString &arg_Path);
    bool           writeAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute, QString arg_Value);
    bool           addAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute, QString arg_Value);
    bool           removeAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute);
    QString        readAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute);
    QList<QString> getRootChildTagList();
    QList<QString> getSubTagList(QString arg_Tag);

    /** base64 ile şifrelemesi */
    void encode(QString);

    /** base64 ile okunması */
    QString decode(QString);

    /** End Project Handling */

private:
    t_ProjectMetaData projectMetaData;
    QString rootTag = "MRADSIM-Space";
};


#endif //MRADSIM_GUI_PROJECTMANAGER_H
