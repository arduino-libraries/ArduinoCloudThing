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


int ArduinoCloudThing::poll(uint8_t* data, size_t size) {

    // check if backing storage and cloud has diverged
    // time interval may be elapsed or property may be changed
    int diff = 0;

    // are there some changed prperies???
    diff = checkNewData();
    if (diff > 0) {
        CborError err;
        CborEncoder encoder, arrayEncoder;
        
        cbor_encoder_init(&encoder, data, size, 0);
        // create a cbor array containing the property that should be updated.
        err = cbor_encoder_create_array(&encoder, &arrayEncoder, CborIndefiniteLength);
        if (err) {
            Serial.println(cbor_error_string(err));
            return -1;
        }

        for (int i = 0; i < list.size(); i++) {
            ArduinoCloudPropertyGeneric *p = list.get(i);
            // If a property should be updated and has read permission from the Cloud point of view
            if (p->shouldBeUpdated() && p->canRead()) {
                // create a cbor object for the property and and add it into array
                p->append(&encoder);

                /*TODO
                Manage buffer "overflow"
                if (err && err != CborErrorOutOfMemory) {

                }*/
            }
        }
        err = cbor_encoder_close_container_checked(&encoder, &arrayEncoder);

        // update properties shadow values, in order to check if variable has changed since last publish
        for (int i = 0; i < list.size(); i++) {
            ArduinoCloudPropertyGeneric *p = list.get(i);
            p->updateShadow();
        }

        // return the number of byte of the CBOR encoded array
        return cbor_encoder_get_buffer_size(&encoder, data);
    }

#if defined(DEBUG_MEMORY) && defined(ARDUINO_ARCH_SAMD)
    PrintFreeRam();
#endif
    // If nothing has to be sent, return diff, that is 0 in this case
    return diff;
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


void ArduinoCloudThing::decode(uint8_t *payload, size_t length) {
    
    CborType type;
    CborError err;
    CborValue *it, *recursed, *recursedMap, *propValue;
    int propId; String propType, propName;
    
    // parse cbor data only if an array of cbor object is received.
    if(cbor_value_get_type(it) != CborArrayType)
        return;
    
    // enter in a cbor container, in this case, the array
    err = cbor_value_enter_container(it, recursed);
    if (err) {
        Serial.println(cbor_error_string(err));
        return;
    }

    // main loop through the cbor array elements
    while (!cbor_value_at_end(it)) {
        // if the current element is not a cbor object, ski it and go ahead.
        if (cbor_value_get_type(it) != CborMapType)
            continue;
        
        // parse cbor object
        err = cbor_value_enter_container(it, recursedMap);
        if (!err) {
            while (!cbor_value_at_end(it)) {
                CborValue name;
                cbor_value_map_find_value(it, "n", &name);
                // check if a property has a name, of string type
                if (name.type != CborTextStringType)
                    break;
                
                // get the name
                char *buf;
                size_t n;
                err = cbor_value_dup_text_string(it, &buf, &n, it);
                if (err) {
                    Serial.println("Cannot get name value of the property");
                    break;     // parse error
                }
                propName = String(buf);
                free(buf);

                // Search for the index of the device property with that name
                propId = findPropertyByName(propName);
                // If property does not exist, skip it and do nothing.
                if (propId < 0) continue;
                ArduinoCloudPropertyGeneric* property = list.get(propId);
                Serial.print("-- Received prop name: "); Serial.println(property->getName());

                // Check for the property type, write method internally check for the permission
                propType = property->getType();

                if (propType == "INT" && !cbor_value_map_find_value(it, "v", propValue)) {
                    // if no key proper key was found, do nothing
                    if (name.type == CborIntegerType) {
                        int val;
                        cbor_value_get_int(it, &val);
                        Serial.print("Received prop int value: "); Serial.println(val);
                        ArduinoCloudProperty<int>* p = (ArduinoCloudProperty<int>*) property;
                        p->write(val);
                    }
                } else if (propType == "BOOL" && !cbor_value_map_find_value(it, "vb", propValue)) {
                    if (name.type == CborBooleanType) {
                        bool val;
                        cbor_value_get_boolean(it, &val);
                        Serial.print("Received prop bool value: "); Serial.println(val);
                        ArduinoCloudProperty<bool>* p = (ArduinoCloudProperty<bool>*) property;
                        p->write(val);
                    }
                } else if (propType == "FLOAT" && !cbor_value_map_find_value(it, "v", propValue)) { 
                    if (name.type == CborFloatType) {
                        float val;
                        cbor_value_get_float(it, &val);
                        Serial.print("Received prop float value: "); Serial.println(val);
                        ArduinoCloudProperty<float>* p = (ArduinoCloudProperty<float>*) property;
                        p->write(val);
                    }
                } else if (propType == "STRING" && !cbor_value_map_find_value(it, "vs", propValue)){ 
                    if (name.type == CborBooleanType) {
                        char *val; size_t n;
                        err = cbor_value_dup_text_string(it, &buf, &n, it);
                        if (err)
                            Serial.println("Cannot get string value of the property");
                        else {
                            ArduinoCloudStringProperty* p = (ArduinoCloudStringProperty*) property;
                            p->write(String(val));
                            free(val);
                        }
                    }
                }

                // If the property is changed call its callback
                if (property->newData()) {
                    if (property->callback != NULL) {
                        property->callback();
                    }
                }   
            }
            // Leave the current cbor object
            cbor_value_leave_container(it, recursedMap);
        }
    }

    // Leave the cbor array
    cbor_value_leave_container(it, recursed);
}