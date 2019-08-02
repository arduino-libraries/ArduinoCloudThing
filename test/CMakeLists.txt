##########################################################################

cmake_minimum_required(VERSION 2.8)

##########################################################################

include_directories(include)
include_directories(../src)
include_directories(external/catch/v.2.5.0/include)

##########################################################################

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################################################

set(TEST_TARGET testArduinoCloudThing)

set(TEST_SRCS
  src/Arduino.cpp
  
  src/main.cpp
  src/test_addPropertyReal.cpp
  src/test_callback.cpp
  src/test_CloudColor.cpp
  src/test_CloudLocation.cpp
  src/test_decode.cpp
  src/test_encode.cpp
  src/test_publishEvery.cpp
  src/test_publishOnChange.cpp
  src/test_publishOnChangeRateLimit.cpp
  src/test_readOnly.cpp
  src/test_writeOnly.cpp
  src/TestUtil.cpp

  ../src/ArduinoCloudThing.cpp
  ../src/ArduinoCloudProperty.cpp
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
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   "--coverage")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage")

##########################################################################

add_executable(
  ${TEST_TARGET}
  ${TEST_SRCS}
)

##########################################################################

