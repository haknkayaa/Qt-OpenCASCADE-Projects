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

#-------------------------------------------------------------------
### OpenCASCADE Official Edition
if (WIN32)
    find_path(OpenCASCADE_DIR OpenCASCADEConfig.cmake
            C:\\Dev\\OpenCASCADE\\OpenCASCADEv740\\cmake
            E:\\OpenCASCADE\\OpenCASCADE-7.4.0-vc14-64\\opencascade-7.4.0\\cmake
            C:\\OpenCASCADE-7.4.0-vc14-64\\opencascade-7.4.0\\cmake
            C:\\OpenCASCADE-7.6.0-vc14-64\\opencascade-7.6.0\\cmake
            C:\\OpenCASCADE\\OCCT\\cmake
            )
else()
    find_path(OpenCASCADE_DIR OpenCASCADEConfig.cmake
            /opt/Dev/OpenCASCADE/lib/cmake/opencascade/
            /opt/opencascade-7.4.0/lib/cmake/opencascade/)
endif ()
find_package(OpenCASCADE REQUIRED)
find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories(src)
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

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCASCADE_LIBRARIES} ${OpenCASCADE_OCAF_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::Xml)
