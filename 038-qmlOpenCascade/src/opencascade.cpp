#include "opencascade.h"

#include <OSD.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Selection.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_ColoredDrawer.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <gce_MakeRotation.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <TDF_Label.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopTools.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include "Geom_Axis2Placement.hxx"
#include <GCE2d_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <XCAFDoc_Area.hxx>
#include <XCAFDoc_Centroid.hxx>
#include <XCAFDoc_Datum.hxx>
#include <XCAFDoc_Dimension.hxx>
#include <XCAFDoc_Location.hxx>
#include <XCAFDoc_Volume.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>

#include <XCAFPrs_DocumentExplorer.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_AssemblyItemId.hxx>
//#include <XCAFDoc_AssemblyItemRef.hxx>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//strore the data from the stepfile.
std::vector<TopoDS_Shape> Shape;
std::vector<Handle(AIS_Shape) > ais_Shape;
std::vector<Quantity_Color> colv;

SEGMENT Segment;
std::vector<SEGMENT> SegmentVec;

Handle(XCAFDoc_ColorTool) aColorTool;
Handle(XCAFDoc_ShapeTool) aShapeTool;

Opencascade::Opencascade(QWidget *parent) : QGLWidget(parent) {
    setBackgroundRole(QPalette::NoRole);
    setMouseTracking(true);
}

void Opencascade::Readstepfile(const std::string &theStepName) {
    SegmentVec.push_back(Segment);

    STEPCAFControl_Controller::Init();

    Handle(TDocStd_Document) aDoc;
    Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
    anApp->NewDocument("MDTV-XCAF", aDoc);

    STEPCAFControl_Reader aStepReader;
    aStepReader.SetColorMode(true);
    aStepReader.SetNameMode(true);
    aStepReader.SetLayerMode(true);
    aStepReader.SetPropsMode(true);

    aStepReader.ReadFile(theStepName.c_str());
    aStepReader.Transfer(aDoc);

    TDF_Label aRootLabel = aDoc->Main();

    aShapeTool = XCAFDoc_DocumentTool::ShapeTool(aRootLabel);
    aColorTool = XCAFDoc_DocumentTool::ColorTool(aRootLabel);

    Visit(aRootLabel);

    m_view->FitAll();
}

