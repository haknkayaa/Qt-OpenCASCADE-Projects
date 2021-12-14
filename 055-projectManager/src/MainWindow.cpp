#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    ProjectManager *projectManager = new ProjectManager();

    projectManager->createProject("../project.xml");
    projectManager->addAttribute("Deneme", "AltTag", "selam", "naber");
    projectManager->addAttribute("Deneme", "AltTag2", "selam2", "naber");

    projectManager->removeTag("Deneme", "jdf");
}
