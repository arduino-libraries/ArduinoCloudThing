#include <ArduinoIoTCloud.h>
#include <WiFiConnectionManager.h>

char ssid[] = SECRET_SSID;    // Network SSID (name)
char pass[] = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

#define THING_ID "cfac3a86-8f3c-476f-b7e3-34d34c6f3dc6"

void onSwitchChange();
void onControllerChange();
void onSwitchAutoSync(ArduinoCloudProperty<bool> property);
void onSwitchForceCloudSync(ArduinoCloudProperty<bool> property);
void onSwitchForceDeviceSync(ArduinoCloudProperty<bool> property);
void onControllerSync(ArduinoCloudProperty<float> property);

bool Switch;
float controller;

void initProperties(){
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(Switch, READWRITE, PROPERTIES_SYNC_AUTO, onSwitchAutoSync, ON_CHANGE, onSwitchChange);
  ArduinoCloud.addProperty(controller, READWRITE, PROPERTIES_SYNC_AUTO, onControllerSync, ON_CHANGE, onControllerChange);
}

ConnectionManager *ArduinoIoTPreferredConnection = new WiFiConnectionManager(SECRET_SSID, SECRET_PASS);
