cmake_minimum_required(VERSION 3.14)

project(LocalSocket)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/mylocalsocket.cpp
        src/mylocalsocket.h
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Network
        )

