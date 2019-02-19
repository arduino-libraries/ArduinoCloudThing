#include <ArduinoIoTCloud.h>
#include <WiFiConnectionManager.h>

char ssid[] = SECRET_SSID;    // Network SSID (name)
char pass[] = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

#define THING_ID "PUT_YOUT_THING_ID_HERE"

void onSwitchChange();
<<<<<<< HEAD
void onControllerChange();
void onSwitchAutoSync(ArduinoCloudProperty<bool> property);
void onSwitchForceCloudSync(ArduinoCloudProperty<bool> property);
void onSwitchForceDeviceSync(ArduinoCloudProperty<bool> property);
void onControllerSync(ArduinoCloudProperty<float> property);
=======

void onSwitchSync(ArduinoCloudProperty<T> property);
>>>>>>> definition of sychronization callback moved into ArduinoCloudThing, test cases completed

bool Switch;
float controller;

void initProperties(){
  ArduinoCloud.setThingId(THING_ID);
<<<<<<< HEAD
  ArduinoCloud.addProperty(Switch, READWRITE, PROPERTIES_SYNC_AUTO, onSwitchAutoSync, ON_CHANGE, onSwitchChange);
  ArduinoCloud.addProperty(controller, READWRITE, PROPERTIES_SYNC_AUTO, onControllerSync, ON_CHANGE, onControllerChange);
=======
  ArduinoCloud.addProperty(Switch, READWRITE, ON_CHANGE, onSwitchChange, onSwitchSync);
>>>>>>> definition of sychronization callback moved into ArduinoCloudThing, test cases completed
}

ConnectionManager *ArduinoIoTPreferredConnection = new WiFiConnectionManager(SECRET_SSID, SECRET_PASS);
