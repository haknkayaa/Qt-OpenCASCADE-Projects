//
// Created by hakan on 13.01.2022.
//

#ifndef SOMELIBRARY_MAINWINDOW_H
#define SOMELIBRARY_MAINWINDOW_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QDialog {
Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);

    ~mainwindow() override;

private:
    Ui::mainwindow *ui;
};


#endif //SOMELIBRARY_MAINWINDOW_H
