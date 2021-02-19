/******************************************************************************
 *    ButtonManager.cpp 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/18/21.
 *     description :
 ******************************************************************************/

#include "ButtonManager.h"
#include <QDebug>

ButtonManager::ButtonManager(QObject *parent)
        : QObject(parent)
{

}

void ButtonManager::onButtonClicked(QString str)
{
    qDebug() << "button: " << str;
}

