cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(FreeCADLearn)

set(CMAKE_DEBUG_POSTFIX "_d")

# set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/build)
message(${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/lib)

# set(PACKAGE_VERSION_NAME "Vulcan")
set(PACKAGE_VERSION_MAJOR "0")
set(PACKAGE_VERSION_MINOR "1")
set(PACKAGE_VERSION_PATCH "0") # number of patch release (e.g. "4" for the 0.18.4 release)

# set(PACKAGE_VERSION_SUFFIX "dev") # either "dev" for development snapshot or "" (empty string)
# set(PACKAGE_BUILD_VERSION "0") # used when the same FreeCAD version will be re-released (for example using an updated LibPack)
set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
message(${PACKAGE_VERSION})
set(PACKAGE_STRING "${PROJECT_NAME} ${PACKAGE_VERSION}")
message(${PACKAGE_STRING})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(cmake/Helpers)

ConfigureCMakeVariables()

# message(STATUS "cmake: ${PYCXX_INCLUDE_DIR}/src")
find_package(PyCXX REQUIRED)

SetupPython()

message(STATUS ${PYCXX_INCLUDE_DIR})

message(STATUS ${PYTHONINTERP_FOUND})

add_subdirectory(src)

option(TEST_OPTION "是否编译测试工程" OFF)
