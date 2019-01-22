/*
 * \authors A. Entinger
 */

SCENARIO("A Arduino cloud property is published on value change but the update rate is limited", "[ArduinoCloudThing::publishOnChange]")
{
  /************************************************************************************/

  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();
    REQUIRE(encode(thing).size() != 0); /* Encoding the 'status' property */

    int test = 0;
    int const MIN_DELTA = 0;
    unsigned long const MIN_TIME_BETWEEN_UPDATES_ms = 500; /* No updates faster than 500 ms */

    thing.addPropertyReal(test, "test", Permission::ReadWrite).publishOnChange(MIN_DELTA, MIN_TIME_BETWEEN_UPDATES_ms);

    unsigned long start = millis();
    /* Encoding the 'test' property - since this is the first encode call since adding the property we shall encode the property */
    REQUIRE(encode(thing).size() != 0);

    for(int i = 0; i < 10; i++)
    {
      test++;
      if((millis() - start) < MIN_TIME_BETWEEN_UPDATES_ms)
      {
        /* Although the value of test has changed considerably we do not update because the MIN_TIME_BETWEEN_UPDATES_ms has not passed since the last encoding */
        REQUIRE(encode(thing).size() == 0);
      }
      else
      {
        /* Now the required time has passed so we will encode the test property */
        REQUIRE(encode(thing).size() != 0);
        start = millis();
      }
      delay(100);
    }
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
