// Qt Libraries
#include <QApplication>

// User Libraries
#include "MainWindow.h"
// bir tane toolbar yaratıp içinde
// 2d çizim yapıp bunları sonra 3d form haline dönüştürebileceği hale getirmelisin
//istersenn önce hazır 3d formlardan başla. Küp, Silindir, Küre gibi hazır çizim
// toolbarı olsun onlara basınca gerekli parametrelerden sonra çizimi gerçekleştirsin
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
