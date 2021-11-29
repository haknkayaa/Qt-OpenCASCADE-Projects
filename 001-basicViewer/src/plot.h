#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>


class plot : public QMainWindow{
Q_OBJECT
public:
    // constructor function
    explicit plot(QWidget *parent = nullptr);

    // functions
    void createMenuBar(); // This function  Creates a menu bar in plot
    void createStatusBar(); // This function Creates a Status bar in plot
    void createMiddleWidget(); //This function Creates a middle widget

private:
    QLabel *openedFolderLabel;
    inline static QLabel *mousePositionLabel;

private slots:
    void importFile();

protected:

};


#endif //MAINWINDOW_H
