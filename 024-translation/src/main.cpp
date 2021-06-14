// Qt Libraries
#include <QApplication>
#include <QTranslator>

// User Libraries
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}
