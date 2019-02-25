/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <TestUtil.h>
#include <ArduinoCloudThing.h>

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
    ArduinoCloudThing thing;
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
    ArduinoCloudThing thing;
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
  GIVEN("CloudProtocol::V2")
  {
    ArduinoCloudThing thing;
    thing.begin();
    encode(thing);

    thing.addPropertyReal(switch_turned_on, "switch_turned_on", Permission::ReadWrite).onUpdate(switch_callback);

    /* [{0: "switch_turned_on", 4: true}] = 81 A2 00 70 73 77 69 74 63 68 5F 74 75 72 6E 65 64 5F 6F 6E 04 F5 */
    uint8_t const payload[] = {0x81, 0xA2, 0x00, 0x70, 0x73, 0x77, 0x69, 0x74, 0x63, 0x68, 0x5F, 0x74, 0x75, 0x72, 0x6E, 0x65, 0x64, 0x5F, 0x6F, 0x6E, 0x04, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length);

    REQUIRE(switch_callback_called == true);

    /* Since the property was reset to its origin state in the callback we
     * expect that on the next call to encode this change is propagated to
     * the cloud.
     */

    /* [{0: "switch_turned_on", 4: false}] = 81 BF 00 70 73 77 69 74 63 68 5F 74 75 72 6E 65 64 5F 6F 6E 04 F4 FF */
    std::vector<uint8_t> const expected = {0x81, 0xBF, 0x00, 0x70, 0x73, 0x77, 0x69, 0x74, 0x63, 0x68, 0x5F, 0x74, 0x75, 0x72, 0x6E, 0x65, 0x64, 0x5F, 0x6F, 0x6E, 0x04, 0xF4, 0xFF};
    std::vector<uint8_t> const actual = encode(thing);
    REQUIRE(actual == expected);
  }
}

/**************************************************************************************/

static bool test = false;
static bool sync_callback_called = false;
static bool change_callback_called = false;


void auto_sync_callback(ArduinoCloudProperty<bool> property)
{
  MOST_RECENT_WINS(property);
  sync_callback_called = true;
}

void change_callback()
{
  change_callback_called = true;
}

SCENARIO("After a connection/reconnection an incoming cbor payload is processed and the synchronization callback is executed. The sync callback applies the AUTO_SYNC policy (the most recent value between the local one and the cloud one is finally assigned to the property). The onUpdate function is called if the cloud value is the most recent one. In this scenario the most updated value is the cloud one.")
{
  GIVEN("CloudProtocol::V2")
  {
    test = false;
    sync_callback_called = false;
    change_callback_called = false;

    ArduinoCloudThing thing;
    thing.begin();

    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(change_callback).onSync(auto_sync_callback);

    thing.updateTimestampOnChangedProperties(1550138809);

    /* [{"bt": 1550138810.00, 0: "test", 4: true}] = 81 A3 62 62 74 FB 41 D7 19 4F 6E 80 00 00 00 64 74 65 73 74 04 F5 */
    uint8_t const payload[] = {0x81 ,0xA3, 0x62, 0x62, 0x74, 0xFB, 0x41, 0xD7, 0x19, 0x4F, 0x6E, 0x80, 0x00, 0x00, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length, true);

    REQUIRE(sync_callback_called == true);
    REQUIRE(change_callback_called == true);

    REQUIRE(test == true);
  }
}

/**************************************************************************************/

SCENARIO("After a connection/reconnection an incoming cbor payload is processed and the synchronization callback is executed. The sync callback apply the AUTO_SYNC policy (the most recent value between the local one and the cloud one is finally assigned to the property). The onUpdate function is called if the cloud value is the most recent one. In this scenario the most updated value is the local one.")
{
  GIVEN("CloudProtocol::V2")
  {
    test = true;
    sync_callback_called = false;
    change_callback_called = false;

    ArduinoCloudThing thing;
    thing.begin();

    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(change_callback).onSync(auto_sync_callback);
    test = false;
    thing.updateTimestampOnChangedProperties(1550138811);

    /* [{"bt": 1550138810.00, 0: "test", 4: true}] = 81 A3 62 62 74 FB 41 D7 19 4F 6E 80 00 00 00 64 74 65 73 74 04 F5 */
    uint8_t const payload[] = {0x81 ,0xA3, 0x62, 0x62, 0x74, 0xFB, 0x41, 0xD7, 0x19, 0x4F, 0x6E, 0x80, 0x00, 0x00, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length, true);

    REQUIRE(sync_callback_called == true);
    REQUIRE(change_callback_called == false);

    REQUIRE(test == false);
  }
}

