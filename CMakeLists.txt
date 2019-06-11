cmake_minimum_required (VERSION 2.6)

project (json_parser)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98" )

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

# google tests
include_directories(gtests)

add_executable(json_parser main.cpp src/json.h src/json.cpp src/support.cpp src/support.h)
