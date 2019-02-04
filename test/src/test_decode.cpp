/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

SCENARIO("Arduino Cloud Properties are decoded", "[ArduinoCloudThing::decode]")
{
  /************************************************************************************/

  WHEN("A boolean property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      bool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "vb": false}] = 81 A2 61 6E 64 74 65 73 74 62 76 62 F4 */
      uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF4};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == false);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      bool test = true;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{0: "test", 4: false}] = 81 A2 00 64 74 65 73 74 04 F4 */
      uint8_t const payload[] = {0x81, 0xA2, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF4};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == false);
    }
  }

  /************************************************************************************/

  WHEN("A int property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "v": 7}] = 81 A2 61 6E 64 74 65 73 74 61 76 07 */
      uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x07};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == 7);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{0: "test", 2: 7}] = 81 A2 00 64 74 65 73 74 02 07 */
      uint8_t const payload[] = {0x81, 0xA2, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x07};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == 7);
    }
  }

  /************************************************************************************/

  WHEN("A float property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      float test = 0.0f;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"n": "test", "v": 3.1459}] = 81 A2 61 6E 64 74 65 73 74 61 76 FB 40 09 2A CD 9E 83 E4 26 */
      uint8_t const payload[] = {0x81, 0xA2, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xFB, 0x40, 0x09, 0x2A, 0xCD, 0x9E, 0x83, 0xE4, 0x26};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == Approx(3.1459).epsilon(0.01));
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      float test = 0.0f;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{0: "test", 2: 3.1459}] = 81 A2 00 64 74 65 73 74 02 FB 40 09 2A CD 9E 83 E4 26 */
      uint8_t const payload[] = {0x81, 0xA2, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0xFB, 0x40, 0x09, 0x2A, 0xCD, 0x9E, 0x83, 0xE4, 0x26};
      int const payload_length = sizeof(payload)/sizeof(uint8_t);
      thing.decode(payload, payload_length);

      REQUIRE(test == Approx(3.1459).epsilon(0.01));
    }
  }

  /************************************************************************************/

  WHEN("A String property is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      String str = "test";
      thing.addPropertyReal(str, "test", Permission::ReadWrite);

      /* [{"n": "test", "vs": "testtt"}] = 9F BF 61 6E 64 74 65 73 74 62 76 73 66 74 65 73 74 74 74 FF FF */
      uint8_t const payload[] = {0x9F, 0xBF, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74, 0xFF, 0xFF};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(str == "testtt");
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      String str = "test";
      thing.addPropertyReal(str, "test", Permission::ReadWrite);

      /* [{0: "test", 3: "testtt"}] = 81 A2 00 64 74 65 73 74 03 66 74 65 73 74 74 74 */
      uint8_t const payload[] = {0x81, 0xA2, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x03, 0x66, 0x74, 0x65, 0x73, 0x74, 0x74, 0x74};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(str == "testtt");
    }
  }

  /************************************************************************************/

  WHEN("Multiple properties is changed via CBOR message")
  {
    GIVEN("CloudProtocol::V1")
    {
      WHEN("Multiple properties of different type are changed via CBOR message")
      {
        ArduinoCloudThing thing(CloudProtocol::V1);
        thing.begin();

        bool   bool_test = false;
        int    int_test = 1;
        float  float_test = 2.0f;
        String str_test("str_test");

        thing.addPropertyReal(bool_test,  "bool_test",  Permission::ReadWrite);
        thing.addPropertyReal(int_test,   "int_test",   Permission::ReadWrite);
        thing.addPropertyReal(float_test, "float_test", Permission::ReadWrite);
        thing.addPropertyReal(str_test,   "str_test",   Permission::ReadWrite);

        /* [{"n": "bool_test", "vb": true}, {"n": "int_test", "v": 10}, {"n": "float_test", "v": 20.0}, {"n": "str_test", "vs": "hello arduino"}]
         * = 84 A2 61 6E 69 62 6F 6F 6C 5F 74 65 73 74 62 76 62 F5 A2 61 6E 68 69 6E 74 5F 74 65 73 74 61 76 0A A2 61 6E 6A 66 6C 6F 61 74 5F 74 65 73 74 61 76 F9 4D 00 A2 61 6E 68 73 74 72 5F 74 65 73 74 62 76 73 6D 68 65 6C 6C 6F 20 61 72 64 75 69 6E 6F
         */
        uint8_t const payload[] = {0x84, 0xA2, 0x61, 0x6E, 0x69, 0x62, 0x6F, 0x6F, 0x6C, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x62, 0xF5, 0xA2, 0x61, 0x6E, 0x68, 0x69, 0x6E, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x0A, 0xA2, 0x61, 0x6E, 0x6A, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0xF9, 0x4D, 0x00, 0xA2, 0x61, 0x6E, 0x68, 0x73, 0x74, 0x72, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x6D, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x61, 0x72, 0x64, 0x75, 0x69, 0x6E, 0x6F};
        thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

        REQUIRE(bool_test  == true);
        REQUIRE(int_test   == 10);
        REQUIRE(float_test == Approx(20.0).epsilon(0.01));
        REQUIRE(str_test   == "hello arduino");
      }

      /********************************************************************************/

      WHEN("Multiple String properties are changed via CBOR message")
      {
        ArduinoCloudThing thing(CloudProtocol::V1);
        thing.begin();

        String str_1("hello"),
               str_2("arduino"),
               str_3("cloud"),
               str_4("test");

        thing.addPropertyReal(str_1, "str_1", Permission::ReadWrite);
        thing.addPropertyReal(str_2, "str_2", Permission::ReadWrite);
        thing.addPropertyReal(str_3, "str_3", Permission::ReadWrite);
        thing.addPropertyReal(str_4, "str_4", Permission::ReadWrite);

        /* [{"n": "str_1", "vs": "I'd like"}, {"n": "str_2", "vs": "a"}, {"n": "str_3", "vs": "cup"}, {"n": "str_4", "vs": "of coffee"}]
         * = 84 A2 61 6E 65 73 74 72 5F 31 62 76 73 68 49 27 64 20 6C 69 6B 65 A2 61 6E 65 73 74 72 5F 32 62 76 73 61 61 A2 61 6E 65 73 74 72 5F 33 62 76 73 63 63 75 70 A2 61 6E 65 73 74 72 5F 34 62 76 73 69 6F 66 20 63 6F 66 66 65 65
         */
        uint8_t const payload[] = {0x84, 0xA2, 0x61, 0x6E, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x31, 0x62, 0x76, 0x73, 0x68, 0x49, 0x27, 0x64, 0x20, 0x6C, 0x69, 0x6B, 0x65, 0xA2, 0x61, 0x6E, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x32, 0x62, 0x76, 0x73, 0x61, 0x61, 0xA2, 0x61, 0x6E, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x33, 0x62, 0x76, 0x73, 0x63, 0x63, 0x75, 0x70, 0xA2, 0x61, 0x6E, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x34, 0x62, 0x76, 0x73, 0x69, 0x6F, 0x66, 0x20, 0x63, 0x6F, 0x66, 0x66, 0x65, 0x65};
        thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

        REQUIRE(str_1 == "I'd like");
        REQUIRE(str_2 == "a");
        REQUIRE(str_3 == "cup");
        REQUIRE(str_4 == "of coffee");
      }
    }

    /**********************************************************************************/

    GIVEN("CloudProtocol::V2")
    {
      WHEN("Multiple properties of different type are changed via CBOR message")
      {
        ArduinoCloudThing thing(CloudProtocol::V2);
        thing.begin();

        bool   bool_test = false;
        int    int_test = 1;
        float  float_test = 2.0f;
        String str_test("str_test");

        thing.addPropertyReal(bool_test,  "bool_test",  Permission::ReadWrite);
        thing.addPropertyReal(int_test,   "int_test",   Permission::ReadWrite);
        thing.addPropertyReal(float_test, "float_test", Permission::ReadWrite);
        thing.addPropertyReal(str_test,   "str_test",   Permission::ReadWrite);

        /* [{0: "bool_test", 4: true}, {0: "int_test", 2: 10}, {0: "float_test", 2: 20.0}, {0: "str_test", 3: "hello arduino"}]
         * = 84 A2 00 69 62 6F 6F 6C 5F 74 65 73 74 04 F5 A2 00 68 69 6E 74 5F 74 65 73 74 02 0A A2 00 6A 66 6C 6F 61 74 5F 74 65 73 74 02 F9 4D 00 A2 00 68 73 74 72 5F 74 65 73 74 03 6D 68 65 6C 6C 6F 20 61 72 64 75 69 6E 6F
         */
        uint8_t const payload[] = {0x84, 0xA2, 0x00, 0x69, 0x62, 0x6F, 0x6F, 0x6C, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x04, 0xF5, 0xA2, 0x00, 0x68, 0x69, 0x6E, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x02, 0x0A, 0xA2, 0x00, 0x6A, 0x66, 0x6C, 0x6F, 0x61, 0x74, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x02, 0xF9, 0x4D, 0x00, 0xA2, 0x00, 0x68, 0x73, 0x74, 0x72, 0x5F, 0x74, 0x65, 0x73, 0x74, 0x03, 0x6D, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x61, 0x72, 0x64, 0x75, 0x69, 0x6E, 0x6F};
        thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

        REQUIRE(bool_test  == true);
        REQUIRE(int_test   == 10);
        REQUIRE(float_test == Approx(20.0).epsilon(0.01));
        REQUIRE(str_test   == "hello arduino");
      }

      /********************************************************************************/

      WHEN("Multiple String properties are changed via CBOR message")
      {
        ArduinoCloudThing thing(CloudProtocol::V2);
        thing.begin();

        String str_1("hello"),
               str_2("arduino"),
               str_3("cloud"),
               str_4("test");

        thing.addPropertyReal(str_1, "str_1", Permission::ReadWrite);
        thing.addPropertyReal(str_2, "str_2", Permission::ReadWrite);
        thing.addPropertyReal(str_3, "str_3", Permission::ReadWrite);
        thing.addPropertyReal(str_4, "str_4", Permission::ReadWrite);

        /* [{0: "str_1", 3: "I'd like"}, {0: "str_2", 3: "a"}, {0: "str_3", 3: "cup"}, {0: "str_4", 3: "of coffee"}]
         * = 84 A2 00 65 73 74 72 5F 31 03 68 49 27 64 20 6C 69 6B 65 A2 00 65 73 74 72 5F 32 03 61 61 A2 00 65 73 74 72 5F 33 03 63 63 75 70 A2 00 65 73 74 72 5F 34 03 69 6F 66 20 63 6F 66 66 65 65
         */
        uint8_t const payload[] = {0x84, 0xA2, 0x00, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x31, 0x03, 0x68, 0x49, 0x27, 0x64, 0x20, 0x6C, 0x69, 0x6B, 0x65, 0xA2, 0x00, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x32, 0x03, 0x61, 0x61, 0xA2, 0x00, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x33, 0x03, 0x63, 0x63, 0x75, 0x70, 0xA2, 0x00, 0x65, 0x73, 0x74, 0x72, 0x5F, 0x34, 0x03, 0x69, 0x6F, 0x66, 0x20, 0x63, 0x6F, 0x66, 0x66, 0x65, 0x65};
        thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

        REQUIRE(str_1 == "I'd like");
        REQUIRE(str_2 == "a");
        REQUIRE(str_3 == "cup");
        REQUIRE(str_4 == "of coffee");
      }
    }
  }

  /************************************************************************************/

  WHEN("A payload containing a CBOR base name is parsed")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      String str = "hello";
      thing.addPropertyReal(str, "test", Permission::ReadWrite);

      /* [{"bn": "some-test-base-name", "n": "test", "vs": "test"}] = 81 A3 62 62 6E 73 73 6F 6D 65 2D 74 65 73 74 2D 62 61 73 65 2D 6E 61 6D 65 61 6E 64 74 65 73 74 62 76 73 64 74 65 73 74 */
      uint8_t const payload[] = {0x81, 0xA3, 0x62, 0x62, 0x6E, 0x73, 0x73, 0x6F, 0x6D, 0x65, 0x2D, 0x74, 0x65, 0x73, 0x74, 0x2D, 0x62, 0x61, 0x73, 0x65, 0x2D, 0x6E, 0x61, 0x6D, 0x65, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x62, 0x76, 0x73, 0x64, 0x74, 0x65, 0x73, 0x74};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(str == "test");
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      String str = "hello";
      thing.addPropertyReal(str, "test", Permission::ReadWrite);

      /* [{-2: "some-test-base-name", 0: "test", 3: "test"}] = 81 A3 21 73 73 6F 6D 65 2D 74 65 73 74 2D 62 61 73 65 2D 6E 61 6D 65 00 64 74 65 73 74 03 64 74 65 73 74 */
      uint8_t const payload[] = {0x81, 0xA3, 0x21, 0x73, 0x73, 0x6F, 0x6D, 0x65, 0x2D, 0x74, 0x65, 0x73, 0x74, 0x2D, 0x62, 0x61, 0x73, 0x65, 0x2D, 0x6E, 0x61, 0x6D, 0x65, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x03, 0x64, 0x74, 0x65, 0x73, 0x74};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(str == "test");
    }
  }

  /************************************************************************************/

  WHEN("A payload containing a CBOR base time is parsed")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"bt": 123.456, "n": "test", "v": 1}] = 81 A3 62 62 74 FB 40 5E DD 2F 1A 9F BE 77 61 6E 64 74 65 73 74 61 76 01 */
      uint8_t const payload[] = {0x81, 0xA3, 0x62, 0x62, 0x74, 0xFB, 0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x01};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(test == 1);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{-3: 123.456, 0: "test", 2: 1}] = 81 A3 22 FB 40 5E DD 2F 1A 9F BE 77 00 64 74 65 73 74 02 01 */
      uint8_t const payload[] = {0x81, 0xA3, 0x22, 0xFB, 0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x01};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(test == 1);
    }
  }

  /************************************************************************************/

  WHEN("A payload containing a CBOR time is parsed")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"t": 123.456, "n": "test", "v": 1}] = 81 A3 61 74 FB 40 5E DD 2F 1A 9F BE 77 61 6E 64 74 65 73 74 61 76 01 */
      uint8_t const payload[] = {0x81, 0xA3, 0x61, 0x74, 0xFB, 0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x01};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(test == 1);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{6: 123.456, 0: "test", 2: 1}] = 81 A3 06 FB 40 5E DD 2F 1A 9F BE 77 00 64 74 65 73 74 02 01 */
      uint8_t const payload[] = {0x81, 0xA3, 0x06, 0xFB, 0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x01};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(test == 1);
    }
  }

  /************************************************************************************/

  WHEN("A payload containing a CBOR BaseName, BaseTime and Time is parsed")
  {
    GIVEN("CloudProtocol::V1")
    {
      ArduinoCloudThing thing(CloudProtocol::V1);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{"bn": "base-name", "bt": 654.321, "t": 123.456, "n": "test", "v": 1}] =
       * 81 A5 62 62 6E 69 62 61 73 65 2D 6E 61 6D 65 62 62 74 FB 40 84 72 91 68 72 B0 21 61 74 FB 40 5E DD 2F 1A 9F BE 77 61 6E 64 74 65 73 74 61 76 01
       */
      uint8_t const payload[] = {0x81, 0xA5, 0x62, 0x62, 0x6E, 0x69, 0x62, 0x61, 0x73, 0x65, 0x2D, 0x6E, 0x61, 0x6D, 0x65, 0x62, 0x62, 0x74, 0xFB, 0x40, 0x84, 0x72, 0x91, 0x68, 0x72, 0xB0, 0x21, 0x61, 0x74, 0xFB, 0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77, 0x61, 0x6E, 0x64, 0x74, 0x65, 0x73, 0x74, 0x61, 0x76, 0x01};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(test == 1);
    }
    GIVEN("CloudProtocol::V2")
    {
      ArduinoCloudThing thing(CloudProtocol::V2);
      thing.begin();

      int test = 0;
      thing.addPropertyReal(test, "test", Permission::ReadWrite);

      /* [{-2: "base-name", -3: 654.321, 6: 123.456, 0: "test", 2: 1}] =
       * 81 A5 21 69 62 61 73 65 2D 6E 61 6D 65 22 FB 40 84 72 91 68 72 B0 21 06 FB 40 5E DD 2F 1A 9F BE 77 00 64 74 65 73 74 02 01
       */
      uint8_t const payload[] = {0x81, 0xA5, 0x21, 0x69, 0x62, 0x61, 0x73, 0x65, 0x2D, 0x6E, 0x61, 0x6D, 0x65, 0x22, 0xFB, 0x40, 0x84, 0x72, 0x91, 0x68, 0x72, 0xB0, 0x21, 0x06, 0xFB, 0x40, 0x5E, 0xDD, 0x2F, 0x1A, 0x9F, 0xBE, 0x77, 0x00, 0x64, 0x74, 0x65, 0x73, 0x74, 0x02, 0x01};
      thing.decode(payload, sizeof(payload)/sizeof(uint8_t));

      REQUIRE(test == 1);
    }
  }

  /************************************************************************************/
}
