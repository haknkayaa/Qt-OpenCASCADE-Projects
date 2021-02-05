#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// User Libraries
#include "Viewer.h"
#include "STEPProcessor.h"

class STEPProcessor;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);
    inline static Viewer *myViewerWidget;

    // functions
    void createMenuBar(); // This function  Creates a menu bar in MainWindow
    void createStatusBar(); // This function Creates a Status bar in MainWindow
    void createMiddleWidget(); //This function Creates a middle widget

private:
    STEPProcessor *mySTEPProcessor;
    QLabel *openedFolderLabel;
    inline static QLabel *mousePositionLabel;

    void initialSettings();

private slots:
    void importFile();
};


#endif //MAINWINDOW_H
