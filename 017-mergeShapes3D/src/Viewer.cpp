// Header Library
#include "Viewer.h"

// Qt Libraries
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>

// OpenCASCADE Libraries
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
//#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Handle_AIS_InteractiveContext.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>

#ifdef WIN32 // Windows Operating System
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX) // MacOS Operating System
#include <Cocoa_Window.hxx>
#else // Unix Operating System

#include <Xw_Window.hxx>

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
    //myViewer = new V3d_Viewer(GetGraphicDriver(), Standard_ExtString("viewer3d"));
    myViewer = new V3d_Viewer(GetGraphicDriver(), name.ToExtString(), "", 300.0, V3d_XposYnegZpos,
                              Quantity_NOC_BLACK, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
                              Standard_True, Standard_True, V3d_TEX_NONE);

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

    //myView->SetBackgroundColor(Quantity_NOC_ALICEBLUE);
    myView->SetBgGradientColors(Quantity_NOC_ALICEBLUE, Quantity_NOC_GRAY50, Aspect_GFM_VER, false);
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);


    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->SetHilightColor(Quantity_NOC_HOTPINK);
    myContext->SelectionColor(Quantity_NOC_GREEN1);
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);

    myView->MustBeResized();
    myView->Redraw();


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

void Viewer::fitAll() {
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw(); //Redraw is mandatory or viewer cannot update changes.
}




/*****************************************************
 *   MOUSE EVENTLERİ
 ********************************************************/
// Mouse tuşu basıldığında
void Viewer::mousePressEvent(QMouseEvent *theEvent) {

    // Mouse'un ilk basılma anındaki position bilgilerini al
    mouseStartPosition = theEvent->pos();

    // Left Click
    if (theEvent->button() == Qt::LeftButton) {
        qDebug() << "Sol click basıldı";

        // önce seçimleri silip görüntüyü updateler
        myContext->ClearSelected(true);

        // eğer detect edilen şekil varsa onu hilight yap
        if(!myContext->DetectedInteractive().IsNull()){
            Handle(AIS_InteractiveObject) obj = myContext->DetectedInteractive();
            myContext->AddOrRemoveSelected(obj, true);
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

        qDebug() << "Menü açılıyor";

    }
}

// Mouse tuşu serbest kaldığında
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


        { // Right Click Menü
            QMenu contextMenu(tr("Context menu"), this);

            QAction action1("Show All Parts", this);
            connect(&action1, SIGNAL(triggered()), this, SLOT(action_Action1()));
            contextMenu.addAction(&action1);

            QAction action2("Show Only", this);
            connect(&action2, SIGNAL(triggered()), this, SLOT(action_Action1()));
            contextMenu.addAction(&action2);

            contextMenu.addSeparator();

            QAction action3("Visible / Unvisible", this);
            action3.setIcon(QIcon::fromTheme("view-fullscreen"));
            connect(&action3, SIGNAL(triggered()), this, SLOT(action_Action1()));
            contextMenu.addAction(&action3);

            contextMenu.addSeparator();

            QAction action4("Fit All", this);
            connect(&action4, SIGNAL(triggered()), this, SLOT(action_Action1()));
            contextMenu.addAction(&action4);

            contextMenu.exec(mapToGlobal(theEvent->pos()));
        } // Right Click Menü
    }

    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

}

