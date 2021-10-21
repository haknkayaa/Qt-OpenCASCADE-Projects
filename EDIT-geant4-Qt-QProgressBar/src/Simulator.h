//
// Created by hakan on 18.10.2021.
//

#ifndef PROJECT_SIMULATOR_H
#define PROJECT_SIMULATOR_H

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

class Simulator {
public:
    Simulator();
    G4String macro;

    void run();
    void setMacro(G4String file){
        macro = file;
    }
};


#endif //PROJECT_SIMULATOR_H
