project(Lemuri-Media-Center)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/)

include(FindPkgConfig)

find_package(Qt5 COMPONENTS
    Core
    DBus
    Quick
    Widgets
    Multimedia
)
find_package(Xapian REQUIRED)
find_package(Taglib REQUIRED)

# Yum does not support this
option(QML_RESOURCE "Build LMC with QML as an internal resource" ON)
set(QML_RESOURCE ${QML_RESOURCE})
message(STATUS "Building LMC with QML as an internal resource: " ${QML_RESOURCE})

# Generate config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Core_DEFINITIONS})
#add_definitions(-fPIC)

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Multimedia_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${XAPIAN_INCLUDE_DIR}
    ${TAGLIB_INCLUDES}
)

add_subdirectory(src)
