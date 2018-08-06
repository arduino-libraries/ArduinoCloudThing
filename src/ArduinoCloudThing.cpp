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
                // create a cbor object for the property and automatically add it into array
                p->append(&arrayEncoder);
            }
        }

        err = cbor_encoder_close_container(&encoder, &arrayEncoder);
        if (err) {
            Serial.println("Problem of CBOR encoding...\nNothing will be sent.");
            Serial.println(cbor_error_string(err)); return 0;
        }

        // update properties shadow values, in order to check if variable has changed since last publish
        for (int i = 0; i < list.size(); i++) {
            ArduinoCloudPropertyGeneric *p = list.get(i);
            p->updateShadow();
        }

        // return the number of byte of the CBOR encoded array
        //size_t dataSize = cbor_encoder_get_buffer_size(&encoder, data);
        //Serial.print("-- Size of payload to send: "); Serial.println(dataSize);
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
    CborParser parser;
    CborValue it, recursedArray, recursedMap, propValue;
    int propId; String propType, propName;
    
    err = cbor_parser_init(payload, length, 0, &parser, &it);
    if(err) {
        Serial.println("Error in the parser creation.");
        Serial.println(cbor_error_string(err));
        return;
    }

    // parse cbor data only if an array of cbor object is received.
    if(cbor_value_get_type(&it) != CborArrayType)
        return;

    // enter in a cbor container, in this case, the array
    err = cbor_value_enter_container(&it, &recursedArray);
    if (err) {
        Serial.println(cbor_error_string(err));
        return;
    }

    // get array length
    size_t len;
    cbor_value_get_array_length(&recursedArray, &len);
    Serial.print("Array size: "); Serial.println(len);

    int arrayIndex = 0;
    Serial.println("-- Decoding...");

    while (!cbor_value_at_end(&recursedArray)) {
    
        CborType type = cbor_value_get_type(&recursedArray);
        Serial.print("Element type: "); Serial.println(type);
        
        if (type == CborMapType) {
            Serial.println("Map element");
            
            err = cbor_value_enter_container(&recursedArray, &recursedMap);
            if(err) {
                Serial.println("Enter container.");
                Serial.println(cbor_error_string(err));
                return;
            }

            cbor_value_get_map_length(&recursedMap, &len);
            Serial.print("Map size: "); Serial.println(len);
            
            while (!cbor_value_at_end(&recursedMap)) {

                type = cbor_value_get_type(&recursedMap); 

                if (type == CborTextStringType) {
                    char *buf;
                    size_t n;
                    err = cbor_value_dup_text_string(&recursedMap, &buf, &n, &recursedMap);
                   
                    String val = String(buf);
                    Serial.println(val);
                    free(buf);
                } else {
                    cbor_value_advance(&recursedMap);
                    Serial.println("Skip value.");
                }
            }

            Serial.println("END map scanning");
            err = cbor_value_leave_container(&recursedArray, &recursedMap);
            Serial.println("container leaved");

        } else {
            Serial.println("Array element is NOT a map..");
            cbor_value_advance(&recursedArray);
        }
        
        Serial.println("Array index: "); Serial.println(++arrayIndex);
    }
    Serial.println("END array scanning");

}
    /*

    // main loop through the cbor array elements
    while (!cbor_value_at_end(&it)) {
            
        // parse cbor object
        err = cbor_value_enter_container(&it, &recursedMap);
        if (!err) {

            // if the current element is not a cbor object, skip it and go ahead.
            CborType type = cbor_value_get_type(&recursedMap);
            if (type != CborMapType) {
                Serial.println("Array element is NOT a map..");
                //continue;
            } else {
                Serial.println("Object into Array.....");
                while (!cbor_value_at_end(&recursedMap)) {

                    CborValue name;

                    cbor_value_map_find_value(&recursedMap, "n", &name);
                    Serial.println(name.type);

                    // check if a property has a name, of string type
                    if (name.type != CborTextStringType)
                        break;

                    // get the name
                    char *buf; size_t n;

                    err = cbor_value_dup_text_string(&name, &buf, &n, NULL);
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
                    
                    if (propType == "FLOAT" && !cbor_value_map_find_value(&recursedMap, "v", &propValue)) {
                        
                        Serial.print("Prop type: ");Serial.println(propValue.type);

                        if (propValue.type == CborFloatType) {
                            Serial.println("Float prop");
                            float val;
                            cbor_value_get_float(&propValue, &val);
                            Serial.print("Received prop float value: "); Serial.println(val);
                            ArduinoCloudProperty<float>* p = (ArduinoCloudProperty<float>*) property;
                            p->write(val);
                        }
                    }

                    // If the property is changed call its callback
                    if (property->newData()) {
                        if (property->callback != NULL) {
                            property->callback();
                        }
                    }   
                }
            }
            // Leave the current cbor object
            Serial.println("Leave map");
            err = cbor_value_leave_container(&it, &recursedMap);
            if(err) {
                Serial.println("Cannot leave map");
                Serial.println(cbor_error_string(err));
            }
        }
    }

    // Leave the cbor array
    cbor_value_leave_container(&it, &recursedArray);
}




/*
                    if (propType == "INT" && !cbor_value_map_find_value(&it, "v", &propValue)) {
                        // if no key proper key was found, do nothing
                        if (name.type == CborIntegerType) {
                            int val;
                            cbor_value_get_int(&it, &val);
                            Serial.print("Received prop int value: "); Serial.println(val);
                            ArduinoCloudProperty<int>* p = (ArduinoCloudProperty<int>*) property;
                            p->write(val);
                        }
                    } else if (propType == "BOOL" && !cbor_value_map_find_value(&it, "vb", &propValue)) {
                        if (name.type == CborBooleanType) {
                            bool val;
                            cbor_value_get_boolean(&it, &val);
                            Serial.print("Received prop bool value: "); Serial.println(val);
                            ArduinoCloudProperty<bool>* p = (ArduinoCloudProperty<bool>*) property;
                            p->write(val);
                        }
                    } else if (propType == "FLOAT" && !cbor_value_map_find_value(&it, "v", &propValue)) { 
                        if (propValue.type == CborFloatType) {
                            float val;
                            cbor_value_get_float(&propValue, &val);
                            Serial.print("Received prop float value: "); Serial.println(val);
                            ArduinoCloudProperty<float>* p = (ArduinoCloudProperty<float>*) property;
                            p->write(val);
                        }
                    } else if (propType == "STRING" && !cbor_value_map_find_value(&it, "vs", &propValue)){ 
                        if (name.type == CborBooleanType) {
                            uint8_t *val; size_t n;
                            err = cbor_value_dup_byte_string(&it, &val, &n, &it);
                            if (err)
                                Serial.println("Cannot get string value of the property");
                            else {
                                ArduinoCloudStringProperty* p = (ArduinoCloudStringProperty*) property;
                                p->write(String((char*)val));
                                free(val);
                            }
                        }
                    }
                    */