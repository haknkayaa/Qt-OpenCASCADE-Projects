cmake_minimum_required(VERSION 3.14)
project(opencascadeProject)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES *.cpp *.c)

file(GLOB HEADER_FILES *.h *.hpp)

if (WIN32)
    find_path(OpenCASCADE_DIR OpenCASCADEConfig.cmake
            C:\\Dev\\OpenCASCADE\\OpenCASCADEv740\\cmake)
else ()
    find_path(OpenCASCADE_DIR OpenCASCADEConfig.cmake
            /opt/Dev/OpenCASCADE/lib/cmake/opencascade/
            /opt/opencascade-7.4.0/lib/cmake/opencascade/)
endif ()
find_package(OpenCASCADE REQUIRED)
find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)

include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

find_package(OpenGL REQUIRED)

find_package(Qt5 COMPONENTS Core Widgets Gui Xml OpenGL REQUIRED)
find_package(Qt5Core REQUIRED NO_DEFAULT_PATH)
find_package(Qt5Widgets REQUIRED NO_DEFAULT_PATH)
find_package(Qt5Gui REQUIRED NO_DEFAULT_PATH)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCASCADE_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::Xml)
