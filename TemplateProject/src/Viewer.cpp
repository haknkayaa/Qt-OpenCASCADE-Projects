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
#include <AIS_Trihedron.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <V3d_TypeOfOrientation.hxx>
#include <ElSLib.hxx>
#include <ProjLib.hxx>
//#include <BRepBuilderAPI_Transform.hxx>

///
static Handle(Graphic3d_GraphicDriver) &GetGraphicDriver() {
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}

///
/// \param parent
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
//    TCollection_ExtendedString name(this->windowTitle().toUtf8().constData());
    myViewer = new V3d_Viewer(GetGraphicDriver());

    // Set up lights etc
    // V3d_ORTHOGRAPHIC
    // V3d_PERSPECTIVE
    myViewer->SetDefaultTypeOfView(V3d_ORTHOGRAPHIC);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();


    myView = myViewer->CreateView();
    myView->SetWindow(wind);
    if (!wind->IsMapped())
        wind->Map();

    //myView->SetBackgroundColor(Quantity_NOC_ALICEBLUE); // one color
    myView->SetBgGradientColors(Quantity_NOC_ALICEBLUE, Quantity_NOC_LIGHTBLUE4, Aspect_GFM_VER, false);
//    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);


    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->HighlightStyle()->SetColor(Quantity_NOC_HOTPINK);
    myContext->SelectionStyle()->SetColor(Quantity_NOC_GREEN1);

    myContext->SetDisplayMode(AIS_Shaded, Standard_True);

    // Camera Animation
    m_cameraAnimation = new ViewerCameraController(myView, this);
    m_cameraAnimation->setDuration(200);

    myContext->MainSelector()->SetPickClosest(Standard_False);

    myView->MustBeResized();
    myView->Redraw();


}

///
/// \return
const Handle(AIS_InteractiveContext) &Viewer::getContext() const {
    return myContext;
}


/// Get paint engine for the OpenGL viewer. [ virtual public ]
QPaintEngine *Viewer::paintEngine() const {
    return nullptr;
}

//
void Viewer::paintEvent(QPaintEvent *theEvent) {
    //qDebug() << "Paint event";
    myView->MustBeResized();
    myView->Redraw();
}

void Viewer::resizeEvent(QResizeEvent *theEvent) {
    //qDebug() << "Resize Event";
    if (!myView.IsNull()) {
        myView->MustBeResized();
    }
}

/*****************************************************
 *   MOUSE EVENTLERİ
 ********************************************************/

/// Mouse tuşu basıldığında
void Viewer::mousePressEvent(QMouseEvent *theEvent) {

    // Mouse'un ilk basılma anındaki position bilgilerini al
    mouseStartPosition = theEvent->pos();


    // Left Click
    if (theEvent->button() == Qt::LeftButton) {
        //qDebug() << "Left button clicked ";

        // ais cube mouse interactions
        auto viewCubeOwner = opencascade::handle<AIS_ViewCubeOwner>::DownCast(myContext->DetectedOwner());
        if (viewCubeOwner) {
            this->setViewCameraOrientation(viewCubeOwner->MainOrientation());
        }

        // önce seçimleri silip görüntüyü updateler
        myContext->ClearSelected(true);

        // eğer detect edilen şekil varsa onu hilight yap
        if (!myContext->DetectedOwner().IsNull()) {
            Handle(AIS_InteractiveObject) obj = myContext->DetectedInteractive();
            myContext->AddOrRemoveSelected(obj, true);

            // viewera signal yay
            emit mouseSelectedShape();

        } else {
            emit mouseSelectedVoid();
        }
    }

        // Middle Click
    else if (theEvent->button() == Qt::MidButton) {
        //qDebug() << "Middle button clicked ";
        myView->StartRotation(mouseStartPosition.x(), mouseStartPosition.y());
    }

        // Right Click
    else if (theEvent->button() == Qt::RightButton) {
        //qDebug() << "Right button clicked";

        // qDebug() << "Opening Menu";

    }
}

