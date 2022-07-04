//
// Created by hkn on 30.06.2022.
//

#include "FileExplorer.h"

FileExplorer::FileExplorer(QString searchPath, QWidget *parent) : QWidget(parent) {

    layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->setAlignment(Qt::AlignCenter);
    this->setLayout(layout);

    auto fileCount = 0;

    QDirIterator it(searchPath, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        QString filename = it.next();
        QFileInfo file(filename);

        if ((file.fileName() == ".") || (file.fileName() == "..")) {
            continue;
        }

        if (file.isDir()) { // Check if it's a dir
//            qDebug() << "----------------------------------------------------";
//            qDebug() << "Directory     : " << file.fileName();
//            qDebug() << "Path          : " << file.absolutePath();
//            qDebug() << "Size          : " << file.size();
//            qDebug() << "Created Time  : " << file.birthTime();
//            qDebug() << "Last Modified : " << file.lastModified();
//            qDebug() << "Last Read     : " << file.lastRead();
//            qDebug() << "----------------------------------------------------";

            MyFileItem item;
            item.setFilePath(file.absolutePath());
            item.setFileName(file.fileName());
            item.setFileSize(file.size());
            item.setCreatedDate(file.birthTime());
            item.setModifiedDate(file.lastModified());
            item.setLastReadDate(file.lastRead());

            MyFileModel *model = new MyFileModel();
            model->setBeforeWidget(file.fileName());
            model->setAfterWidget(item.getLabel());
//            layout->addWidget(model->getWidget());

            fileWidgets.append(model->getWidget());
            fileCount++;
            continue;
        } else if (file.isFile()) {
//            qDebug() << "----------------------------------------------------";
//            qDebug() << "File     : " << file.fileName();
//            qDebug() << "----------------------------------------------------";
            continue;
        } else {
            qDebug() << "Unknown: " << file.fileName();
            continue;
        }
    }


    // Place grid layout a widgets
    for (int i = 0; i < fileWidgets.size(); i++) {
        if (this->width() / 200 > 0) {
            layout->addWidget(fileWidgets[i], i / (this->width() / 200), i % (this->width() / 200));
        } else {
            layout->addWidget(fileWidgets[i], i / 1, i % 1);
        }
    }


//    this->setStyleSheet("border: 1px solid #e0e0e0; border-radius: 4px;");
}

FileExplorer::~FileExplorer() {

}

void FileExplorer::resizeEvent(QResizeEvent *event) {

    while (layout->takeAt(0) != 0) {
        delete layout->takeAt(0);
    }

// Place grid layout a widgets
    for (int i = 0; i < fileWidgets.size(); i++) {
        if (this->width() / 200 > 0) {
            layout->addWidget(fileWidgets[i], i / (this->width() / 200), i % (this->width() / 200));
        } else {
            layout->addWidget(fileWidgets[i], i / 1, i % 1);
        }
    }
}