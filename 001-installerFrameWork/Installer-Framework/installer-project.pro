system(echo "Installer initing...")

##############################################
# Path
##############################################
INPUT         = $$PWD/config/config.xml $$PWD/packages
BINARYCREATOR = /opt/Qt/Tools/QtInstallerFramework/4.1/bin/binarycreator

# Application
APPLICATION_DIR_PATH = $$PWD/../Application/
CONFIG_FILE          = $$PWD/config/config.xml
PACKAGE_DIR          = $$PWD/packages

##############################################
# Settings
##############################################
TEMPLATE  = aux
INSTALLER = installer


example.input          = INPUT
example.output         = $$INSTALLER
example.commands       = $$BINARYCREATOR -c $$CONFIG_FILE -p $$PACKAGE_DIR ${QMAKE_FILE_OUT}
example.CONFIG        += target_predeps no_link combine



QMAKE_EXTRA_COMPILERS += example

OTHER_FILES            = README

system(echo "DONE!")
system(pwd)