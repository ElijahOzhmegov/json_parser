cmake_minimum_required(VERSION 2.6)
project(json-parser-io-unit-test)

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98" )

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB UNIT_SOURCES unit/*.cpp unit/*.h)

set(UNIT_TEST_SOURCE_FILES
        tmain.cpp
        ${UNIT_SOURCES}
        )

include_directories(../src)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(json-parser-io-unit-test ${UNIT_TEST_SOURCE_FILES})
target_link_libraries(json-parser-io-unit-test ${GTEST_LIBRARIES} pthread)
