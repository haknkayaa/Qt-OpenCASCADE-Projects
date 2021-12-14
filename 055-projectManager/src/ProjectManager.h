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
    bool           writeAttribute(const QString& arg_Tag, const QString& arg_Key, const QString& arg_Attribute, const QString& arg_Value);
    bool           addAttribute(const QString& arg_Tag, const QString& arg_Key, const QString& arg_Attribute, const QString& arg_Value);
    bool           removeAttribute(const QString& arg_Tag, const QString& arg_Key, const QString& arg_Attribute) const;
    QString        readAttribute(const QString& arg_Tag, const QString& arg_Key, const QString& arg_Attribute);
    bool           removeTag(const QString &arg_Tag);

    QList<QString> getRootChildTagList();
    QList<QString> getSubTagList(const QString& arg_Tag);
    QList<QString> getAttributes(const QString& arg_subTag) const;


    /** base64 ile şifrelemesi */
    static void encode(const QString&);

    /** base64 ile okunması */
    static QString decode(const QString&);

    /** End Project Handling */

private:
    t_ProjectMetaData projectMetaData;
    QString rootTag = "MRADSIM-Space";
};


#endif //MRADSIM_GUI_PROJECTMANAGER_H
