cmake_minimum_required(VERSION 2.8.0)
project(json-parser-unit-test)

set(CMAKE_PROJECT_VERSION_MAJOR "1")
set(CMAKE_PROJECT_VERSION_MINOR "0")
set(CMAKE_PROJECT_VERSION_PATCH "0")
set(CMAKE_PROJECT_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "QNX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm")
endif(NOT CMAKE_SYSTEM_NAME STREQUAL "QNX")

add_subdirectory(../../googletest gtest)
include_directories(../../googletest/include)
include_directories(unit)

file(GLOB UNIT_SOURCES unit/*.cpp unit/*.h)

set(UNIT_TEST_SOURCE_FILES
        tmain.cpp
        ../src/json.cpp
        ../src/json.h
        ${UNIT_SOURCES}
        ${SOURCES}
        )

include_directories(../)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(json-parser-unit-test ${UNIT_TEST_SOURCE_FILES})

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    message(${CMAKE_SYSTEM_NAME})
    target_link_libraries(json-parser-unit-test gtest socket)
else()
    target_link_libraries(json-parser-unit-test gtest rt)
endif()

