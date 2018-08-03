#ifndef ArduinoCloudPropertyGeneric_h
#define ArduinoCloudPropertyGeneric_h

#include <Arduino.h>
// include of the cbor library
#include "lib/CBOR/cbor-lib.h"

// Define the policy for the property.
// If policy !ON_CHANGE is considered a timing one as default
static const int ON_CHANGE = -1;

// Define the permissionh type for a generic property
enum permissionType {
    READ = 0b01,
    READWRITE = 0b11
};

// Declaration of Generic property interface. How a property must look like.
class ArduinoCloudPropertyGeneric {
public:
    virtual void append(CborEncoder* encoder) = 0;
    virtual String& getName() = 0;
    virtual void setName(String _name) = 0;
    virtual ArduinoCloudPropertyGeneric& setTag(int _tag) = 0;
    virtual ArduinoCloudPropertyGeneric& readOnly() = 0;
    virtual ArduinoCloudPropertyGeneric& minimumDelta(void* delta) = 0;
    virtual ArduinoCloudPropertyGeneric& setPermission(permissionType _permission) = 0;
    virtual ArduinoCloudPropertyGeneric& onUpdate(void(*fn)(void)) = 0;
    virtual ArduinoCloudPropertyGeneric& publishEvery(long seconds) = 0;
    virtual int getTag() = 0;
    virtual permissionType getPermission() = 0;
    virtual bool newData() = 0;
    virtual bool shouldBeUpdated() = 0;
    virtual void updateShadow() = 0;
    virtual bool canRead() = 0;
    virtual void printinfo() = 0;
    virtual String getType() = 0;
    
    void(*callback)(void) = NULL;
};

#endif