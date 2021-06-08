// Header Library
#include "Viewer.h"

// Qt Libraries
#include <QApplication>

#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>
#include <QDebug>
#include <QMenu>

//#include <QtWidgets>

// OpenCASCADE Libraries
#include <OpenGl_GraphicDriver.hxx>

#include <V3d_View.hxx>
#include <Aspect_DisplayConnection.hxx>
//#include <Handle_AIS_InteractiveContext.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>


#ifdef WIN32 // Windows Operating System
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX) // MacOS Operating System
#include <Cocoa_Window.hxx>
#else // Unix Operating System

#include <Xw_Window.hxx>

#endif

#include <AIS_ViewCube.hxx>
#include <AIS_ConnectedInteractive.hxx>
#include <AIS_Trihedron.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <V3d_TypeOfOrientation.hxx>
#include <ElSLib.hxx>
#include <ProjLib.hxx>

static Handle(Graphic3d_GraphicDriver) &GetGraphicDriver() {
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}

static Aspect_TypeOfTriedronPosition toOccCorner(Qt::Corner corner) {
    switch (corner) {
        case Qt::TopLeftCorner:
            return Aspect_TOTP_LEFT_UPPER;
        case Qt::TopRightCorner:
            return Aspect_TOTP_RIGHT_UPPER;
        case Qt::BottomLeftCorner:
            return Aspect_TOTP_LEFT_LOWER;
        case Qt::BottomRightCorner:
            return Aspect_TOTP_RIGHT_LOWER;
    }

    return Aspect_TOTP_LEFT_UPPER; // Fallback
}

Viewer::Viewer(QWidget *parent)
        : QWidget(parent),
          myRectBand(nullptr) {
    // No Background
    setBackgroundRole(QPalette::NoRole);

    // set focus policy to threat QContextMenuEvent from keyboard
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking(true);

    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull()) {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId) winId();

    // Create appropriate window for platform
#ifdef WNT
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *) window_handle);
#else
    Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window) window_handle);
#endif

    // Create V3dViewer and V3d_View
    TCollection_ExtendedString name(this->windowTitle().toUtf8().constData());
    myViewer = new V3d_Viewer(GetGraphicDriver(), Standard_ExtString("viewer3d"));
//    myViewer = new V3d_Viewer(GetGraphicDriver(), name.ToExtString(), "", 300.0,  V3d_XposYnegZpos,
//                              Quantity_NOC_BLACK, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
//                              Standard_True, Standard_True, V3d_TEX_NONE);

    // Set up lights etc
    // V3d_ORTHOGRAPHIC
    // V3d_PERSPECTIVE
    myViewer->SetDefaultTypeOfView(V3d_PERSPECTIVE);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();


    myView = myViewer->CreateView();


    myView->SetWindow(wind);
    if (!wind->IsMapped())
        wind->Map();

    //myView->SetBackgroundColor(Quantity_NOC_ALICEBLUE);
    myView->SetBgGradientColors(Quantity_NOC_ALICEBLUE, Quantity_NOC_LIGHTBLUE4, Aspect_GFM_VER, false);
//    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);

    myView->Redraw();
    myView->Update();


}


const Handle(AIS_InteractiveContext) &Viewer::getContext() const {
    return myContext;
}

const Handle(V3d_Viewer) &Viewer::getViewer() const {
    return myViewer;
}

