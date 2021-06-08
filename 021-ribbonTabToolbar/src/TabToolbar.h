/******************************************************************************
 *    TabToolbar.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 3/2/21.
 *     description :
 ******************************************************************************/

#ifndef RIBBONTABTOOLBAR_TABTOOLBAR_H
#define RIBBONTABTOOLBAR_TABTOOLBAR_H

#include <QtWidgets>

class TabToolbar : public QWidget{
Q_OBJECT
public:
    explicit TabToolbar(QWidget *parent = nullptr);


    void addPage(QString pageName);
    void addGroup(QString groupName);
    void addWidget(QWidget widget);

    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
};


#endif //RIBBONTABTOOLBAR_TABTOOLBAR_H
