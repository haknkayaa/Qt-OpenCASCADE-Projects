// Qt Libraries
#include <QApplication>

// User Libraries
#include "ProjectManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *projectManager = new ProjectManager();
    projectManager->createProject(QDir::homePath() + "/Desktop/TestProject/Test.mrad");

    projectManager->writeAttribute("General", "Project", "Path", "123");
    QString response = projectManager->readAttribute("General", "Project", "Path");
    qDebug() << "Response :" << response;

    projectManager->writeAttribute("General", "Project2", "Path", "abc");
    response = projectManager->readAttribute("General", "Project2", "Path");
    qDebug() << "Response :" << response;

    projectManager->writeAttribute("GUI", "Viewer", "grid", "false");
    response = projectManager->readAttribute("GUI", "Viewer", "grid");
    qDebug() << "Response :" << response;

    projectManager->writeAttribute("Hakan", "Geometry", "aaaaaa", "hdsgfhdjsgfsf.step");
    response = projectManager->readAttribute("Hakan", "Geometry", "Path");
    qDebug() << "Response :" << response;

    projectManager->writeAttribute("dssf", "Geometry", "Path", "sabanci.step");
    response = projectManager->readAttribute("Sufuk", "Geometry", "Path");
    qDebug() << "Response :" << response;


    for (int i = 0; i < projectManager->getRootChildTagList().size(); ++i) {
        qDebug() << "Tag " << projectManager->getRootChildTagList().at(i);
    }

    for (int i = 0; i < projectManager->getSubTagList("General").size(); ++i) {
        qDebug() << "SubTag " << projectManager->getSubTagList("General").at(i);
    }


    projectManager->addAttribute("dssf", "Geometry", "asda", "sabanci.step");
    response = projectManager->readAttribute("dssf", "Geometry", "Path");
    qDebug() << "Response :" << response;

    projectManager->removeAttribute("dssf", "Geometry", "asda");
    response = projectManager->readAttribute("dssf", "Geometry", "Path");
    qDebug() << "Response :" << response;

    return a.exec();
}