/// Mouse tuşu serbest kaldığında
void Viewer::mouseReleaseEvent(QMouseEvent *theEvent) {
    // left tuş basıldığında
    if (theEvent->button() == Qt::LeftButton) {
        //qDebug() << "Left button released";

        // eğer ekranda rubberband var ise
        if (myRectBand) {
            myRectBand->setVisible(false);
            myView->Update();
        }

    }
        // orta tuş ile rotate fonksiyonu
    else if (theEvent->button() == Qt::MidButton) {
        //qDebug() << "Middle button released";
    }
        // sağ click ile pan
    else if (theEvent->button() == Qt::RightButton) {
        //qDebug() << "Right button released";


        { // Right Click Menü
            QMenu contextMenu(tr("Context menu"), this);

            QAction action1(tr("Show All Parts"), this);
            connect(&action1, SIGNAL(triggered()), this, SLOT(slot_action_showAllParts()));
            contextMenu.addAction(&action1);

            QAction action2(tr("Show Only"), this);
            connect(&action2, SIGNAL(triggered()), this, SLOT(slot_action_showOnly()));
            contextMenu.addAction(&action2);

            contextMenu.addSeparator();

            QAction selectDetector(tr("Select Detector"), this);
            connect(&selectDetector, &QAction::triggered, this, &Viewer::slot_action_selectDetector);
            contextMenu.addAction(&selectDetector);

            contextMenu.addSeparator();
            QAction action3("Visible / Invisible", this);
            action3.setIcon(QIcon::fromTheme("view-fullscreen"));
            connect(&action3, SIGNAL(triggered()), this, SLOT(slot_action_visibleOrInvisible()));
            contextMenu.addAction(&action3);

            contextMenu.addSeparator();

            QAction action4("Fit All", this);
            connect(&action4, SIGNAL(triggered()), this, SLOT(fitAll()));
            contextMenu.addAction(&action4);

            contextMenu.exec(mapToGlobal(theEvent->pos()));
        } // Right Click Menü
    }

    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

}

