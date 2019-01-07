/*
 * \authors A. Entinger
 */

SCENARIO("A Arduino cloud property is marked 'read only'", "[ArduinoCloudThing::decode]")
{
  /************************************************************************************/

  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();

    int test = 0;
    thing.addPropertyReal(test, "test", Permission::Read);

    /* [{"n": "test", "v": 7}] = 81 A2 61 6E 64 74 65 73 74 61 76 07 */

    uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length);

    REQUIRE(test == 0);
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
