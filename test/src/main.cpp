/*
 * \authors A. Entinger
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

#include "test_encode.cpp"
