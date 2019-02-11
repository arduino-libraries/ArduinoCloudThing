/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

SCENARIO("Send a getLastValues message to the Arduino Cloud", "[ArduinoCloudThing::getLastValues]")
{
  /************************************************************************************/

  WHEN("Message is fixed and should be CBOR encoded")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      /* {"n": "r:m", "vs": "getLastValues"} = 81 A2 61 6E 63 72 3A 6D 62 76 73 6D 67 65 74 4C 61 73 74 56 61 6C 75 65 73 */
      std::vector<uint8_t> const expected = {0x81, 0xA2, 0x61, 0x6E, 0x63, 0x72, 0x3A, 0x6D, 0x62, 0x76, 0x73, 0x6D, 0x67, 0x65, 0x74, 0x4C, 0x61, 0x73, 0x74, 0x56, 0x61, 0x6C, 0x75, 0x65, 0x73};
      uint8_t data[200];
      int const length = thing.getLastValues(data, sizeof(data));
      std::vector<uint8_t> const actual = std::vector<uint8_t>(data, data + length);

      REQUIRE(actual == expected);
      REQUIRE(length > 0);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      /* {0: "r:m", 3: "getLastValues"} = 81 A2 00 63 72 3A 6D 03 6D 67 65 74 4C 61 73 74 56 61 6C 75 65 73 */
      std::vector<uint8_t> const expected = {0x81, 0xA2, 0x00, 0x63, 0x72, 0x3A, 0x6D, 0x03, 0x6D, 0x67, 0x65, 0x74, 0x4C, 0x61, 0x73, 0x74, 0x56, 0x61, 0x6C, 0x75, 0x65, 0x73};
      uint8_t data[200];
      int const length = thing.getLastValues(data, sizeof(data));
      std::vector<uint8_t> const actual = std::vector<uint8_t>(data, data + length);

      REQUIRE(actual == expected);
      REQUIRE(length > 0);
    }
  }

  /************************************************************************************/
}
