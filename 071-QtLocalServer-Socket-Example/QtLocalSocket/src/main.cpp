#include "mylocalsocket.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    MyLocalSocket socket("MyLocalServer2");
    socket.show();

    return app.exec();
}