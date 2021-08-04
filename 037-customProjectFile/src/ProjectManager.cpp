// Header
#include "ProjectManager.h"

// Qt Libraries
#include <QtWidgets>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtXml>


ProjectManager::ProjectManager() {
    QString version = "0.0.1";
    qDebug() << "Project Manager version:" << version.toLocal8Bit();
}

ProjectManager::~ProjectManager() {

}

/// Yeni proje yaratmak için çağrılacak fonksiyondur.
/// \param arg_ProjectName : Proje ismi
/// \param arg_Path : Projenin pathi (ismi ile birlikte)
bool ProjectManager::createProject(const QString &arg_Path) {

    qDebug() << "Project creating...";

    QFileInfo fileInfo(arg_Path);
    QDir dir;

    if (fileInfo.exists()) { // dosya var mı?
        qDebug() << "Hata! Dosya daha önce yaratılmış.";

        projectMetaData.projectName = fileInfo.baseName();
        projectMetaData.projectPath = arg_Path;
        projectMetaData.projectDate = QDateTime::currentDateTime().toString();

        return false;
    } else {
        qDebug() << "Klasör oluşturuluyor";

        dir.mkpath(fileInfo.absoluteDir().absolutePath());

        qDebug() << "Proje dosyası oluşturuluyor.";

        QFile file(arg_Path);
        file.open(QIODevice::WriteOnly);

        // File content
        QDomDocument document;
        QDomProcessingInstruction instruction = document.createProcessingInstruction("xml",
                                                                                     "version=\"1.0\" encoding=\"UTF-8\"");
        document.appendChild(instruction);

        QDomElement root = document.createElement("MRADSIM-Space");
        /**/    QDomElement general = document.createElement("General");
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
void ProjectManager::encode(QString arg_AbsolutePath) {
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
QString ProjectManager::decode(QString arg_AbsolutePath) {
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
bool ProjectManager::writeAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute, QString arg_Value) {
    qDebug() << endl << "Writing attribute... ->"
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
                    qDebug() << "Root elementi altındaki " + arg_Tag + " bulundu";
                    tagIsAdded = true;
                    if (first_child.hasChildNodes()) {
                        for (int j = 0; j < first_child.childNodes().size(); ++j) {
                            QDomNode second_child = first_child.childNodes().at(j);

                            if (second_child.toElement().tagName() == arg_Key) {
                                qDebug() << arg_Tag + " içinde " + arg_Key + " bulundu.";
                                /**/QDomElement keyElement = document.createElement(arg_Key);
                                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                                /**/first_child.replaceChild(keyElement, second_child);
                                keyIsAdded = true;
                            } else {
                                qDebug() << arg_Tag + " var ama içinde " + arg_Key + " bulunamadı.";

                            }
                        }

                        if (!keyIsAdded) {
                            qDebug() << arg_Key + " daha önce hiç eklenmemiş. Şimdi ekleniyor...";


                            /**/QDomElement keyElement = document.createElement(arg_Key);
                            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                            /**/first_child.toElement().appendChild(keyElement);


                            qDebug() << "Eklendi.";
                        }
                    } else {
                        qDebug() << arg_Tag + " alt elementlere sahip değil.";
                    }

                } else {
                    qDebug() << "Root altında aranan element bu değil. Node: " + first_child.toElement().tagName() +
                                " Aranan: " + arg_Tag;
                }
            }

            if (!tagIsAdded) {
                qDebug() << arg_Tag + " daha önce hiç eklenmemiş. Şimdi ekleniyor...";

                QDomElement domElement = document.createElement(arg_Tag);
                /**/QDomElement keyElement = document.createElement(arg_Key);
                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                /**/domElement.appendChild(keyElement);
                root.appendChild(domElement);

                qDebug() << "Eklendi.";
            }

        } else {
            qDebug() << "Root alt elementlere sahip değil";
            qDebug() << "Root daha önce hiç eklenmemiş. Şimdi ekleniyor...";

            QDomProcessingInstruction instruction = document.createProcessingInstruction("xml",
                                                                                         "version=\"1.0\" encoding=\"UTF-8\"");
            document.appendChild(instruction);

            QDomElement root = document.createElement(rootTag);
            QDomElement domElement = document.createElement(arg_Tag);
            /**/QDomElement keyElement = document.createElement(arg_Key);
            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
            /**/domElement.appendChild(keyElement);
            root.appendChild(domElement);
            document.appendChild(root);

            qDebug() << "Eklendi.";
        }


        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            document.save(out, 4);
            file.close();
        }
        return true;

    } else {
        qDebug() << "Hata! WriteAttribute failed. Dosya açılamadı";
        return false;
    }
}

