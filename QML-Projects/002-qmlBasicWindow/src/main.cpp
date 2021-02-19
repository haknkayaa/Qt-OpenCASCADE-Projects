/******************************************************************************
 *    main.cpp 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/18/21.
 *     description :
 ******************************************************************************/


#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "BackEndClass.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // Sinifimizi kaydettirdigimiz yer
    qmlRegisterType<BackEndClass>("Hakan.Kaya.BackEndClass", 1, 0, "BackEndClass");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/../qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}