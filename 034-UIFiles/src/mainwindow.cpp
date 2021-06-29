#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "STEPProcessor.h"

QTextBrowser *MainWindow::text = 0;

/** qDebug() ifadesiyle terminale basılan mesajların program içerisindeki textbrowser'a yönlendirilmesi
 *
 * @param type: handle edilecek mesaj tipi QtDebugMsg, QtInfoMsg,
 * @param context: mesajın kaynak bilgileri
 * @param msg: mesajın kendisi
 */
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {

    if (MainWindow::text == 0) {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
            case QtDebugMsg:
                fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                        context.function);
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
                if (MainWindow::text != 0) {
                    QFont myfont("Monospace", 10);
                    myfont.setPixelSize(12);
                    MainWindow::text->setStyleSheet("color: rgb(174,50,160)");
                    MainWindow::text->setFont(myfont);
                    //"Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    //        context.function;
                    QString output = "Debug : " + msg;
                    MainWindow::text->append(output);
                }

                break;
            case QtFatalMsg:
                abort();
        }
    }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    text = ui->text;
    myViewerWidget = ui->myViewerWidget;
    modelTreeWidget = ui->modelTreeWidget;
    qInstallMessageHandler(myMessageOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_test() {
    qDebug() << "importing start";
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                   "step files",
                                   QDir::homePath(),
                                   "*.stp");
    STEPProcessor *STEPProcessor = new class STEPProcessor(fileName, this);
}

