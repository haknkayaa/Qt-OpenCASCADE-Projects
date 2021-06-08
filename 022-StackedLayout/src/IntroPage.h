/******************************************************************************
 *    IntroPage.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/24/21.
 *     description :
 ******************************************************************************/

#ifndef MRADSIM_GUI_INTROPAGE_H
#define MRADSIM_GUI_INTROPAGE_H


#include <QtWidgets>


class IntroPage : public QWidget{
Q_OBJECT
public:
    IntroPage(QWidget *parent);
    QStackedWidget *stackedWidget;
    QWidget *rightContentWidget;

signals:

private slots:
    void newProject_button();
    void openProject_button();
    void settings_button();
    void examples_button();
};


#endif //MRADSIM_GUI_INTROPAGE_H
