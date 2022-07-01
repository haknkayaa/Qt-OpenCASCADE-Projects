#include "MainWindow.h"


#include <QDirIterator>


#include "FileExplorer.h"

// Kurucu fonksiyon
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OpenCASCADE Window");
    resize(700, 500);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    centralWidget->setLayout(layout);

    FileExplorer *fileExplorer = new FileExplorer("../");
    layout->addWidget(fileExplorer);

}


