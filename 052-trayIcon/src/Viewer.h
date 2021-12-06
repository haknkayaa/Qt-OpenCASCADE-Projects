#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>

#include <AIS_InteractiveContext.hxx>

class QMenu;

class QRubberBand;

//! Adapted a QWidget for OpenCASCADE viewer.
class Viewer : public QWidget {
Q_OBJECT

public:

public:
    //! constructor.
    Viewer(QWidget *parent);

    const Handle(AIS_InteractiveContext) &getContext() const;

signals:

    void selectionChanged(void);

public slots:

    void fitAll(void);



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

protected:

    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);


private:

    //! the occ viewer.
    Handle(V3d_Viewer) myViewer;

    //! the occ view.
    Handle(V3d_View) myView;

    //! the occ context.
    Handle(AIS_InteractiveContext) myContext;


    QPoint mouseStartPosition;

    //! rubber rectangle for the mouse selection.
    QRubberBand *myRectBand;

};

#endif // _Viewer_H_