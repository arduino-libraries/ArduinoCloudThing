/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * GLOBAL CONSTANTS
 **************************************************************************************/

static bool callback_called_protocol_v1 = false;
static bool callback_called_protocol_v2 = false;

/**************************************************************************************
 * TEST HELPER FUNCTIONS
 **************************************************************************************/

void externalCallbackV1()
{
  callback_called_protocol_v1 = true;
}

void externalCallbackV2()
{
  callback_called_protocol_v2 = true;
}

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("A callback is registered to be called on property change", "[ArduinoCloudThing::decode]")
{
  /************************************************************************************/

  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();

    int test = 10;
    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(externalCallbackV1);

    /* [{"n": "test", "v": 6}] = 9F BF 61 6E 64 74 65 73 74 61 76 6 FF FF */

    uint8_t const payload[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x6, 0xFF, 0xFF};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length);

    REQUIRE(callback_called_protocol_v1 == true);
  }
  GIVEN("CloudProtocol::V2")
  {
    /* TODO */
  }

  /************************************************************************************/
}
