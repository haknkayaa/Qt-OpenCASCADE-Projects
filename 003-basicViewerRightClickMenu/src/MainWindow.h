#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

// User Libraries
#include "Viewer.h"

class MainWindow : public QMainWindow{
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);

    // functions
    void createMenuBar();
    void createStatusBar();
    void createToolbars();

    void createMiddleWidget();


private:
    Viewer *myViewerWidget;

    QLabel *openedFolderLabel;


private slots:
    void importFile();
    void changeViewProjectionMode();

    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();


};


#endif //MAINWINDOW_H
