cmake_minimum_required(VERSION 3.16)
project(ProjectName)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5Core       5.3 REQUIRED)
find_package(Qt5Gui        5.3 REQUIRED)
find_package(Qt5Widgets    5.3 REQUIRED)
find_package(TabToolbar    REQUIRED)



set(SRCROOT ${PROJECT_SOURCE_DIR})

set(SRC_LIST
    ${SRCROOT}/main.cpp
    ${SRCROOT}/MainWindow.cpp
    ${SRCROOT}/MainWindow.h
)

set(UIS
    ${SRCROOT}/MainWindow.ui
)

set(RES
    ${SRCROOT}/res.qrc
)

add_executable(${PROJECT_NAME} 
    ${SRC_LIST}
    ${UIS}
    ${RES}
)

target_link_libraries(${PROJECT_NAME} ${TabToolbar} Qt5::Core Qt5::Gui Qt5::Widgets)




