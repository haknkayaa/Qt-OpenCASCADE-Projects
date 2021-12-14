// Qt Libraries
#include <QApplication>

// User Libraries
#include "MainWindow.h"
#include "GDMLReader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    GDMLReader *reader = new GDMLReader();
//    reader->readFile("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/Example - GDML Files/cube.gdml");
    reader->readFile("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/Example - GDML Files/sabanci5_1.gdml");
//    reader->readFile("/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/Example - GDML Files/ctechV05.gdml");


    return app.exec();
}
