#include <Arduino.h>
#include <ArduinoCloudThing.h>

#if defined(DEBUG_MEMORY) && defined(ARDUINO_ARCH_SAMD)
extern "C" char *sbrk(int i);
void PrintFreeRam (void)
{
    char stack_dummy = 0;
    Serial.print("Free RAM: "); Serial.println(&stack_dummy - sbrk(0));
}
#endif

#ifdef ARDUINO_ARCH_SAMD
static void utox8(uint32_t val, char* s) {
    for (int i = 0; i < 8; i++) {
        int d = val & 0XF;
        val = (val >> 4);

        s[7 - i] = d > 9 ? 'A' + d - 10 : '0' + d;
    }
}
#endif

#ifdef ARDUINO_ARCH_MRAA
#define Serial DebugSerial
#endif

ArduinoCloudThing::ArduinoCloudThing() {
#ifdef ARDUINO_ARCH_SAMD
    #define SERIAL_NUMBER_WORD_0    *(volatile uint32_t*)(0x0080A00C)
    #define SERIAL_NUMBER_WORD_1    *(volatile uint32_t*)(0x0080A040)
    #define SERIAL_NUMBER_WORD_2    *(volatile uint32_t*)(0x0080A044)
    #define SERIAL_NUMBER_WORD_3    *(volatile uint32_t*)(0x0080A048)

    utox8(SERIAL_NUMBER_WORD_0, &uuid[0]);
    utox8(SERIAL_NUMBER_WORD_1, &uuid[8]);
    utox8(SERIAL_NUMBER_WORD_2, &uuid[16]);
    utox8(SERIAL_NUMBER_WORD_3, &uuid[24]);
    uuid[32] = '\0';
#endif
}


void ArduinoCloudThing::begin() {

    status = ON;
    addPropertyReal(status, "status").readOnly();
}

int ArduinoCloudThing::publish(CborArray& object, uint8_t* data, size_t size) {

    ssize_t len = object.encode(data, size);

#ifdef TESTING_PROTOCOL
    decode(data, len);
#endif

    for (int i = 0; i < list.size(); i++) {
        ArduinoCloudPropertyGeneric *p = list.get(i);
        p->updateShadow();
    }

    return len;
}

int ArduinoCloudThing::poll(uint8_t* data, size_t size) {

    // check if backing storage and cloud has diverged
    // time interval may be elapsed or property may be changed
    int diff = 0;

    diff = checkNewData();
    if (diff > 0) {
        CborBuffer buffer(1024);
        CborArray object = CborArray(buffer);
        compress(object, buffer);
        diff = publish(object, data, size);
    }

#if defined(DEBUG_MEMORY) && defined(ARDUINO_ARCH_SAMD)
    PrintFreeRam();
#endif

    return diff;
}

void ArduinoCloudThing::compress(CborArray& object, CborBuffer& buffer) {

    for (int i = 0; i < list.size(); i++) {
        ArduinoCloudPropertyGeneric *p = list.get(i);
        // If a property should be updated and has read permission from the Cloud point of view
        if (p->shouldBeUpdated() && p->canRead()) {
            CborObject child = CborObject(buffer);
            // Create the CBOR obj representing the property
            p->append(child);
            CborVariant variant = CborVariant(buffer, child);
            // Add the encoded property to the CBOR array that has to be sent.
            object.add(variant);
        }
    }
}

int ArduinoCloudThing::checkNewData() {
    int counter = 0;
    for (int i = 0; i < list.size(); i++) {
        ArduinoCloudPropertyGeneric *p = list.get(i);
        if (p->shouldBeUpdated()) {
            counter++;
        }
    }
    // return number of props that has tu be updated
    return counter;
}

ArduinoCloudPropertyGeneric* ArduinoCloudThing::exists(String &name) {
    for (int i = 0; i < list.size(); i++) {
        ArduinoCloudPropertyGeneric *p = list.get(i);
        // Check the property existance just comparing its name with existent ones
        if (p->getName() == name) {
            return p;
        }
    }
    return NULL;
}

