#ifndef VIEWER_H
#define VIEWER_H

// User Libraries
#include "ViewerCameraController.h"

// OpenCASCADE Libararies
#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>
#include <QEasingCurve>
#include <functional>
#include <QAbstractAnimation>

// Qt Libraries
#include <QWidget>

class QMenu;

class QRubberBand;

//! Adapted a QWidget for OpenCASCADE viewer.
class Viewer : public QWidget {
Q_OBJECT

public:

    explicit Viewer(QWidget *parent = nullptr);

    const Handle(AIS_InteractiveContext) &getContext() const;

    void setViewCameraOrientation(V3d_TypeOfOrientation projection);

    void runViewCameraAnimation(const std::function<void(Handle_V3d_View)> &fnViewChange);

    void stopViewCameraAnimation();

    gp_Pnt getCursor3DPosition(QPoint);

signals:

    void mouseSelectedShape();

    void mouseSelectedVoid();

    void showAllParts();

    void showOnly();

    void visibleOrInvisible();

    void selectDetector();

    void mousePosChanged(QPoint);

public slots: /* Connect fonksiyonları */

    void toggleViewProjectionMode();

    void changeProjectionMode(QString currentMode);

    bool isEnabledTrihedronCube();

    bool showTrihedronCube(bool theState);

    bool isEnabledPerformanceStats();

    bool showPerformanceStats(int theState);

    bool isEnabled3DGrid();

    bool show3DGrid(bool theState);

    void slot_changeProjectionAxis(int);

    void slot_showShape(Handle_AIS_Shape);

    void slot_viewReset();

    void slot_viewZoomIn();

    void slot_viewZoomOut();

    void slot_viewStylePoints();

    void slot_viewStyleWireFrame();

    void slot_viewStyleHiddenLine();

    void slot_viewStyleNoShading();

    void slot_viewStyleShaded();

    void slot_action_showAllParts();

    void slot_action_showOnly();

    void slot_action_visibleOrInvisible();

    void fitAll();

    void slot_action_selectDetector();

private:
    QPaintEngine *paintEngine() const override;

    void paintEvent(QPaintEvent *theEvent) override;

    void resizeEvent(QResizeEvent *theEvent) override;

    void mousePressEvent(QMouseEvent *theEvent) override;

    void mouseReleaseEvent(QMouseEvent *theEvent) override;

    void mouseMoveEvent(QMouseEvent *theEvent) override;

    void wheelEvent(QWheelEvent *theEvent) override;

    void drawRubberBand(int minX, int minY, int maxX, int maxY);

    ViewerCameraController *viewCameraAnimation() const { return m_cameraAnimation; }

    /* Main Viewer Object */
    Handle(V3d_Viewer) myViewer;

    /* Main View Object*/
    Handle(V3d_View) myView;

    /* Showed object */
    Handle(AIS_InteractiveContext) myContext;

    // Viewer Components
    opencascade::handle<AIS_ViewCube> aisViewCube;

    // Viewer Global Variables
    QPoint mouseStartPosition;
    QRubberBand *myRectBand;

    //
    ViewerCameraController *m_cameraAnimation;


};


#endif // VIEWER_H