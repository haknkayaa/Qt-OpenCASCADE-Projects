#pragma once
#ifndef MRADSIMLINKER_H
#define MRADSIMLINKER_H

// User Libraries
#include "simulation/B4DetectorConstruction.hh"
#include "simulation/B4aActionInitialization.hh"

#ifdef G4MULTITHREADED

#include "G4MTRunManager.hh"

#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


#include <QtWidgets>

class MRADSIMLinker {
public:
    explicit MRADSIMLinker();
    ~MRADSIMLinker();

    void simulationRun(G4String macroFile);
    void sendCommand(G4String command);

    QString fileName;
    G4String macro;



};


#endif //MRADSIMLINKER_H
