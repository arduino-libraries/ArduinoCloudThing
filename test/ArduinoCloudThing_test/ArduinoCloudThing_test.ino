/*
   To run this test suite, you must first install the ArduinoUnit library
   to your Arduino/libraries/ directory.
   You can get ArduinoUnit here: https://github.com/mmurdoch/arduinounit
   Download version 2.0 or greater or install it via the Arduino library manager.
*/

#include <ArduinoUnit.h>
#include <ArduinoCloudThing.h>

#if defined(ARDUINO_ARCH_MRAA)
#define Serial DebugSerial
#endif

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

/*
  ENCODE TEST
*/
test(beginAddStatusProperty)
{
  ArduinoCloudThing thing;
  thing.begin();
  unsigned char buf[200];
  memset(buf, 0, 200);
  int ret = thing.encode((uint8_t*)buf, 200);

  unsigned char expected[] = {0x81, 0xBF, 0x61, 0x6E, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x62, 0x76, 0x62, 0xF5, 0xFF, 0x0};
  assertEqual((char*)expected, (char*)buf);
}

test(addThingAndChangeValue)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  unsigned char buf[200];
  memset(buf, 0, 200);

  thing.addProperty(test_1, "test", Permission::ReadWrite);
  thing.encode((uint8_t*)buf, 200);

  memset(buf, 0, 200);

  test_1 = 6;
  int ret = thing.encode((uint8_t*)buf, 200);

  unsigned const char expected[] = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x06, 0xFF, 0x00};
  // 13 is the lenght that this array must have ( 1 element array, containing indefinite lenght map)
  assertEqual(ret, 13);
  assertEqual((char*)expected, (char*)buf);

  test_1 = 7;
  ret = thing.encode((uint8_t*)buf, 200);

  unsigned char expected2[] = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07, 0xFF, 0x00};
  assertEqual(ret, 13);
  assertEqual((char*)expected2, (char*)buf);
}


/*
  DECODE TESTS
*/
test(decodeBuffer)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addProperty(test_1, "test", Permission::ReadWrite);

  unsigned char buf[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual(test_1, 7);
}

test(decodeProperties) {

  ArduinoCloudThing thing;
  thing.begin();

  int test_uint = 0, test_sint = 0;
  float test_float = 0.0;
  bool test_bool = false;
  String test_string = "xxx";

  thing.addProperty(test_uint, "test_uint", Permission::ReadWrite);
  thing.addProperty(test_sint, "test_sint", Permission::ReadWrite);
  thing.addProperty(test_float, "test_float", Permission::ReadWrite);
  thing.addProperty(test_bool, "test_bool", Permission::ReadWrite);
  thing.addProperty(test_string, "test_string", Permission::ReadWrite);

  unsigned char buf[] = {0x86,0xA3,0x62,0x62,0x74,0x1A,0x5B,0x98,0xD7,0x80,0x61,0x6E,0x69,0x74,0x65,0x73,0x74,0x5F,0x75,0x69,0x6E,0x74,0x61,0x76,0x04,0xA3,0x62,0x62,0x74,0x1A,0x5B,0x98,0xD7,0x80,0x61,0x6E,0x69,0x74,0x65,0x73,0x74,0x5F,0x73,0x69,0x6E,0x74,0x61,0x76,0x23,0xA3,0x62,0x62,0x74,0x1A,0x5B,0x98,0xD7,0x80,0x61,0x6E,0x6A,0x74,0x65,0x73,0x74,0x5F,0x66,0x6C,0x6F,0x61,0x74,0x61,0x76,0xFB,0x40,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0xA3,0x62,0x62,0x74,0x1A,0x5B,0x98,0xD7,0x80,0x61,0x6E,0x6B,0x74,0x65,0x73,0x74,0x5F,0x73,0x74,0x72,0x69,0x6E,0x67,0x62,0x76,0x73,0x6A,0x74,0x65,0x73,0x74,0x20,0x76,0x61,0x6C,0x75,0x65,0xA3,0x62,0x62,0x74,0x1A,0x5B,0x98,0xD7,0x80,0x61,0x6E,0x69,0x74,0x65,0x73,0x74,0x5F,0x62,0x6F,0x6F,0x6C,0x62,0x76,0x62,0xF5,0xA3,0x62,0x62,0x74,0x1A,0x5B,0x98,0xD7,0x80,0x61,0x6E,0x6B,0x74,0x65,0x73,0x74,0x5F,0x64,0x6F,0x75,0x62,0x6C,0x65,0x61,0x76,0xFB,0x7F,0xEF,0xFF,0xFC,0x57,0xCA,0x82,0xAE,0x00};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual(test_uint, 4);
  assertEqual(test_sint, -4);
  assertEqual(test_float, 4.5);
  assertEqual(test_bool, true);
  assertEqual(test_string.c_str(), "test value");
}

test(decodeBufferShouldnUpdateIfReadonly)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addProperty(test_1, "test", Permission::Read);

  unsigned char buf[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual(test_1, 0);
}

