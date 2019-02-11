# Arduino Cloud Thing Middleware
[![Build Status](https://travis-ci.org/arduino-libraries/ArduinoCloudThing.svg?branch=master)](https://travis-ci.org/arduino-libraries/ArduinoCloudThing)

This library contains the implementation of the class ArduinoCloudThing which allows to add **int**, **bool**, **float**, **String** properties which can be read from or written to the cloud. To achieve this goal ArduinoCloudThing possesses functions for encoding and decoding CBOR (Concise Binary Object Representation) encoded data which is used to transfer the properties value between a **thing** and the **cloud**.

The main focus of the library is on resource usage (never use more data than needed), transport independence and user friendliness.

## How-to-Use

* Instantiate a **thing** with

`ArduinoCloudThing thing`

* Initialize the **thing** (initialisation adds a `status` property) via

`thing.begin()`

* Now various properties can be registered with the **thing** which are then readable/writeable by the **cloud**. For adding a new property call

`thing.addProperty(my_property_variable, my_property_name, my_property_permission)`

`my_property_variable` can be an `int`, a `bool`, a `String` or a `float`. You can use that variable anywhere and its value will be transparently updated to the cloud.

`my_property_name` is a string which uniquely identifies a property for the system.

`my_property_permission` can be either `Permission::Read`, `Permission::Write` or `Permission::ReadWrite`.

Additional configuration can be added to the property via composition

`.publishEvery(unsigned long seconds)` configures a property to report its status (even if unchanged) every x seconds

`.publishOnChange()`

`.onUpdate(functionPointer)` configures the property to call `functionPointer` when the value is changed in the cloud.

A typical property creation could look like that:
```
void onUpdateCallback() {
  /* Do stuff */
}
...
int       int_property = 0;
int const min_delta    = 6;
...
thing.addProperty(int_property, "test_int_property", Permission::ReadWrite).publishOnChange(min_delta).onUpdate(onUpdateCallback);
...
```

* **encode** Checks all properties if their value has changed and if so populates the buffer with the desired CBOR update message.

`int encode(uint8_t * data, size_t const size)`

* **decode** decodes a CBOR buffer received from the cloud and updates writeable properties accordingly. Also the update callbacks are called, if the value of a property has changed.

`decode(uint8_t const * const data, size_t const length)`

## Unit Tests

The code of `ArduinoCloudThing` is verified via unit tests which are build and executed by the Travis CI system after every commit. However, one can also build and execute the unit tests manually using the following steps:
```bash
cd test && mkdir build && cd build
cmake ..
make -j4
bin/testArduinoCloudThing
```
