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
 * TEST CODE
 **************************************************************************************/

SCENARIO("Arduino Cloud Properties are encoded", "[ArduinoCloudThing::encode]")
{
  GIVEN("No properties besides the default 'status' property have been added")
  {
    ArduinoCloudThing thing;
    thing.begin();

    uint8_t actual[200] = {0};
    int const bytes_actual = thing.encode(actual, 200);
    std::vector<uint8_t> const actual_vect(actual, actual + bytes_actual);

   /* http://cbor.me    [{0: "status", 4: true}] = 81 BF 00 66 73 74 61 74 75 73 04 F5 FF */
    std::vector<uint8_t> const expected_vect = {0x81, 0xBF, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x04, 0xF5, 0xFF};

    REQUIRE(actual_vect == expected_vect);
  }
}
