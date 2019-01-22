/*
 * \authors A. Entinger
 */

SCENARIO("Arduino Cloud Properties are decoded", "[ArduinoCloudThing::decode]")
{
  /************************************************************************************/

  WHEN("A boolean property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      bool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "vb": false}] = 81 A2 61 6E 64 74 65 73 74 62 76 62 F4 */

      uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF4};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == false);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      /* TODO */
    }
  }

  /************************************************************************************/

  WHEN("A int property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "v": 7}] = 81 A2 61 6E 64 74 65 73 74 61 76 07 */

      uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == 7);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      /* TODO */
    }
  }

  /************************************************************************************/

  WHEN("A float property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      float test = 0.0f;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "v": 3.1459}] = 81 A2 61 6E 64 74 65 73 74 61 76 FB 40 09 2A CD 9E 83 E4 26 */

      uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x09, 0x2A, 0xCD, 0x9E, 0x83, 0xE4, 0x26};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == Approx(3.1459).epsilon(0.01));
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      /* TODO */
    }
  }

  /************************************************************************************/

  WHEN("A String property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      String str = "test";
      thing.addPropertyReal(str, "test", Permission::ReadWrite);

      /* [{"n": "test", "vs": "testtt"}] = 9F BF 61 6E 64 74 65 73 74 62 76 73 66 74 65 73 74 74 74 FF FF */
      uint8_t const payload[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74, 0xFF, 0xFF};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(str == "testtt");
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      /* TODO */
    }
  }

  /************************************************************************************/
}
