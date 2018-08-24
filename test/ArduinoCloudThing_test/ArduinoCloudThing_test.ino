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
}

void loop()
{
  Test::run();
}

test(beginAddsStatusProperty)
{
  ArduinoCloudThing thing;
  thing.begin();
  unsigned char buf[200];
  memset(buf, 0, 200);
  thing.poll((uint8_t*)buf, 200);

  unsigned char expected[] = {0x9F, 0xBF, 0x61, 0x6E, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x61, 0x76, 0xF4, 0xFF, 0xFF, 0x0};
  assertEqual((char*)expected, (char*)buf);
}

test(addThingAndChangeValue)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  unsigned char buf[200];
  memset(buf, 0, 200);

  thing.addPropertyReal(test_1, "test").publishEvery(ON_CHANGE);
  thing.poll((uint8_t*)buf, 200);

  memset(buf, 0, 200);

  test_1 = 6;
  thing.poll((uint8_t*)buf, 200);

  unsigned char expected[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x6, 0xFF, 0xFF, 0x0};
  assertEqual((char*)expected, (char*)buf);

  test_1 = 7;
  thing.poll((uint8_t*)buf, 200);

  unsigned char expected2[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0xFF, 0xFF, 0x0};
  assertEqual((char*)expected2, (char*)buf);
}

test(decodeBuffer)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test");

  unsigned char buf[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0xFF, 0xFF, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual(test_1, 7);
}

test(decodeBufferShouldnUpdateIfReadonly)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test").readOnly();

  unsigned char buf[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0xFF, 0xFF, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual(test_1, 0);
}

test(intAndFloatDiffer)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  uint8_t buf2[200];

  int test_1 = 10;
  thing.addPropertyReal(test_1, "test");
  thing.poll((uint8_t*)buf, 200);


  float test_2 = 10.0f;
  thing.addPropertyReal(test_2, "test");
  thing.poll((uint8_t*)buf, 200);

  assertNotEqual((char*)buf, (char*)buf2);
}

test(stringProperty)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  thing.poll((uint8_t*)buf, 200);

  String s = "test";
  thing.addPropertyReal(s, "test");
  memset(buf, 0, 200);

  thing.poll((uint8_t*)buf, 200);

  unsigned char expected[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x64, 0x74, 0x65, 0x73, 0x74, 0xFF, 0xFF, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual((char*)buf, (char*)expected);

  unsigned char newstring[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74, 0xFF, 0xFF, 0x0};
  thing.decode((uint8_t*)newstring, sizeof(newstring));

  assertEqual(s, "testtt");
}

test(createaManyProperties)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];

  float test_2 = 10.0f;
  int test_1 = 10;
  bool stuff = false;
  String otherStuff = "weyyyy";

  thing.addPropertyReal(test_1, "test_1");
  thing.addPropertyReal(test_2, "test_2");
  thing.addPropertyReal(stuff, "stuff");
  thing.addPropertyReal(otherStuff, "otherStuff");

  int ret = thing.poll((uint8_t*)buf, 200);

  assertEqual(ret, 85);
}
