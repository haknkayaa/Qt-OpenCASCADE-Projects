//
// Created by alper on 21.10.2021.
//

#ifndef RIGHTCLICK_H
#define RIGHTCLICK_H

#include <QWidget>

class RightClick : public QWidget
{
Q_OBJECT
public:
    explicit RightClick(QWidget* parent = 0);

public slots:
    void customContextMenuShow(const QPoint& pos);
};

#endif // RIGHTCLICK_H