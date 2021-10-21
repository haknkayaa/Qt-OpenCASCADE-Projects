//
// Created by hakan on 18.10.2021.
//
#include "Simulator.h"

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


Simulator::Simulator() {

}

void Simulator::run() {

    G4String session;
#ifdef G4MULTITHREADED
    G4int nThreads = 0;
#endif


    // Detect interactive mode (if no macro provided) and define UI session
    //
//    G4UIExecutive* ui = nullptr;
//    if ( ! macro.size() ) {
//        ui = new G4UIExecutive(argc, argv, session);
//    }

    // Optionally: choose a different Random engine...
    //
    // G4Random::setTheEngine(new CLHEP::MTwistEngine);

    // Construct the default run manager
    //
#ifdef G4MULTITHREADED
    auto runManager = new G4MTRunManager;
    if (nThreads > 0) {
        runManager->SetNumberOfThreads(nThreads);
    }
#else
    auto runManager = new G4RunManager;
#endif

    // Set mandatory initialization classes
    //
    auto detConstruction = new B4DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    auto physicsList = new FTFP_BERT;
    runManager->SetUserInitialization(physicsList);

    auto actionInitialization = new B4aActionInitialization(detConstruction);
    runManager->SetUserInitialization(actionInitialization);

    // Initialize visualization
    //
    auto visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //

    // batch mode
    G4String command = "/control/execute ";
//    macro = "/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/EDIT-geant4-Qt-QProgressBar/exampleB4.in";
    UImanager->ApplyCommand(command + macro);



    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

    delete visManager;
    delete runManager;
}