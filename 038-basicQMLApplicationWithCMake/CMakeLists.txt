cmake_minimum_required(VERSION 3.2)

project(QMLProject)

# CMake Configuration ------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")

set(CMAKE_AUTOMOC ON)
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
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})
# End Qt5 Package ---------------------------------------------


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
        )
# End Build ---------------------------------------------------