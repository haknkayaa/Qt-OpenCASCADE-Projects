cmake_minimum_required(VERSION 3.14)
project(opencascadeProject)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

message("--> Searching source files....")
file(GLOB SOURCE_FILES src/*.cpp *.c)
message("Found: ${SOURCE_FILES}")

message("--> Searching header files....")
file(GLOB HEADER_FILES src/*.h *.hpp)
message("Found: ${HEADER_FILES}")

# OPENCASCADE COMMUNITY EDITION
find_package(OCE REQUIRED)
include_directories(${OCE_INCLUDE_DIRS})

SET(OpenCASCADE_LIBS
        TKernel;TKMath
        TKG2d;TKG3d;TKGeomBase;TKBRep
        TKGeomAlgo;TKTopAlgo;TKPrim;TKBO;TKShHealing;TKBool;TKHLR;TKFillet;TKOffset;TKFeat;TKMesh;TKXMesh
        TKService;TKV3d;TKOpenGl;TKMeshVS
        TKCDF;TKLCAF;TKCAF;TKBinL;TKXmlL;TKBin;TKXml;TKStdL;TKStd;TKTObj;TKBinTObj;TKXmlTObj;TKVCAF
        TKXSBase;TKSTEPBase;TKSTEPAttr;TKSTEP209;TKSTEP;TKIGES;TKXCAF;TKXDEIGES;TKXDESTEP;TKSTL;TKVRML;TKXmlXCAF;TKBinXCAF;TKRWMesh
        TKDraw;TKTopTest;TKViewerTest;TKXSDRAW;TKDCAF;TKXDEDRAW;TKTObjDRAW;TKQADraw;
        -lGL -lGLU #-lglut
        TKernel TKMath TKSTL TKSTEP TKV3d TKOpenGl
        )

# OPENGL
find_package(OpenGL REQUIRED)

#QT5
find_package(Qt5 COMPONENTS Core Widgets Gui Xml OpenGL REQUIRED)
find_package(Qt5Core REQUIRED NO_DEFAULT_PATH)
find_package(Qt5Widgets REQUIRED NO_DEFAULT_PATH)
find_package(Qt5Gui REQUIRED NO_DEFAULT_PATH)


message("--> Printing QT5 Modules")
message("Qt5               : " ${Qt5_DIR})
message("Qt5Core           : " ${Qt5Core_DIR})
message("Qt5Widgets        : " ${Qt5Widgets_DIR})
message("Qt5Gui            : " ${Qt5Gui_DIR})
message("Qt5OpenGL         : " ${Qt5OpenGL_DIR})

message("--> Printing Qt5 Libraries")
get_target_property(QtCore_location Qt5::Core LOCATION)
message("Qt5::Core Lib    : ${QtCore_location}")
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
message("Qt5::Widgets Lib : ${QtWidgets_location}")
get_target_property(QtGui_location Qt5::Gui LOCATION)
message("Qt5::Gui Lib     : ${QtGui_location}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCASCADE_LIBS} Qt5::Core Qt5::Widgets Qt5::Xml )


