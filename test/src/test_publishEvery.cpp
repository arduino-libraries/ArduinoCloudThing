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

    bool test_property_published = false;

    unsigned long start = millis();
    for(size_t i = 0; i < 5; )
    {
      /* If we have not reached the publish interval yet - no data should be generated in 'encode' */
      if((millis() - start) < (PUBLISH_INTERVAL_SEC * 1000))
      {
        REQUIRE(encode(thing).size() == 0); /* 'test' property should not be encoded */
      }
      /* If we have reached the publish interval - data should be generated in 'encode' */
      else
      {
        test_property_published = encode(thing).size() != 0; /* Encoding the 'test' property */
        REQUIRE(test_property_published == true);
        if(test_property_published)
        {
          i++;
          test_property_published = false;
          start = millis();
        }
      }
      delay(10); /* Delay 10 ms */
    }
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