/// Mouse hareket eventi
void Viewer::mouseMoveEvent(QMouseEvent *theEvent) {

    QPoint aPoint = theEvent->pos();
    Standard_Integer x, y;
    x = aPoint.x();
    y = aPoint.y();

    myContext->MoveTo(x, y, myView, true);


    // Sol Click basolıysa QRubberBand çiz
    if (theEvent->buttons() & Qt::LeftButton) {
        // Mouse cursorunu değiştirme
//        QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
//
//        drawRubberBand(mouseStartPosition.x(), mouseStartPosition.y(), theEvent->pos().x(), theEvent->pos().y());


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

/// Mouse tekerlek eventi
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

/// RubberBand çizen fonksiyon
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

/// Projeksiyon modunu değiştirir
/// \param currentMode : şuanlık kullanılmıyor
void Viewer::changeProjectionMode(const QString &currentMode) {
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

/// "Show Trihedron Cube" checkbox'ın durumu değiştirildiğinde çalışacak event
/// \param theState: boş iken 0, tik olursa 2 döndürür
bool Viewer::showTrihedronCube(bool theState) {
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
        aisTrihedron->SetSize(50);
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
        aisViewCube->SetSize(30);
        aisViewCube->SetFontHeight(8);
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
            myContext->Remove(aisViewCube, True); // tamamen kaldırır hiç yaratılmamış gibi
            //myContext->Erase(aisViewCube, True); // sadece gizler
        }
    }

    return true;
}

///
/// \return
bool Viewer::isEnabledTrihedronCube() {
    if (myContext->IsDisplayed(aisViewCube)) {
        return true;
    } else {
        return false;
    }
}

/// "Show Performance Stats" checkbox'ın durumu değiştiriğinde çalışacak event
/// \param theState: boş iken 0, tik olursa 2 döndürür
bool Viewer::showPerformanceStats(int theState) {
    if (theState) {
        qDebug() << "Show Performance Stats: " << QString::number(theState);

        myView->ChangeRenderingParams().ToShowStats = true;
        myView->Redraw();

    } else {
        qDebug() << "Show Performance Stats: " << QString::number(theState);

        myView->ChangeRenderingParams().ToShowStats = false;
        myView->Redraw();
    }
    return true;
}

///
/// \return
bool Viewer::isEnabledPerformanceStats() {
    if (myView->ChangeRenderingParams().ToShowStats) {
        return true;
    } else {
        return false;
    }
}

/// "Show 3D Grid" chechbox'ın durumu değiştirildiğinde çalışacak event
/// \param theState: boş iken, tik olursa 2 döndürür
bool Viewer::show3DGrid(bool theState) {
    if (theState) { // ON
        qDebug() << "Show 3D Grid : " << QString::number(theState);

        myViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
        myViewer->Update();

    } else {
        qDebug() << "Show 3D Grid : " << QString::number(theState);

        myViewer->DeactivateGrid();
        myView->Update();

    }
    return true;
}

///
/// \return
bool Viewer::isEnabled3DGrid() {
    if (myViewer->IsActive()) {
        return false;
    } else {
        return true;
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


/// Bir şekli display etmek için kullanılabilecek method.
/// \param shape AIS_Shape formatındaki model objesi
void Viewer::slot_showShape(Handle_AIS_Shape shape) {
    myContext->Display(shape, true);
}


/// Değişen bakış açısından animasyon yaratır.
/// \param projection : animasyon için uygulanacak açı
void Viewer::setViewCameraOrientation(V3d_TypeOfOrientation projection) {
    this->runViewCameraAnimation([=](const Handle_V3d_View &view) {
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

/// View Mode 1: Style Points
///  Seçilen objenin görünümünü nokta bulutu halinde gösterir.
void Viewer::slot_viewStylePoints() {
    qDebug() << "slot_viewStylePoints";
    //Todo implement this
}

/// View Mode 2: Style WireFrame
/// Seçilen objenin görünümünü tel örgü halinde gösterir.
void Viewer::slot_viewStyleWireFrame() {
    qDebug() << "slot_viewStyleWireFrame";

    myContext->SetDisplayMode(AIS_WireFrame, true);
}

/// View Mode 3: Style Hidden Line
void Viewer::slot_viewStyleHiddenLine() {
    qDebug() << "slot_viewStyleHiddenLine";

}

/// View Mode 4: Style No Shading
void Viewer::slot_viewStyleNoShading() {
    qDebug() << "slot_viewStyleNoShading";
    myView->SetShadingModel(V3d_COLOR);

    myContext->EraseAll(false);
    myContext->DisplayAll(true);
}

/// View Mode 5: Style No Shading
void Viewer::slot_viewStyleShaded() {
    qDebug() << "slot_viewStyleShaded";

    myContext->SetDisplayMode(AIS_Shaded, true);
}

void Viewer::toggleViewProjectionMode() {
    if (myView->Type() == V3d_ORTHOGRAPHIC) {
        qDebug() << "Projection_Perspective has been set.";

        myView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
        myView->Update();

    } else {
        qDebug() << "Projection_Orthographic has been set.";

        myView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
        myView->Update();
    }
}

void Viewer::slot_viewReset() {
    qDebug() << "Reset View";

//    myView->SetProj(V3d_XposYnegZpos);
//    myView->FitAll(0.2);
//    myView->Update();
    this->setViewCameraOrientation(V3d_XposYnegZpos);
}

/// Görüntüdeki öğeleri sıkıştırılacak şekilde ayarlar
void Viewer::fitAll() {
    myView->FitAll(0.2);
    myView->ZFitAll();
    myView->Redraw(); //Redraw is mandatory or viewer cannot update changes.
}

void Viewer::slot_viewZoomIn() {
    qDebug() << "slot_viewZoomIn";
    myView->Zoom(0, 0, 50, 50);
}

void Viewer::slot_viewZoomOut() {
    qDebug() << "slot_viewZoomOut";
    myView->Zoom(50, 50, 0, 0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VIEWER SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ContextMenu üzerinde "show all parts" actionı tetiklendiğinde çalışır.
void Viewer::slot_action_showAllParts() {
    emit mouseSelectedShape();
    emit showAllParts();
}

/// ContextMenu üzerinde "show only" actionı tetiklendiğinde çalışır.
void Viewer::slot_action_showOnly() {
    emit mouseSelectedShape();
    emit showOnly();
}

/// ContextMenu üzerinde "visible or invisible" actionı tetiklendiğinde çalışır.
void Viewer::slot_action_visibleOrInvisible() {
    emit mouseSelectedShape();
    emit visibleOrInvisible();
}

/// ContextMenu üzeirnde "select detector" actionı tetiklendiğinde çalışır.
void Viewer::slot_action_selectDetector() {
    qDebug() << "Select Detector triggered";


    emit selectDetector();

}
