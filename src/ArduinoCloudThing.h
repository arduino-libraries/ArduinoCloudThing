#ifndef ArduinoCloudThing_h
#define ArduinoCloudThing_h

#include <ArduinoCloudProperty.h>
#include <ArduinoCloudStringProperty.h>

#include <Client.h>
#include <Stream.h>
#include "lib/LinkedList/LinkedList.h"
#include "lib/ArduinoCbor/src/ArduinoCbor.h"


enum boolStatus {
    ON,
    OFF
};

// Time definition, useful for the user, he avoit convert time in seconds
enum times {
    SECONDS = 1,
    MINUTES = 60,
    HOURS = 3600,
    DAYS = 86400,
};

class ArduinoCloudThing {
public:
    ArduinoCloudThing();
    void begin();
    ArduinoCloudPropertyGeneric& addPropertyReal(int& property, String name);
    ArduinoCloudPropertyGeneric& addPropertyReal(bool& property, String name);
    ArduinoCloudPropertyGeneric& addPropertyReal(float& property, String name);
    ArduinoCloudPropertyGeneric& addPropertyReal(void* property, String name);
    ArduinoCloudPropertyGeneric& addPropertyReal(String& property, String name);
    // poll should return > 0 if something has changed
    int poll(uint8_t* data, size_t size);
    void decode(uint8_t * payload, size_t length);

private:
    int publish(CborArray& object, uint8_t* data, size_t size);

    void update();
    int checkNewData();
    void compress(CborArray& object, CborBuffer& buffer);

    ArduinoCloudPropertyGeneric* exists(String &name);
    int findPropertyByName(String &name);

    bool status = OFF;
    char uuid[33];

    LinkedList<ArduinoCloudPropertyGeneric*> list;
    int currentListIndex = -1;
};

#endif
