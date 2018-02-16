#ifndef MRAA_BACKEND
#include <WiFi101.h>
#else
#include "WiFi.h"
#endif
#include <ArduinoCloudNew.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////

char ssid[] = "BCMIlabs-2g";     //  your network SSID (name)
char pass[] = "SpaccaBCMISpacca";  // your network password

// Arduino Cloud settings and credentials
const char userName[] = SECRET_USER_NAME;
const char thingName[] = SECRET_THING_NAME;
const char thingId[] = SECRET_THING_ID;
const char thingPsw[]  = SECRET_THING_PSW;

//WiFiSSLClient sslClient;
WiFiClient sslClient;

// build a new object "cloudObject"
ArduinoCloudThing cloudObject;

const int buttonPin = 6;
bool position = OFF;
int counter = 0;

#ifdef ARDUINO_ARCH_MRAA
#define Serial DebugSerial
#endif

void test() {
  Serial.print("callback on position: ");
  Serial.println(position);
}

void setup() {

  // configure the button pin as input
  pinMode(buttonPin, INPUT);

  Serial.begin (9600);
  while (!Serial);

  int status = WL_IDLE_STATUS;     // the WiFi radio's status

#ifndef ARDUINO_ARCH_MRAA
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }
#endif

  // setup the "cloudObject"

  Serial.println("Calling cloud.begin()");

  cloudObject.begin(sslClient);

  Serial.println("Calling cloud.addProperty()");

  // define the properties
  cloudObject.addProperty(position, READ).publishEvery(10*SECONDS).onUpdate(test);
  cloudObject.addProperty(counter, READWRITE);
}

void loop() {

  Serial.println("Calling cloud.poll()");
  // subscribes to RW properties and look at the connections status
  cloudObject.poll();

  // read the button
  if (counter == 6) {
    // button is pressed, write position as "on"
    position = ON;
  } else {
    // button is released, write position as "off"
    position = OFF;
  }

  Serial.println(counter++);

  if (counter == 100) {
    counter = 0;
  }

  delay(1000);

  if ( WiFi.status() != WL_CONNECTED) {
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
      // unsuccessful, retry in 4 seconds
      Serial.print("failed ... ");
      delay(4000);
      Serial.print("retrying ... ");
    }
  }

}
