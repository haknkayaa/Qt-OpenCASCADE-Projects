//
// Created by hakan on 8.07.2021.
//

#ifndef PROJECT_MATERIALDIALOG_H
#define PROJECT_MATERIALDIALOG_H

#include <QDialog>



#include "DBManager.h"
#include "DataStructs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MaterialDialog; }
QT_END_NAMESPACE

class MaterialDialog : public QDialog {
Q_OBJECT

public:
    explicit MaterialDialog(QWidget *parent = nullptr);
    ~MaterialDialog() override;

    bool updateMaterialTableWidget();

private:
    Ui::MaterialDialog *ui;
    DBManager *db;
    int i = 0;
};


#endif //PROJECT_MATERIALDIALOG_H
