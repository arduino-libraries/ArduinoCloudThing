/*
   To run this test suite, you must first install the ArduinoUnit library
   to your Arduino/libraries/ directory.
   You can get ArduinoUnit here: https://github.com/mmurdoch/arduinounit
   Download version 2.0 or greater or install it via the Arduino library manager.
*/

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <ArduinoUnit.h>
#include <ArduinoCloudThing.h>

/******************************************************************************
 * DEFINE
 ******************************************************************************/

#if defined(ARDUINO_ARCH_MRAA)
#define Serial DebugSerial
#endif

/******************************************************************************
 * SETUP/LOOP
 ******************************************************************************/

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Test::out = &Serial;
  //Test::exclude("*");
  //Test::include("shouldNotDecode");
}

void loop()
{
  Test::run();

#if defined(ARDUINO_ARCH_MRAA)
  if (Test::remaining() == 0) {
    exit(Test::getCurrentFailed());
  }
#endif
}

/******************************************************************************
 * TEST HELPER CODE
 ******************************************************************************/

bool isEqual(uint8_t const * expected, int const expected_length, uint8_t const * actual, int const actual_length)
{
  if(expected_length != actual_length) return false;
  
  for(int b = 0; b < actual_length; b++)
  {
    if(expected[b] != actual[b]) return false;
  }

  return true;
}

void debugPrint(uint8_t const * buf, int const len)
{
  for(int b=0; b < len; b++)
  {
    Serial.print(buf[b], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

/******************************************************************************
 * ENCODE TESTS
 ******************************************************************************/

test(encodeStatusProperty) /* which is transmitted upon the first call to encode and indicates that the 'thing' is ON */
{
  ArduinoCloudThing thing;
  thing.begin();
  
  uint8_t actual[200] = {0};
  int const bytes_actual = thing.encode(actual, 200);

 /* http://cbor.me
  * [{0: "status", 4: true}] = 81 BF 00 66 73 74 61 74 75 73 04 F5 FF
  */

  uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x04, 0xF5, 0xFF};
  int const bytes_expected = sizeof(expected)/sizeof(uint8_t);

  assertTrue(isEqual(expected, bytes_expected, actual, bytes_actual));
}

/******************************************************************************/

test(encodeBoolProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t actual[200];
  thing.encode(actual, 200); /* Get the 'status' property encoded and out of the way of the test */
  memset(actual, 0, 200);

  bool test_1 = true;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  int const bytes_actual = thing.encode(actual, 200);

  /* http://cbor.me/
   * [{0: "test", 4: true}] = 81 BF 00 64 74 65 73 74 04 F5 FF
   */
  uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5, 0xFF};
  uint8_t const bytes_expected = sizeof(expected) / sizeof(uint8_t);

  assertTrue(isEqual(expected, bytes_expected, actual, bytes_actual));
}

/******************************************************************************/

test(encodeIntProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t actual[200];
  thing.encode(actual, 200); /* Get the 'status' property encoded and out of the way of the test */
  memset(actual, 0, 200);

  int test_1 = 123;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  int const bytes_actual = thing.encode(actual, 200);

  /* http://cbor.me/
   * [{0: "test", 3: 123}] = 81 BF 00 64 74 65 73 74 02 18 7B FF
   */
  uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x18, 0x7B, 0xFF};
  uint8_t const bytes_expected = sizeof(expected) / sizeof(uint8_t);

  assertTrue(isEqual(expected, bytes_expected, actual, bytes_actual));
}

/******************************************************************************/

test(encodeFloatProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t actual[200];
  thing.encode(actual, 200); /* Get the 'status' property encoded and out of the way of the test */
  memset(actual, 0, 200);

  float test_1 = 3.14159;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  int const bytes_actual = thing.encode(actual, 200);

  /* http://cbor.me/
   * [{0: "test", 2: 3.141590118408203}] = 81 BF 00 64 74 65 73 74 02 FA 40 49 0F D0 FF
   */
  uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0xFA, 0x40, 0x49, 0x0F, 0xD0, 0xFF};
  uint8_t const bytes_expected = sizeof(expected) / sizeof(uint8_t);

  assertTrue(isEqual(expected, bytes_expected, actual, bytes_actual));
}

/******************************************************************************/

