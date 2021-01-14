#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>


class MainWindow : public QMainWindow{
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);

    // functions
    void createMenuBar();
    void createStatusBar();
    void createMiddleWidget();

private:
    QLabel *openedFolderLabel;
    inline static QLabel *mousePositionLabel;

private slots:
    void importFile();

protected:

};


#endif //MAINWINDOW_H
