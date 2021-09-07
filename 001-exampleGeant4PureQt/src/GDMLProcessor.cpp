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

    // Strip properties from file to avoid a GEANT bug
    G4cout << "Stripping extras from >" << filename << "<" << G4endl;
    QFile ifo(filename.data());


    G4GDMLParser p;
    G4cout << "Started reading (may take a while)..." << G4endl;
    p.Read(ifo.fileName().toUtf8().constData(), false);
    G4cout << "Done reading..." << G4endl;
    GeoOption g;
    g.name = G4String(filename);
    g.vol = p.GetWorldVolume();
    char buf[30];
    sprintf(buf, "-%d", 1);
    // Recursively modify volume names, because otherwise the reader
    // invokes GetVolume to scan through the list of all so-far created
    // logical/physical volumes, and picks the first one matching the
    // name
    recursiveNameAppend(g.vol, buf);
    g.suffix = buf;
    opts.push_back(g);
    G4cout << "Done converting..." << G4endl;
    p.Clear();

    MainWindow::myGDMLViewer->updateData(opts, tracks);
    QAction *temp = new QAction();
    MainWindow::myGDMLViewer->changeGeometry(temp);
    MainWindow::myGDMLViewer->changeGeometry(temp);
}