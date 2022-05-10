cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES src/*.h)
file(GLOB RESOURCE_FILES resources/*.qrc)


#QT5
find_package(Qt5 COMPONENTS Core Widgets Gui Xml OpenGL  REQUIRED)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-modules)

# Begin Qwt Library ------------------------------------------------------------------------
set(QWT_INCLUDE_DIRS_PATHS /usr/local/qwt-6.1.5/include
        /usr/local/include)

set(QWT_LIBRARY_PATHS /usr/local/qwt-6.1.5/lib/
        /usr/local/lib/)

find_path(QWT_INCLUDE_DIR
        qwt.h
        PATH_SUFFIXES include
        PATHS ${QWT_INCLUDE_DIRS_PATHS}
        )
include_directories(QWT_INCLUDE_DIR)
#find_path(QWT_LIBARY_DIR
#        libqwt.so.6
#        PATH_SUFFIXES lib64 lib
#        PATHS ${QWT_LIBRARY_PATHS})

find_library(QWT_LIBRARY
        qwt libqwt.so.6
        PATH_SUFFIXES lib64 lib
        PATHS ${QWT_LIBRARY_PATHS})


if (NOT ${QWT_LIBRARY})
    find_package(Qwt REQUIRED)
    include_directories(${QWT_INCLUDE_DIRS})
endif ()

#include_directories(${QWT_INCLUDE_DIR})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DDEVELOPERMOD)
endif ()
# End Qwt Library ------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
target_link_libraries(${PROJECT_NAME} PRIVATE ${QWT_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::Xml)


