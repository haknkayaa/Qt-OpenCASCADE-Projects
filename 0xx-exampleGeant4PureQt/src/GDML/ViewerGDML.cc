/* SPDX-License-Identifier: GPL-3.0-only */
#include "ViewerGDML.hh"

#include "ColorConfig.hh"
#include "CustomWidgets.hh"
#include "Navigator.hh"
#include "Overview.hh"
#include "RenderWidget.hh"
#include "Shaders.hh"
#include "VectorTrace.hh"
#include "VectorWindow.hh"

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
    for (const GeoOption &g : geo_options) {
        includeMaterials(g.vol->GetLogicalVolume(), materials);
    }
    QList<QPair<QString, const G4Material *>> mlist;
    for (const G4Material *g : materials) {
        mlist.append(QPair<QString, const G4Material *>(
                QString(g->GetName().data()), g));
    }
    qStableSort(mlist);
    std::vector<const G4Material *> mtl_list;
    for (const QPair<QString, const G4Material *> &p : mlist) {
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
    for (int32_t q : m.keys()) {
        rests.type_ids.push_back(
                QPair<int32_t, const G4ParticleDefinition *>(q, m[q]));
        rests.type_visible[q] = true;
    }
    rests.types = m.values().toSet().toList().toVector();
    return rests;
}

ViewerGDML::ViewerGDML(const std::vector<GeoOption> &options,
                       const std::vector<TrackData> &trackopts)
        : QWidget() {

    which_geo = 0;
    geo_options = options;
    track_options = trackopts;
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
    which_tracks = track_options.size() > 0 ? 1 : 0;

    TrackData td =
            which_tracks == 0 ? TrackData() : track_options[which_tracks - 1];
    for (size_t i = 0; i < track_options.size(); i++) {
        TrackRestriction rests = calcTrackRestriction(track_options[i]);
        track_res_actual.push_back(rests);
        track_res_bounds.push_back(rests);
    }
    if (which_tracks == 0) {
        trackdata = TrackData();
    } else {
        TrackRestriction current = track_res_actual[which_tracks - 1];
        trackdata = TrackData(td, vd, current);
    }
    rayiter = 0;


    clicked = false;
    shift = false;


    QVBoxLayout *layout = new QVBoxLayout(this);


    rwidget = new RenderWidget(vd, trackdata);
    //    this->setCentralWidget(rwidget);
    layout->addWidget(rwidget);

    rwidget->setFocusPolicy(Qt::WheelFocus);
    connect(rwidget, SIGNAL(frameTime(qreal)), this,
            SLOT(showFrameTime(qreal)));
    connect(rwidget, SIGNAL(forwardKey(QKeyEvent * )), this,
            SLOT(processKey(QKeyEvent * )));
    connect(rwidget, SIGNAL(forwardMouse(QMouseEvent * )), this,
            SLOT(processMouse(QMouseEvent * )));
    connect(rwidget, SIGNAL(forwardWheel(QWheelEvent * )), this,
            SLOT(processWheel(QWheelEvent * )));
    connect(rwidget, SIGNAL(forwardContextMenu(QContextMenuEvent * )), this,
            SLOT(processContextMenu(QContextMenuEvent * )));
    connect(rwidget, SIGNAL(forwardResize(QResizeEvent * )), this,
            SLOT(processResize(QResizeEvent * )));

    rwidget->rerender(CHANGE_VIEWPORT);

    std::vector<const G4Material *> mtl_list = constructMaterialList(geo_options);

    color_config->mergeMaterials(mtl_list);
    int cr_change = color_config->reassignColors();
//    tree_model->recalculate();
//    tree_view->collapseAll();
//    tree_view->expandToDepth(1);
    indicateElement(NULL);
    rwidget->rerender(CHANGE_GEO | cr_change);
}


ViewerGDML::~ViewerGDML() {}

void ViewerGDML::showFrameTime(qreal t) {
//    frame_time_display->setText(
//            QString("%1 ms").arg(t * 1e3, 7, 'f', 1, QChar(0x2007)));
}

void ViewerGDML::restClip() { dock_clip->setVisible(true); }

void ViewerGDML::restTree() { dock_tree->setVisible(true); }

void ViewerGDML::restInfo() { dock_info->setVisible(true); }

