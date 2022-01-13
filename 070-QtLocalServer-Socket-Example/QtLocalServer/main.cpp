#include "mylocalserver.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    MyLocalServer server("MyLocalServer2");
    server.show();

    return app.exec();
}