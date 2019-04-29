/*
   Copyright (c) 2019 Arduino.  All rights reserved.
*/

/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <TestUtil.h>
#include <ArduinoCloudThing.h>

/**************************************************************************************
  TEST CODE
 **************************************************************************************/

/************************************************************************************/
SCENARIO("Arduino Cloud Properties ", "[ArduinoCloudThing::CloudColor]") {
  WHEN("Set invalid color HSB") {
    GIVEN("CloudProtocol::V2") {


      CloudColor color_test = CloudColor(0.0, 0.0, 0.0);

      Color value_color_test = color_test.getValue();
      REQUIRE(value_color_test.setColorHSB(500.0, 20.0, 30.0) == false);

    }
  }

  WHEN("Set and Get different RGB colors") {
    GIVEN("CloudProtocol::V2") {

      bool verify;
      uint8_t r, g, b;

      CloudColor color_test = CloudColor(0.0, 0.0, 0.0);

      Color value_color_test = color_test.getValue();

      value_color_test.setColorRGB(128, 64, 64);
      value_color_test.getRGB(r, g, b);
      verify = r == 128 && g == 64 && b == 64;

      REQUIRE(verify);

      value_color_test.setColorRGB(126, 128, 64);
      value_color_test.getRGB(r, g, b);
      verify = r == 126 && g == 128 && b == 64;

      REQUIRE(verify);

      value_color_test.setColorRGB(64, 128, 64);
      value_color_test.getRGB(r, g, b);
      verify = r == 64 && g == 128 && b == 64;

      REQUIRE(verify);

      value_color_test.setColorRGB(64, 64, 128);
      value_color_test.getRGB(r, g, b);
      verify = r == 64 && g == 64 && b == 128;

      REQUIRE(verify);

      value_color_test.setColorRGB(255, 0, 255);
      value_color_test.getRGB(r, g, b);
      verify = r == 255 && g == 0 && b == 255;

      REQUIRE(verify);

      value_color_test.setColorRGB(0, 0, 0);
      value_color_test.getRGB(r, g, b);
      verify = r == 0 && g == 0 && b == 0;

      REQUIRE(verify);

      value_color_test.setColorRGB(50, 100, 20);
      value_color_test.getRGB(r, g, b);
      verify = r == 50 && g == 100 && b == 20;

      REQUIRE(verify);


    }
  }

  WHEN("Set HSB colors and get RGB") {
    GIVEN("CloudProtocol::V2") {
      bool verify;
      uint8_t r, g, b;

      CloudColor color_test = CloudColor(0.0, 0.0, 0.0);

      Color value_color_test = color_test.getValue();

      value_color_test.setColorHSB(240, 50, 50);
      value_color_test.getRGB(r, g, b);
      verify = r == 64 && g == 64 && b == 128;

      REQUIRE(verify);

      value_color_test.setColorHSB(120, 50, 50);
      value_color_test.getRGB(r, g, b);
      verify = r == 64 && g == 128 && b == 64;

      REQUIRE(verify);

    }
  }
}