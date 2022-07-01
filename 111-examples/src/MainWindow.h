#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>



class MainWindow : public QMainWindow {
Q_OBJECT
public:
    // constructor function
    explicit MainWindow(QWidget *parent = nullptr);


private:
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QList<QWidget *> fileWidgets;
};


#endif //MAINWINDOW_H