test(encodeStringProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t actual[200];
  thing.encode(actual, 200); /* Get the 'status' property encoded and out of the way of the test */
  memset(actual, 0, 200);

  String s = "test";
  thing.addPropertyReal(s, "test", Permission::ReadWrite);

  int const bytes_actual = thing.encode(actual, 200);

  /* http://cbor.me/
   * [{0: "test", 3: "test"}] = 81 BF 00 64 74 65 73 74 03 64 74 65 73 74 FF
   */
  uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x03, 0x64, 0x74, 0x65, 0x73, 0x74, 0xFF};
  uint8_t const bytes_expected = sizeof(expected) / sizeof(uint8_t);

  assertTrue(isEqual(expected, bytes_expected, actual, bytes_actual));
}

test(encodeMultipleProperties)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t actual[200] = {0};

  float test_2 = 10.0f;
  int test_1 = 10;
  bool stuff = false;
  String otherStuff = "weyyyy";

  thing.addPropertyReal(test_2, "test_2", Permission::ReadWrite);
  thing.addPropertyReal(stuff, "stuff", Permission::ReadWrite);
  thing.addPropertyReal(test_1, "test_1", Permission::ReadWrite);
  thing.addPropertyReal(otherStuff, "otherStuff", Permission::ReadWrite);

  int const bytes_actual = thing.encode(actual, 200);

  /* http://cbor.me/
   * [{0: "status", 4: true}, 
   *  {0: "stuff", 4: false}, 
   *  {0: "test_1", 2: 10}, 
   *  {0: "test_2", 2: 10.0}, 
   *  {0: "otherStuff", 3: "weyyyy"}] = ...
   */
  uint8_t const expected[] = {0x85, 0xBF, 0x0, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x4, 0xF5, 0xFF, 0xBF, 0x0, 0x65, 0x73, 0x74, 0x75, 0x66, 0x66, 0x4, 0xF4, 0xFF, 0xBF, 0x0, 0x66, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x31, 0x2, 0xA, 0xFF, 0xBF, 0x0, 0x66, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x32, 0x2, 0xFA, 0x41, 0x20, 0x0, 0x0, 0xFF, 0xBF, 0x0, 0x6A, 0x6F, 0x74, 0x68, 0x65, 0x72, 0x53, 0x74, 0x75, 0x66, 0x66, 0x3, 0x66, 0x77, 0x65, 0x79, 0x79, 0x79, 0x79, 0xFF};
  int const bytes_expected = sizeof(expected)/sizeof(uint8_t);

  assertTrue(isEqual(expected, bytes_expected, actual, bytes_actual));
}

/******************************************************************************
 * DECODE TESTS
 ******************************************************************************/

test(decodeBoolProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  bool test_1 = true;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  /* http://cbor.me/
   * [{"n": "test", "vb": false}] = 81 A2 61 6E 64 74 65 73 74 62 76 62 F4 
   */
   
  uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF4};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertTrue(test_1 == false);
}

/******************************************************************************/
test(decodeIntProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  /* http://cbor.me/
   * [{"n": "test", "v": 7}] = 81 A2 61 6E 64 74 65 73 74 61 76 07
   */
   
  uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertEqual(test_1, 7);
}

/******************************************************************************/

test(decodeFloatProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  float test_1 = 0.0f;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  /* http://cbor.me/
   * [{"n": "test", "v": 3.1459}] = 81 A2 61 6E 64 74 65 73 74 61 76 FB 40 09 2A CD 9E 83 E4 26
   */
   
  uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x09, 0x2A, 0xCD, 0x9E, 0x83, 0xE4, 0x26};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertNear(test_1, 3.1459, 0.01);
}

/******************************************************************************/

test(decodeStringProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  String str = "test";
  thing.addPropertyReal(str, "test", Permission::ReadWrite);

  /* http://cbor.me/
   * [{"n": "test", "vs": "testtt"}] = 9F BF 61 6E 64 74 65 73 74 62 76 73 66 74 65 73 74 74 74 FF FF
   */
  uint8_t const payload[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74, 0xFF, 0xFF};
  thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

  assertEqual(str, "testtt");
}

