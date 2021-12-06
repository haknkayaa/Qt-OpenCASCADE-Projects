// Header
#include "ProjectManager.h"

// Qt Libraries
#include <QtWidgets>
#include <QFile>
#include <QXmlStreamReader>
#include <QtXml>

/// Constructor function
ProjectManager::ProjectManager() {

}

/// Destroyer function
ProjectManager::~ProjectManager() = default;

/// Yeni proje yaratmak için çağrılacak fonksiyondur.
/// \param arg_Path : Projenin pathi (ismi ile birlikte)
bool ProjectManager::createProject(const QString &arg_Path) {

    QFileInfo fileInfo(arg_Path);
    QDir dir;

    if (fileInfo.exists()) { // dosya var mı?

        projectMetaData.projectName = fileInfo.baseName();
        projectMetaData.projectPath = arg_Path;
        projectMetaData.projectDate = QDateTime::currentDateTime().toString();

        return false;
    } else {
        qDebug() << "Creating directory...";

        dir.mkpath(fileInfo.absoluteDir().absolutePath());

        qDebug() << "Creating Output file...";
        dir.mkpath(fileInfo.absoluteDir().absolutePath() + "/Output");
        qDebug() << "Creating project file...";

        QFile file(arg_Path);
        file.open(QIODevice::WriteOnly);

        // File content
        QDomDocument document;
        QDomProcessingInstruction instruction = document.createProcessingInstruction("xml",
                                                                                     R"(version="1.0" encoding="UTF-8")");
        document.appendChild(instruction);

        QDomElement root = document.createElement("MRADSIM-Space");
        /**/    QDomElement general = document.createElement("General");
        /**/        QDomElement project = document.createElement("Project");
        /**/             project.setAttribute("Path", "");
        /**/        general.appendChild(project);
        /**/    root.appendChild(general);
        document.appendChild(root);

        QTextStream out(&file);
        document.save(out, 4);
        // End file content

        file.close();

        projectMetaData.projectName = fileInfo.baseName();
        projectMetaData.projectPath = arg_Path;
        projectMetaData.projectDate = QDateTime::currentDateTime().toString();

        return true;
    }
}

///
/// \param arg_AbsolutePath
void ProjectManager::encode(const QString &arg_AbsolutePath) {
    qDebug() << "Encoding...";

    QFile file(arg_AbsolutePath);

    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll().toBase64();
    file.close();
    QFile::remove(arg_AbsolutePath);

    QFile file2(arg_AbsolutePath);
    file2.open(QIODevice::WriteOnly);
    QTextStream stream(&file2);
    stream << data;
    file2.close();

//        file.setPermissions(QFile::WriteUser | QFile::WriteGroup | QFile::WriteOther);
//        file.setPermissions(QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther);
//        file.setPermissions(QFile::ExeUser | QFile::ExeGroup | QFile::ExeOther);
    file.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ExeUser);

    qDebug() << "Writing is done";


}

///
/// \param arg_AbsolutePath
/// \return
QString ProjectManager::decode(const QString &arg_AbsolutePath) {
    qDebug() << "Decoding...";

    QFile f(arg_AbsolutePath);

    if (f.open(QFile::ReadOnly)) {
        QByteArray data = f.readAll();

//        QString base64 = data.toStdString();
//        qDebug() << QByteArray::fromBase64(data);

        f.close();

        return QByteArray::fromBase64(data);
    } else {
        return nullptr;
    }
}