void ViewerGDML::restRay() { dock_ray->setVisible(true); }

void ViewerGDML::restColor() { dock_color->setVisible(true); }

void ViewerGDML::restOrient() { dock_orient->setVisible(true); }

static QPointF yflip(const QPointF &p) { return QPoint(p.x(), -p.y()); }

void ViewerGDML::processKey(QKeyEvent *event) {
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
}

void ViewerGDML::processMouse(QMouseEvent *event) {
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

            G4RotationMatrix next = CLHEP::HepRotationY(nalph.x()) *
                                    CLHEP::HepRotationZ(-nalph.y());
            G4RotationMatrix prev = CLHEP::HepRotationY(palph.x()) *
                                    CLHEP::HepRotationZ(-palph.y());

            G4RotationMatrix rot = prev.inverse() * next;
            // rotate
            vd.camera =
                    (vd.orientation.inverse() * rot.inverse() * vd.orientation) *
                    (vd.camera - vd.base_offset) +
                    vd.base_offset;
            vd.orientation = rot * vd.orientation;
            lastpt = yflip(event->localPos());
        }
        rwidget->rerender(CHANGE_VIEWPORT);
    }
}

void ViewerGDML::processWheel(QWheelEvent *event) {
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

void ViewerGDML::processContextMenu(QContextMenuEvent *event) {
//    QMenu menu(this);
//    menu.addMenu(tpicker_menu);
//    menu.addMenu(gpicker_menu);
//    menu.addMenu(view_menu);
//    menu.addMenu(screenshot_menu);
//    menu.exec(event->globalPos());
}

void ViewerGDML::processResize(QResizeEvent *event) {
//    screen_action->setText(QStringLiteral("Screenshot (%1 x %2)")
//                                   .arg(event->size().width())
//                                   .arg(event->size().height()));
}

Plane plane_transform(const Plane &p, const G4RotationMatrix &rot,
                      const G4ThreeVector &offset) {
    Plane q;
    q.normal = rot * p.normal;
    q.offset = p.offset - p.normal * offset;
    return q;
}

Plane plane_itransform(const Plane &p, const G4RotationMatrix &rot,
                       const G4ThreeVector &offset) {
    Plane q;
    q.normal = rot.inverse() * p.normal;
    q.offset = p.offset + q.normal * offset;
    return q;
}

void ViewerGDML::updatePlanes() {
    vd.clipping_planes = std::vector<Plane>();
    for (int i = 0; i < 3; i++) {
        Plane p = plane_edit[i]->getPlane();
        if (p.normal != G4ThreeVector()) {
            Plane q = plane_itransform(p, vd.base_rotation, vd.base_offset);
            vd.clipping_planes.push_back(q);
        }
    }
    updateTracks(true);
}

void ViewerGDML::updateTracks(bool planes_also_changed) {
    if (which_tracks > 0) {
        double tlow, thigh;
        times_range->value(tlow, thigh);
        tlow *= CLHEP::ns;
        thigh *= CLHEP::ns;
        double elow, ehigh;
        energy_range->value(elow, ehigh);
        elow *= CLHEP::eV;
        ehigh *= CLHEP::eV;
        size_t nlow = size_t(count_lower->value());
        size_t nhigh = size_t(count_upper->value());
        size_t glow = size_t(nanc_lower->value());
        size_t ghigh = size_t(nanc_upper->value());

        TrackRestriction &res = track_res_actual[which_tracks - 1];
        res.energy = {std::min(elow, ehigh), std::max(elow, ehigh)};
        res.time = {std::min(tlow, thigh), std::max(tlow, thigh)};
        res.seqno = {std::min(nlow, nhigh), std::max(nlow, nhigh)};
        res.ngen = {std::min(glow, ghigh), std::max(glow, ghigh)};

        for (int i = 0; i < res.types.size(); i++) {
            QListWidgetItem *it = line_type_selection->item(i);
            bool visible = it->checkState() == Qt::Checked;
            const G4ParticleDefinition *pdef =
                    (const G4ParticleDefinition *) it->data(Qt::UserRole)
                            .toULongLong();
            for (int j = 0; j < res.type_ids.size(); j++) {
                if (res.type_ids[j].second == pdef) {
                    res.type_visible[res.type_ids[j].first] = visible;
                }
            }
        }

        const TrackData &base_track_data = track_options[which_tracks - 1];
        trackdata = TrackData(base_track_data, vd, res);
        if (0) {
            // Temp disabled on grounds of lag
            QVector<QPointF> ep, tp;
            base_track_data.constructRangeHistograms(tp, ep, res.time,
                                                     res.energy);
            energy_range->setHistogram(ep);
            times_range->setHistogram(tp);
        }
        linecount_label->setText(QString("Tracks: %1/%2")
                                         .arg(trackdata.getNTracks())
                                         .arg(base_track_data.getNTracks()));
    } else {
        // Q: how to pull QSharedData on the Elements as well
        trackdata = TrackData();
    }

    if (planes_also_changed) {
        rwidget->rerender(CHANGE_VIEWPORT);
    } else {
        rwidget->rerender(CHANGE_TRACK);
    }
}

void ViewerGDML::updateColors() {
    // ColorConfig handles ViewData updates
    int render_change = color_config->reassignColors();
    info_model->setElement(info_model->curE(), vd);
    rwidget->rerender(render_change);
}

void ViewerGDML::changeGeometry(QAction *act) {
    QString s = act->toolTip();
    G4String s2(s.toUtf8().constData());
    for (size_t i = 0; i < geo_options.size(); i++) {
        if (s2 == geo_options[i].name) {
            if (which_geo != i) {
                // Change geometry
                which_geo = i;
                vd.orig_vol = geo_options[which_geo].vol;
                vd.elements.clear();
                convertCreation(vd.elements, geo_options[which_geo].vol,
                                geo_options[which_geo].suffix);
                vd.scene_radius =
                        vd.elements[0].solid->GetExtent().GetExtentRadius();
                if (4 * vd.scene_radius > vd.camera.mag()) {
                    vd.camera *= 4 * vd.scene_radius / vd.camera.mag();
                }
                // Reset pivot list
                QString old_selection_name = pivot_volume->currentText();
                pivot_volume->blockSignals(true);
                pivot_volume->clear();
                for (const Element &e : vd.elements) {
                    pivot_volume->addItem(QString::fromUtf8(e.name.c_str()),
                                          e.ecode);
                }
                pivot_volume->setCurrentIndex(0);
                for (int k = 0; k < pivot_volume->count(); k++) {
                    if (pivot_volume->itemText(k) == old_selection_name) {
                        pivot_volume->setCurrentIndex(k);
                        break;
                    }
                }
                pivot_volume->blockSignals(false);

                std::vector<const G4Material *> mtl_list =
                        constructMaterialList(geo_options);
                color_config->mergeMaterials(mtl_list);
                int cr_change = color_config->reassignColors();
                tree_model->recalculate();
                tree_view->collapseAll();
                tree_view->expandToDepth(1);
                indicateElement(NULL);
                rwidget->rerender(CHANGE_GEO | cr_change);
                return;
            }
        }
    }
}

void ViewerGDML::changeTracks(QAction *act) {
    const QList<QAction *> &o = act->actionGroup()->actions();
    size_t c = 0;
    for (QAction *q : o) {
        if (q == act) {
            break;
        }
        c++;
    }
    which_tracks = c;
    bool active = which_tracks > 0;
    if (active) {
        times_range->blockSignals(true);
        energy_range->blockSignals(true);
        count_lower->blockSignals(true);
        count_upper->blockSignals(true);
        nanc_lower->blockSignals(true);
        nanc_upper->blockSignals(true);

        const TrackRestriction &res = track_res_bounds[which_tracks - 1];
        const TrackRestriction &cur = track_res_actual[which_tracks - 1];
        times_range->setRange(res.time.low / CLHEP::ns,
                              res.time.high / CLHEP::ns);
        times_range->setValue(cur.time.low / CLHEP::ns,
                              cur.time.high / CLHEP::ns);
        energy_range->setRange(res.energy.low / CLHEP::eV,
                               res.energy.high / CLHEP::eV);
        energy_range->setValue(cur.energy.low / CLHEP::eV,
                               cur.energy.high / CLHEP::eV);
        count_lower->setRange(1, int(res.seqno.high));
        count_upper->setRange(1, int(res.seqno.high));
        count_lower->setValue(int(cur.seqno.low));
        count_upper->setValue(int(cur.seqno.high));
        int step = exp10(
                std::max(0, int(std::floor(std::log10(res.seqno.high / 15.)))));
        count_lower->setSingleStep(step);
        count_upper->setSingleStep(step);

        nanc_lower->setRange(res.ngen.low, res.ngen.high);
        nanc_upper->setRange(res.ngen.low, res.ngen.high);
        nanc_lower->setValue(cur.ngen.low);
        nanc_upper->setValue(cur.ngen.high);

        times_range->blockSignals(false);
        energy_range->blockSignals(false);
        count_lower->blockSignals(false);
        count_upper->blockSignals(false);
        nanc_lower->blockSignals(false);
        nanc_upper->blockSignals(false);

        line_type_selection->clear();
        reloadLineTypeSelection();
    }
    times_range->setEnabled(active);
    energy_range->setEnabled(active);
    count_lower->setEnabled(active);
    count_upper->setEnabled(active);
    nanc_lower->setDisabled(active);
    nanc_upper->setDisabled(active);
    updateTracks(false);
}

void ViewerGDML::indicateElement(const Element *e) {
    info_model->setElement(e, vd);
}

void ViewerGDML::rayLookup() {
    QList<QListWidgetItem *> li = ray_table->selectedItems();
    if (li.size() != 1) {
        return;
    }
    int r = ray_table->row(li[0]);
    // e may be null if for special placeholder slot
    const Element *e = ray_list[r];
    if (e) {
        QModelIndex index = tree_model->indexFromElement(e);
        tree_view->scrollTo(index, QAbstractItemView::PositionAtCenter);
        tree_view->selectionModel()->clearSelection();
        tree_view->selectionModel()->select(index, QItemSelectionModel::Select);
    }
    // ^ Triggers indicateElement
}

void ViewerGDML::updateNavigator() {
    vd.navigator = navig_sel->currentData().toInt();
    rwidget->rerender(CHANGE_GEO);
}

void ViewerGDML::updateGShader() {
    vd.gshader = gshader_sel->currentData().toInt();
    rwidget->rerender(CHANGE_COLOR);
}

void ViewerGDML::updateTShader() {
    vd.tshader = tshader_sel->currentData().toInt();
    rwidget->rerender(CHANGE_TRACK);
}

void ViewerGDML::updateVoxDens() {
    vd.voxel_base_density = std::exp2(vox_density->value());
    rwidget->rerender(CHANGE_COLOR);
}

void ViewerGDML::updateShowLines() {
    rwidget->setFullDetail(mtl_showlines->isChecked());
}

void ViewerGDML::screenshot(int sx) {
    int w = rwidget->width() * sx, h = rwidget->height() * sx;
    RenderSaveObject *rso = new RenderSaveObject(vd, trackdata, w, h);
    rso->start();
}

void ViewerGDML::vectorScreenshot() {
    QString name = "vector.svg";
    VectorTracer *vt = new VectorTracer(vd, trackdata, name);
    vt->reset(QSize(1000, 1000), name);
    vt->renderFull();
    delete vt;
}

void ViewerGDML::vectorPreview() {
    VectorPreview *vp = new VectorPreview(vd, trackdata);
    vp->show();
}


void ViewerGDML::reloadLineTypeSelection() {
    line_type_selection->blockSignals(true);

    if (which_tracks > 0) {
        const TrackRestriction &r = track_res_actual[which_tracks - 1];
        for (const G4ParticleDefinition *p : r.types) {
            QListWidgetItem *lwi =
                    new QListWidgetItem(QString(p->GetParticleName().c_str()));
            lwi->setFlags(lwi->flags() | Qt::ItemIsUserCheckable);
            bool visible = false;
            for (QPair<int32_t, const G4ParticleDefinition *> q : r.type_ids) {
                if (q.second == p && r.type_visible[q.first]) {
                    visible = true;
                }
            }
            lwi->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
            lwi->setData(Qt::UserRole, QVariant((qulonglong) p));
            line_type_selection->addItem(lwi);
        }
    }
    line_type_selection->blockSignals(false);
}

void recursiveNameAppend(G4VPhysicalVolume *vp, const char *suffix) {
    G4String name = vp->GetName() + suffix;
    vp->SetName(name);
    vp->GetLogicalVolume()->SetName(name);
    int nd = vp->GetLogicalVolume()->GetNoDaughters();
    for (G4int i = 0; i < nd; i++) {
        recursiveNameAppend(vp->GetLogicalVolume()->GetDaughter(i), suffix);
    }
}

void ViewerGDML::openGeometry() {
    QString selected = "GDML (*.gdml *.gdml.gz)";
    QString fn = QFileDialog::getOpenFileName(
            NULL, "Open Geometry", QDir::currentPath(),
            "All files (*.*);;GDML (*.gdml *.gdml.gz)", &selected);
    if (fn.size() >= 8 && fn.right(8) == ".gdml.gz") {
        system("rm -f /tmp/copy.gdml.gz");
        QString ar = "cp " + fn + " /tmp/copy.gdml.gz";
        system(ar.toUtf8().constData());
        QString gu = "gzip -df /tmp/copy.gdml.gz";
        system(gu.toUtf8().constData());
        fn = "/tmp/copy.gdml";
    }
    if (fn.size() >= 5 && fn.right(5) == ".gdml") {

        G4GDMLParser p;
        p.SetAddPointerToName(true);
        G4cout << "Started reading (may take a while)..." << G4endl;
        GeoOption g;
        g.name = G4String(fn.toUtf8().constData());
        p.Read(g.name, false);
        G4cout << "Done reading..." << G4endl;
        g.vol = p.GetWorldVolume();
        char buf[30];
        sprintf(buf, "-%d", int(geo_options.size() + 1));
        // Recursively modify volume names, because otherwise the reader
        // invokes GetVolume to scan through the list of all so-far created
        // logical/physical volumes, and picks the first one matching the
        // name
        recursiveNameAppend(g.vol, buf);
        g.suffix = buf;
        geo_options.push_back(g);
        G4cout << "Done converting..." << G4endl;
        p.Clear();

    } else {
        qDebug("File not a GDML file!");
    }
}

void ViewerGDML::openTracks() {
    QString selected = "Track data (*.dat *.dat.gz *.track *.track.gz)";
    QString fn =
            QFileDialog::getOpenFileName(NULL, "Open Tracks", QDir::currentPath(),
                                         "All files (*.*);;" + selected, &selected);
    if (fn.size() >= 3 && fn.right(3) == ".gz") {
        system("rm -f /tmp/copy.dat.gz");
        QString ar = "cp " + fn + " /tmp/copy.dat.gz";
        system(ar.toUtf8().constData());
        QString gu = "gzip -df /tmp/copy.dat.gz";
        system(gu.toUtf8().constData());
        fn = "/tmp/copy.dat";
    }
    if ((fn.size() >= 4 && fn.right(4) == ".dat") ||
        (fn.size() >= 6 && fn.right(4) == ".track")) {
        TrackData nxt = TrackData(fn.toUtf8().constData());
        TrackRestriction rests = calcTrackRestriction(nxt);
        track_res_actual.push_back(rests);
        track_res_bounds.push_back(rests);
        track_options.push_back(nxt);
    } else {
        qDebug("File not a track data file!");
    }
}

void ViewerGDML::setViewRotation(int sel) {
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
        vd.orientation =
                CLHEP::HepRotation(a, b, c).inverse() * vd.base_rotation;
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
        vd.camera =
                (vd.orientation.inverse() * rot.inverse() * vd.orientation) *
                (vd.camera - vd.base_offset) +
                vd.base_offset;
        vd.orientation = rot * vd.orientation;
    }

    rwidget->rerender(CHANGE_VIEWPORT);
}

void ViewerGDML::updatePivot() {
    G4ThreeVector old_offset = vd.base_offset;
    G4RotationMatrix old_rotation = vd.base_rotation;
    const Element &e = vd.elements[pivot_volume->currentData().toInt()];
    vd.base_offset = -e.global_offset;
    vd.base_rotation = e.rot;
    if (vd.base_offset != old_offset || vd.base_rotation != old_rotation) {
        for (int i = 0; i < 3; i++) {
            if (plane_edit[i]->isActive()) {
                Plane p = plane_edit[i]->getPlane();
                Plane q = plane_transform(
                        plane_itransform(p, old_rotation, old_offset),
                        vd.base_rotation, vd.base_offset);
                plane_edit[i]->setPlane(q);
            }
        }
    }
}
