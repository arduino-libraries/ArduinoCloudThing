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

    WHEN("update time not yet passed, first call to 'encode'")
    {
      set_millis(0);
      THEN("'encode' should encode the property")
      {
        REQUIRE(encode(thing).size() != 0);
        WHEN("We are 1 ms before the 1st update interval")
        {
          set_millis(999);
          THEN("'encode' should not encode the property")
          {
            REQUIRE(encode(thing).size() == 0);
            WHEN("Exactly 1 second has passed")
            {
              set_millis(1000);
              THEN("'encode' should encode the property")
              {
                REQUIRE(encode(thing).size() != 0);
                WHEN("We are 1 ms before the 2nd update interval")
                {
                 set_millis(1999);
                 THEN("'encode' should not encode the property")
                 {
                   REQUIRE(encode(thing).size() == 0);
                   WHEN("Exactly 2 seconds have passed")
                   {
                     set_millis(2000);
                     THEN("'encode' should encode the property")
                     {
                       REQUIRE(encode(thing).size() != 0);
                     }
                   }
                 }
                }
              }
            }
          }
        }
      }
    }
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