///
/// \param arg_Tag
/// \param arg_Attribute
/// \param arg_Value
bool ProjectManager::writeAttribute(const QString &arg_Tag, const QString &arg_Key, const QString &arg_Attribute,
                                    const QString &arg_Value) {
    qDebug() << endl << "Writing attribute:"
             << arg_Tag << "->"
             << arg_Key << "->"
             << arg_Attribute << "="
             << arg_Value;

    QDomDocument document;
    QFile file(projectMetaData.projectPath);

    if (file.open(QIODevice::ReadWrite)) {
        document.setContent(&file);
        file.close();

        QDomElement root = document.documentElement();

        bool tagIsAdded = false;
        bool keyIsAdded = false;
        qDebug() << "Checking..";

        if (root.hasChildNodes() && !root.isNull()) {
            for (int i = 0; i < root.childNodes().size(); ++i) {
                QDomNode first_child = root.childNodes().at(i);

                if (first_child.toElement().tagName() == arg_Tag) {
                    qDebug() << "- " + arg_Tag + " found under Root element.";
                    tagIsAdded = true;
                    if (first_child.hasChildNodes()) {
                        for (int j = 0; j < first_child.childNodes().size(); ++j) {
                            QDomNode second_child = first_child.childNodes().at(j);

                            if (second_child.toElement().tagName() == arg_Key) {
                                qDebug() << "Found: " + arg_Key + " inside " + arg_Tag;
                                /**/QDomElement keyElement = document.createElement(arg_Key);
                                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                                /**/first_child.replaceChild(keyElement, second_child);
                                keyIsAdded = true;
                            } else {
                                qDebug() << "Found: " + arg_Tag + " but has no " + arg_Key;

                            }
                        }

                        if (!keyIsAdded) {
                            qDebug() << arg_Key + " has no added. Adding now.";

                            /**/QDomElement keyElement = document.createElement(arg_Key);
                            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                            /**/first_child.toElement().appendChild(keyElement);
                        }
                    } else {
                        qDebug() << arg_Tag + " has no subs.";
                    }

                } else {
                    qDebug() << "This is not the element sought under root. Node: " + first_child.toElement().tagName() +
                                " found: " + arg_Tag;
                }
            }

            if (!tagIsAdded) {
                qDebug() << arg_Tag + " has never been added before. Now adding...";

                QDomElement domElement = document.createElement(arg_Tag);
                /**/QDomElement keyElement = document.createElement(arg_Key);
                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                /**/domElement.appendChild(keyElement);
                root.appendChild(domElement);

                qDebug() << "Added.";
            }

        } else {
            qDebug() << "Root does not have child elements.";
            qDebug() << "Root has never been added before. Now adding...";

            QDomProcessingInstruction instruction = document.createProcessingInstruction("xml",
                                                                                         R"(version="1.0" encoding="UTF-8")");
            document.appendChild(instruction);

            QDomElement root = document.createElement(rootTag);
            QDomElement domElement = document.createElement(arg_Tag);
            /**/QDomElement keyElement = document.createElement(arg_Key);
            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
            /**/domElement.appendChild(keyElement);
            root.appendChild(domElement);
            document.appendChild(root);

            qDebug() << "Added.";
        }


        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            document.save(out, 4);
            file.close();
        }
        return true;

    } else {
        qDebug() << "Error! WriteAttribute failed. File not opened.";
        return false;
    }
}