// Mouse hareket eventi
void Viewer::mouseMoveEvent(QMouseEvent *theEvent) {

    QPoint aPoint = theEvent->pos();
    Standard_Integer  x,y;
    x = aPoint.x();
    y = aPoint.y();

    myContext->MoveTo(x, y, myView);


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

void Viewer::changeViewProjectionType() {
    if (myView->Type() == V3d_ORTHOGRAPHIC) {
        qDebug() << "Projection_Perspective ayarlandı.";

        myView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
    } else {
        qDebug() << "Projection_Orthographic ayarlandı.";

        myView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
    }
}

void Viewer::cube(const QString &name, const double &dx, const double &dy, const double &dz) {
    qDebug() << "Cube";
    // Make Box
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(dx, dy, dz).Shape();
    AIS_Shape *anAisBox = new AIS_Shape(aTopoBox);

    anAisBox->SetColor(Quantity_NOC_AZURE);
    Shape newShape;
    newShape.name = name;
    newShape.shape = anAisBox;

    shapes.push_back(newShape);
    getContext()->Display(anAisBox, Standard_True);
    fitAll();
}

/**
 * @param r yarıçap
 * @param h uzunluk
 */
void Viewer::cylinder(const QString &name, const double &r, const double &h) {
    qDebug() << "Cylinder";
    // Make Cylinder
    TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder (r, h).Shape();
    AIS_Shape *anAisCylinder = new AIS_Shape(aTopoCylinder);

    anAisCylinder->SetColor(Quantity_NOC_AZURE);
    Shape newShape;
    newShape.name = name;
    newShape.shape = anAisCylinder;
    shapes.push_back(newShape);
    getContext()->Display(anAisCylinder, Standard_True);
    fitAll();
}

/**
 * @param r yarıçap
 */
void Viewer::sphere(const QString &name, const double &r) {
    qDebug() << "Sphere";
    // Make Cylinder
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(r).Shape();
    AIS_Shape *anAisSphere = new AIS_Shape(aTopoSphere);

    anAisSphere->SetColor(Quantity_NOC_AZURE);
    Shape newShape;
    newShape.name = name;
    newShape.shape = anAisSphere;
    shapes.push_back(newShape);

    getContext()->Display(anAisSphere, Standard_True);
    fitAll();
}

void Viewer::viewTop() {
    qDebug() << "Top View";

    myView->SetProj(V3d_Zpos);
    myView->FitAll(0.2);
    myView->Update();
}

void Viewer::viewBottom() {
    qDebug() << "Bottom View";

    myView->SetProj(V3d_Zneg);
    myView->FitAll(0.2);
    myView->Update();
}

void Viewer::viewLeft() {
    qDebug() << "Left View";

    myView->SetProj(V3d_Yneg);
    myView->FitAll(0.2);
    myView->Update();
}

void Viewer::viewRight() {
    qDebug() << "Right View";

    myView->SetProj(V3d_Ypos);
    myView->FitAll(0.2);
    myView->Update();
}

void Viewer::action_Action1() {
    qDebug() << "CLicking action 1";
}

/**
 *
 * @return : TopoDS_Shape
 */
TopoDS_Shape Viewer::settingCurrentSelectedShape() {

    return myContext->DetectedShape();
}

/**
 * !TODO: bazı şekillerde çökme durumu oluyor..
 * birleştirme sırasına göre ve şekil sayısına göre kontrol edilmeli!.
 * @param s1 ilk shapein treewidgettaki ismi
 * @param s2 ikinci shapein treewidgettaki ismi
 */
void Viewer::merge(const QString &s1, const QString &s2){
    TopoDS_Compound aCompound;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aCompound);

    int index1, index2;
    //birlestirilcek olan shapelerin shapes vectorundeki indexini bulur.
    for (int i = 0; i < shapes.size(); ++i) {
        int x = QString::compare(shapes[i].name, s1, Qt::CaseInsensitive);
        int y = QString::compare(shapes[i].name, s2, Qt::CaseInsensitive);
        if (!x)
            index1 = i;
        else if (!y)
            index2 = i;
    }

    TopoDS_Shape firstShape = shapes[index1].shape->Shape();
    TopoDS_Shape secondShape = shapes[index2].shape->Shape();

    aBuilder.Add(aCompound, firstShape);
    aBuilder.Add(aCompound, secondShape);
    AIS_Shape *anAISShape = new AIS_Shape(aCompound);

    getContext()->Display(anAISShape, Standard_True);

    //Birlestirilen shapeleri sahneden ve shapes vectorunden siler
    getContext()->Remove(shapes[index1].shape);
    getContext()->Remove(shapes[index2].shape);
    shapes.erase(shapes.begin()+index1);
    shapes.erase(shapes.begin()+index2);

    //Oluşan shape i shapes vectorüne ekler
    Shape newShape;
    newShape.name = QString("Birlesitirilen Obje");
    newShape.shape = anAISShape;
    shapes.push_back(newShape);

    myContext->UpdateCurrentViewer();
    myView->Redraw();
}