// It return the index of the property, inside the local array, with the name passed as parameter. (-1 if it does not exist.)
int ArduinoCloudThing::findPropertyByName(String &name) {
    for (int i = 0; i < list.size(); i++) {
        ArduinoCloudPropertyGeneric *p = list.get(i);
        // Check the property existance just comparing its name with existent ones
        if (p->getName() == name) {
            return i;
        }
    }
    return -1;
}

ArduinoCloudPropertyGeneric& ArduinoCloudThing::addPropertyReal(int& property, String name) {
    if (ArduinoCloudPropertyGeneric* p = exists(name)) {
        return *p;
    }
    ArduinoCloudProperty<int> *propertyObj = new ArduinoCloudProperty<int>(property, name);
    list.add(propertyObj);
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(propertyObj));
}

ArduinoCloudPropertyGeneric& ArduinoCloudThing::addPropertyReal(bool& property, String name) {
    if (ArduinoCloudPropertyGeneric* p = exists(name)) {
        return *p;
    }
    ArduinoCloudProperty<bool> *propertyObj = new ArduinoCloudProperty<bool>(property, name);
    list.add(propertyObj);
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(propertyObj));
}

ArduinoCloudPropertyGeneric& ArduinoCloudThing::addPropertyReal(float& property, String name) {
    if (ArduinoCloudPropertyGeneric* p = exists(name)) {
        return *p;
    }
    ArduinoCloudProperty<float> *propertyObj = new ArduinoCloudProperty<float>(property, name);
    list.add(propertyObj);
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(propertyObj));
}

ArduinoCloudPropertyGeneric& ArduinoCloudThing::addPropertyReal(String& property, String name) {
    if (ArduinoCloudPropertyGeneric* p = exists(name)) {
        return *p;
    }
    ArduinoCloudStringProperty *propertyObj = new ArduinoCloudStringProperty(property, name);
    list.add(propertyObj);
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(propertyObj));
}


void ArduinoCloudThing::decode(uint8_t * payload, size_t length) {
    CborBuffer buffer(200);
    CborVariant total = buffer.decode(payload, length);
    int propId;
    String propType;
    // It will contain the entire array of properties
    CborArray array = total.asArray();

    // Scan all properties into received array
    for (int i=0; ;i++) {
        CborVariant variant = array.get(i);

        // Check CBOR object validity, if it is not valid, there are no more received properties.
        if (!variant.isValid()) break;
        CborObject object = variant.asObject();

        String name = "";
        // get the property name, if no name do nothing!
        if (object.get("n").isValid()) {
            name = object.get("n").asString();

            // Search for the index of the device property with that name
            propId = findPropertyByName(name);
            // If property does not exist, skip it and do nothing.
            if (propId < 0) continue;
            ArduinoCloudPropertyGeneric* property = list.get(propId);
            Serial.print("-- Received prop name: "); Serial.println(property->getName());

            // Check for the property type, write method internally check for the permission
            propType = property->getType();

            if (propType == "INT" && object.get("v").isValid()) {
                int value = object.get("v").asInteger();
                Serial.print("Received prop int value: "); Serial.println(object.get("v").asInteger());
                ArduinoCloudProperty<int>* p = (ArduinoCloudProperty<int>*) property;
                p->write(value);
            } else if (propType == "BOOL" && object.get("vb").isValid()) {
                bool value = object.get("vb").asInteger();
                Serial.print("Received prop bool value: "); Serial.println(value);
                ArduinoCloudProperty<bool>* p = (ArduinoCloudProperty<bool>*) property;
                p->write(value);
            } else if (propType == "FLOAT" && object.get("v").isValid()) { 
                float value = object.get("v").asFloat();
                Serial.print("Received prop float value: "); Serial.println(value);
                ArduinoCloudProperty<float>* p = (ArduinoCloudProperty<float>*) property;
                p->write(value);
            } else if (propType == "STRING" && object.get("vs").isValid()){ 
                String value = object.get("vs").asString();
                Serial.print("Received prop string value: "); Serial.println(value);
                ArduinoCloudStringProperty* p = (ArduinoCloudStringProperty*) property;
                p->write(value);
            }

            // If the property is changed call its callback
            if (property->newData()) {
                if (property->callback != NULL) {
                    property->callback();
                }
            }
        }
    }
}