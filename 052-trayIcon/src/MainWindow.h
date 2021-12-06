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
    QSystemTrayIcon         * trayIcon;

private slots:
    void importFile();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    /* Virtual function of the parent class in our class
     * Overridden to change the behavior of the application,
     * That it is minimized to tray when we want
     */
    void closeEvent(QCloseEvent * event);

};


#endif //MAINWINDOW_H
