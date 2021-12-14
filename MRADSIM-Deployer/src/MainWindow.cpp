//
// Created by sufuk on 4.10.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (MainWindow::consoleWidget == nullptr) {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
            case QtDebugMsg:
                fprintf(stderr, "Debug: %s \t (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
//                fprintf(stderr, "Debug: %s \n", localMsg.constData());
                break;
            case QtInfoMsg:
                fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtWarningMsg:
                fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtCriticalMsg:
                fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                break;
            case QtFatalMsg:
                fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
                abort();
        }
    } else {
        switch (type) {
            case QtDebugMsg:
            case QtWarningMsg:
            case QtCriticalMsg:
                if (MainWindow::consoleWidget != nullptr) {
                    QFont myfont("Monospace", 10);
                    myfont.setPixelSize(12);
                    MainWindow::consoleWidget->setStyleSheet("color: rgb(174,50,160)");
                    MainWindow::consoleWidget->setFont(myfont);
                    //"Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    //        context.function;
                    QString output = "Debug : " + msg;
                    MainWindow::consoleWidget->append(output);
                }

                break;
            case QtFatalMsg:
                abort();
            case QtInfoMsg:
                break;
        }
    }
}


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    consoleWidget = ui->consoleWidget;
    qInstallMessageHandler(myMessageOutput);

    connect(ui->gui_button20, &QPushButton::clicked, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "mradsim-space file", QDir::homePath());
        ui->gui_lineEdit20->setText(filePath);
    });

    connect(ui->gui_lineEdit20, &QLineEdit::textChanged, [this]() {
        QString filePath = ui->gui_lineEdit20->text();
        QFileInfo fileInfo(filePath);
        ui->gui_checkbox20->setChecked(fileInfo.exists());


    });

    connect(ui->gui_button18, &QPushButton::clicked, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "mradsim-space file", QDir::homePath());
        ui->gui_lineEdit18->setText(filePath);
    });
    connect(ui->gui_lineEdit18, &QLineEdit::textChanged, [this]() {
        QString filePath = ui->gui_lineEdit18->text();
        QFileInfo fileInfo(filePath);
        ui->gui_checkbox18->setChecked(fileInfo.exists());

    });

    connect(ui->deployButton, &QPushButton::clicked, this, &MainWindow::deployButtonClicked);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::deployButtonClicked() {


    if (ui->gui_checkbox20->isChecked() && ui->gui_checkbox18->isChecked()) {
        QString installerFile = QFileDialog::getSaveFileName(this, "installerFile", QDir::homePath());
        if (!installerFile.isNull() || installerFile.isEmpty()) {
            auto *process = new QProcess(this);
            QDir mradsimDeployerDir(QApplication::applicationDirPath());
            QString binaryCreatorPath, mradsimTemplatePath;
#ifdef DEVELOPERMOD
            binaryCreatorPath = "binarycreator";
            mradsimTemplatePath = "/home/sufuk/CLionProjects/mradsim-simulation/VERSIONS/mradsim-0.1.0/MRADSIM-MULTI";
#else
            binaryCreatorPath = mradsimDeployerDir.absolutePath() + "/binarycreator";
            mradsimDeployerDir.cdUp();
            mradsimTemplatePath = mradsimDeployerDir.absolutePath() + "/Data";
#endif

            {
                QString mradsimguipath18 = mradsimTemplatePath + "/com.IRADETS.MRADSIM_ubuntu18/data/bin/mradsim-space";

                QString mradsimguipath20 = mradsimTemplatePath + "/com.IRADETS.MRADSIM_ubuntu20/data/bin/mradsim-space";

                QFileInfo fileInfo1(mradsimguipath18);
                fileInfo1.dir().remove(fileInfo1.fileName());

                fileInfo1.setFile(mradsimguipath20);
                fileInfo1.dir().remove(fileInfo1.fileName());

                QFile::copy(ui->gui_lineEdit18->text(), mradsimguipath18);
                QFile::copy(ui->gui_lineEdit20->text(), mradsimguipath20);

            }


            process->setProgram(binaryCreatorPath);
            QStringList arguments;
            arguments << "-c";
            arguments << QString(mradsimTemplatePath + "/config/config.xml");
            arguments << "-p";
            arguments << QString(mradsimTemplatePath + "/packages/");
            arguments << "-v";
            arguments << installerFile;

            process->setArguments(arguments);
            process->start(QIODevice::ReadWrite);
            process->waitForStarted();
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

                QFileInfo fileInfo(installerFile);
                if (fileInfo.exists()) {
                    QMessageBox::information(this, "Finished", "Deployment finished successfully");
                } else {
                    QMessageBox::warning(this, "Finished", "Deployment was failed");
                }

            }

        } else {
            QMessageBox::critical(this, "Error", "Please specify installer location correctly");
        }


    } else {
        QMessageBox::critical(this, "Error", "Something is wrong please check the files");
    }
}

