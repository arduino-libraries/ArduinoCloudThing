#include <ArduinoCloudStringProperty.h>


ArduinoCloudStringProperty::ArduinoCloudStringProperty(String& _property, String _name) :
    property(_property), name(_name) {}

bool ArduinoCloudStringProperty::write(String value) {
    /* permissions are intended as seen from cloud */
    if ((permission & READWRITE) >> 1) {
        property = value;
        return true;
    }
    return false;
}

void ArduinoCloudStringProperty::printinfo() {
    Serial.println("name: " + name + " value: " + property + " shadow: " + shadow_property + " permission: " + String(permission));
}

void ArduinoCloudStringProperty::updateShadow() {
    shadow_property = property;
}

String ArduinoCloudStringProperty::read() {
    /* permissions are intended as seen from cloud */
    if (permission & READ) {
        return property;
    }
}

bool ArduinoCloudStringProperty::canRead() {
    return (permission & READ);
}

String& ArduinoCloudStringProperty::getName() {
    return name;
}

void ArduinoCloudStringProperty::setName(String _name) {
    name = _name;
}

ArduinoCloudPropertyGeneric& ArduinoCloudStringProperty::setTag(int _tag) {
    tag = _tag;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

int ArduinoCloudStringProperty::getTag() {
    return tag;
}

ArduinoCloudPropertyGeneric& ArduinoCloudStringProperty::setPermission(permissionType _permission) {
    permission = _permission;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

ArduinoCloudPropertyGeneric& ArduinoCloudStringProperty::readOnly() {
    permission = READ;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

ArduinoCloudPropertyGeneric& ArduinoCloudStringProperty::minimumDelta(void* delta) {
    
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

permissionType ArduinoCloudStringProperty::getPermission() {
    return permission;
}

ArduinoCloudPropertyGeneric& ArduinoCloudStringProperty::onUpdate(void(*fn)(void)) {
    callback = fn;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

void ArduinoCloudStringProperty::append(CborObject &cbor) {
    if (!canRead()) {
        return;
    }
    if (tag != -1) {
        cbor.set("t", tag);
    } else {
        cbor.set("n", name.c_str());
    }
    appendValue(cbor);
    lastUpdated = millis();
}

ArduinoCloudPropertyGeneric& ArduinoCloudStringProperty::publishEvery(long seconds) {
    updatePolicy = seconds;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

bool ArduinoCloudStringProperty::newData() {
    return property != shadow_property;
}

bool ArduinoCloudStringProperty::shouldBeUpdated() {
    if (updatePolicy == ON_CHANGE) 
        return newData();
    // If data has not changed, even if updatePolicy elapsed, prop does not require to be updated
    return (newData() && millis() - lastUpdated > updatePolicy * 1000) ;
}

// Return property type
String ArduinoCloudStringProperty::getType() {
    String type = "STRING";
    return type;
}

inline void ArduinoCloudStringProperty::appendValue(CborObject &cbor) {
    cbor.set("vs", property.c_str());
};
 
