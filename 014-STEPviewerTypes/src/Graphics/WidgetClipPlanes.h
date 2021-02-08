/******************************************************************************
 *    WidgetClipPlanes.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/8/21.
 *     description :
 ******************************************************************************/

#ifndef WIDGETCLIPPLANES_H
#define WIDGETCLIPPLANES_H

#include <QWidget>

class WidgetClipPlanes: public QWidget {
Q_OBJECT
public:
    WidgetClipPlanes(const Handle_V3d_View& arg_view3d, QWidget* parent = nullptr);
    ~WidgetClipPlanes();

    void setRanges(const Bnd_Box& box);
    void setClippingOn(bool on);
};


#endif //WIDGETCLIPPLANES_H
