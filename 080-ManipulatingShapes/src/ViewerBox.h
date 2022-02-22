//
// Created by sufuk on 22.02.2022.
//

#ifndef OPENCASCADEPROJECT_VIEWERBOX_H
#define OPENCASCADEPROJECT_VIEWERBOX_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ViewerBox; }
QT_END_NAMESPACE

class ViewerBox : public QWidget {
Q_OBJECT

public:
    explicit ViewerBox(QWidget *parent = nullptr);

    ~ViewerBox() override;

private:
    Ui::ViewerBox *ui;
};


#endif //OPENCASCADEPROJECT_VIEWERBOX_H
