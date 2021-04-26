/******************************************************************************
 *    TabToolbar.cpp 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 3/2/21.
 *     description :
 ******************************************************************************/

#include "TabToolbar.h"

TabToolbar::TabToolbar(QWidget *parent) : QWidget(parent) {

    setStyleSheet("background-color: green");

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);

    QPushButton *test = new QPushButton("Test", this);
    mainLayout->addWidget(test);

    QPushButton *test2 = new QPushButton("Test 2", this);
    mainLayout->addWidget(test2);

    setLayout(mainLayout);
}

void TabToolbar::addPage(QString pageName) {

}

void TabToolbar::addGroup(QString groupName) {

}

void TabToolbar::addWidget(QWidget widget) {

}

