#ifndef ArduinoCloudProperty_h
#define ArduinoCloudProperty_h

// This class implements the ArduinoCloudPropertyGeneric interface
#include <ArduinoCloudPropertyGeneric.h>

template <typename T>
class ArduinoCloudProperty : public ArduinoCloudPropertyGeneric {
    public:
        ArduinoCloudProperty(T& _property, String _name);
        bool write(T value);
        void printinfo();
        void updateShadow();
        T read();
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
        void appendValue(CborEncoder* mapEncoder);
        void append(CborEncoder* encoder);
        ArduinoCloudPropertyGeneric& publishEvery(long seconds);
        bool newData();
        bool shouldBeUpdated();
        String getType();


    // This section is not visible from outside but can be inherited
    protected:
        T& property;
        T shadow_property;
        String name;
        int tag = -1;
        long lastUpdated = 0;
        long updatePolicy = ON_CHANGE;
        T minDelta = 0;
        permissionType permission = READWRITE;
        static int tagIndex;
};

#endif