/*
   Copyright (c) 2019 Arduino.  All rights reserved.
*/

/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <TestUtil.h>
#include <ArduinoCloudThing.h>

/**************************************************************************************
   TEST CODE
 **************************************************************************************/

SCENARIO("Arduino Cloud Properties are encoded", "[ArduinoCloudThing::encode]") {
  /************************************************************************************/

  WHEN("A 'bool' property is added") {
    GIVEN("CloudProtocol::V2") {
      ArduinoCloudThing thing;
      thing.begin();
      encode(thing);

      CloudBool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{0: "test", 4: true}] = 9F BF 00 64 74 65 73 74 04 F5 FF FF*/
      std::vector<uint8_t> const expected = {0x9F, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5, 0xFF, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'int' property is added") {
    GIVEN("CloudProtocol::V2") {
      ArduinoCloudThing thing;
      thing.begin();
      encode(thing);

      CloudInt int_test = 123;
      thing.addPropertyReal(int_test, "test", Permission::ReadWrite);

      /* [{0: "test", 3: 123}] = 9F BF 00 64 74 65 73 74 02 18 7B FF FF */
      std::vector<uint8_t> const expected = {0x9F, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x18, 0x7B, 0xFF, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'float' property is added") {
    GIVEN("CloudProtocol::V2") {
      ArduinoCloudThing thing;
      thing.begin();
      encode(thing);

      CloudFloat float_test = 3.14159;
      thing.addPropertyReal(float_test, "test", Permission::ReadWrite);

      /* [{0: "test", 2: 3.141590118408203}] = 9F BF 00 64 74 65 73 74 02 FA 40 49 0F D0 FF FF */
      std::vector<uint8_t> const expected = {0x9F, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0xFA, 0x40, 0x49, 0x0F, 0xD0, 0xFF, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'String' property is added") {
    GIVEN("CloudProtocol::V2") {
      ArduinoCloudThing thing;
      thing.begin();
      encode(thing);

      CloudString string_test;
      string_test = "test";
      thing.addPropertyReal(string_test, "test", Permission::ReadWrite);

      /* [{0: "test", 3: "test"}] = 9F BF 00 64 74 65 73 74 03 64 74 65 73 74 FF FF*/
      std::vector<uint8_t> const expected = {0x9F, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x03, 0x64, 0x74, 0x65, 0x73, 0x74, 0xFF, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("Multiple properties are added") {
    GIVEN("CloudProtocol::V2") {
      ArduinoCloudThing thing;
      thing.begin();

      CloudInt    int_test = 1;
      CloudBool   bool_test = false;
      CloudFloat  float_test = 2.0f;
      CloudString str_test;
      str_test = "str_test";

      thing.addPropertyReal(int_test,   "int_test",   Permission::ReadWrite);
      thing.addPropertyReal(bool_test,  "bool_test",  Permission::ReadWrite);
      thing.addPropertyReal(float_test, "float_test", Permission::ReadWrite);
      thing.addPropertyReal(str_test,   "str_test",   Permission::ReadWrite);

      /* [{0: "int_test", 2: 1}, {0: "bool_test", 4: false},  {0: "float_test", 2: 2.0}, {0: "str_test", 3: "str_test"}]
       * = 9F BF 00 68 69 6E 74 5F 74 65 73 74 02 01 FF BF 00 6A 66 6C 6F 61 74 5F 74 65 73 74 02 FA 40 00 00 00 FF BF 00 6A 66 6C 6F 61 74 5F 74 65 73 74 02 FA 40 00 00 00 FF BF 00 68 73 74 72 5F 74 65 73 74 03 68 73 74 72 5F 74 65 73 74 FF FF
       */
      std::vector<uint8_t> const expected = {0x9F, 0xBF, 0x00, 0x68, 0x69, 0x6E, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x02, 0x01, 0xFF, 0xBF, 0x00, 0x69, 0x62, 0x6F, 0x6F, 0x6C, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF4, 0xFF, 0xBF, 0x00, 0x6A, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x02, 0xFA, 0x40, 0x00, 0x00, 0x00, 0xFF, 0xBF, 0x00, 0x68, 0x73, 0x74, 0x72, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x03, 0x68, 0x73, 0x74, 0x72, 0x5F, 0x74, 0x65, 0x73, 0x74, 0xFF, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/
}
