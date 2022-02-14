//
// Created by sufuk on 14.02.2022.
//

#ifndef OPENCASCADEPROJECT_CUTHOLEDIALOG_H
#define OPENCASCADEPROJECT_CUTHOLEDIALOG_H

#include <QDialog>
#include "NodeInteractive.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CutHoleDialog; }
QT_END_NAMESPACE

class CutHoleDialog : public QDialog {
Q_OBJECT

public:
    explicit CutHoleDialog(QWidget *parent = nullptr);

    ~CutHoleDialog() override;

    NodeInteractive *getNodeInteractive() const;

    void setNodeInteractive(NodeInteractive *nodeInteractive);

public slots:
    void cutHoleClicked();

private:
    Ui::CutHoleDialog *ui;
    NodeInteractive *nodeInteractive;
};


#endif //OPENCASCADEPROJECT_CUTHOLEDIALOG_H
