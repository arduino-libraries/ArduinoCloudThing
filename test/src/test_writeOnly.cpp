/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

SCENARIO("A Arduino cloud property is marked 'write only'", "[ArduinoCloudThing::encode]")
{
  /************************************************************************************/

  ArduinoCloudThing thing;
  thing.begin();
  REQUIRE(encode(thing).size() != 0); /* Encoding the 'status' property */

  int test = 0;
  thing.addPropertyReal(test, "test", Permission::Write);

  REQUIRE(encode(thing).size() == 0); /* Since 'test' is 'write only' it should not be encoded */

  /************************************************************************************/
}
