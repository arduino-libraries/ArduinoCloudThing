__Arduino Cloud Thing middleware__

This library contains an implementation of a so-called Thing, with compressors and decompressors from Cbor.

The main focus is on resource usage (never use more data than needed), transport indipendence and user friendlyness.

_How to use_

* Instantiate a Thing with

`ArduinoCloudThing thing`

* To initialize it (this adds `status` property)

`thing.begin()`

* To add a property

`thing.addProperty(variable)`

`variable` can be an `int`, a `bool`, a `String` or a `float`. You can use that variable anywhere and its value will be transparently updated when the remote backend writes it.

Additional configuration can be added to the property via composition

`.publishEvery(long seconds)` configures a property to report its status (even if unchanged) every x seconds

`.readOnly()`

`.writeOnly()`

`.onUpdate(functionPointer)` configures the property to call `functionPointer` when the value is called by the backend. This ensures the minumum amount of check can be performed inside the actual loop()

* Update the status and prepare a buffer with packed data

`int poll(uint8_t* data)`

populates the buffer with properties which need to be sent and returns the buffer length

* To decode a raw buffer

`decode(uint8_t* data, int length)`

Decodes the buffer, updates the variables and calls the configured callbacks
