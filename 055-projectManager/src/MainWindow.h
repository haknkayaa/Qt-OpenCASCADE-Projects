#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "ProjectManager.h"

class ProjectManager;

class MainWindow : public QMainWindow{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);


};


#endif //MAINWINDOW_H
