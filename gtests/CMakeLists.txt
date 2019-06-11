cmake_minimum_required(VERSION 2.6)
project(json-parser-io-unit-test)

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98" )

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(UNIT_TEST_SOURCE_FILES
        tmain.cpp
        ${UNIT_SOURCES}
        )

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
