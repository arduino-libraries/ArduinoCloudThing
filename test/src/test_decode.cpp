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

  WHEN("Multiple properties is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      bool   bool_test = false;
      int    int_test = 1;
      float  float_test = 2.0f;
      String str_test("str_test");

      thing.addPropertyReal(bool_test,  "bool_test",  Permission::ReadWrite);
      thing.addPropertyReal(int_test,   "int_test",   Permission::ReadWrite);
      thing.addPropertyReal(float_test, "float_test", Permission::ReadWrite);
      thing.addPropertyReal(str_test,   "str_test",   Permission::ReadWrite);

      /* [{"n": "bool_test", "vb": true}, {"n": "int_test", "v": 10}, {"n": "float_test", "v": 20.0}, {"n": "str_test", "vs": "hello arduino"}]
       * = 84 A2 61 6E 69 62 6F 6F 6C 5F 74 65 73 74 62 76 62 F5 A2 61 6E 68 69 6E 74 5F 74 65 73 74 61 76 0A A2 61 6E 6A 66 6C 6F 61 74 5F 74 65 73 74 61 76 F9 4D 00 A2 61 6E 68 73 74 72 5F 74 65 73 74 62 76 73 6D 68 65 6C 6C 6F 20 61 72 64 75 69 6E 6F
       */
      uint8_t const payload[] = {0x84, 0xA2, 0x61, 0x6E, 0x69, 0x62, 0x6F, 0x6F, 0x6C, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF5, 0xA2, 0x61, 0x6E, 0x68, 0x69, 0x6E, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x0A, 0xA2, 0x61, 0x6E, 0x6A, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xF9, 0x4D, 0x00, 0xA2, 0x61, 0x6E, 0x68, 0x73, 0x74, 0x72, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x6D, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x61, 0x72, 0x64, 0x75, 0x69, 0x6E, 0x6F};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(bool_test  == true);
      REQUIRE(int_test   == 10);
      REQUIRE(float_test == Approx(20.0).epsilon(0.01));
      REQUIRE(str_test   == "hello arduino");
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
