/*
 * \authors A. Entinger
 */

SCENARIO("Arduino Cloud Properties are decoded", "[ArduinoCloudThing::decode]")
{
  WHEN("A boolean property has been added")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      bool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* http://cbor.me/
       * [{"n": "test", "vb": false}] = 81 A2 61 6E 64 74 65 73 74 62 76 62 F4
       */

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
}
