//
// Created by hakan on 6.09.2021.
//
#include "MainWindow.h"
#include "GDMLProcessor.h"

GDMLProcessor::GDMLProcessor() {

}

void GDMLProcessor::readGDML(G4String filename) {

    bool vector = false;
    std::vector<GeoOption> opts;
    std::vector<TrackData> tracks;


    G4cout << "Started reading (may take a while)..." << filename << G4endl;

    G4GDMLParser p;
    p.Read(filename, false);

    G4cout << "Done reading." << G4endl;

    GeoOption g;
    g.name = G4String(filename);
    g.vol = p.GetWorldVolume();

    char buf[30];
    sprintf(buf, "-%d", 1);

    recursiveNameAppend(g.vol, buf);
    g.suffix = buf;

    opts.push_back(g);

    G4cout << "Done converting." << G4endl;
    p.Clear();

    G4cout << "Displaying..." << G4endl;

    MainWindow::myGDMLViewer = new ViewerGDML(opts, tracks);
    MainWindow::myGDMLViewer->openGeometry();
}