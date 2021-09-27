/* SPDX-License-Identifier: GPL-3.0-only */
#include "GDMLViewer.hh"

#include "ColorConfig.hh"
#include "CustomWidgets.hh"
#include "Navigator.hh"
#include "Overview.hh"
#include "RenderWidget.hh"
#include "Shaders.hh"

#include <G4Electron.hh>
#include <G4GDMLParser.hh>
#include <G4Gamma.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4OpticalPhoton.hh>
#include <G4ParticleDefinition.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VSolid.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisExtent.hh>

#include <QAction>
#include <QActionGroup>
#include <QCheckBox>
#include <QCollator>
#include <QComboBox>
#include <QContextMenuEvent>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QItemSelection>
#include <QLabel>
#include <QListWidget>
#include <QMenuBar>
#include <QPair>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSignalMapper>
#include <QTableView>
#include <QTableWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidgetAction>

static int exp10(int exp) {
    int r = 1;
    int base = 10;
    while (exp) {
        if (exp & 1)
            r *= base;
        exp >>= 1;
        base *= base;
    }
    return r;
}

static void includeMaterials(const G4LogicalVolume *p,
                             QSet<const G4Material *> &mtls) {
    const G4Material *mtl = p->GetMaterial();
    mtls.insert(mtl);
    for (int i = 0; i < p->GetNoDaughters(); i++) {
        includeMaterials(p->GetDaughter(i)->GetLogicalVolume(), mtls);
    }
}

std::vector<const G4Material *>
constructMaterialList(const std::vector<GeoOption> &geo_options) {
    QSet<const G4Material *> materials;
    for (const GeoOption &g: geo_options) {
        includeMaterials(g.vol->GetLogicalVolume(), materials);
    }
    QList<QPair<QString, const G4Material *>> mlist;
    for (const G4Material *g: materials) {
        mlist.append(
                QPair<QString, const G4Material *>(QString(g->GetName().data()), g));
    }
    qStableSort(mlist);
    std::vector<const G4Material *> mtl_list;
    for (const QPair<QString, const G4Material *> &p: mlist) {
        mtl_list.push_back(p.second);
    }
    return mtl_list;
}

static TrackRestriction calcTrackRestriction(const TrackData &t) {
    TrackRestriction rests;
    t.calcTimeBounds(rests.time.low, rests.time.high);
    rests.time.high += 0.01 * CLHEP::ns;
    t.calcEnergyBounds(rests.energy.low, rests.energy.high);
    rests.energy.high *= 1.2;
    rests.energy.low /= 1.2;
    rests.energy.low = std::max(1.0 * CLHEP::eV, rests.energy.low);
    rests.seqno.low = 1;
    rests.seqno.high = t.getNTracks();
    rests.ngen.low = 1;
    rests.ngen.high = t.calcMaxGenerations();
    rests.type_ids.clear();
    rests.type_visible.clear();
    const QMap<int32_t, const G4ParticleDefinition *> &m = t.calcTypes();
    for (int32_t q: m.keys()) {
        rests.type_ids.push_back(
                QPair<int32_t, const G4ParticleDefinition *>(q, m[q]));
        rests.type_visible[q] = true;
    }
    rests.types = m.values().toSet().toList().toVector();
    return rests;
}

GDMLViewer::GDMLViewer()
        : QWidget() {
    rwidget = new RenderWidget(vd, trackdata);

    rwidget->resize(400, 400);

    auto layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->addWidget(rwidget);
}

GDMLViewer::~GDMLViewer() {}