test(decodeMultipleProperties)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_uint = 0, test_sint = 0;
  float test_float = 0.0;
  bool test_bool = false;
  String test_string = "xxx";

  thing.addPropertyReal(test_uint, "test_uint", Permission::ReadWrite);
  thing.addPropertyReal(test_sint, "test_sint", Permission::ReadWrite);
  thing.addPropertyReal(test_float, "test_float", Permission::ReadWrite);
  thing.addPropertyReal(test_bool, "test_bool", Permission::ReadWrite);
  thing.addPropertyReal(test_string, "test_string", Permission::ReadWrite);

  /* http://cbor.me/
   * [{"n": "test_uint", "v": 4},
   *  {"n": "test_sint", "v": -4},
   *  {"n": "test_float", "v": 4.5},
   *  {"n": "test_bool", "vb": true},
   *  {"n": "test_string", "vs": "test value"}] = ...
   */
  uint8_t const payload[] = {0x85, 0xA2, 0x61, 0x6E, 0x69, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x75, 0x69, 0x6E, 0x74, 0x61, 0x76, 0x04, 0xA2, 0x61, 0x6E, 0x69, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x73, 0x69, 0x6E, 0x74, 0x61, 0x76, 0x23, 0xA2, 0x61, 0x6E, 0x6A, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x61, 0x76, 0xF9, 0x44, 0x80, 0xA2, 0x61, 0x6E, 0x69, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x62, 0x6F, 0x6F, 0x6C, 0x62, 0x76, 0x62, 0xF5, 0xA2, 0x61, 0x6E, 0x6B, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x62, 0x76, 0x73, 0x6A, 0x74, 0x65, 0x73, 0x74, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertEqual(test_uint, 4);
  assertEqual(test_sint, -4);
  assertEqual(test_float, 4.5);
  assertEqual(test_bool, true);
  assertEqual(test_string.c_str(), "test value");
}

/******************************************************************************
 * DECODER supplied with invalid data
 ******************************************************************************/

test(testDecodeWhenInvalidDataFormatViolated)
{
  ArduinoCloudThing thing;
  thing.begin();

  float test = 3.14159268;
  thing.addPropertyReal(test, "test", Permission::ReadWrite);

  /* Test #1: received CBOR is a float value
   * http://cbor.me/ 
   * 2.474512 = FB 4003CBCCF28C79F6 # primitive(4612754524460579318)
   */
  uint8_t const buf_1[] = {0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6};
  thing.decode(buf_1, sizeof(buf_1)/sizeof(uint8_t));
  assertNear(test, 3.14, 0.01);

  /* Test #2: received CBOR is an map and not an array
   * http://cbor.me/
   * {"n": "test", "v": 2.474512} = 
   * BF                    # map(*)
   *   61                  # text(1)
   *     6E                # "n"
   *   64                  # text(4)
   *     74657374          # "test"
   *   61                  # text(1)
   *     76                # "v"
   *   FB 4003CBCCF28C79F6 # primitive(4612754524460579318)
   *   FF                  # primitive(*)
   */
  uint8_t const buf_2[] = {0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0xFF};
  thing.decode(buf_2, sizeof(buf_2)/sizeof(uint8_t));
  assertNear(test, 3.14, 0.01);

  /* Test #3: received CBOR is an array of values
   * http://cbor.me/
   * [2.474512, 10, true, "ciao"] = 
   * 84                    # array(4)
   *   FB 4003CBCCF28C79F6 # primitive(4612754524460579318)
   *   0A                  # unsigned(10)
   *   F5                  # primitive(21)
   *   64                  # text(4)
   *     6369616F          # "ciao"
   */
  uint8_t const buf_3[] = {0x84, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0A, 0xF5, 0x64, 0x63, 0x69, 0x61, 0x6F, 0x0};
  thing.decode(buf_3, sizeof(buf_3)/sizeof(uint8_t));
  assertNear(test, 3.14, 0.01);
}

/******************************************************************************/

test(testDecodeWhenInvalidDataProtocolViolated)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test", Permission::Read);

  /* break stop code outside indefinite length item */
  uint8_t const buf_1[] = {0x81, 0xFF, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
  thing.decode(buf_1, sizeof(buf_1)/sizeof(uint8_t));

  /* break stop code outside indefinite length item */
  uint8_t const buf_2[] = {0x81, 0xA2, 0xFF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
  thing.decode(buf_2, sizeof(buf_2)/sizeof(uint8_t));

  assertEqual(test_1, 0);
}

/******************************************************************************
 * READ ONLY / WRITE ONLY TESTS
 ******************************************************************************/

test(readOnly)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test", Permission::Read);

  /* http://cbor.me/
   * [{"n": "test", "v": 7}] = 81 A2 61 6E 64 74 65 73 74 61 76 07
   */

  uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertEqual(test_1, 0);
}

