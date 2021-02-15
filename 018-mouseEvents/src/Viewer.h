#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
class QMenu;
class QRubberBand;

struct Shape{
    QString name;
    AIS_Shape *shape;
};

//! Adapted a QWidget for OpenCASCADE viewer.
class Viewer : public QWidget {
Q_OBJECT

public:
    //! constructor.
    Viewer(QWidget *parent);

    const Handle(AIS_InteractiveContext) &getContext() const;

    void changeViewProjectionType();
    TopoDS_Shape settingCurrentSelectedShape();
    void viewTop();
    void viewBottom();
    void viewLeft();
    void viewRight();

    void cube(const QString &name, const double &dx , const double &dy, const double &dz);
    void cylinder(const QString &name,const double &r, const double &h);
    void sphere(const QString &name,const double &r);

    void moveTo(AIS_Shape *currentItem, const int &x, const int &y, const int &z);
    void merge(const QString &s1, const QString &s2);
    void selectionMode(const int &mode);
    void multiSelMode();
    std::vector <Shape> shapes;
signals:

public slots:

    void fitAll(void);

    static void action_Action1();

private:
    QPaintEngine *paintEngine() const override;

    // Paint events.
    void paintEvent(QPaintEvent *theEvent) override;
    void resizeEvent(QResizeEvent *theEvent) override;

    // Mouse events.
    void mousePressEvent(QMouseEvent *theEvent) override; //This function used to trigger functions when mouse button pressed
    void mouseReleaseEvent(QMouseEvent *theEvent) override;
    void mouseMoveEvent(QMouseEvent *theEvent) override;
    void wheelEvent(QWheelEvent *theEvent) override;

    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    bool multiMode = false;



    //! the occ viewer.
    Handle(V3d_Viewer) myViewer;

    //! the occ view.
    Handle(V3d_View) myView;

    //! the occ context.
    Handle(AIS_InteractiveContext) myContext;


    QPoint mouseStartPosition;;

    //! rubber rectangle for the mouse selection.
    QRubberBand *myRectBand;

};

#endif // _Viewer_H_