void Opencascade::Visit(const TDF_Label &theLabel) {
    //theLabel.EntryDump(std::cout);
    Quantity_Color aColor;
    TopoDS_Shape aShape;

    Handle(TDataStd_Name) aName;
    if (theLabel.FindAttribute(TDataStd_Name::GetID(), aName)) {
        //std::cout << "  Name: " << aName->Get() << std::endl;
    }

    bool skip = 0;
    if (aShapeTool->IsShape(theLabel)) {
        //TopoDS_Shape aShape;
        //if a shape can be made from the current label, look for the color of that label:
        if (aShapeTool->GetShape(theLabel, aShape)) {
            //std::cout<<"made a shape"<<std::endl;

            if (aColorTool->GetColor(aShape, XCAFDoc_ColorSurf, aColor)) {
                skip = 0;
            } else { skip = 1; }

        }

        if (skip == 0) {

            SegmentVec.back().Ais_ShapeVec.push_back(new AIS_Shape(aShape));
            //if(aColor.Name()!=Quantity_NOC_YELLOW && aColor.Name()!=Quantity_NOC_RED){
            SegmentVec.back().Ais_ShapeVec.back()->SetColor(aColor);
            //}
            SegmentVec.back().Ais_ShapeVec.back()->SetDisplayMode(AIS_Shaded);
            SegmentVec.back().Ais_ShapeVec.back()->Attributes()->SetFaceBoundaryDraw(true);
            SegmentVec.back().Ais_ShapeVec.back()->Attributes()->SetFaceBoundaryAspect(
                    new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
            SegmentVec.back().Ais_ShapeVec.back()->Attributes()->SetIsoOnTriangulation(true);
            m_context->Display(SegmentVec.back().Ais_ShapeVec.back(), Standard_False);

        }
    }

    /*! Repeat the visit function for each childmember. */
    for (TDF_ChildIterator c(theLabel); c.More(); c.Next()) {
        Visit(c.Value());
    }
}

void Opencascade::Redraw() {
    //m_context->CurrentViewer()->Redraw();
    m_view->Redraw();
}

void Opencascade::m_initialize_context() {
    if (m_context.IsNull()) {
        Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();

        if (m_graphic_driver.IsNull()) {
            m_graphic_driver = new OpenGl_GraphicDriver(m_display_donnection);
        }

        WId window_handle = (WId) winId();

#ifdef _WIN32
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#else
        Handle(Xw_Window) wind = new Xw_Window(m_display_donnection, (Window) window_handle);
#endif
        m_viewer = new V3d_Viewer(m_graphic_driver);

        m_view = m_viewer->CreateView();
        m_view->SetWindow(wind);

        /*! Set the camera in perspective mode */
        m_view->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);

        if (!wind->IsMapped()) {
            wind->Map();
        }
        m_context = new AIS_InteractiveContext(m_viewer);

        m_viewer->SetDefaultLights();
        m_viewer->SetLightOn();

        /// This is the interactivve 3d box
        //  View : top, bottom, side, 3d, etc.

        opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
        aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
        //aisViewCube->SetFixedAnimationLoop(false);
        aisViewCube->SetDrawAxes(false);
        aisViewCube->SetSize(55);
        aisViewCube->SetFontHeight(12);
        aisViewCube->SetTransformPersistence(
                new Graphic3d_TransformPers(
                        Graphic3d_TMF_TriedronPers,
                        Aspect_TOTP_LEFT_UPPER,
                        Graphic3d_Vec2i(85, 85)));
        m_context->Display(aisViewCube, false);
        //aisViewCube->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
        //const Handle_Prs3d_DatumAspect& datumAspect = aisViewCube->Attributes()->DatumAspect();
        //datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
        //datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
        //datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
        //m_aisViewCube = aisViewCube;

        /// Set background homogenius, one color.
        //  m_view->SetBackgroundColor(Quantity_NOC_GRAY49);
        //  m_viewer->SetDefaultShadingModel(Graphic3d_TypeOfShadingModel::V3d_COLOR);

        /// Set background with gradient stylesheet
        //  Gradient sheme's for SetBgGradientColors:
        //  Aspect_GFM_CORNER1
        //  Aspect_GFM_CORNER2
        //  Aspect_GFM_CORNER3
        //  Aspect_GFM_CORNER4
        //  Aspect_GFM_DIAG1
        //  Aspect_GFM_DIAG2
        //  Aspect_GFM_HOR
        //  Aspect_GFM_NONE
        //  Aspect_GFM_VER

        Quantity_Color cola, colb;
        cola.SetValues(0.3, 0.3, 0.3, Quantity_TOC_RGB);
        colb.SetValues(0.6, 0.6, 0.6, Quantity_TOC_RGB);
        m_view->SetBgGradientColors(cola, colb, Aspect_GFM_DIAG2, false);

        /// View as wireframe or shaded
        //m_context->SetDisplayMode(AIS_WireFrame, Standard_False);
        m_context->SetDisplayMode(AIS_Shaded, Standard_False);

        Handle(Prs3d_Drawer) t_hilight_style = m_context->HighlightStyle();
        t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
        t_hilight_style->SetColor(Quantity_NOC_LIGHTYELLOW);
        t_hilight_style->SetDisplayMode(1);
        t_hilight_style->SetTransparency(0.2f);

        Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();
        t_select_style->SetMethod(Aspect_TOHM_COLOR);
        t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);
        t_select_style->SetDisplayMode(1);
        t_select_style->SetTransparency(0.4f);

        /// Show grid
        m_viewer->SetRectangularGridValues(0, 0, 1, 1, 0);
        m_viewer->SetRectangularGridGraphicValues(2.01, 2.01, 0);
        m_viewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);

        /// Show triedron. This is the 3d axis cross at the lower left of the screen.
        m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

        /*
        /// Show triedron at a specific place on the screen
        axis = new Geom_Axis2Placement(gp::XOY());
        aisTrihedron = new AIS_Trihedron(axis);
        aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
        aisTrihedron->SetDrawArrows(true);
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
        aisTrihedron->SetTransformPersistence(
                    new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location()));
        aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
        aisTrihedron->SetInfiniteState(true);
        m_context->Display(aisTrihedron,Standard_False);
        */

        m_view->MustBeResized();

        //this->CreateDemoScene();

    }
}

