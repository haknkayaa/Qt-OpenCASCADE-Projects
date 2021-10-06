/******************************************************************************
 *    main.cpp
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/23/21.
 *     description :
 ******************************************************************************/


// Qt Libraries
#include <QApplication>
#include <QCoreApplication>

//#include <QBreakHandler.h>

// User Libraries
#include "src/MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();



    return app.exec();
}