/******************************************************************************/

test(writeOnly)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t actual[200];
  thing.encode(actual, 200); /* Get the 'status' property encoded and out of the way of the test */
  memset(actual, 0, 200);

  int test_1 = 10;
  thing.addPropertyReal(test_1, "test_1", Permission::Write);
  
  assertTrue(thing.encode(actual, 200) == 0);
}

/******************************************************************************
 * CALLBACK TESTS (onUpdate)
 ******************************************************************************/

static bool callback_called = false;
void externalCallback()
{
  callback_called = true;
}

test(testIfCallbackIsCalledWhenPropertyIsChanged)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 10;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite).onUpdate(externalCallback);
  
  /* http://cbor.me/
   * [{"n": "test", "v": 6}] =
   * 9F             # array(*)
   *   BF           # map(*)
   *     61         # text(1)
   *       6E       # "n"
   *     64         # text(4)
   *       74657374 # "test"
   *     61         # text(1)
   *       76       # "v"
   *     06         # unsigned(6)
   *     FF         # primitive(*)
   *   FF           # primitive(*)
   */

  uint8_t const payload[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x6, 0xFF, 0xFF};
  thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

  assertTrue(callback_called);
}

/******************************************************************************
 * TIME INTERVAL/ON CHANGE PUBLISHING TESTS (publishEvery, publishOnChange)
 ******************************************************************************/

test(publishEvery)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  bool    bool_property = true;
  
  unsigned long const PUBLISH_INTERVAL_SEC = 1 * SECONDS;
  
  thing.addPropertyReal(bool_property, "bool_property", Permission::ReadWrite).publishEvery(PUBLISH_INTERVAL_SEC);
  
  assertNotEqual(thing.encode(buf, 200), 0); /* Call 'encode' once since on the first call the data is always encoded */


  unsigned long start = millis();
  for(int i = 0; i < 3; i++) {
    /* If we have not reached the publish interval yet - no data should be generated in 'encode' */
    if((millis() - start) < (PUBLISH_INTERVAL_SEC * 1000)) {
      assertEqual(thing.encode(buf, 200), 0);
    }
    /* If we have reached the publish interval - no data should be generated in 'encode' */
    else {
      assertNotEqual(thing.encode(buf, 200), 0);
      start = millis();
    }
    delay(100);
  }
}


test(publishOnChange)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  int test_1 = 10;
  int delta = 6;

  thing.addPropertyReal(test_1, "test", Permission::ReadWrite).publishOnChange(delta);
  assertTrue(thing.encode(buf, 200) != 0); /* 1st time we always transmit data */

  test_1 += 4;
  assertTrue(thing.encode(buf, 200) == 0); /* test_1 = 14, delta to previous encoded change = 4 which is < 6, therefore no data shall be encoded */

  test_1 += 4;
  assertTrue(thing.encode(buf, 200) != 0); /* test_1 = 18, delta to previous encoded change = 8 which is > 6, therefore data shall be encoded */
}

/******************************************************************************/

test(publishOnChangeRateLimit)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t   buf[200];
  int       int_property = 0;
  int const min_delta    = 0;
  unsigned long const min_time_between_updates_ms = 200; /* No updates faster than 200 ms */

  thing.addPropertyReal(int_property, "int_property", Permission::ReadWrite).publishOnChange(min_delta, min_time_between_updates_ms);
  thing.encode(buf, 200); /* The first time encode is called we are relaying all data to the server for the first time - ergo it should not count */

  unsigned long start = millis();
  for(int i = 0; i < 50; i++) {
    int_property++;
    if((millis() - start) < min_time_between_updates_ms) {
      assertTrue(thing.encode(buf, 200) == 0);
    }
    else {
      assertTrue(thing.encode(buf, 200) != 0);
      start = millis();
    }
    delay(10);
  }
}

/******************************************************************************/