///
/// \param arg_Tag
/// \param arg_Key
/// \param arg_Attribute
/// \param arg_Value
/// \return
bool ProjectManager::addAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute, QString arg_Value) {
    qDebug() << endl << "Writing attribute... ->"
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
                    qDebug() << "Root elementi altındaki " + arg_Tag + " bulundu";
                    tagIsAdded = true;
                    if (first_child.hasChildNodes()) {
                        for (int j = 0; j < first_child.childNodes().size(); ++j) {
                            QDomNode second_child = first_child.childNodes().at(j);

                            if (second_child.toElement().tagName() == arg_Key) {
                                qDebug() << arg_Tag + " içinde " + arg_Key + " bulundu.";
                                /**/QDomElement keyElement = second_child.toElement();
                                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                                /**/first_child.replaceChild(keyElement, second_child);
                                keyIsAdded = true;
                            } else {
                                qDebug() << arg_Tag + " var ama içinde " + arg_Key + " bulunamadı.";

                            }
                        }

                        if (!keyIsAdded) {
                            qDebug() << arg_Key + " daha önce hiç eklenmemiş. Şimdi ekleniyor...";


                            /**/QDomElement keyElement = document.createElement(arg_Key);
                            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                            /**/first_child.toElement().appendChild(keyElement);


                            qDebug() << "Eklendi.";
                        }
                    } else {
                        qDebug() << arg_Tag + " alt elementlere sahip değil.";
                    }

                } else {
                    qDebug() << "Root altında aranan element bu değil. Node: " + first_child.toElement().tagName() +
                    " Aranan: " + arg_Tag;
                }
            }

            if (!tagIsAdded) {
                qDebug() << arg_Tag + " daha önce hiç eklenmemiş. Şimdi ekleniyor...";

                QDomElement domElement = document.createElement(arg_Tag);
                /**/QDomElement keyElement = document.createElement(arg_Key);
                /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
                /**/domElement.appendChild(keyElement);
                root.appendChild(domElement);

                qDebug() << "Eklendi.";
            }

        } else {
            qDebug() << "Root alt elementlere sahip değil";
            qDebug() << "Root daha önce hiç eklenmemiş. Şimdi ekleniyor...";

            QDomProcessingInstruction instruction = document.createProcessingInstruction("xml",
                                                                                         "version=\"1.0\" encoding=\"UTF-8\"");
            document.appendChild(instruction);

            QDomElement root = document.createElement(rootTag);
            QDomElement domElement = document.createElement(arg_Tag);
            /**/QDomElement keyElement = document.createElement(arg_Key);
            /**/    keyElement.setAttribute(arg_Attribute, arg_Value);
            /**/domElement.appendChild(keyElement);
            root.appendChild(domElement);
            document.appendChild(root);

            qDebug() << "Eklendi.";
        }


        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            document.save(out, 4);
            file.close();
        }
        return true;

    } else {
        qDebug() << "Hata! WriteAttribute failed. Dosya açılamadı";
        return false;
    }
}

///
/// \param arg_Tag
/// \param arg_Key
/// \param arg_Attribute
/// \return
QString ProjectManager::readAttribute(QString arg_Tag, QString arg_Key, QString arg_Attribute) {
    qDebug() << "Reading attribute... ->"
             << arg_Tag << "->"
             << arg_Key << "->"
             << arg_Attribute;

    QFile file(projectMetaData.projectPath);
    if (file.open(QIODevice::ReadOnly)) {

        QXmlStreamReader *xmlStreamReader = new QXmlStreamReader(&file);
        QString response;

        while (xmlStreamReader->readNextStartElement()) {
            if (xmlStreamReader->name() == rootTag) {
                while (xmlStreamReader->readNextStartElement()) {
                    if (xmlStreamReader->name() == arg_Tag) {
                        while (xmlStreamReader->readNextStartElement()) {
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
        qDebug() << "Hata! ReadAttribute failed. Dosya açılamadı";
        return nullptr;
    }
}

///
/// \return
QList<QString> ProjectManager::getRootChildTagList() {
    QList<QString> tagList;

    QFile file(projectMetaData.projectPath);
    if (file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader *xmlStreamReader = new QXmlStreamReader(&file);

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
        qDebug() << "Hata! GetRootChildTagList failed. Dosya açılamadı";
        return tagList;
    }
}

///
/// \param arg_Tag
/// \return
QList<QString> ProjectManager::getSubTagList(QString arg_Tag) {
    QList<QString> tagList;

    QFile file(projectMetaData.projectPath);
    if (file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader *xmlStreamReader = new QXmlStreamReader(&file);

        while (xmlStreamReader->readNextStartElement()) {
            if (xmlStreamReader->name() == rootTag) {
                while (xmlStreamReader->readNextStartElement()) {
                    if(xmlStreamReader->name() == arg_Tag){
                        while(xmlStreamReader->readNextStartElement()){
                            tagList.append(xmlStreamReader->name().toString());
                            xmlStreamReader->skipCurrentElement();
                        }

                    }else{
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
        qDebug() << "Hata! GetSubTagList failed. Dosya açılamadı";
        return tagList;
    }
}

