cmake_minimum_required(VERSION 3.2)

project(QMLProject)

# CMake Configuration ------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")

#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# End CMake Configuration --------------------------------------------

# Source Files ------------------------------------------------
message("--> Searching source files....")
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
message("Found: ${SOURCE_FILES}")
# End Source Files --------------------------------------------

# Header Files ------------------------------------------------
message("--> Searching header files....")
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)
message("Found: ${HEADER_FILES}")
# End Header Files --------------------------------------------

# Resource Files -----------------------------------------------
message("--> Searching resource files...")
file(GLOB RESOURCE_FILES ${PROJECT_SOURCE_DIR}/resource/*.qrc)
message("Found: ${RESOURCE_FILES}")
# End Resource Files -------------------------------------------

# Qt5 Package --------------------------------------------------
find_package(Qt5 COMPONENTS Widgets Qml Quick OpenGL REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
# End Qt5 Package ---------------------------------------------

#-------------------------------------------------------------------
option(USE_OCE "Use OpenCASCADE Community Edition " OFF)

if (USE_OCE)
    # OPENCASCADE COMMUNITY EDITION
    set(OCE_DIR /usr/lib/x86_64-linux-gnu/oce-0.17)
    find_package(OCE REQUIRED)
    include_directories(${OCE_INCLUDE_DIRS})
    SET(OpenCASCADE_LIBRARIES
            TKernel;TKMath
            TKG2d;TKG3d;TKGeomBase;TKBRep
            TKGeomAlgo;TKTopAlgo;TKPrim;TKBO;TKShHealing;TKBool;TKHLR;TKFillet;TKOffset;TKFeat;TKMesh;TKXMesh
            TKService;TKV3d;TKOpenGl;TKMeshVS
            TKCDF;TKLCAF;TKCAF;TKBinL;TKXmlL;TKBin;TKXml;TKTObj;TKBinTObj;TKXmlTObj #;TKVCAF #TKStdL;TKStd;
            TKXSBase;TKSTEPBase;TKSTEPAttr;TKSTEP209;TKSTEP;TKIGES;TKXCAF;TKXDEIGES;TKXDESTEP;TKSTL;TKVRML;TKXmlXCAF;TKBinXCAF; #TKRWMesh
            #TKDraw;TKTopTest;TKViewerTest;TKXSDRAW;TKDCAF;TKXDEDRAW;TKTObjDRAW;TKQADraw;
            -lGL -lGLU #-lglut
            TKernel TKMath TKSTL TKSTEP TKV3d TKOpenGl
            )
else()
    ### OpenCASCADE Official Edition
    add_definitions(-DOCCT)
    if(WIN32)
        find_path(OpenCASCADE_DIR OpenCASCADEConfig.cmake
                C:\\Dev\\OpenCASCADE\\OpenCASCADEv740\\cmake)
    else()
        find_path(OpenCASCADE_DIR OpenCASCADEConfig.cmake
                /opt/Dev/OpenCASCADE/lib/cmake/opencascade/
                /opt/opencascade-7.4.0/lib/cmake/opencascade/)
    endif()
    find_package(OpenCASCADE REQUIRED)
    find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
    find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
    find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
    include_directories(${OpenCASCADE_INCLUDE_DIR})
endif ()
set(QT_USE_QTOPENGL TRUE)
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)


# Build -------------------------------------------------------
add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${RESOURCE_FILES}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        ${OpenCASCADE_LIBRARIES}
        )
# End Build ---------------------------------------------------