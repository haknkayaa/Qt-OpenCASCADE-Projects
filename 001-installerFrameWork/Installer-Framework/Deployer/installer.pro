# installer.pro 
# Automatic generated (2021-10-01_17-09-24)

TEMPLATE  = aux
INSTALLER = installer

example.input    = /home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/001-installerFrameWork/Installer-Framework/Deployer/config/config.xml /home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/001-installerFrameWork/Installer-Framework/Deployer/packages
example.output   = installer
example.commands = /opt/Qt/Tools/QtInstallerFramework/4.1/bin/binarycreator -c /home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/001-installerFrameWork/Installer-Framework/Deployer/config/config.xml -p /home/hakan/CLionProjects/Qt-OpenCASCADE-Projects/001-installerFrameWork/Installer-Framework/Deployer/packages ${QMAKE_FILE_OUT}
example.CONFIG  += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += example
