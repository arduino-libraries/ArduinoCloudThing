# Arduino Cloud Thing Middleware
[![Build Status](https://travis-ci.org/arduino-libraries/ArduinoCloudThing.svg?branch=master)](https://travis-ci.org/arduino-libraries/ArduinoCloudThing)
[![Code Coverage](https://codecov.io/gh/arduino-libraries/ArduinoCloudThing/branch/master/graph/badge.svg)](https://codecov.io/gh/arduino-libraries/ArduinoCloudThing)

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

`.onUpdate(functionPointer)` configures the property to call `functionPointer` when the value is changed by the cloud.

`.onSync(syncFunctionPointer)` configures the property to call `syncFunctionPointer` when as consequence of a connection/reconnection (so after a period passed offline) the value of the property must be synchronized with the one stored in the cloud. The implementation `syncFunctionPointer()` should contain some synchronization logic; standard implementations for this functions are:
  * MOST_RECENT_WINS: that compares the **cloud** timestamp of the last change with the corresponding **device** timestamp. The property is assigned the value with the higher timestamp. If the **cloud** value is used the `functionPointer` is called
  * CLOUD_WINS: the property is assigned the value coming from **cloud** regardless of timestamps and **device** value. 
  * DEVICE_WINS: the device property value is kept. The cloud property value will be updated at the next update cycle.

A custom synchronization logic may be implemented by setting a custom callback function with the following signature: `void (*_sync_callback_func)(ArduinoCloudProperty<T> property)`; use one of the specific types supported. The `property` object exposes several methods which will enable the custom logic to select the appropriate value.


A typical property creation could look like that:
```
void onUpdateCallback() {
  /* Do stuff */
}
...
int       int_property = 0;
int const min_delta    = 6;
...
thing.addProperty(int_property, "test_int_property", Permission::ReadWrite).publishOnChange(min_delta).onUpdate(onUpdateCallback).onSync(onSynchronizationCallback);
...
```

* **encode** Checks all properties if their value has changed and if so populates the buffer with the desired CBOR update message.

`int encode(uint8_t * data, size_t const size)`

* **decode**  decodes a CBOR buffer received from the cloud and if the syncMessage parameter is set to false (default), updates writeable properties accordingly and calls the update callback if the value of a property has changed. If the syncMessage parameter is set to true, the value of the property received by the cloud and the relative last change timestamp are passed to a synchronization callback. The synchronization callback could apply the logic to assign a value to the property choosing between the local value and the cloud one. The synchronization logic also decides if the call to the update callback is necessary.

`decode(uint8_t const * const payload, size_t const length, bool syncMessage = false)`

## Unit Tests

The code of `ArduinoCloudThing` is verified via unit tests which are build and executed by the Travis CI system after every commit. However, one can also build and execute the unit tests manually using the following steps:
```bash
cd test && mkdir build && cd build
cmake ..
make -j4
bin/testArduinoCloudThing
```
