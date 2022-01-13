#include <QApplication>
#include "mylocalserver.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


//    MyLocalServer server("MyLocalServer2");
//    server.show();
    mainwindow window;
    window.show();

    return app.exec();
}