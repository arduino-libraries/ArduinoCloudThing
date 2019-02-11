##########################################################################

cmake_minimum_required(VERSION 2.8)

##########################################################################

include_directories(include)
include_directories(../src)
include_directories(external/catch/current/include)

##########################################################################

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################################################

set(TEST_TARGET testArduinoCloudThing)

set(TEST_SRCS
  src/Arduino.cpp
  src/main.cpp
  ../src/ArduinoCloudPropertyContainer.cpp
  ../src/ArduinoCloudPropertyContainer.ipp
  ../src/ArduinoCloudProperty.ipp
  ../src/ArduinoCloudThing.cpp
  ../src/lib/tinycbor/src/cborencoder.c
  ../src/lib/tinycbor/src/cborencoder_close_container_checked.c
  ../src/lib/tinycbor/src/cborerrorstrings.c
  ../src/lib/tinycbor/src/cborparser.c
  ../src/lib/tinycbor/src/cborparser_dup_string.c
  ../src/lib/tinycbor/src/cborpretty.c
  ../src/lib/tinycbor/src/cborpretty_stdio.c
  ../src/lib/tinycbor/src/cbortojson.c
  ../src/lib/tinycbor/src/cborvalidation.c
  ../src/lib/tinycbor/src/open_memstream.c
)

##########################################################################

add_definitions(-DHOST_BUILD)

set(CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   "--coverage")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage")

##########################################################################

add_executable(
  ${TEST_TARGET}
  ${TEST_SRCS}
)

##########################################################################