void GDMLViewer::readGDML(QString path) {

    std::vector<GeoOption> geo_options;
    std::vector<TrackData> track_options;

    G4GDMLParser p;

    G4cout << "Started reading (may take a while)..." << G4endl;

    p.Read(path.toUtf8().constData(), false);

    G4cout << "Done reading..." << G4endl;
    GeoOption g;
    g.name = G4String(path.toUtf8().constData());
    g.vol = p.GetWorldVolume();
    char buf[30];

    g.suffix = buf;
    geo_options.push_back(g);

    G4cout << "Done converting..." << G4endl;

    p.Clear();

    which_geo = 0;

    vd.orig_vol = geo_options[which_geo].vol;
    vd.octree = nullptr;
    vd.elements.clear();
    convertCreation(vd.elements, geo_options[which_geo].vol,
                    geo_options[which_geo].suffix);
    vd.scene_radius = vd.elements[0].solid->GetExtent().GetExtentRadius();
    vd.scale = vd.scene_radius / 2;
    vd.base_offset = G4ThreeVector();
    vd.base_rotation = CLHEP::HepRotation::IDENTITY;
    vd.camera = G4ThreeVector(-4 * vd.scene_radius, 0, 0);
    vd.orientation = G4RotationMatrix();
    vd.level_of_detail = 0; // 0 is full; 1 is 1/9, 2 is 1/81; depends on timing
    vd.split_by_material = true;
    vd.force_opaque = false;
    vd.navigator = nFastVolNav;
    vd.tshader = tshaderRainbow;
    vd.gshader = gshaderDefault;
    vd.voxel_base_density = 1.0;
    vd.clipping_planes = std::vector<Plane>();

    which_tracks = !track_options.empty() ? 1 : 0;

    TrackData td = which_tracks == 0 ? TrackData() : track_options[which_tracks - 1];
    for (auto &track_option: track_options) {
        TrackRestriction rests = calcTrackRestriction(track_option);
        track_res_actual.push_back(rests);
        track_res_bounds.push_back(rests);
    }
    if (which_tracks == 0) {
        trackdata = TrackData();
    } else {
        TrackRestriction current = track_res_actual[which_tracks - 1];
        trackdata = TrackData(td, vd, current);
    }

    clicked = false;
    shift = false;

    rwidget->setFocusPolicy(Qt::WheelFocus);

    connect(rwidget, SIGNAL(forwardKey(QKeyEvent * )), this,
            SLOT(processKey(QKeyEvent * )));
    connect(rwidget, SIGNAL(forwardMouse(QMouseEvent * )), this,
            SLOT(processMouse(QMouseEvent * )));
    connect(rwidget, SIGNAL(forwardWheel(QWheelEvent * )), this,
            SLOT(processWheel(QWheelEvent * )));

    std::vector<const G4Material *> mtl_list = constructMaterialList(geo_options);
    color_config = new ColorConfig(vd, mtl_list);
    color_config->reassignColors();


    this->setMinimumSize(QSize(400, 400));


}

static QPointF yflip(const QPointF &p) { return QPoint(p.x(), -p.y()); }

void GDMLViewer::processKey(QKeyEvent *event) {
    if (event->type() != QEvent::KeyPress) {
        return;
    }

    G4double mvd = 0.125;

    G4ThreeVector trans;
    G4RotationMatrix rot;
    switch (event->key()) {
        case Qt::Key_Up:
            trans = -vd.scale * vd.orientation.rowY() * mvd;
            break;
        case Qt::Key_Down:
            trans = vd.scale * vd.orientation.rowY() * mvd;
            break;
        case Qt::Key_Left:
            trans = vd.scale * vd.orientation.rowZ() * mvd;
            break;
        case Qt::Key_Right:
            trans = -vd.scale * vd.orientation.rowZ() * mvd;
            break;
            // Note: corotate the camera vector
        case Qt::Key_W:
            rot = CLHEP::HepRotationZ(atan2(vd.scale, vd.scene_radius) / 12);
            break;
        case Qt::Key_S:
            rot = CLHEP::HepRotationZ(-atan2(vd.scale, vd.scene_radius) / 12);
            break;
        case Qt::Key_A:
            rot = CLHEP::HepRotationY(-atan2(vd.scale, vd.scene_radius) / 12);
            break;
        case Qt::Key_D:
            rot = CLHEP::HepRotationY(atan2(vd.scale, vd.scene_radius) / 12);
            break;
        case Qt::Key_PageUp:
            vd.scale *= std::pow(2, 1 / 12.);
            break;
        case Qt::Key_PageDown:
            vd.scale /= std::pow(2, 1 / 12.);
            break;
        default:
            return;
    }
    vd.camera -= trans;
    vd.camera = (vd.orientation.inverse() * rot.inverse() * vd.orientation) *
                (vd.camera - vd.base_offset) +
                vd.base_offset;
    vd.orientation = rot * vd.orientation;

#if 0
    G4cout << "Scale: " << vd.scale << G4endl;
    G4cout << "Camera: " << vd.camera << G4endl;
    G4cout << "Ori: " << vd.orientation << G4endl;
#endif
    rwidget->rerender(CHANGE_VIEWPORT);

    std::vector<const G4Material *> mtl_list = constructMaterialList(geo_options);
    color_config = new ColorConfig(vd, mtl_list);
    color_config->reassignColors();
    connect(color_config, SIGNAL(colorChange()), this, SLOT(updateColors()));
}

