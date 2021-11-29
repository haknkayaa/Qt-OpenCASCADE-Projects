// Qt Libraries
#include <QApplication>

// User Libraries
#include "plot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    plot window;
    window.show();

    return app.exec();
}
