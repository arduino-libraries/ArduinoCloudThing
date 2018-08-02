#ifndef ArduinoCloudStringProperty_h
#define ArduinoCloudStringProperty_h

// This class implements the ArduinoCloudPropertyGeneric interface
#include <ArduinoCloudPropertyGeneric.h>

class ArduinoCloudStringProperty : public ArduinoCloudPropertyGeneric {
    public:
        ArduinoCloudStringProperty(String& _property, String _name);
        bool write(String value);
        void printinfo();
        void updateShadow();
        String read();
        bool canRead();
        String& getName();
        void setName(String _name);
        ArduinoCloudPropertyGeneric& setTag(int _tag);
        int getTag();
        ArduinoCloudPropertyGeneric& setPermission(permissionType _permission);
        ArduinoCloudPropertyGeneric& readOnly();
        ArduinoCloudPropertyGeneric& minimumDelta(void* delta);
        permissionType getPermission();
        ArduinoCloudPropertyGeneric& onUpdate(void(*fn)(void));
        void appendValue(CborObject &cbor);
        void append(CborObject &cbor);
        ArduinoCloudPropertyGeneric& publishEvery(long seconds);
        bool newData();
        bool shouldBeUpdated();
        String getType();


    // This section is not visible from outside but can be inherited
    protected:
        String& property;
        String shadow_property;
        String name;
        int tag = -1;
        long lastUpdated = 0;
        long updatePolicy = ON_CHANGE;
        permissionType permission = READWRITE;
        static int tagIndex;
};

#endif