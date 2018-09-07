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

#if defined(ARDUINO_ARCH_MRAA)
  if (Test::remaining() == 0) {
    exit(Test::getCurrentFailed());
  }
#endif
}

test(beginAddsStatusProperty)
{
  ArduinoCloudThing thing;
  thing.begin();
  unsigned char buf[200];
  memset(buf, 0, 200);
  thing.poll((uint8_t*)buf, 200);

  unsigned char expected[] = {0x81, 0xA2, 0x61, 0x6E, 0x66, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x61, 0x76, 0xF4, 0x0};
  assertEqual((char*)expected, (char*)buf);
}

test(addThingAndChangeValue)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  unsigned char buf[200];
  memset(buf, 0, 200);

  thing.addPropertyReal(test_1, "test", READWRITE, ON_CHANGE);
  thing.poll((uint8_t*)buf, 200);

  memset(buf, 0, 200);

  test_1 = 6;
  thing.poll((uint8_t*)buf, 200);

#if 0
  for (int i = 0; buf[i] != 0; i++) {
    if (buf[i] < 16) {
      Serial.print("0");
    }
    Serial.print(buf[i] , HEX);
  }
  Serial.println();
#endif

  unsigned char expected[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x6, 0x0};
  assertEqual((char*)expected, (char*)buf);

  test_1 = 7;
  thing.poll((uint8_t*)buf, 200);

  unsigned char expected2[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  assertEqual((char*)expected2, (char*)buf);
}

test(decodeBuffer)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test");

  unsigned char buf[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
  thing.decode((uint8_t*)buf, sizeof(buf));

  assertEqual(test_1, 7);
}

test(decodeBufferShouldnUpdateIfReadonly)
{
  ArduinoCloudThing thing;
  thing.begin();

  int test_1 = 0;
  thing.addPropertyReal(test_1, "test", READ);

  unsigned char buf[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x7, 0x0};
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

  unsigned char expected[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x64, 0x74, 0x65, 0x73, 0x74, 0x0};
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
  memset(buf, 0, 200);

  float test_2 = 10.0f;
  int test_1 = 10;
  bool stuff = false;
  String otherStuff = "weyyyy";

  thing.addPropertyReal(test_2, "test_2");
  thing.addPropertyReal(stuff, "stuff");
  thing.addPropertyReal(test_1, "test_1");
  thing.addPropertyReal(otherStuff, "otherStuff");

  int ret = thing.poll((uint8_t*)buf, 200);

#if 0
  for (int i = 0; i < 85; i++) {
    if (buf[i] < 16) {
      Serial.print("0");
    }
    Serial.print(buf[i] , HEX);
  }
  Serial.println();
#endif

  assertEqual(ret, 79);
}


test(reportEvery)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];

  int test_1 = 10;

  thing.addPropertyReal(test_1, "test_1", READWRITE, 1 * SECONDS);
  int ret = thing.poll((uint8_t*)buf, 200);

  ret = thing.poll((uint8_t*)buf, 200);
  assertEqual(ret, 0);

  delay(2000);
  ret = thing.poll((uint8_t*)buf, 200);
  assertNotEqual(ret, 0);
}

test(writeOnly)
{
  ArduinoCloudThing thing;
  thing.begin();

  uint8_t buf[200];
  thing.poll((uint8_t*)buf, 200);

  int test_1 = 10;

  thing.addPropertyReal(test_1, "test_1", WRITE);
  int ret = thing.poll((uint8_t*)buf, 200);

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
  thing.poll((uint8_t*)buf, 200);

  int test_1 = 10;

  thing.addPropertyReal(test_1, "test", READWRITE, ON_CHANGE, externalCallback);

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

  thing.addPropertyReal(test_1, "test", READWRITE, ON_CHANGE, NULL, delta);
  thing.poll((uint8_t*)buf, 200);

  test_1 += 4;
  int ret = thing.poll((uint8_t*)buf, 200);
  assertEqual(ret, 0);

  test_1 += 4;
  ret = thing.poll((uint8_t*)buf, 200);
  assertNotEqual(ret, 0);
}
