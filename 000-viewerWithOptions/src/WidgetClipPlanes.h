/******************************************************************************
 *    WidgetClipPlanes.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 6/10/21.
 *     description :
 ******************************************************************************/

#ifndef PROJECT_WIDGETCLIPPLANES_H
#define PROJECT_WIDGETCLIPPLANES_H

#include <QtWidgets>
#include <QtCore/QFile>

#include <Bnd_Box.hxx>
#include <Graphic3d_ClipPlane.hxx>
#include <Graphic3d_Texture2Dmanual.hxx>
#include <Image_AlienPixMap.hxx>

class WidgetClipPlanes : public QWidget{
Q_OBJECT
public:
    WidgetClipPlanes(QWidget *parent = nullptr);
};


#endif //PROJECT_WIDGETCLIPPLANES_H