const Handle(V3d_View) &Viewer::getView() const {
    return myView;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine *Viewer::paintEngine() const {
    return nullptr;
}

//
void Viewer::paintEvent(QPaintEvent *theEvent) {
    qDebug() << "Paint event";
    myView->MustBeResized();
    myView->Redraw();
}

void Viewer::resizeEvent(QResizeEvent *theEvent) {
    qDebug() << "Resize Event";

    if (!myView.IsNull()) {
        myView->MustBeResized();
    }
}

void Viewer::fitAll() {
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}



/*
 *  MOUSE
 *  EVENTLERİ
 */

///  Mouse basıldığında çalışacak event
/// \param theEvent
void Viewer::mousePressEvent(QMouseEvent *theEvent) {

    // Mouse'un ilk basılma anındaki position bilgilerini al
    mouseStartPosition = theEvent->pos();

    // Left Click
    if (theEvent->button() == Qt::LeftButton) {
        qDebug() << "Sol click basıldı";
    }

        // Middle Click
    else if (theEvent->button() == Qt::MidButton) {
        qDebug() << "Orta click basıldı";
        myView->StartRotation(mouseStartPosition.x(), mouseStartPosition.y());
    }

        // Right Click
    else if (theEvent->button() == Qt::RightButton) {
        qDebug() << "Sağ click basıldı";

    }
}

/// Mouse tuşu basıldıktan sonra serbest kaldığında çalışacak event
/// \param theEvent
void Viewer::mouseReleaseEvent(QMouseEvent *theEvent) {
    // left tuş basıldığında
    if (theEvent->button() == Qt::LeftButton) {
        qDebug() << "Sol click serbest kaldı";

        // eğer ekranda rubberband var ise
        if (myRectBand) {
            myRectBand->setVisible(false);
            myView->Update();
        }
    }
        // orta tuş ile rotate fonksiyonu
    else if (theEvent->button() == Qt::MidButton) {
        qDebug() << "Orta click serbest kaldı";
    }
        // sağ click ile pan
    else if (theEvent->button() == Qt::RightButton) {
        qDebug() << "Sağ click serbest kaldı";
    }

    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

/// Mouse hareket halindeyken çalışacak event
/// \param theEvent
void Viewer::mouseMoveEvent(QMouseEvent *theEvent) {

//    qDebug() << theEvent->pos().x() << ", " << theEvent->pos().y();

//    mouseX = theEvent->pos().x();
//    mouseY = theEvent->pos().y();

    emit mousePosChanged(theEvent->pos());

    // Sol Click basolıysa QRubberBand çiz
    if (theEvent->buttons() & Qt::LeftButton) {
        // Mouse cursorunu değiştirme
        QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));

        drawRubberBand(mouseStartPosition.x(), mouseStartPosition.y(), theEvent->pos().x(), theEvent->pos().y());
        //dragEvent(theEvent->post().x(), theEvent->pos().y());
    }

        // Orta tuş kombinasyonu ile döndürme
    else if (theEvent->buttons() & Qt::MidButton) {
        // Mouse cursorunu değiştirme
        QApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));

        // görüntüyü rotate ettirmeye devam et
        myView->Rotation(theEvent->pos().x(), theEvent->pos().y());
    }

        // Sağ click basılıysa taşıma yap
    else if (theEvent->buttons() & Qt::RightButton) {
        // Mouse cursorunu değiştirme
        QApplication::setOverrideCursor(QCursor(Qt::OpenHandCursor));

        // Görüntüyü taşıma
        myView->Pan(theEvent->pos().x() - mouseStartPosition.x(), mouseStartPosition.y() - theEvent->pos().y());
        mouseStartPosition = theEvent->pos();
    }
}

/// Mouse tekerlek etkileşimi olduğunda çalışan event
/// \param theEvent
void Viewer::wheelEvent(QWheelEvent *theEvent) {
    Standard_Integer aFactor = 16;

    Standard_Integer aX = theEvent->pos().x();
    Standard_Integer aY = theEvent->pos().y();

    if (theEvent->delta() > 0) {
        aX += aFactor;
        aY += aFactor;
    } else {
        aX -= aFactor;
        aY -= aFactor;
    }

    myView->Zoom(theEvent->pos().x(), theEvent->pos().y(), aX, aY);
}


void Viewer::drawRubberBand(const int minX, const int minY, const int maxX, const int maxY) {
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!myRectBand) {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));

    }

    myRectBand->setGeometry(aRect);
    myRectBand->show();
}



/// Viewer'da cursorun 3d pozisyon bilgi döndürür
/// \param currPos :
/// \return gp_Pnt: x, y, z şeklinde point döndürür.
gp_Pnt Viewer::getCursor3DPosition(QPoint currPos) {
    double xEye, yEye, zEye, xAt, yAt, zAt;
    myView->Eye(xEye, yEye, zEye);
    myView->At(xAt, yAt, zAt);
    const gp_Pnt pntEye(xEye, yEye, zEye);
    const gp_Pnt pntAt(xAt, yAt, zAt);

    const gp_Vec vecEye(pntEye, pntAt);
    const gp_Dir dirEye(vecEye);

    const gp_Pln planeView(pntAt, dirEye);
    double px, py, pz;
    const int ix = static_cast<int>(std::round(currPos.x()));
    const int iy = static_cast<int>(std::round(currPos.y()));

    myView->Convert(ix, iy, px, py, pz);

    const gp_Pnt pntConverted(px, py, pz);
    const gp_Pnt2d pntConvertedOnPlane = ProjLib::Project(planeView, pntConverted);

    const gp_Pnt pos3d = ElSLib::Value(pntConvertedOnPlane.X(), pntConvertedOnPlane.Y(), planeView);

    return pos3d;
}



