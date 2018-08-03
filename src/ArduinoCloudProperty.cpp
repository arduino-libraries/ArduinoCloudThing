#include <ArduinoCloudProperty.h>


template <typename T>
ArduinoCloudProperty<T>::ArduinoCloudProperty(T& _property, String _name) :
    property(_property), name(_name) {}

template <typename T>
bool ArduinoCloudProperty<T>::write(T value) {
    /* permissions are intended as seen from cloud */
    if ((permission & READWRITE) >> 1) {
        property = value;
        return true;
    }
    return false;
}

template <>
bool ArduinoCloudProperty<String*>::write(String *value) {
    /* permissions are intended as seen from cloud */
    if ((permission & READWRITE) >> 1) {
        *property = *value;
        return true;
    }
    return false;
}

template <typename T>
void ArduinoCloudProperty<T>::printinfo() {
    Serial.println("name: " + name + " value: " + String(property) + " shadow: " + String(shadow_property) + " permission: " + String(permission));
}

template <>
void ArduinoCloudProperty<String*>::printinfo() {
    Serial.println("name: " + name + " value: " + *property + " shadow: " + *shadow_property + " permission: " + String(permission));
}

template <typename T>
void ArduinoCloudProperty<T>::updateShadow() {
    shadow_property = property;
}

template <>
void ArduinoCloudProperty<String*>::updateShadow() {
    shadow_property = new String(*property);
}

template <typename T>
T ArduinoCloudProperty<T>::read() {
    /* permissions are intended as seen from cloud */
    if (permission & READ) {
        return property;
    }
}

template <typename T>
bool ArduinoCloudProperty<T>::canRead() {
    return (permission & READ);
}

template <typename T>
String& ArduinoCloudProperty<T>::getName() {
    return name;
}

template <typename T>
void ArduinoCloudProperty<T>::setName(String _name) {
    name = _name;
}

template <typename T>
ArduinoCloudPropertyGeneric& ArduinoCloudProperty<T>::setTag(int _tag) {
    tag = _tag;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

template <typename T>
int ArduinoCloudProperty<T>::getTag() {
    return tag;
}

template <typename T>
ArduinoCloudPropertyGeneric& ArduinoCloudProperty<T>::setPermission(permissionType _permission) {
    permission = _permission;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

template <typename T>
ArduinoCloudPropertyGeneric& ArduinoCloudProperty<T>::readOnly() {
    permission = READ;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

template <typename T>
ArduinoCloudPropertyGeneric& ArduinoCloudProperty<T>::minimumDelta(void* delta) {
    minDelta = *(T*)delta;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

template <typename T>
permissionType ArduinoCloudProperty<T>::getPermission() {
    return permission;
}

template <typename T>
ArduinoCloudPropertyGeneric& ArduinoCloudProperty<T>::onUpdate(void(*fn)(void)) {
    callback = fn;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

template <typename T>
void ArduinoCloudProperty<T>::append(CborEncoder* encoder) {
    if (!canRead()) 
        return;
    
    CborEncoder *mapEncoder;
    cbor_encoder_create_map(encoder, mapEncoder, CborIndefiniteLength);
    if (tag != -1) {
        cbor_encode_text_stringz(mapEncoder, "t");
        cbor_encode_int(mapEncoder, tag);
    } else {
        cbor_encode_text_stringz(mapEncoder, "n");
        cbor_encode_text_stringz(mapEncoder, name.c_str());
    }
    appendValue(mapEncoder);
    cbor_encoder_close_container(encoder, mapEncoder);
    lastUpdated = millis();
}

template <typename T>
ArduinoCloudPropertyGeneric& ArduinoCloudProperty<T>::publishEvery(long seconds) {
    updatePolicy = seconds;
    return *(reinterpret_cast<ArduinoCloudPropertyGeneric*>(this));
}

template <typename T>
bool ArduinoCloudProperty<T>::newData() {
    return (property != shadow_property && abs(property - shadow_property) > minDelta );
}

template <>
bool ArduinoCloudProperty<String*>::newData() {
    return *property != *shadow_property;
}

template <>
bool ArduinoCloudProperty<bool>::newData() {
    return property != shadow_property;
}

template <typename T>
bool ArduinoCloudProperty<T>::shouldBeUpdated() {
    if (updatePolicy == ON_CHANGE) 
        return newData();
    // If data has not changed, even if updatePolicy elapsed, prop does not require to be updated
    return (newData() && millis() - lastUpdated > updatePolicy * 1000) ;
}

// Return property type
template <typename T>
String ArduinoCloudProperty<T>::getType() {
    String type = "UNKNOWN";
    return type;
}
template <>
String ArduinoCloudProperty<int>::getType() {
    String type = "INT";
    return type;
}
template <>
String ArduinoCloudProperty<float>::getType() {
    String type = "FLOAT";
    return type;
}
template <>
String ArduinoCloudProperty<bool>::getType() {
    String type = "BOOL";
    return type;
}
template <>
String ArduinoCloudProperty<String>::getType() {
    String type = "STRING";
    return type;
}

// Specific template definitions for different data types
template <>
inline void ArduinoCloudProperty<int>::appendValue(CborEncoder* mapEncoder) {
    cbor_encode_text_stringz(mapEncoder, "v");
    cbor_encode_float(mapEncoder, (float)property);
};

template <>
inline void ArduinoCloudProperty<float>::appendValue(CborEncoder* mapEncoder) {
    cbor_encode_text_stringz(mapEncoder, "v");
    cbor_encode_float(mapEncoder, property);
};

template <>
inline void ArduinoCloudProperty<bool>::appendValue(CborEncoder* mapEncoder) {
    cbor_encode_text_stringz(mapEncoder, "vb");
    cbor_encode_boolean(mapEncoder, property);
};

// Explicit compiler
template class ArduinoCloudProperty<int>;
template class ArduinoCloudProperty<float>;
template class ArduinoCloudProperty<bool>; 

