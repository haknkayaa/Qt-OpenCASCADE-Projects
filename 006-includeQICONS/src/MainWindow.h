#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QFont>

// User Libraries
#include "Viewer.h"
#include "STEPProcessor.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);

    // global public variables
    static QTextBrowser *text;

private:
    Viewer *myViewerWidget;
    STEPProcessor *mySTEPProcessor;

    // functions
    void initialSettings();
    void createMenuBar();
    void createStatusBar();
    void createToolbars();
    void createMiddleWidget();

    QLabel *openedFolderLabel;

private slots:

    void importFile();

    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();
    void changeViewProjectionMode();

};


#endif //MAINWINDOW_H
