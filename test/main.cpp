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

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("Arduino Cloud Properties are encoded", "[ArduinoCloudThing::encode]")
{
  GIVEN("Cloud Protocol V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();

    WHEN("No properties besides the default 'status' property have been added")
    {
      std::vector<uint8_t> const actual   = encode(thing);
      /* [{"n": "test", "vb": true}] = 81 A2 61 6E 64 74 65 73 74 62 76 62 F5 */
      std::vector<uint8_t> const expected = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF5};
      REQUIRE(actual == expected);
    }
  }
  /************************************************************************************/
  GIVEN("Cloud Protocol V2")
  {
    ArduinoCloudThing thing(CloudProtocol::V2);
    thing.begin();

    WHEN("No properties besides the default 'status' property have been added")
    {
      std::vector<uint8_t> const actual   = encode(thing);
      /* [{0: "test", 4: true}] = 81 BF 00 64 74 65 73 74 04 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x04, 0xF5, 0xFF};
      REQUIRE(actual == expected);
    }
  }

}
