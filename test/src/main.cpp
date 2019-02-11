/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <ArduinoCloudThing.h>

#include <vector>

/**************************************************************************************
 * MAIN
 **************************************************************************************/

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

/**************************************************************************************
 * TEST HELPER FUNCTIONS
 **************************************************************************************/

std::vector<uint8_t> encode(ArduinoCloudThing & thing)
{
  uint8_t buf[200] = {0};
  int const bytes_buf = thing.encode(buf, 200);
  return std::vector<uint8_t>(buf, buf + bytes_buf);
}

void print(std::vector<uint8_t> const & vect)
{
  for(auto i = vect.begin(); i != vect.end(); i++)
  {
    std::cout << std::uppercase
              << std::hex
              << std::setw(2)
              << std::setfill('0')
              << static_cast<size_t>(*i)
              << std::dec
              << std::nouppercase
              << " ";
  }
  std::cout << std::endl;
}

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

#include "test_callback.cpp"
#include "test_decode.cpp"
#include "test_encode.cpp"
#include "test_publishEvery.cpp"
#include "test_publishOnChange.cpp"
#include "test_publishOnChangeRateLimit.cpp"
#include "test_readOnly.cpp"
#include "test_writeOnly.cpp"
#include "test_getLastValues.cpp"