/// "Show Performance Stats" checkbox'ın durumu değiştiriğinde çalışacak event
/// \param theState: boş iken 0, tik olursa 2 döndürür
void Viewer::slot_showPerformanceStats(int theState) {
    if (theState) {
        qDebug() << "Show Performance Stats: " << QString::number(theState);

        myView->ChangeRenderingParams().ToShowStats = true;
        myView->Redraw();

    } else {
        qDebug() << "Show Performance Stats: " << QString::number(theState);

        myView->ChangeRenderingParams().ToShowStats = false;
        myView->Redraw();
    }
}

/// "Show Trihedron Cube" checkbox'ın durumu değiştirildiğinde çalışacak event
/// \param theState: boş iken 0, tik olursa 2 döndürür
void Viewer::slot_showTrihedronCube(int theState) {
    if (theState) {

        qDebug() << "Show Trihedron : " << QString::number(theState);


        // Centered Trihedron Axis
        Handle_Geom_Axis2Placement axis = new Geom_Axis2Placement(gp::XOY());
        Handle_AIS_Trihedron aisTrihedron = new AIS_Trihedron(axis);
        aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
        aisTrihedron->SetDrawArrows(false);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
        aisTrihedron->SetLabel(Prs3d_DP_XAxis, "");
        aisTrihedron->SetLabel(Prs3d_DP_YAxis, "");
        aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "");
        //aisTrihedron->SetTextColor(Quantity_NOC_GRAY40);
        aisTrihedron->SetSize(60);
        //  aisTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers,Aspect_TOTP_LEFT_UPPER,Graphic3d_Vec2i(50, 50)));
        aisTrihedron->SetTransformPersistence(
                new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location()));
        aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
        aisTrihedron->SetInfiniteState(true);
//    myContext->Display(aisTrihedron, false);

        // AIS_ViewCube
        aisViewCube = new AIS_ViewCube;
        aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
        //aisViewCube->SetFixedAnimationLoop(false);
        aisViewCube->SetSize(55);
        aisViewCube->SetFontHeight(12);
        aisViewCube->SetAxesLabels("X", "Y", "Z");
        aisViewCube->SetTransformPersistence(
                new Graphic3d_TransformPers(
                        Graphic3d_TMF_TriedronPers,
                        Aspect_TOTP_LEFT_LOWER,
                        Graphic3d_Vec2i(85, 85)));

        const Handle_Prs3d_DatumAspect &datumAspect = aisViewCube->Attributes()->DatumAspect();
        datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
        datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
        datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
        aisViewCube->Attributes()->SetDatumAspect(datumAspect); // bu neden gerekli bilmiyorum

        myContext->Display(aisViewCube, true);

    } else {
        qDebug() << "Show Trihedron : " << QString::number(theState);

        if (aisViewCube) {
            myContext->Remove(aisViewCube, True); // tamamen kaldırır hiç yaratılmamış gibi
            //myContext->Erase(aisViewCube, True); // sadece gizler
        }
    }
}

/// "Show 3D Grid" chechbox'ın durumu değiştirildiğinde çalışacak event
/// \param theState: boş iken, tik olursa 2 döndürür
void Viewer::slot_show3DGrid(int theState) {
    if (theState) { // ON
        qDebug() << "Show 3D Grid : " << QString::number(theState);

        myViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
        myViewer->Update();
    } else {
        qDebug() << "Show 3D Grid : " << QString::number(theState);

        myViewer->DeactivateGrid();
        myView->Update();

    }
}


void Viewer::slot_changeProjectionAxis(int axis) {
    qDebug() << "Triggered :" << QString::number(axis);

    switch (axis) {
        case 1:
            myView->SetProj(V3d_Xpos);
            myView->Update();
            break;

        case 2:
            myView->SetProj(V3d_Xneg);
            myView->Update();
            break;

        case 3:
            myView->SetProj(V3d_Ypos);
            myView->Update();
            break;

        case 4:
            myView->SetProj(V3d_Yneg);
            myView->Update();
            break;

        case 5:
            myView->SetProj(V3d_Zpos);
            myView->Update();
            break;

        case 6:
            myView->SetProj(V3d_Zneg);
            myView->Update();
            break;
    }
}




