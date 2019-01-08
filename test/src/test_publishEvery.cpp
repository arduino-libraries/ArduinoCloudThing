/*
 * \authors A. Entinger
 */

SCENARIO("A Arduino cloud property is published periodically", "[ArduinoCloudThing::publishEvery]")
{
  /************************************************************************************/

  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();
    REQUIRE(encode(thing).size() != 0); /* Encoding the 'status' property */

    bool test = true;
    unsigned long const PUBLISH_INTERVAL_SEC = 1 * SECONDS;

    thing.addPropertyReal(test, "test", Permission::ReadWrite).publishEvery(PUBLISH_INTERVAL_SEC);
    REQUIRE(encode(thing).size() != 0); /* Initial encoding of the 'test' property */

    unsigned long start = millis();
    for(int i = 0; i < 3; i++) {
      /* If we have not reached the publish interval yet - no data should be generated in 'encode' */
      if((millis() - start) < (PUBLISH_INTERVAL_SEC * 1000)) {
        REQUIRE(encode(thing).size() == 0); /* 'test' property should not be encoded */
      }
      /* If we have reached the publish interval - no data should be generated in 'encode' */
      else {
        REQUIRE(encode(thing).size() != 0); /* Encoding the 'test' property */
        start = millis();
      }
      delay(1000);
    }
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
