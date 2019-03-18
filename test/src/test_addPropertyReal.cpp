/*
 * Copyright (c) 2019 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <ArduinoCloudThing.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

SCENARIO("The same arduino cloud properties are added multiple times", "[ArduinoCloudThing::addPropertyReal]")
{
  WHEN("The same bool property is added multiple times")
  {
    ArduinoCloudThing thing;
    thing.begin();

    bool bool_property = false;

    ArduinoCloudProperty<bool> * bool_property_ptr_1 = &thing.addPropertyReal(bool_property, "bool_property", Permission::ReadWrite);
    ArduinoCloudProperty<bool> * bool_property_ptr_2 = &thing.addPropertyReal(bool_property, "bool_property", Permission::ReadWrite);
    THEN("No new property is added and the first added property is returned instead of a new one")
    {
      REQUIRE(bool_property_ptr_1 == bool_property_ptr_2);      
    }
  }

  /**************************************************************************************/

  WHEN("the same int property is added multiple times")
  {
    ArduinoCloudThing thing;
    thing.begin();

    int int_property = 1;

    ArduinoCloudProperty<int> * int_property_ptr_1 = &thing.addPropertyReal(int_property, "int_property", Permission::ReadWrite);
    ArduinoCloudProperty<int> * int_property_ptr_2 = &thing.addPropertyReal(int_property, "int_property", Permission::ReadWrite);

    THEN("No new property is added and the first added property is returned instead of a new one")
    {
      REQUIRE(int_property_ptr_1 == int_property_ptr_2);
    }
  }

  /**************************************************************************************/

  WHEN("the same float property is added multiple times")
  {
    ArduinoCloudThing thing;
    thing.begin();

    float float_property = 1.0f;

    ArduinoCloudProperty<float> * float_property_ptr_1 = &thing.addPropertyReal(float_property, "float_property", Permission::ReadWrite);
    ArduinoCloudProperty<float> * float_property_ptr_2 = &thing.addPropertyReal(float_property, "float_property", Permission::ReadWrite);
    
    THEN("No new property is added and the first added property is returned instead of a new one")
    {
      REQUIRE(float_property_ptr_1 == float_property_ptr_2);
    }
  }

  /**************************************************************************************/

  WHEN("the same String property is added multiple times")
  {
    ArduinoCloudThing thing;
    thing.begin();

    String str_property;

    ArduinoCloudProperty<String> * str_property_ptr_1 = &thing.addPropertyReal(str_property, "str_property", Permission::ReadWrite);
    ArduinoCloudProperty<String> * str_property_ptr_2 = &thing.addPropertyReal(str_property, "str_property", Permission::ReadWrite);
    
    THEN("No new property is added and the first added property is returned instead of a new one")
    {
      REQUIRE(str_property_ptr_1 == str_property_ptr_2);
    }
  }
}