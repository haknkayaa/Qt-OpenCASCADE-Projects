#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>


class MainWindow : public QMainWindow{
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);

    // functions
    void createMenuBar(); // This function  Creates a menu bar in MainWindow
    void createStatusBar(); // This function Creates a Status bar in MainWindow
    void createMiddleWidget(); //This function Creates a middle widget
    QTranslator englishTranslator;
    QTranslator turkishTranslator;
private:
    QLabel *openedFolderLabel;
    inline static QLabel *mousePositionLabel;

private slots:
    void importFile();
    void turkishLang();
    void englishLang();


protected:

};


#endif //MAINWINDOW_H