///
/// \param arg_Tag
/// \param arg_Key
/// \param arg_Attribute
/// \param arg_Value
/// \return
bool ProjectManager::addAttribute(const QString &arg_Tag, const QString &arg_Key, const QString &arg_Attribute,
                                  const QString &arg_Value) {
    qDebug() << endl << "Writing attribute:"
             << arg_Tag << "->"
             << arg_Key << "->"
             << arg_Attribute << "="
             << arg_Value;

    QDomDocument document;
    QFile file(projectMetaData.projectPath);

    if (file.open(QIODevice::ReadWrite)) {
        document.setContent(&file);
        file.close();

        QDomElement root = document.documentElement();

        bool tagIsAdded = false;
        bool keyIsAdded = false;
        qDebug() << "Checking..";

        if (root.hasChildNodes() && !root.isNull()) {
            for (int i = 0; i < root.childNodes().size(); ++i) {
                QDomNode first_child = root.childNodes().at(i);

                if (first_child.toElement().tagName() == arg_Tag) {
                    qDebug() << "Found: " + arg_Tag + " under Root tag";
                    tagIsAdded = true;
                    if (first_child.hasChildNodes()) {
                        for (int j = 0; j < first_child.childNodes().size(); ++j) {
                            QDomNode second_child = first_child.childNodes().at(j);

                            if (second_child.toElement().tagName() == arg_Key) {
                                qDebug() << "Found: " + arg_Key + " inside " + arg_Tag + ".";
                                /**/QDomElement keyElement = second_child.toElement();
                                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                                /**/first_child.replaceChild(keyElement, second_child);
                                keyIsAdded = true;
                            } else {
                                qDebug() << "Found: " + arg_Tag + " but has no " + arg_Key + ".";

                            }
                        }

                        if (!keyIsAdded) {
                            qDebug() << arg_Key + " has never been added before. Now adding...";


                            /**/QDomElement keyElement = document.createElement(arg_Key);
                            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                            /**/first_child.toElement().appendChild(keyElement);


                            qDebug() << "Added.";
                        }
                    } else {
                        qDebug() << arg_Tag + " alt elementlere sahip değil.";
                    }

                } else {
                    qDebug() << "Warning. Found: " + first_child.toElement().tagName() +
                                " Search: " + arg_Tag;
                }
            }

            if (!tagIsAdded) {
                qDebug() << arg_Tag + " has never been added before. Now adding...";

                QDomElement domElement = document.createElement(arg_Tag);
                /**/QDomElement keyElement = document.createElement(arg_Key);
                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                /**/domElement.appendChild(keyElement);
                root.appendChild(domElement);

                qDebug() << "Added.";
            }

        } else {
            qDebug() << "Root does not have child elements";
            qDebug() << "Root has never been added before. Now adding...";

            QDomProcessingInstruction instruction = document.createProcessingInstruction("xml",
                                                                                         R"(version="1.0" encoding="UTF-8")");
            document.appendChild(instruction);

            QDomElement root = document.createElement(rootTag);
            QDomElement domElement = document.createElement(arg_Tag);
            /**/QDomElement keyElement = document.createElement(arg_Key);
            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
            /**/domElement.appendChild(keyElement);
            root.appendChild(domElement);
            document.appendChild(root);

            qDebug() << "Added.";
        }


        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            document.save(out, 4);
            file.close();
        }
        return true;

    } else {
        qDebug() << "Error! WriteAttribute failed. File not opened.";
        return false;
    }
}
///
/// \param arg_Tag
/// \param arg_Key
/// \param arg_Attribute
/// \return !todo remove key attirbute
bool ProjectManager::removeAttribute(const QString &arg_Tag, const QString &arg_Key, const QString &arg_Attribute) const {
    qDebug() << endl << "Remove attribute... ->"
             << arg_Tag << "->"
             << arg_Key << "->"
             << arg_Attribute;

    QDomDocument document;
    QFile file(projectMetaData.projectPath);

    if (file.open(QIODevice::ReadWrite)) {
        document.setContent(&file);
        file.close();

        QDomElement root = document.documentElement();

        bool tagIsAdded = false;
        bool keyIsAdded = false;
        qDebug() << "Checking..";

        if (root.hasChildNodes() && !root.isNull()) {
            for (int i = 0; i < root.childNodes().size(); ++i) {
                QDomNode first_child = root.childNodes().at(i);

                if (first_child.toElement().tagName() == arg_Tag) {
                    qDebug() << "Found: " + arg_Tag + " under Root element";
                    tagIsAdded = true;
                    if (first_child.hasChildNodes()) {
                        QDomElement keyElement;
                        for (int j = 0; j < first_child.childNodes().size(); ++j) {
                            QDomNode second_child = first_child.childNodes().at(j);

                            if (second_child.toElement().tagName() == arg_Key) {
                                qDebug() << "Found: " + arg_Key + " inside " + arg_Tag + ".";
                                /**/keyElement = second_child.toElement();
//                                /**/    keyElement.removeAttribute(arg_Attribute);
//                                /**/first_child.replaceChild(keyElement, second_child);
                                keyIsAdded = true;
                            } else {
                                qDebug() << arg_Tag + " founded but  " + arg_Key + " not found.";

                            }
                        }

                        if(first_child.childNodes().size() == 1){
                            root.removeChild(first_child);
                        }else{
                            first_child.removeChild(keyElement.toElement());
                        }


                        if (!keyIsAdded) {
                            qDebug() << arg_Key + " has never been added before. Now adding..";
//                            /**/QDomElement keyElement = document.createElement(arg_Key);
//                            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
//                            /**/first_child.toElement().appendChild(keyElement);
                            qDebug() << "Added.";
                        }
                    } else {
                        qDebug() << arg_Tag + " has no subs.";
                    }

                } else {
                    qDebug() << "Warning. Found: " + first_child.toElement().tagName() +
                                " Search: " + arg_Tag;
                }
            }

            if (!tagIsAdded) {
                qDebug() << arg_Tag + " has never been added before.";


            }

        } else {
            qDebug() << "Root does not have child elements.";
            qDebug() << "Root has never been added before.";
        }


        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            document.save(out, 4);
            file.close();
        }
        return true;

    } else {
        qDebug() << "Error! WriteAttribute failed. File not opened.";
        return false;
    }
}

/// Root altındaki ana taglerden birini kaldıramak için kullanılabilir.
/// \param arg_Tag Kaldıralacak tag adı
/// \return
bool ProjectManager::removeTag(const QString &arg_Tag) {
    qDebug() << endl << "Remove tag... ->"
             << arg_Tag << "->";


    QDomDocument document;
    QFile file(projectMetaData.projectPath);

    if (file.open(QIODevice::ReadWrite)) {
        document.setContent(&file);
        file.close();

        QDomElement root = document.documentElement();

        bool tagIsAdded = false;
        bool keyIsAdded = false;

        qDebug() << "Checking..";

        if (root.hasChildNodes() && !root.isNull()) {
            for (int i = 0; i < root.childNodes().size(); ++i) {
                QDomNode iterator_childNode = root.childNodes().at(i);

                if (iterator_childNode.toElement().tagName() == arg_Tag) {
                    qDebug() << "Found: " + arg_Tag + " under Root element";
                    tagIsAdded = true;
                    root.removeChild(iterator_childNode);
                } else {
                    qDebug() << "Error! Found: " + iterator_childNode.toElement().tagName() + " Search: " + arg_Tag;
                }
            }

            if (!tagIsAdded) {
                qDebug() << arg_Tag + " has never been added before.";
            }

        } else {
            qDebug() << "Root does not have child elements.";
            qDebug() << "Root has never been added before.";
        }


        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            document.save(out, 4);
            file.close();
        }
        return true;

    } else {
        qDebug() << "Error! WriteAttribute failed. File not opened.";
        return false;
    }
}

