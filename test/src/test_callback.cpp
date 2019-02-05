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

SCENARIO("A callback is registered via 'onUpdate' to be called on property change", "[ArduinoCloudThing::decode]")
{
  /************************************************************************************/

  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();

    int test = 10;
    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(externalCallbackV1);

    /* [{"n": "test", "v": 7}] = 81 A2 61 6E 64 74 65 73 74 61 76 07 */
    uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length);

    REQUIRE(callback_called_protocol_v1 == true);
  }
  GIVEN("CloudProtocol::V2")
  {
    ArduinoCloudThing thing(CloudProtocol::V2);
    thing.begin();

    int test = 10;
    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(externalCallbackV2);

    /* [{0: "test", 2: 7}] = 81 A2 00 64 74 65 73 74 02 07 */
    uint8_t const payload[] = {0x81, 0xA2, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x07};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length);

    REQUIRE(callback_called_protocol_v2 == true);
  }

  /************************************************************************************/
}

/**************************************************************************************/

static bool switch_turned_on       = false;
static bool switch_callback_called = false;

void switch_callback()
{
  switch_turned_on       = false;
  switch_callback_called = true;
}

SCENARIO("A (boolean) property is manipulated in the callback to its origin state", "[ArduinoCloudThing::decode]")
{
  GIVEN("CloudProtocol::V1")
  {
    ArduinoCloudThing thing(CloudProtocol::V1);
    thing.begin();
    encode(thing);

    thing.addPropertyReal(switch_turned_on, "switch_turned_on", Permission::ReadWrite).onUpdate(switch_callback);

    /* [{"n": "switch_turned_on", "vb": true}] = 81 A2 61 6E 70 73 77 69 74 63 68 5F 74 75 72 6E 65 64 5F 6F 6E 62 76 62 F5 */
    uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x70, 0x73, 0x77, 0x69, 0x74, 0x63, 0x68, 0x5F, 0x74, 0x75, 0x72, 0x6E, 0x65, 0x64, 0x5F, 0x6F, 0x6E, 0x62, 0x76, 0x62, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length);

    REQUIRE(switch_callback_called == true);

    /* Since the property was reset to its origin state in the callback we
     * expect that on the next call to encode this change is propagated to
     * the cloud.
     */

    /* [{"n": "switch_turned_on", "vb": false}] = 81 BF 61 6E 70 73 77 69 74 63 68 5F 74 75 72 6E 65 64 5F 6F 6E 62 76 62 F4 FF */
    std::vector<uint8_t> const expected = {0x81, 0xBF, 0x61, 0x6E, 0x70, 0x73, 0x77, 0x69, 0x74, 0x63, 0x68, 0x5F, 0x74, 0x75, 0x72, 0x6E, 0x65, 0x64, 0x5F, 0x6F, 0x6E, 0x62, 0x76, 0x62, 0xF4, 0xFF};
    std::vector<uint8_t> const actual = encode(thing);
    REQUIRE(actual == expected);
  }
}
