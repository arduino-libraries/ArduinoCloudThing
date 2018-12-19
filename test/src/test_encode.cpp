/*
 * \authors A. Entinger
 */

SCENARIO("Arduino Cloud Properties are encoded", "[ArduinoCloudThing::encode]")
{
  WHEN("Only the boolean 'status' default property has been added")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      /* [{"n": "status", "vb": true}] = 81 BF 61 6E 66 73 74 61 74 75 73 62 76 62 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x61, 0x6E, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x62, 0x76, 0x62, 0xF5, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      /* [{0: "status", 4: true}] = 81 BF 00 66 73 74 61 74 75 73 04 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x04, 0xF5, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'bool' property is added")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();
      encode(thing);

      bool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "vb": true}] = 81 BF 61 6E 64 74 65 73 74 62 76 62 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF5, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      encode(thing);

      bool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{0: "test", 4: true}] = 81 BF 00 64 74 65 73 74 04 F5 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'int' property is added")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();
      encode(thing);

      int int_test = 123;
      thing.addPropertyReal(int_test, "test", Permission::ReadWrite);

      /* [{"n": "test", "v": 123}] = 81 BF 61 6E 64 74 65 73 74 61 76 18 7B FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x18, 0x7B, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      encode(thing);

      int int_test = 123;
      thing.addPropertyReal(int_test, "test", Permission::ReadWrite);

      /* [{0: "test", 3: 123}] = 81 BF 00 64 74 65 73 74 02 18 7B FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x18, 0x7B, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'float' property is added")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();
      encode(thing);

      float float_test = 3.14159;
      thing.addPropertyReal(float_test, "test", Permission::ReadWrite);

      /* [{"n": "test", "v": 3.141590118408203}] = 81 BF 61 6E 64 74 65 73 74 61 76 FA 40 49 0F D0 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFA, 0x40, 0x49, 0x0F, 0xD0, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      encode(thing);

      float float_test = 3.14159;
      thing.addPropertyReal(float_test, "test", Permission::ReadWrite);

      /* [{0: "test", 2: 3.141590118408203}] = 81 BF 00 64 74 65 73 74 02 FA 40 49 0F D0 FF */
      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0xFA, 0x40, 0x49, 0x0F, 0xD0, 0xFF};
      std::vector<uint8_t> const actual = encode(thing);
      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/

  WHEN("A 'String' property is added")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();
      encode(thing);

      /* TODO */
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();
      encode(thing);

//      String string_test("test");
//      thing.addPropertyReal(string_test, "test", Permission::ReadWrite);
//
//      /* [{0: "test", 3: "test"}] = 81 BF 00 64 74 65 73 74 03 64 74 65 73 74 FF */
//      std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x03, 0x64, 0x74, 0x65, 0x73, 0x74, 0xFF};
//      std::vector<uint8_t> const actual = encode(thing);
//      REQUIRE(actual == expected);
    }
  }

  /************************************************************************************/
}
