// Qt Libraries
#include <QApplication>

// User Libraries
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto *process = new QProcess();
    QDir mradsimguiDir(QApplication::applicationDirPath());
    QString mradsimCoreDir;

#ifdef DEVELOPERMOD
    mradsimguiDir.cdUp();
        mradsimCoreDir = mradsimguiDir.absolutePath() + "/mradsim-simulation/bin/mradsim";
        qDebug() << "MRADSIM: " << mradsimCoreDir;
#else
    mradsimCoreDir = "/home/hakan/CLionProjects/mradsim-simulation/VERSIONS/mradsim-0.1.0/cmake-build-debug/mradsim-simulation/bin/mradsim";
#endif

    process->setProgram(mradsimCoreDir);
    QStringList arguments;
    QString simulationFilePath = "/home/hakan/Desktop/Untitled2/simulation.mac";
    arguments.append(simulationFilePath);

    process->setArguments(arguments);
//    process->start(QIODevice::ReadWrite);
//    process->waitForStarted();
//
    process->execute(mradsimCoreDir, arguments);

    while (process->state() == QProcess::Running) {

        QApplication::processEvents();
        QString line = process->readAllStandardOutput();
        QStringList strLines = QString(line).split("\n");

        for (const QString &itr: strLines) {
            if (itr != "") {
                qDebug() << itr;
            }
        }
    }
    if (process->state() == QProcess::NotRunning) {
        qDebug() << "======================================";
        qDebug() << "====           FINISHED           ====";
        qDebug() << "======================================";
    }

    return app.exec();
}
