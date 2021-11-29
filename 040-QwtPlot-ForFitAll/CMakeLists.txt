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

find_path(QWT_INCLUDE_DIR qwt.h HINTS /usr/local/qwt-6.2.0/include)
find_library(QWT_LIBRARY qwt /usr/include/qwt)

if (NOT ${QWT_LIBRARY})
    find_package(Qwt REQUIRED)
    include_directories(${QWT_INCLUDE_DIRS})

endif ()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
target_link_libraries(${PROJECT_NAME} PRIVATE ${QWT_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::Xml  )
