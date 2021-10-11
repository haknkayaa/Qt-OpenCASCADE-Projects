#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::setlocale(LC_ALL, "en_US.UTF-8");
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    QFile styleFile(":/qss/style.qss");
    styleFile.open(QFile::ReadOnly);
    // Apply the loaded stylesheet
    QString style(styleFile.readAll());
    qApp->setStyleSheet(style);

    MainWindow w;
    w.show();
    return a.exec();
}
