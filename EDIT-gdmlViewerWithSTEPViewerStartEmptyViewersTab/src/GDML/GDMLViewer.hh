/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include "RenderWorker.hh"

#include <QMainWindow>
#include <QMap>

#include <vector>

class G4VPhysicalVolume;
class G4VUserDetectorConstruction;
class G4Material;

class QTableWidget;
class QTreeView;
class QDoubleSpinBox;
class QComboBox;
class QPushButton;
class QListWidget;
class QSpinBox;
class QCheckBox;
class QLabel;
class QTableView;
class ColorConfig;

class PlaneEdit;
class OverView;
class ExpoSpinBox;
class RenderWidget;
class InfoModel;
class NameSelector;
class HistogrammicRangeSlider;

typedef struct {
  G4String name;
  G4VPhysicalVolume *vol;
  G4String suffix;
} GeoOption;

class GDMLViewer : public QWidget {
  Q_OBJECT
public:
  GDMLViewer(QWidget *parent = nullptr);
  virtual ~GDMLViewer();

  void readGDML(QString path);
  void changeGeo(QString path);

public slots:
  void processKey(QKeyEvent *e);
  void processMouse(QMouseEvent *);
  void processWheel(QWheelEvent *);

  void setViewRotation(int);

private:
  std::vector<GeoOption> geo_options;
  std::vector<TrackData> track_options;
  std::vector<TrackRestriction> track_res_bounds;
  std::vector<TrackRestriction> track_res_actual;

  int which_geo = 0;
  size_t which_tracks;
  ViewData vd;
  TrackData trackdata;
  RenderWidget *rwidget;

  ColorConfig *color_config;

  QPointF clickpt;
  QPointF lastpt;
  bool clicked;
  bool shift;
};
