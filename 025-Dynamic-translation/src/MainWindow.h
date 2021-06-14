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

private:
    QLabel *openedFolderLabel;
    inline static QLabel *mousePositionLabel;
    QTranslator qtLanguageTranslator;

    QMenu *subMenu;
    QMenu *langMenu;
    QAction *importFileAction;
    QAction *turkishLangAction;
    QAction *englishLangAction;

private slots:
    void importFile();
    void turkishLang();
    void englishLang();
    void retranslate();
protected:
    void changeEvent(QEvent * event) override;
};


#endif //MAINWINDOW_H
