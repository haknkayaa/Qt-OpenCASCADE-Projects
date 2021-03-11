// Qt Libraries
#include <QApplication>
#include <QTranslator>

// User Libraries
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("turkish");
    app.installTranslator(&translator);
    MainWindow window;
    window.show();

    return app.exec();
}
