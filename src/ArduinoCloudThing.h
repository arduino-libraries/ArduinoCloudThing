//
// This file is part of ArduinoCloudThing
//
// Copyright 2019 ARDUINO SA (http://www.arduino.cc/)
//
// This software is released under the GNU General Public License version 3,
// which covers the main part of ArduinoCloudThing.
// The terms of this license can be found at:
// https://www.gnu.org/licenses/gpl-3.0.en.html
//
// You can be released from the requirements of the above licenses by purchasing
// a commercial license. Buying such a license is mandatory if you want to modify or
// otherwise use the software for commercial activities involving the Arduino
// software without disclosing the source code of your own applications. To purchase
// a commercial license, send an email to license@arduino.cc.
//

#ifndef ARDUINO_CLOUD_THING_H_
#define ARDUINO_CLOUD_THING_H_

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "ArduinoCloudProperty.hpp"
#include "lib/LinkedList/LinkedList.h"
#include "types/CloudBool.h"
#include "types/CloudFloat.h"
#include "types/CloudInt.h"
#include "types/CloudString.h"
#include "types/CloudLocation.h"
#include "types/CloudColor.h"
#include "types/CloudWrapperBase.h"
#include "types/CloudWrapperBool.h"
#include "types/CloudWrapperFloat.h"
#include "types/CloudWrapperInt.h"
#include "types/CloudWrapperString.h"


/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static bool ON  = true;
static bool OFF = false;

static long const ON_CHANGE = -1;
static long const SECONDS   = 1;
static long const MINUTES   = 60;
static long const HOURS     = 3600;
static long const DAYS      = 86400;

typedef enum {
  READ      = 0x01,
  WRITE     = 0x02,
  READWRITE = READ | WRITE
} permissionType;

/******************************************************************************
   SYNCHRONIZATION CALLBACKS
 ******************************************************************************/

void onAutoSync(ArduinoCloudProperty & property);
#define MOST_RECENT_WINS onAutoSync
void onForceCloudSync(ArduinoCloudProperty & property);
#define CLOUD_WINS onForceCloudSync
void onForceDeviceSync(ArduinoCloudProperty & property);
#define DEVICE_WINS onForceDeviceSync // The device property value is already the correct one. The cloud property value will be synchronized at the next update cycle.

/******************************************************************************
   CLASS DECLARATION
 ******************************************************************************/

class ArduinoCloudThing {

  public:
    ArduinoCloudThing();

    void begin();

#define addProperty( v, ...) getThing().addPropertyReal(v, #v, __VA_ARGS__)

    static unsigned long const DEFAULT_MIN_TIME_BETWEEN_UPDATES_MILLIS = 500; /* Data rate throttled to 2 Hz */

    void addPropertyReal(ArduinoCloudProperty& property, String name, permissionType permission_type = READWRITE, long seconds = ON_CHANGE, void(*fn)(void) = NULL, float minDelta = 0.0f, void(*synFn)(ArduinoCloudProperty& property) = CLOUD_WINS) {
      Permission permission = Permission::ReadWrite;
      if (permission_type == READ) {
        permission = Permission::Read;
      } else if (permission_type == WRITE) {
        permission = Permission::Write;
      } else {
        permission = Permission::ReadWrite;
      }

      if (seconds == ON_CHANGE) {
        addPropertyReal(property, name, permission).publishOnChange(minDelta, DEFAULT_MIN_TIME_BETWEEN_UPDATES_MILLIS).onUpdate(fn).onSync(synFn);
      } else {
        addPropertyReal(property, name, permission).publishEvery(seconds).onUpdate(fn).onSync(synFn);
      }
    }
    void addPropertyReal(bool& property, String name, permissionType permission_type = READWRITE, long seconds = ON_CHANGE, void(*fn)(void) = NULL, float minDelta = 0.0f, void(*synFn)(ArduinoCloudProperty & property) = CLOUD_WINS) {
      ArduinoCloudProperty *p = new CloudWrapperBool(property);
      addPropertyReal(*p, name, permission_type, seconds, fn, minDelta, synFn);
    }
    void addPropertyReal(float& property, String name, permissionType permission_type = READWRITE, long seconds = ON_CHANGE, void(*fn)(void) = NULL, float minDelta = 0.0f, void(*synFn)(ArduinoCloudProperty & property) = CLOUD_WINS) {
      ArduinoCloudProperty *p = new CloudWrapperFloat(property);
      addPropertyReal(*p, name, permission_type, seconds, fn, minDelta, synFn);
    }
    void addPropertyReal(int& property, String name, permissionType permission_type = READWRITE, long seconds = ON_CHANGE, void(*fn)(void) = NULL, float minDelta = 0.0, void(*synFn)(ArduinoCloudProperty & property) = CLOUD_WINS) {
      ArduinoCloudProperty *p = new CloudWrapperInt(property);
      addPropertyReal(*p, name, permission_type, seconds, fn, minDelta, synFn);
    }
    void addPropertyReal(String& property, String name, permissionType permission_type = READWRITE, long seconds = ON_CHANGE, void(*fn)(void) = NULL, float minDelta = 0.0f, void(*synFn)(ArduinoCloudProperty & property) = CLOUD_WINS) {
      ArduinoCloudProperty *p = new CloudWrapperString(property);
      addPropertyReal(*p, name, permission_type, seconds, fn, minDelta, synFn);
    }

