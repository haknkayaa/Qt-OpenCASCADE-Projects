#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>


#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>

class QMenu;

class QRubberBand;

//! Adapted a QWidget for OpenCASCADE viewer.
class Viewer : public QWidget {
Q_OBJECT

public:

    //! constructor.
    Viewer(QWidget *parent);

    const Handle(AIS_InteractiveContext) &getContext() const;
    const Handle(V3d_Viewer) &getViewer() const;
    const Handle(V3d_View) &getView() const;

    gp_Pnt getCursor3DPosition(QPoint);



signals:
    void mousePosChanged(QPoint);

public slots:

    void fitAll(void);

    void slot_showTrihedronCube(int);

    void slot_showPerformanceStats(int);

    void slot_show3DGrid(int);

    void slot_changeProjectionAxis(int);

    void slot_showShape(Handle_AIS_Shape);

protected:
    QPaintEngine *paintEngine() const override;

    // Paint events.
    void paintEvent(QPaintEvent *theEvent) override;

    void resizeEvent(QResizeEvent *theEvent) override;

    // Mouse events.
    void mousePressEvent(QMouseEvent *theEvent) override;

    void mouseReleaseEvent(QMouseEvent *theEvent) override;

    void mouseMoveEvent(QMouseEvent *theEvent) override;

    void wheelEvent(QWheelEvent *theEvent) override;

    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);


private:

    //! the occ viewer.
    Handle(V3d_Viewer) myViewer;

    //! the occ view.
    Handle(V3d_View) myView;

    //! the occ context.
    Handle(AIS_InteractiveContext) myContext;

    opencascade::handle<AIS_ViewCube> aisViewCube;

    QPoint mouseStartPosition;

    //! rubber rectangle for the mouse selection.
    QRubberBand *myRectBand;

};

#endif // _Viewer_H_