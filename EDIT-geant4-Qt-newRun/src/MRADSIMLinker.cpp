#include "MRADSIMLinker.h"
#include "MainWindow.h"

MRADSIMLinker::MRADSIMLinker() {
    G4cout << "Mradsim Start Linking...";

//    G4String session;
//#ifdef G4MULTITHREADED
//    G4int nThreads = 0;
//#endif
//
//
//#ifdef G4MULTITHREADED
//    auto runManager = new G4MTRunManager;
//    if (nThreads > 0) {
//        runManager->SetNumberOfThreads(nThreads);
//    }
//#else
////    auto runManager = new G4RunManager;
//#endif

//    auto detConstruction = new B4DetectorConstruction();
//    runManager->SetUserInitialization(detConstruction);
//
//    auto physicsList = new FTFP_BERT;
//    runManager->SetUserInitialization(physicsList);
//
//    auto actionInitialization = new B4aActionInitialization(detConstruction);
//    runManager->SetUserInitialization(actionInitialization);

//    auto visManager = new G4VisExecutive;
//    visManager->Initialize();


}

MRADSIMLinker::~MRADSIMLinker() {
//    delete this;
}

void MRADSIMLinker::simulationRun(G4String macroFile) {

    UImanager = G4UImanager::GetUIpointer();

    G4String session;
#ifdef G4MULTITHREADED
    G4int nThreads = 0;
#endif


#ifdef G4MULTITHREADED
    auto runManager = new G4MTRunManager;
    if (nThreads > 0) {
        runManager->SetNumberOfThreads(nThreads);
    }
#else
    auto runManager = new G4RunManager;
#endif

    auto detConstruction = new B4DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    auto physicsList = new FTFP_BERT;
    runManager->SetUserInitialization(physicsList);

    auto actionInitialization = new B4aActionInitialization(detConstruction);
    runManager->SetUserInitialization(actionInitialization);

    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
//    MainWindow::UImanager = G4UImanager::GetUIpointer();

    // batch mode
    G4String command = "/control/execute ";
//    macro = "exampleB4.in";
    UImanager->ApplyCommand(command + macroFile);

    delete visManager;
    delete runManager;
}

void MRADSIMLinker::sendCommand(G4String command){
    UImanager->ApplyCommand(command);
}


