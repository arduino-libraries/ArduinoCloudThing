##########################################################################

cmake_minimum_required(VERSION 2.8)

##########################################################################

include_directories(../src)
include_directories(external/catch/current/include)

##########################################################################

set(CMAKE_CXX_STANDARD 11)

set(TEST_TARGET testArduinoCloudThing)
set(TEST_SRCS
  main.cpp
  ../src/ArduinoCloudPropertyContainer.cpp
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

add_executable(
  ${TEST_TARGET}
  ${TEST_SRCS}
)

##########################################################################

