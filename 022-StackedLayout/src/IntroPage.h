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

#include "NewProjectPage.h"
#include "OpenProjectPage.h"
#include "SettingsPage.h"
#include "ExamplesPage.h"
class NewProjectPage;
class OpenProjectPage;
class SettingsPage;
class ExamplesPage;

class IntroPage : public QWidget{
Q_OBJECT
public:
    IntroPage(QWidget *parent);
    NewProjectPage *newProjectPageContent;
    OpenProjectPage *openProjectPageContent;
    SettingsPage *settingsPageContent;
    ExamplesPage *examplesPageContent;
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