/**************************************************************************************/

void force_device_sync_callback(ArduinoCloudProperty<bool> property)
{
  DEVICE_WINS(property);
  sync_callback_called = true;
}

SCENARIO("After a connection/reconnection an incoming cbor payload is processed and the synchronization callback is executed. The sync callback applies the FORCE_DEVICE_SYNC policy (the property keeps the local value and, if the cloud value is different from the local one, the value is propagated to the cloud). The onUpdate function is not executed")
{
  GIVEN("CloudProtocol::V2")
  {

    test = false;
    sync_callback_called = false;
    change_callback_called = false;

    ArduinoCloudThing thing;
    thing.begin();

    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(change_callback).onSync(force_device_sync_callback);

    /* [{"bt": 1550138810.00, 0: "test", 4: true}] = 81 A3 62 62 74 FB 41 D7 19 4F 6E 80 00 00 00 64 74 65 73 74 04 F5 */
    uint8_t const payload[] = {0x81 ,0xA3, 0x62, 0x62, 0x74, 0xFB, 0x41, 0xD7, 0x19, 0x4F, 0x6E, 0x80, 0x00, 0x00, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length, true);

    REQUIRE(sync_callback_called == true);
    REQUIRE(change_callback_called == false);

    REQUIRE(test == false);
  }
}


/**************************************************************************************/

void force_cloud_sync_callback(ArduinoCloudProperty<bool> property)
{
  CLOUD_WINS(property);
  sync_callback_called = true;
}

SCENARIO("After a connection/reconnection an incoming cbor payload is processed and the synchronization callback is executed. The sync callback applies the FORCE_CLOUD_SYNC policy (the property always assumes the value incoming from the broker message). The onUpdate function is executed only if the local value of the property was different from the one taken from the incoming message")
{
  GIVEN("CloudProtocol::V2")
  {

    test = false;
    sync_callback_called = false;
    change_callback_called = false;

    ArduinoCloudThing thing;
    thing.begin();

    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(change_callback).onSync(force_cloud_sync_callback);

    /* [{"bt": 1550138810.00, 0: "test", 4: true}] = 81 A3 62 62 74 FB 41 D7 19 4F 6E 80 00 00 00 64 74 65 73 74 04 F5 */
    uint8_t const payload[] = {0x81 ,0xA3, 0x62, 0x62, 0x74, 0xFB, 0x41, 0xD7, 0x19, 0x4F, 0x6E, 0x80, 0x00, 0x00, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length, true);

    REQUIRE(sync_callback_called == true);
    REQUIRE(change_callback_called == true);

    REQUIRE(test == true);
  }
}

/**************************************************************************************/

SCENARIO("After a connection/reconnection an incoming cbor payload is processed. Any synchronization function is passed to the property so the value in the incoming message is discarded")
{
  GIVEN("CloudProtocol::V2")
  {

    test = false;
    sync_callback_called = false;
    change_callback_called = false;

    ArduinoCloudThing thing;
    thing.begin();

    thing.addPropertyReal(test, "test", Permission::ReadWrite).onUpdate(change_callback);

    /* [{"bt": 1550138810.00, 0: "test", 4: true}] = 81 A3 62 62 74 FB 41 D7 19 4F 6E 80 00 00 00 64 74 65 73 74 04 F5 */
    uint8_t const payload[] = {0x81 ,0xA3, 0x62, 0x62, 0x74, 0xFB, 0x41, 0xD7, 0x19, 0x4F, 0x6E, 0x80, 0x00, 0x00, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5};
    int const payload_length = sizeof(payload)/sizeof(uint8_t);
    thing.decode(payload, payload_length, true);

    REQUIRE(sync_callback_called == false);
    REQUIRE(change_callback_called == false);

    REQUIRE(test == false);
  }
}