///
/// \param arg_Tag
/// \param arg_Key
/// \param arg_Attribute
/// \return
QString ProjectManager::readAttribute(const QString &arg_Tag, const QString &arg_Key, const QString &arg_Attribute) {
    qDebug() << "Reading attribute... ->"
             << arg_Tag << "->"
             << arg_Key << "->"
             << arg_Attribute;

    QFile file(projectMetaData.projectPath);
    if (file.open(QIODevice::ReadOnly)) {

        auto *xmlStreamReader = new QXmlStreamReader(&file);
        QString response;

        while (xmlStreamReader->readNextStartElement()) {
            QApplication::processEvents();
            if (xmlStreamReader->name() == rootTag) {
                while (xmlStreamReader->readNextStartElement()) {
                    QApplication::processEvents();
                    if (xmlStreamReader->name() == arg_Tag) {
                        while (xmlStreamReader->readNextStartElement()) {
                            QApplication::processEvents();
                            if (xmlStreamReader->name() == arg_Key) {
                                response = xmlStreamReader->attributes().value(arg_Attribute).toString();
                            } else {
                                xmlStreamReader->skipCurrentElement();
                            }
                        }
                    } else {
                        xmlStreamReader->skipCurrentElement();
                    }
                }
            } else {
                xmlStreamReader->skipCurrentElement();
            }
        }

        file.close();

        return response;
    } else {
        qDebug() << "Error! ReadAttribute failed. File not opened.";
        return nullptr;
    }
}

///
/// \return
QList<QString> ProjectManager::getRootChildTagList() {
    QList<QString> tagList;

    QFile file(projectMetaData.projectPath);
    if (file.open(QIODevice::ReadOnly)) {
        auto *xmlStreamReader = new QXmlStreamReader(&file);

        while (xmlStreamReader->readNextStartElement()) {
            if (xmlStreamReader->name() == rootTag) {
                while (xmlStreamReader->readNextStartElement()) {
                    tagList.append(xmlStreamReader->name().toString());
                    xmlStreamReader->skipCurrentElement();
                }
            } else {
                xmlStreamReader->skipCurrentElement();
            }
        }

        file.close();

        return tagList;
    } else {
        qDebug() << "Error! GetRootChildTagList failed. File not opened.";
        return tagList;
    }
}

///
/// \param arg_Tag
/// \return
QList<QString> ProjectManager::getSubTagList(const QString &arg_Tag) {
    QList<QString> tagList;

    QFile file(projectMetaData.projectPath);
    if (file.open(QIODevice::ReadOnly)) {
        auto *xmlStreamReader = new QXmlStreamReader(&file);

        while (xmlStreamReader->readNextStartElement()) {
            if (xmlStreamReader->name() == rootTag) {
                while (xmlStreamReader->readNextStartElement()) {
                    if (xmlStreamReader->name() == arg_Tag) {
                        while (xmlStreamReader->readNextStartElement()) {
                            tagList.append(xmlStreamReader->name().toString());
                            xmlStreamReader->skipCurrentElement();
                        }

                    } else {
                        xmlStreamReader->skipCurrentElement();
                    }

                }
            } else {
                xmlStreamReader->skipCurrentElement();
            }
        }

        file.close();

        return tagList;
    } else {
        qDebug() << "Error! GetSubTagList failed. File not opened.";
        return tagList;
    }
}

///
/// \param arg_subTag
/// \return
QList<QString> ProjectManager::getAttributes(const QString &arg_subTag) const {

    QFile file(projectMetaData.projectPath);
    file.open(QIODevice::ReadOnly);
    auto *xmlStreamReader = new QXmlStreamReader(&file);
    QList<QString> returnList;

    while (!xmlStreamReader->atEnd()) {
        xmlStreamReader->readNext();
        if (xmlStreamReader->name() == arg_subTag) {
            for (int i = 0; i < xmlStreamReader->attributes().size(); ++i) {
                qDebug() << xmlStreamReader->attributes().at(i).name().toString()
                         << xmlStreamReader->attributes().at(i).value().toString();
                returnList << xmlStreamReader->attributes().at(i).name().toString() + "\t" +
                              xmlStreamReader->attributes().at(i).value().toString();
            }
        }
    }


    return returnList;
}

