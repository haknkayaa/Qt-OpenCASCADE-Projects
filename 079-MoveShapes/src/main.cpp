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
#include "MainWindow.h"
#include "DataStructs.h"



int main(int argc, char *argv[]) {


    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    std::setlocale(LC_ALL, "en_US.UTF-8");
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    app.setApplicationName("MRADSIM Space");
    app.setApplicationVersion("0.2.0.3");

    MainWindow window;
    window.show();



    return app.exec();
}