void GDMLViewer::processMouse(QMouseEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            if (event->modifiers() & Qt::ShiftModifier) {
                shift = true;
            } else {
                shift = false;
            }
            clicked = true;
            clickpt = yflip(event->localPos());
            lastpt = clickpt;
        } else if (event->button() == Qt::RightButton) {
            // TODO: context menu
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        // or mouse exit?
        clicked = false;
    } else if (event->type() == QEvent::MouseMove) {
        // Ray tracking
        QPoint coord = rwidget->mapFromGlobal(event->globalPos());
        GridSpec grid(rwidget->width(), rwidget->height(), 1);
        QPointF pt = grid.toViewCoord(coord.x(), coord.y());

        const int M = 50;
        Intersection ints[M + 1];
        int td, nelem;
        countTree(vd.elements, 0, td, nelem);
        Q_UNUSED(td);
        Navigator *nav = Navigator::create(
                vd, vd.navigator == nFastVolNav ? nGeantNav : nFastVolNav);
        RayPoint rpt = nav->traceRay(initPoint(pt, vd),
                                     forwardDirection(vd.orientation), ints, M);
        delete nav;
        //        debugRayPoint(rpt, vd.elements);

        if (!clicked)
            return;
        int dmm = std::min(this->width(), this->height());
        if (shift) {
            // Translate with mouse
            QPointF delta = yflip(event->localPos()) - lastpt;
            QPointF dp = vd.scale * delta / dmm * 2.0;
            vd.camera -=
                    vd.orientation.rowZ() * dp.x() + vd.orientation.rowY() * dp.y();
            lastpt = yflip(event->localPos());
        } else {
            // All rotations in progress are relative to start point
            G4double step = 3.0 * atan2(vd.scale, vd.scene_radius);
            QPointF nalph = (yflip(event->localPos()) - clickpt) / dmm * step;
            QPointF palph = (lastpt - clickpt) / dmm * step;

            G4RotationMatrix next =
                    CLHEP::HepRotationY(nalph.x()) * CLHEP::HepRotationZ(-nalph.y());
            G4RotationMatrix prev =
                    CLHEP::HepRotationY(palph.x()) * CLHEP::HepRotationZ(-palph.y());

            G4RotationMatrix rot = prev.inverse() * next;
            // rotate
            vd.camera = (vd.orientation.inverse() * rot.inverse() * vd.orientation) *
                        (vd.camera - vd.base_offset) +
                        vd.base_offset;
            vd.orientation = rot * vd.orientation;
            lastpt = yflip(event->localPos());
        }
        rwidget->rerender(CHANGE_VIEWPORT);
    }
}

void GDMLViewer::processWheel(QWheelEvent *event) {
    // Argh, still not good....
    vd.scale *= std::pow(2, event->delta() / 4800.);
    if (vd.scale > 8 * vd.scene_radius) {
        vd.scale = 8 * vd.scene_radius;
    }
    if (vd.scale < vd.scene_radius / 1048576.) {
        vd.scale = vd.scene_radius / 1048576.;
    }
    rwidget->rerender(CHANGE_VIEWPORT);
}

void GDMLViewer::setViewRotation(int sel) {
    if (sel < 6) {
        // Align orientation with axis
        const CLHEP::Hep3Vector atv[6] = {
                G4ThreeVector(1, 0, 0), G4ThreeVector(0, 1, 0),
                G4ThreeVector(0, 0, 1), G4ThreeVector(-1, 0, 0),
                G4ThreeVector(0, -1, 0), G4ThreeVector(0, 0, -1)};
        const CLHEP::Hep3Vector upv[6] = {
                G4ThreeVector(0, 1, 0), G4ThreeVector(0, 0, 1),
                G4ThreeVector(1, 0, 0), G4ThreeVector(0, -1, 0),
                G4ThreeVector(0, 0, -1), G4ThreeVector(-1, 0, 0)};
        G4ThreeVector a = atv[sel];
        G4ThreeVector b = upv[sel];
        G4ThreeVector c = atv[sel].cross(upv[sel]);
        vd.orientation = CLHEP::HepRotation(a, b, c).inverse() * vd.base_rotation;
        G4ThreeVector noff(-2 * vd.scene_radius, 0, 0);
        vd.camera = vd.base_offset + vd.orientation.inverse() * noff;
    } else {
        // Rotate by 45 degrees
        G4RotationMatrix rot;
        if (sel == 6) {
            rot = CLHEP::HepRotationX(CLHEP::pi / 4);
        } else {
            rot = CLHEP::HepRotationX(-CLHEP::pi / 4);
        }
        vd.camera = (vd.orientation.inverse() * rot.inverse() * vd.orientation) *
                    (vd.camera - vd.base_offset) +
                    vd.base_offset;
        vd.orientation = rot * vd.orientation;
    }

    rwidget->rerender(CHANGE_VIEWPORT);
}
