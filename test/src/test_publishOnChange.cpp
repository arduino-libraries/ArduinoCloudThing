/*
 * \authors A. Entinger
 */

SCENARIO("A Arduino cloud property is published on value change", "[ArduinoCloudThing::publishOnChange]")
{
  /************************************************************************************/

  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();
    REQUIRE(encode(thing).size() != 0); /* Encoding the 'status' property */

    int       test  = 10;
    int const DELTA = 6;

    thing.addPropertyReal(test, "test", Permission::ReadWrite).publishOnChange(DELTA);

    /* Encoding the 'test' property - we always transmit the first time since the change */
    REQUIRE(encode(thing).size() != 0);

    test += 4;
    /* test = 14, delta to previous encoded change = 4 which is < 6, therefore no data shall be encoded */
    REQUIRE(encode(thing).size() == 0);

    test += 4;
    /* test = 18, delta to previous encoded change = 8 which is > 6, therefore data shall be encoded */
    REQUIRE(encode(thing).size() != 0);
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
