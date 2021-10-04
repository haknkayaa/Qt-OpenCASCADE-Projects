
#!/bin/sh

BASE_DIR=/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/001-installerFrameWork/Installer-Framework/Deployer
export LD_LIBRARY_PATH=/lib/:::/opt/Qt/5.15.2/gcc_64/
export LD_LIBRARY_PATH=/lib/systemLibs:::/opt/Qt/5.15.2/gcc_64/
export QML_IMPORT_PATH=/qml/:
export QML2_IMPORT_PATH=/qml/:
export QT_PLUGIN_PATH=/plugins/:
export QTWEBENGINEPROCESS_PATH=/bin//QtWebEngineProcess
export QTDIR=
export CQT_PKG_ROOT=
export CQT_RUN_FILE=/mradsim-gui.sh
export G4NEUTRONHPDATA=/Data/G4NDL4.6
export G4LEDATA=/Data/G4EMLOW7.9.1
export G4LEVELGAMMADATA=/Data/PhotonEvaporation5.5
export G4RADIOACTIVEDATA=/Data/RadioactiveDecay5.4
export G4PARTICLEXSDATA=/Data/G4PARTICLEXS2.1
export G4PIIDATA=/Data/G4PII1.3
export G4REALSURFACEDATA=/Data/RealSurface2.1.1
export G4SAIDXSDATA=/Data/G4SAIDDATA2.0
export G4ABLADATA=/Data/G4ABLA3.1
export G4INCLDATA=/Data/G4INCL1.0
export G4ENSDFSTATEDATA=/Data/G4ENSDFSTATE2.2
export QT_QPA_PLATFORM_PLUGIN_PATH=/plugins//platforms:



"$BASE_DIR/bin/mradsim-gui" "$@"

