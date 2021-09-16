#include <QApplication>
#include <QQmlApplicationEngine>

#include <iostream>

int main(int argc, char **argv) {
    QApplication app(argc, argv);


    QQmlApplicationEngine engine;

    engine.load(QUrl("../qml/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        std::cout << "Engine list empty" << std::endl;
        return -1;

    }

    return app.exec();
}

