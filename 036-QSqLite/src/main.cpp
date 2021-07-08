//
// Created by hakan on 8.07.2021.
//

// QT Libraries
#include <QApplication>

// User Libraries
#include "MainWindow.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}