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
  /************************************************************************************/
  WHEN("Only the boolean 'status' default property has been added")
  {
    /**********************************************************************************/
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      std::vector<uint8_t> const actual = encode(thing);
      /* [{"n": "test", "vb": true}] = 81 BF 61 6E 66 73 74 61 74 75 73 62 76 62 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x61, 0x6E, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x62, 0x76, 0x62, 0xF5, 0xFF};
      REQUIRE(actual == expected);
    }
    /**********************************************************************************/
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      std::vector<uint8_t> const actual = encode(thing);
      /* [{0: "test", 4: true}] = 81 BF 00 64 74 65 73 74 04 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x04, 0xF5, 0xFF};
      REQUIRE(actual == expected);
    }
  }
  /************************************************************************************/
}
