//
// Created by hakan on 29.06.2021.
//

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char ** argv ){

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

