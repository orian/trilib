cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(Trilib)

set(VERSION_MAJOR 1)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(CMAKE_BUILD_TYPE_DEV True)
  SET(CMAKE_CXX_FLAGS_DEV "${CMAKE_CXX_FLAGS_DEV} -Wall --pedantic -g")
  SET(CMAKE_C_FLAGS_DEV "${CMAKE_C_FLAGS_DEV} -g")
ENDIF()

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -stc=c99")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
