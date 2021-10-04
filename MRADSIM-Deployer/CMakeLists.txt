# MRADSIM SPACE
# version 0.1.0
#
# Requirements
#   CMake for compiler tools     https://cmake.org/
#   Qt 5.12 for GUI              https://www.qt.io/
#   OpenCASCADE 7.4 for 3D model https://www.opencascade.com/
#   TabToolbar                   https://github.com/SeriousAlexej/TabToolbar
#   JKQtPlotter                  https://github.com/jkriege2/JKQtPlotter
#   Doxygen for documentation    https://www.doxygen.nl/index.html
#   QWT Library for charts       https://qwt.sourceforge.io/

# Begin Compiling Settings -------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(MRADSIM-Deployer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")


if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(" ")
    message("CMAKE IN DEBUG MODE")
    message(" ")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message(" ")
    message("CMAKE IN RELEASE MODE")
    message(" ")
endif ()

# End Compiling Settings ---------------------------------------------------------------

# Begin SOURCE Files -------------------------------------------------------------------
file(GLOB SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/*.cpp *.c
        ${PROJECT_SOURCE_DIR}/src/*.cpp *.c
        )


# Begin HEADER Files -------------------------------------------------------------------
file(GLOB HEADER_FILES
        ${PROJECT_SOURCE_DIR}/*.h *.hpp
        ${PROJECT_SOURCE_DIR}/src/*.h *.hpp
        )

include_directories()
include_directories(src/)

# End HEADER Files -------------------------------------------------------------------

# Begin RESOURCES Files --------------------------------------------------------------
file(GLOB RESOURCE_FILES
        ${PROJECT_SOURCE_DIR}/resources/*.qrc
        )
# End RESOURCE Files -------------------------------------------------------------------

message("--> Searching UI files....")
set(CMAKE_AUTOUIC_SEARCH_PATHS uis/)
file(GLOB UI_FILES ${PROJECT_SOURCE_DIR}/uis/*.ui)

# End OpenCASCADE Library ---------------------------------------------------------------
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Svg REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)
find_package(Qt5 COMPONENTS LinguistTools REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DDEVELOPERMOD)
endif ()
# Begin Building Settings ------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${RESOURCE_FILES}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Xml
        Qt5::Network
        Qt5::Sql
        Qt5::PrintSupport
        Qt5::Svg)
# End Building Settings ------------------------------------------------------------------------