test(shouldNotDecode)
{
  ArduinoCloudThing thing;
  thing.begin();

  float test = 3.14159268;
  thing.addProperty(test, "test", Permission::ReadWrite);

  // received CBOR is a float value
  unsigned char buf_1[] = {0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0};
  thing.decode((uint8_t*)buf_1, sizeof(buf_1));
  assertNear(test, 3.14, 0.01);

  // received CBOR is an map and not an array
  unsigned char buf_2[] = {0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0xFF, 0x0};
  thing.decode((uint8_t*)buf_2, sizeof(buf_2));
  assertNear(test, 3.14, 0.01);

  // recived CBOR is an array of values
  unsigned char buf_3[] = {0x84, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0A, 0xF5, 0x64, 0x63, 0x69, 0x61, 0x6F, 0x0};
  thing.decode((uint8_t*)buf_3, sizeof(buf_3));
  assertNear(test, 3.14, 0.01);

/*
  // recived CBOR is an array of values with a correct map
  unsigned char buf_4[] = {0x85, 0x0A, 0x17, 0x63, 0x61, 0x62, 0x63, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0xF4, 0x0};
  thing.decode((uint8_t*)buf_4, sizeof(buf_4));
  assertNear(test, 2.47, 0.01);
*/

  unsigned char buf_4[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x03, 0xCB, 0xCC, 0xF2, 0x8C, 0x79, 0xF6, 0x0};
  thing.decode((uint8_t*)buf_4, sizeof(buf_4));
  assertNear(test, 2.47, 0.01);

}


test(decodeShouldNotHang)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addProperty(test_1, "test", Permission::Read);

  unsigned char buf_1[] = {0x81, 0xff, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  thing.decode((uint8_t*)buf_1, sizeof(buf_1));

  unsigned char buf_2[] = {0x81, 0xA2, 0xff, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  thing.decode((uint8_t*)buf_2, sizeof(buf_2));

/*
  unsigned char buf_3[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0xff, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  thing.decode((uint8_t*)buf_3, sizeof(buf_3));
  Serial.println("c");
*/
  assertEqual(test_1, 0);
}


/* DATA TYPES TESTS */
test(intAndFloatDiffer)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  uint8_t buf2[200];

  int test_1 = 10;
  thing.addProperty(test_1, "test", Permission::ReadWrite);
  thing.encode((uint8_t*)buf, 200);


  float test_2 = 10.0f;
  thing.addProperty(test_2, "test", Permission::ReadWrite);
  thing.encode((uint8_t*)buf, 200);

  assertNotEqual((char*)buf, (char*)buf2);
}

test(stringProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  thing.encode((uint8_t*)buf, 200);

  String s = "test";
  thing.addProperty(s, "test", Permission::ReadWrite);
  memset(buf, 0, 200);
  int ret = thing.encode((uint8_t*)buf, 200);

  unsigned char expected[] = {0x81, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x64, 0x74, 0x65, 0x73, 0x74, 0xFF, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual((char*)buf, (char*)expected);

  unsigned char newstring[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74, 0xFF, 0xFF, 0x0};
  thing.decode((uint8_t*)newstring, sizeof(newstring));

  assertEqual(s, "testtt");
}

test(createaManyProperties)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  memset(buf, 0, 200);

  float test_2 = 10.0f;
  int test_1 = 10;
  bool stuff = false;
  String otherStuff = "weyyyy";

  thing.addProperty(test_2, "test_2", Permission::ReadWrite);
  thing.addProperty(stuff, "stuff", Permission::ReadWrite);
  thing.addProperty(test_1, "test_1", Permission::ReadWrite);
  thing.addProperty(otherStuff, "otherStuff", Permission::ReadWrite);

  int ret = thing.encode((uint8_t*)buf, 200);
  assertEqual(ret, 87);
}

/*
  PROPERTIES BUSINESS LOGIC TESTS
*/
test(reportEvery)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];

  int test_1 = 10;

  thing.addProperty(test_1, "test_1", Permission::ReadWrite).publishEvery(1 * SECONDS);
  int ret = thing.encode((uint8_t*)buf, 200);

  ret = thing.encode((uint8_t*)buf, 200);
  assertEqual(ret, 0);

  delay(2000);
  ret = thing.encode((uint8_t*)buf, 200);
  assertNotEqual(ret, 0);
}

test(writeOnly)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  thing.encode((uint8_t*)buf, 200);

  int test_1 = 10;

  thing.addProperty(test_1, "test_1", Permission::Write);
  int ret = thing.encode((uint8_t*)buf, 200);

  assertEqual(ret, 0);
}

static int globalVal = 0;
void externalCallback() {
  globalVal = 0xAA;
}

test(callback)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  thing.encode((uint8_t*)buf, 200);

  int test_1 = 10;

  thing.addProperty(test_1, "test", Permission::ReadWrite).onUpdate(externalCallback);

  globalVal = 0;
  unsigned char expected[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x6, 0xFF, 0xFF, 0x0};
  thing.decode(expected, sizeof(expected));

  assertEqual(globalVal, 0xAA);
}

test(minimumDelta)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  int test_1 = 10;
  int delta = 6;

  thing.addProperty(test_1, "test", Permission::ReadWrite).publishOnChange(delta);
  thing.encode((uint8_t*)buf, 200);

  test_1 += 4;
  int ret = thing.encode((uint8_t*)buf, 200);
  assertEqual(ret, 0);

  test_1 += 4;
  ret = thing.encode((uint8_t*)buf, 200);
  assertNotEqual(ret, 0);
}

test(testRateLimitWhenPublishOnChange)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t   buf[200];
  int       int_property = 0;
  int const min_delta    = 0;
  unsigned long const min_time_between_updates_ms = 200; /* No updates faster than 200 ms */

  thing.addProperty(int_property, "int_property", Permission::ReadWrite).publishOnChange(min_delta, min_time_between_updates_ms);
  thing.encode(buf, 200); /* The first time encode is called we are relaying all data to the server for the first time - ergo it should not count */

  unsigned long start = millis();
  for(int i = 0; i < 500; i++) {
    int_property++;
    unsigned long current = millis();
    int const bytes_encoded = thing.encode(buf, 200);
    if(current < min_time_between_updates_ms) {
      assertEqual(bytes_encoded, 0);
    } else {
      assertNotEqual(bytes_encoded, 0);
      start = millis();
    }
  }
}
