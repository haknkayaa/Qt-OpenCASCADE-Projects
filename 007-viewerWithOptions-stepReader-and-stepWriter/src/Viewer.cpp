/*
 *  OpenCASCADE Viewer
 *  version: 0.0.1
 */

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
#ifdef WIN32
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

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->HighlightStyle()->SetColor(Quantity_NOC_HOTPINK);
    myContext->SelectionStyle()->SetColor(Quantity_NOC_GREEN1);
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);

    // Camera Animation
    m_cameraAnimation = new ViewerCameraController(myView, this);
    m_cameraAnimation->setDuration(200);

    // Done
    myView->MustBeResized();
    myView->Redraw();
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

        // ais cube mouse interactions
        auto viewCubeOwner = opencascade::handle<AIS_ViewCubeOwner>::DownCast(myContext->DetectedOwner());
        if (viewCubeOwner) {
            this->setViewCameraOrientation(viewCubeOwner->MainOrientation());
        }


        // önce seçimleri silip görüntüyü updateler
        myContext->ClearSelected(true);

        if (!myContext->DetectedOwner().IsNull()) {
            Handle(AIS_InteractiveObject) obj = myContext->DetectedInteractive();
            myContext->AddOrRemoveSelected(obj, true);

            // viewera signal yay
//            emit mouseSelectedShape();
        } else {
//            emit mouseSelectedVoid();
        }
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

//        // eğer ekranda rubberband var ise
//        if (myRectBand) {
//            myRectBand->setVisible(false);
//            myView->Update();
//        }
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
    QPoint aPoint = theEvent->pos();
    Standard_Integer x, y;
    x = aPoint.x();
    y = aPoint.y();
    myContext->MoveTo(x, y, myView, true);

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

///
/// \param minX
/// \param minY
/// \param maxX
/// \param maxY
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



/// Ögeleri ekrana sığacak şekilde günceller
void Viewer::fitAll() {
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
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



/// Projeksiyon modunu değiştirir
/// \param currentMode : şuanlık kullanılmıyor
void Viewer::slot_changeProjectionMode(QString currentMode) {
    qDebug() << "Current mode: " << currentMode;

    auto mode = myView->Camera()->ProjectionType();

    if (mode == Graphic3d_Camera::Projection_Perspective) {
        myView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
        myView->Update();
    } else {
        myView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
        myView->Update();
    }
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
        aisViewCube->SetDisplayMode(1);
        myContext->Display(aisViewCube, true);

    } else {
        qDebug() << "Show Trihedron : " << QString::number(theState);

        if (aisViewCube) {
            myContext->Remove(aisViewCube, true); // tamamen kaldırır hiç yaratılmamış gibi
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

/// Axis değerine göre görüntüyü döndürür.
/// \param axis 1,2,3,4,5,6
void Viewer::slot_changeProjectionAxis(int axis) {
    qDebug() << "Triggered :" << QString::number(axis);

    switch (axis) {
        case 1: // X+
            myView->SetProj(V3d_Xpos);
            myView->Update();
            fitAll();
            break;

        case 2: // X-
            myView->SetProj(V3d_Xneg);
            myView->Update();
            fitAll();
            break;

        case 3: // Y+
            myView->SetProj(V3d_Ypos);
            myView->Update();
            fitAll();
            break;

        case 4: // Y-
            myView->SetProj(V3d_Yneg);
            myView->Update();
            fitAll();
            break;

        case 5: // Z+
            myView->SetProj(V3d_Zpos);
            myView->Update();
            fitAll();
            break;

        case 6: // Z-
            myView->SetProj(V3d_Zneg);
            myView->Update();
            fitAll();
            break;

        case 7: // Isometric
            myView->SetProj(V3d_XposYposZpos);
            myView->Update();
            fitAll();
            break;
    }
}

/// Checkbox ve slider üzerinden explode işlemini uygular
/// \param active : explode aktif mi değil mi?
/// \param explodeRatio : explode oranı
//!todo explode fonksiyonunu tamamla
void Viewer::slot_explode(int active, int explodeRatio) {
    qDebug() << "Checkbox: " << active << " Value: " << explodeRatio;

//    double m_explodingFactor = explodeRatio / 100.;
//    for (const GraphicsEntity& entity : m_vecGraphicsEntity) {
//        const gp_Pnt entityCenter = BndBoxCoords::get(entity.bndBox).center();
//        for (const GraphicsEntity::Object& object : entity.vecObject) {
//            const gp_Vec vecDirection(entityCenter, BndBoxCoords::get(object.bndBox).center());
//            gp_Trsf trsfMove;
//            trsfMove.SetTranslation(2 * t * vecDirection);
//            m_gfxScene.setObjectTransformation(object.ptr, trsfMove * object.trsfOriginal);
//        }
//    }
//
//    m_gfxScene.redraw();
}

/// Bir şekli display etmek için kullanılabilecek method.
/// \param shape AIS_Shape formatındaki model objesi
void Viewer::slot_showShape(Handle_AIS_Shape shape) {
    myContext->Display(shape, false);
    this->fitAll();
}



/// Değişen bakış açısından animasyon yaratır.
/// \param projection : animasyon için uygulanacak açı
void Viewer::setViewCameraOrientation(V3d_TypeOfOrientation projection) {
    this->runViewCameraAnimation([=](Handle_V3d_View view) {
        view->SetProj(projection);
        fitAll();
    });
}

/// Yapılan anlık değişiklik posiyonlarını kaydedip animation yaratır ve başlatır.
/// \param fnViewChange: yapılan anlık değişik
void Viewer::runViewCameraAnimation(const std::function<void(Handle_V3d_View)> &fnViewChange) {
    m_cameraAnimation->configure(fnViewChange);
    m_cameraAnimation->start(QAbstractAnimation::KeepWhenStopped);
}

/// Kamera animasyonunu durdurur.
void Viewer::stopViewCameraAnimation() {
    m_cameraAnimation->stop();
}


