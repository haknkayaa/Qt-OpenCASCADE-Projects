configurations(){
  VERSION="0.0.1"
  BINARY_FILE="/home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/001-installerFrameWork/Application/cmake-build-debug/opencascadeProject"
  OUTPUT_DIR="./InstallerPack-$VERSION"
  OUTPUT_EXE_FILE="installer"
  QMAKE_FILE="installer.pro"
  INPUT="$PWD/config/config.xml $PWD/packages"
  BINARYCREATOR="/opt/Qt/Tools/QtInstallerFramework/4.1/bin/binarycreator"
  CONFIG_FILE="$PWD/config/config.xml"
  PACKAGE_DIR="$PWD/packages"
}

create_file_dir(){
  echo "-> Paketleme için kullanılacak klasör yapısı yaratılıyor..."

  # installerpack folder
  mkdir $OUTPUT_DIR

  # packages folder
  mkdir $PACKAGE_DIR

  # for ubuntu 18
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/"
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/data"
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/data/bin"
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/data/lib"
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/data/Data"
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/data/plugins"

  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/meta"

  echo """
#!/bin/sh

BASE_DIR=$(dirname "$(readlink -f "$0")")
export LD_LIBRARY_PATH="$BASE_DIR"/lib/:"$BASE_DIR":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$BASE_DIR"/lib/systemLibs:"$BASE_DIR":$LD_LIBRARY_PATH
export QML_IMPORT_PATH="$BASE_DIR"/qml/:$QML_IMPORT_PATH
export QML2_IMPORT_PATH="$BASE_DIR"/qml/:$QML2_IMPORT_PATH
export QT_PLUGIN_PATH="$BASE_DIR"/plugins/:$QT_PLUGIN_PATH
export QTWEBENGINEPROCESS_PATH="$BASE_DIR"/bin//QtWebEngineProcess
export QTDIR="$BASE_DIR"
export CQT_PKG_ROOT="$BASE_DIR"
export CQT_RUN_FILE="$BASE_DIR/mradsim-gui.sh"
export G4NEUTRONHPDATA="$BASE_DIR/Data/G4NDL4.6"
export G4LEDATA="$BASE_DIR/Data/G4EMLOW7.9.1"
export G4LEVELGAMMADATA="$BASE_DIR/Data/PhotonEvaporation5.5"
export G4RADIOACTIVEDATA="$BASE_DIR/Data/RadioactiveDecay5.4"
export G4PARTICLEXSDATA="$BASE_DIR/Data/G4PARTICLEXS2.1"
export G4PIIDATA="$BASE_DIR/Data/G4PII1.3"
export G4REALSURFACEDATA="$BASE_DIR/Data/RealSurface2.1.1"
export G4SAIDXSDATA="$BASE_DIR/Data/G4SAIDDATA2.0"
export G4ABLADATA="$BASE_DIR/Data/G4ABLA3.1"
export G4INCLDATA="$BASE_DIR/Data/G4INCL1.0"
export G4ENSDFSTATEDATA="$BASE_DIR/Data/G4ENSDFSTATE2.2"
export QT_QPA_PLATFORM_PLUGIN_PATH="$BASE_DIR"/plugins//platforms:$QT_QPA_PLATFORM_PLUGIN_PATH



\"\$BASE_DIR/bin/mradsim-gui\" \"\$@\"
""" > $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu18/data/"mradsim-gui.sh


  # for ubuntu 20
  mkdir $PACKAGE_DIR"/com.IRADETS.MRADSIM_ubuntu20/"


}

create_qmake_file(){
  echo "-> $QMAKE_FILE dosyası yaratılıyor..."

  echo "# installer.pro "> $QMAKE_FILE
  echo "# Automatic generated ("`date +%Y-%m-%d_%H-%M-%S`")" >> $QMAKE_FILE

  echo >> $QMAKE_FILE

  echo "TEMPLATE  = aux" >> $QMAKE_FILE
  echo "INSTALLER = $OUTPUT_EXE_FILE" >> $QMAKE_FILE

  echo >> $QMAKE_FILE

  echo "example.input    = $INPUT" >> $QMAKE_FILE
  echo "example.output   = $OUTPUT_EXE_FILE" >> $QMAKE_FILE
  echo "example.commands = $BINARYCREATOR -c $CONFIG_FILE -p $PACKAGE_DIR \${QMAKE_FILE_OUT}" >> $QMAKE_FILE
  echo "example.CONFIG  += target_predeps no_link combine" >> $QMAKE_FILE

  echo >> $QMAKE_FILE

  echo "QMAKE_EXTRA_COMPILERS += example" >> $QMAKE_FILE

  echo "Done."

}

compile_qmake_file(){
  echo "-> $QMAKE_FILE dosyası derleniyor..."

  qmake $QMAKE_FILE

  echo "Done."
}

main(){
  echo "Starting ..."

  # settings global enviroment
  configurations

  create_file_dir

#  create_qmake_file
#  compile_qmake_file
}

main