    ArduinoCloudProperty   & addPropertyReal(ArduinoCloudProperty   & property, String const & name, Permission const permission);

    /* encode return > 0 if a property has changed and encodes the changed properties in CBOR format into the provided buffer */
    int encode(uint8_t * data, size_t const size);
    /* decode a CBOR payload received from the cloud */
    void decode(uint8_t const * const payload, size_t const length, bool isSyncMessage = false);

    bool isPropertyInContainer(String const & name);
    int appendChangedProperties(CborEncoder * arrayEncoder);
    inline void addThingProperty(ArduinoCloudProperty   * property_obj) {
      _property_list.add(property_obj);
    }
    ArduinoCloudProperty * getProperty(String const & name);
    void updateTimestampOnLocallyChangedProperties();
    void updateProperty(String propertyName, unsigned long cloudChangeEventTime);

  private:
    LinkedList<ArduinoCloudProperty *>   _property_list;
    /* Keep track of the number of primitive properties in the Thing. If 0 it allows the early exit in updateTimestampOnLocallyChangedProperties() */
    int                                  _numPrimitivesProperties;
    /* Indicates the if the message received to be decoded is a response to the getLastValues inquiry */
    bool                                 _isSyncMessage;
    /* List of map data that will hold all the attributes of a property */
    LinkedList<CborMapData *>            _map_data_list;
    /* Current property name during decoding: use to look for a new property in the senml value array */
    String                               _currentPropertyName;
    unsigned long                        _currentPropertyBaseTime,
             _currentPropertyTime;

    enum class MapParserState {
      EnterMap,
      MapKey,
      UndefinedKey,
      BaseVersion,
      BaseName,
      BaseTime,
      Name,
      Value,
      StringValue,
      BooleanValue,
      Time,
      LeaveMap,
      Complete,
      Error
    };

    MapParserState handle_EnterMap(CborValue * map_iter, CborValue * value_iter, CborMapData **map_data);
    MapParserState handle_MapKey(CborValue * value_iter);
    MapParserState handle_UndefinedKey(CborValue * value_iter);
    MapParserState handle_BaseVersion(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_BaseName(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_BaseTime(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_Name(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_Value(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_StringValue(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_BooleanValue(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_Time(CborValue * value_iter, CborMapData * map_data);
    MapParserState handle_LeaveMap(CborValue * map_iter, CborValue * value_iter, CborMapData * map_data);

    static bool   ifNumericConvertToDouble(CborValue * value_iter, double * numeric_val);
    static double convertCborHalfFloatToDouble(uint16_t const half_val);
    void freeMapDataList(LinkedList<CborMapData *> *map_data_list);

};

#endif /* ARDUINO_CLOUD_THING_H_ */
