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
 * TEST CODE
 ******************************************************************************/

/*
 * ENCODE TEST
 */
test(beginAddStatusProperty)
{
  ArduinoCloudThing thing;
  thing.begin();
  
  uint8_t actual[200] = {0};
  int const bytes_encoded = thing.encode(actual, 200);

 /* http://cbor.me
  * [{0: "status", 4: true}] = 
  * 81                 # array(1)
  *   BF               # map(*)
  *     00             # unsigned(0)
  *     66             # text(6)
  *       737461747573 # "status"
  *    04              # unsigned(4)
  *    F5              # primitive(21)
  *    FF              # primitive(*)
  */

  uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x04, 0xF5, 0xFF};
  int const bytes_expected = sizeof(expected)/sizeof(uint8_t);

  assertEqual(bytes_expected, bytes_encoded);
  for(uint8_t b = 0; b < bytes_encoded; b++)
  {
    assertEqual(expected[b], actual[b]);
  }
}

/******************************************************************************/

test(addThingAndChangeValue)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  uint8_t actual[200] = {0};

  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);
  thing.encode(actual, 200); /* In this first call to encode the 'status' property will be encoded - see testcase 'beginAddStatusProperty' */

  {
    memset(actual, 0, 200);
    test_1 = 6;
    int const bytes_encoded = thing.encode(actual, 200);
  
    /* http://cbor.me
     * [{0: "test", 2: 6}] =
     * 81             # array(1)
     *   BF           # map(*)
     *     00         # unsigned(0)
     *     64         # text(4)
     *       74657374 # "test"
     *     02         # unsigned(2)
     *     06         # unsigned(6)
     *     FF         # primitive(*)
     */
  
    uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x06, 0xFF};
    int bytes_expected = sizeof(expected)/sizeof(uint8_t);
  
    assertEqual(bytes_expected, bytes_encoded);
    for(uint8_t b = 0; b < bytes_encoded; b++)
    {
      assertEqual(expected[b], actual[b]);
    }
  }
  
  {
    memset(actual, 0, 200);
    test_1 = 7;
    int const bytes_encoded = thing.encode(actual, 200);
  
    /* http://cbor.me
     * [{0: "test", 2: 7}] =
     * 81              # array(1)
     *   BF            # map(*)
     *     00          # unsigned(0)
     *     64          # text(4)
     *       74657374  # "test"
     *     02          # unsigned(2)
     *     07          # unsigned(7)
     *     FF          # primitive(*)
     */
  
    uint8_t const expected[] = {0x81, 0xBF, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x07, 0xFF};
    int bytes_expected = sizeof(expected)/sizeof(uint8_t);
  
    assertEqual(bytes_expected, bytes_encoded);
    for(uint8_t b = 0; b < bytes_encoded; b++)
    {
      assertEqual(expected[b], actual[b]);
    }
  }
}

/******************************************************************************/

/*
 * DECODE TESTS
 */
test(decodeBuffer)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);

  /* http://cbor.me/
   * [{"n": "test", "v": 7}] =
   * 81           # array(1)
   *   A2         # map(2)
   *   61         # text(1)
   *     6E       # "n"
   *   64         # text(4)
   *     74657374 # "test"
   *  61          # text(1)
   *     76       # "v"
   *  07          # unsigned(7)
   */
   
  uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertEqual(test_1, 7);
}

/******************************************************************************/

test(decodeProperties)
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
  uint8_t payload[] = {0x85, 0xA2, 0x61, 0x6E, 0x69, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x75, 0x69, 0x6E, 0x74, 0x61, 0x76, 0x04, 0xA2, 0x61, 0x6E, 0x69, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x73, 0x69, 0x6E, 0x74, 0x61, 0x76, 0x23, 0xA2, 0x61, 0x6E, 0x6A, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x61, 0x76, 0xF9, 0x44, 0x80, 0xA2, 0x61, 0x6E, 0x69, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x62, 0x6F, 0x6F, 0x6C, 0x62, 0x76, 0x62, 0xF5, 0xA2, 0x61, 0x6E, 0x6B, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x62, 0x76, 0x73, 0x6A, 0x74, 0x65, 0x73, 0x74, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65};
  int const payload_length = sizeof(payload)/sizeof(uint8_t);
  thing.decode(payload, payload_length);

  assertEqual(test_uint, 4);
  assertEqual(test_sint, -4);
  assertEqual(test_float, 4.5);
  assertEqual(test_bool, true);
  assertEqual(test_string.c_str(), "test value");
}

