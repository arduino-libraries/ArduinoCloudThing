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

    WHEN("test not modified, update time not yet passed, first call to 'encode'")
    {
      set_millis(0);
      THEN("'encode' should encode the property")
      {
        REQUIRE(encode(thing).size() != 0);
        WHEN("test modified, update time not yet passed")
        {
          test++;
          set_millis(499);
          THEN("'encode' should not encode any property")
          {
            REQUIRE(encode(thing).size() == 0);
            WHEN("test modified, update time passed")
            {
              test++;
              set_millis(500);
              THEN("'encode' should encode the property")
              {
                REQUIRE(encode(thing).size() != 0);
                WHEN("test modified, update time not yet passed")
                {
                  test++;
                  set_millis(999);
                  THEN("'encode' should not encode any property")
                  {
                    REQUIRE(encode(thing).size() == 0);
                    WHEN("test modified, update time passed")
                    {
                      test++;
                      set_millis(1000);
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
