cmake_minimum_required(VERSION 3.14)

project(LocalServer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

# START SOURCE FILES
message("--> Kaynak dosyalari araniyor...")

file(GLOB SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.c
        ${PROJECT_SOURCE_DIR}/src/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/*.ui)

# Source Files Dump
foreach(_file ${SOURCE_FILES})
    message("Dosya: ${_file}")
endforeach()
# END SOURCE FILES

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Network
        )