//test(decodeBufferShouldnUpdateIfReadonly)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  int test_1 = 0;
//  thing.addPropertyReal(test_1, "test", Permission::Read);
//
//  uint8_t buf[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
//  thing.decode(buf, sizeof(buf));
//
//  assertEqual(test_1, 0);
//}
//
//test(shouldNotDecode)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  float test = 3.14159268;
//  thing.addPropertyReal(test, "test", Permission::ReadWrite);
//
//  // received CBOR is a float value
//  uint8_t buf_1[] = {0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0};
//  thing.decode(buf_1, sizeof(buf_1));
//  assertNear(test, 3.14, 0.01);
//
//  // received CBOR is an map and not an array
//  uint8_t buf_2[] = {0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0xFF, 0x0};
//  thing.decode(buf_2, sizeof(buf_2));
//  assertNear(test, 3.14, 0.01);
//
//  // recived CBOR is an array of values
//  uint8_t buf_3[] = {0x84, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0A, 0xF5, 0x64, 0x63, 0x69, 0x61, 0x6F, 0x0};
//  thing.decode(buf_3, sizeof(buf_3));
//  assertNear(test, 3.14, 0.01);
//
///*
//  // recived CBOR is an array of values with a correct map
//  uint8_t buf_4[] = {0x85, 0x0A, 0x17, 0x63, 0x61, 0x62, 0x63, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0xF4, 0x0};
//  thing.decode(buf_4, sizeof(buf_4));
//  assertNear(test, 2.47, 0.01);
//*/
//
//  uint8_t buf_4[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0};
//  thing.decode(buf_4, sizeof(buf_4));
//  assertNear(test, 2.47, 0.01);
//
//}
//
//
//test(decodeShouldNotHang)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  int test_1 = 0;
//  thing.addPropertyReal(test_1, "test", Permission::Read);
//
//  uint8_t buf_1[] = {0x81, 0xff, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
//  thing.decode(buf_1, sizeof(buf_1));
//
//  uint8_t buf_2[] = {0x81, 0xA2, 0xff, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
//  thing.decode(buf_2, sizeof(buf_2));
//
///*
//  uint8_t buf_3[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0xff, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
//  thing.decode(buf_3, sizeof(buf_3));
//  Serial.println("c");
//*/
//  assertEqual(test_1, 0);
//}
//
//
///* DATA TYPES TESTS */
//test(intAndFloatDiffer)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  uint8_t buf2[200];
//
//  int test_1 = 10;
//  thing.addPropertyReal(test_1, "test", Permission::ReadWrite);
//  thing.encode(buf, 200);
//
//
//  float test_2 = 10.0f;
//  thing.addPropertyReal(test_2, "test", Permission::ReadWrite);
//  thing.encode(buf, 200);
//
//  assertNotEqual((char*)buf, (char*)buf2);
//}
//
//test(stringProperty)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  thing.encode(buf, 200);
//
//  String s = "test";
//  thing.addPropertyReal(s, "test", Permission::ReadWrite);
//  memset(buf, 0, 200);
//  int ret = thing.encode(buf, 200);
//
//  uint8_t expected[] = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x64, 0x74, 0x65, 0x73, 0x74, 0xFF, 0x0};
//  thing.decode(buf, sizeof(buf));
//
//  assertEqual((char*)buf, (char*)expected);
//
//  uint8_t newstring[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74, 0xFF, 0xFF, 0x0};
//  thing.decode((uint8_t*)newstring, sizeof(newstring));
//
//  assertEqual(s, "testtt");
//}
//
//test(createaManyProperties)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  memset(buf, 0, 200);
//
//  float test_2 = 10.0f;
//  int test_1 = 10;
//  bool stuff = false;
//  String otherStuff = "weyyyy";
//
//  thing.addPropertyReal(test_2, "test_2", Permission::ReadWrite);
//  thing.addPropertyReal(stuff, "stuff", Permission::ReadWrite);
//  thing.addPropertyReal(test_1, "test_1", Permission::ReadWrite);
//  thing.addPropertyReal(otherStuff, "otherStuff", Permission::ReadWrite);
//
//  int ret = thing.encode(buf, 200);
//  assertEqual(ret, 87);
//}
//
///*
//  PROPERTIES BUSINESS LOGIC TESTS
//*/
//test(reportEvery)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//
//  int test_1 = 10;
//
//  thing.addPropertyReal(test_1, "test_1", Permission::ReadWrite).publishEvery(1 * SECONDS);
//  int ret = thing.encode(buf, 200);
//
//  ret = thing.encode(buf, 200);
//  assertEqual(ret, 0);
//
//  delay(2000);
//  ret = thing.encode(buf, 200);
//  assertNotEqual(ret, 0);
//}
//
//test(writeOnly)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  thing.encode(buf, 200);
//
//  int test_1 = 10;
//
//  thing.addPropertyReal(test_1, "test_1", Permission::Write);
//  int ret = thing.encode(buf, 200);
//
//  assertEqual(ret, 0);
//}
//
//static bool callback_called = false;
//void externalCallback() {
//  callback_called = true;
//}
//
//test(testIfCallbackIsCalledWhenPropertyIsChanged)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  thing.encode(buf, 200);
//
//  int test_1 = 10;
//
//  thing.addPropertyReal(test_1, "test", Permission::ReadWrite).onUpdate(externalCallback);
//
//  uint8_t expected[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x6, 0xFF, 0xFF, 0x0};
//  thing.decode(expected, sizeof(expected));
//
//  assertTrue(callback_called);
//}
//
//test(testMinimumDeltaWhenPublishOnChange)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  int test_1 = 10;
//  int delta = 6;
//
//  thing.addPropertyReal(test_1, "test", Permission::ReadWrite).publishOnChange(delta);
//  int ret = thing.encode(buf, 200);
//  assertNotEqual(ret, 0);
//
//  test_1 += 4;
//  ret = thing.encode(buf, 200);
//  assertEqual(ret, 0);
//
//  test_1 += 4;
//  ret = thing.encode(buf, 200);
//  assertNotEqual(ret, 0);
//}
//
//test(testWhenPublishOnChangeIfRateLimitWorks)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t   buf[200];
//  int       int_property = 0;
//  int const min_delta    = 0;
//  unsigned long const min_time_between_updates_ms = 200; /* No updates faster than 200 ms */
//
//  thing.addPropertyReal(int_property, "int_property", Permission::ReadWrite).publishOnChange(min_delta, min_time_between_updates_ms);
//  thing.encode(buf, 200); /* The first time encode is called we are relaying all data to the server for the first time - ergo it should not count */
//
//  unsigned long start = millis();
//  for(int i = 0; i < 50; i++) {
//    int_property++;
//    if((millis() - start) < min_time_between_updates_ms) {
//      assertEqual(thing.encode(buf, 200), 0);
//    }
//    else {
//      assertNotEqual(thing.encode(buf, 200), 0);
//      start = millis();
//    }
//    delay(10);
//  }
//}
//
//test(testWhenPublishEveryNSecondsIfDataIsReallyOnlyPublishedEveryNSeconds)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  bool    bool_property = true;
//  
//  unsigned long const PUBLISH_INTERVAL_SEC = 1;
//  
//  thing.addPropertyReal(bool_property, "bool_property", Permission::ReadWrite).publishEvery(PUBLISH_INTERVAL_SEC);
//  
//  assertNotEqual(thing.encode(buf, 200), 0); /* Call 'encode' once since on the first call the data is always encoded */
//
//
//  unsigned long start = millis();
//  for(int i = 0; i < 3; i++) {
//    /* If we have not reached the publish interval yet - no data should be generated in 'encode' */
//    if((millis() - start) < (PUBLISH_INTERVAL_SEC * 1000)) {
//      assertEqual(thing.encode(buf, 200), 0);
//    }
//    /* If we have reached the publish interval - no data should be generated in 'encode' */
//    else {
//      assertNotEqual(thing.encode(buf, 200), 0);
//      start = millis();
//    }
//    delay(100);
//  }
//}
//
//test(testWhenPublishOnChangeIfPropertyIsNotChangedValueShouldBeEncodedOnlyOnceAtStartup)
//{
//  ArduinoCloudThing thing;
//  thing.begin();
//
//  uint8_t buf[200];
//  bool    bool_property = true;
//  
//  thing.addPropertyReal(bool_property, "bool_property", Permission::ReadWrite);
//  
//  assertNotEqual(thing.encode(buf, 200), 0); /* This is the 1st call to 'encode' - therefore we need to send data to the cloud */
//  assertEqual   (thing.encode(buf, 200), 0); /* This is the 2nd call to 'encode' - since the bool_property did not change we do NOT need to send data to the cloud */
//  assertEqual   (thing.encode(buf, 200), 0); /* This is the 3rd call to 'encode' - since the bool_property did not change we do NOT need to send data to the cloud */
//}
