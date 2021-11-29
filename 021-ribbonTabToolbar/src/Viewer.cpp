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
#include <AIS_ViewCube.hxx>

#endif



static Handle(Graphic3d_GraphicDriver) &GetGraphicDriver() {
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
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


    myViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);

    myView = myViewer->CreateView();

    myView->SetWindow(wind);
    if (!wind->IsMapped())
        wind->Map();

    //myView->SetBackgroundColor(Quantity_NOC_ALICEBLUE);
    myView->SetBgGradientColors(Quantity_NOC_ALICEBLUE, Quantity_NOC_LIGHTBLUE4, Aspect_GFM_VER, false);
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);

    myView->Redraw();
    myView->Update();


    opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    //aisViewCube->SetFixedAnimationLoop(false);
    aisViewCube->SetDrawEdges(True);
    aisViewCube->SetDrawVertices(True);
    aisViewCube->SetBoxTransparency(0);
    aisViewCube->SetDrawAxes(false);
    aisViewCube->SetSize(40);
    aisViewCube->SetFontHeight(8);
    aisViewCube->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers,Aspect_TOTP_LEFT_UPPER,Graphic3d_Vec2i(50, 50)));
    aisViewCube->SetAutoStartAnimation (true);

//    TCollection_AsciiString emptyStr;
//    aisViewCube->SetBoxSideLabel(V3d_Xpos, "ON");
//    aisViewCube->SetBoxSideLabel(V3d_Ypos, "SOL YAN");
//    aisViewCube->SetBoxSideLabel(V3d_Zpos, "ÜST");
//    aisViewCube->SetBoxSideLabel(V3d_Xneg, "ARKA");
//    aisViewCube->SetBoxSideLabel(V3d_Yneg, "SAG YAN");
//    aisViewCube->SetBoxSideLabel(V3d_Zneg, "ALT");
    aisViewCube->SetDisplayMode(1);
    myContext->Display(aisViewCube, false);

}


const Handle(AIS_InteractiveContext) &Viewer::getContext() const {

    return myContext;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine *Viewer::paintEngine() const {
    return nullptr;
}

//
void Viewer::paintEvent(QPaintEvent * theEvent) {
    qDebug() << "Paint event";
    myView->MustBeResized();
    myView->Redraw();
}

void Viewer::resizeEvent(QResizeEvent * theEvent ) {
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

// Mouse tuşu basıldığında
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

// Mouse tuşu serbest kaldığında
void Viewer::mouseReleaseEvent(QMouseEvent *theEvent) {
    // left tuş basıldığında
    if (theEvent->button() == Qt::LeftButton) {
        qDebug() << "Sol click serbest kaldı";

        // eğer ekranda rubberband var ise
        if(myRectBand){
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

// Mouse hareket eventi
void Viewer::mouseMoveEvent(QMouseEvent *theEvent) {

//    qDebug() << theEvent->pos().x() << ", " << theEvent->pos().y();

//    mouseX = theEvent->pos().x();
//    mouseY = theEvent->pos().y();
    emit mousePosChanged(theEvent->pos().x(), theEvent->pos().y());

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

// Mouse tekerlek eventi
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
