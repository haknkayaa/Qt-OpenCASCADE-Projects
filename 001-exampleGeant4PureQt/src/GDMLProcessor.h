//
// Created by hakan on 6.09.2021.
//

#ifndef PROJECT_GDMLPROCESSOR_H
#define PROJECT_GDMLPROCESSOR_H

#include <QtWidgets>
#include "GDML/VectorWindow.hh"
#include "GDML/ViewerGDML.hh"

#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QTemporaryFile>
#include <QTextStream>
#include <QTime>

#include <G4GDMLParser.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4PhysicalVolumeStore.hh>

class GDMLProcessor {

public:
    GDMLProcessor();

    void readGDML(G4String filename);


};


#endif //PROJECT_GDMLPROCESSOR_H