void Opencascade::printtext() {
    std::cout << "connection is working, this is printed from opencascade class" << std::endl;

}

void Opencascade::Set_orthographic() {
    m_view->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
    m_view->Update();
}

void Opencascade::Set_perspective() {
    m_view->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
    m_view->Redraw();
    //m_view->Update();
}

void Opencascade::paintEvent(QPaintEvent *) {
    if (m_context.IsNull()) {
        m_initialize_context();
    }
    m_view->Redraw();
}

void Opencascade::resizeEvent(QResizeEvent *) {
    if (!m_view.IsNull()) {
        m_view->MustBeResized();
    }
}

void Opencascade::mousePressEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton)) {
        m_x_max = event->x();
        m_y_max = event->y();
    } else if (event->buttons() & Qt::LeftButton) {
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_view, Standard_True);


//        auto t_pick_status = AIS_SOP_NothingSelected;
//        if (qApp->keyboardModifiers() == Qt::ControlModifier) {
//            t_pick_status = m_context->ShiftSelect(true);
//        } else {
//            t_pick_status = m_context->Select(true);
//        }
    } else if (event->buttons() & Qt::MidButton) {
        m_x_max = event->x();
        m_y_max = event->y();
        m_view->StartRotation(event->x(), event->y());
    }
}

void Opencascade::mouseReleaseEvent(QMouseEvent *event) {
    m_context->MoveTo(event->pos().x(), event->pos().y(), m_view, Standard_True);
}

void Opencascade::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton)) {
        m_view->Pan(event->pos().x() - m_x_max, m_y_max - event->pos().y());
        m_x_max = event->x();
        m_y_max = event->y();
    } else if (event->buttons() & Qt::MidButton) {
        if (qApp->keyboardModifiers() == Qt::ShiftModifier) {
            m_view->Pan(event->pos().x() - m_x_max, m_y_max - event->pos().y());
            m_x_max = event->x();
            m_y_max = event->y();
        } else {
            m_view->Rotation(event->x(), event->y());
        }
    } else {
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_view, Standard_True);
    }
}

void Opencascade::wheelEvent(QWheelEvent *event) {
    m_view->StartZoomAtPoint(event->pos().x(), event->pos().y());
    m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0);
}

void Opencascade::clear() {

    // If stepfiles are loaded, clear the vectors. (stored data buckets)
    Shape.clear();
    ais_Shape.clear();
    colv.clear();
    SegmentVec.clear();

    m_view->Redraw();
    m_context->EraseAll(true);

    std::cout << "clear" << std::endl;
}

void Opencascade::createrobotscene() {

    m_initialize_context();

    Readstepfile("config/robot/kuka_base.step");
    Readstepfile("config/robot/kuka_joint_1.step");
    Readstepfile("config/robot/kuka_joint_2.step");
    Readstepfile("config/robot/kuka_joint_3.step");
    Readstepfile("config/robot/kuka_joint_4.step");
    Readstepfile("config/robot/kuka_joint_5.step");
    Readstepfile("config/robot/kuka_joint_6.step");
    Readstepfile("config/robot/kuka_tool_cone.step");
    Readstepfile("config/robot/kuka_track.step");

}

void Opencascade::createdemoscene() {
    m_initialize_context();

    // Create a bisque cone at [0, 10, 0].
    gp_Ax2 axis2;
    axis2.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape bisqueCone = BRepPrimAPI_MakeCone(axis2, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) bisqueConeShape = new AIS_Shape(bisqueCone);
    bisqueConeShape->SetColor(Quantity_NOC_BISQUE);

    // Create a chocolate cone at [8, 10, 0].
    axis2.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape chocoCone = BRepPrimAPI_MakeCone(axis2, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) chocoConeShape = new AIS_Shape(chocoCone);
    chocoConeShape->SetColor(Quantity_NOC_CHOCOLATE);

    m_context->Display(bisqueConeShape, Standard_True);
    m_context->Display(chocoConeShape, Standard_True);

    // Fit all into the view.
    m_view->FitAll();
}
