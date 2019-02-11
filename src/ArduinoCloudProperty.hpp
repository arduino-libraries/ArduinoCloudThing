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

#ifndef ARDUINO_CLOUD_PROPERTY_HPP_
#define ARDUINO_CLOUD_PROPERTY_HPP_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino.h>

#include "lib/tinycbor/cbor-lib.h"

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

enum class CloudProtocol {
  V1, /* [{"n": "test", "vb": true}] */
  V2  /* [{0: "test", 4: true}]      */
};

enum class Permission {
  Read, Write, ReadWrite
};

enum class Type {
  Bool, Int, Float, String
};

enum class UpdatePolicy {
  OnChange, TimeInterval
};

/* Source: https://tools.ietf.org/html/rfc8428#section-6 */
enum class CborIntegerMapKey : int
{
   BaseVersion  = -1, /* bver */
   BaseName     = -2, /* bn   */
   BaseTime     = -3, /* bt   */
   BaseUnit     = -4, /* bu   */
   BaseValue    = -5, /* bv   */
   BaseSum      = -6, /* bs   */
   Name         =  0, /* n    */
   Unit         =  1, /* u    */
   Value        =  2, /* v    */
   StringValue  =  3, /* vs   */
   BooleanValue =  4, /* vb   */
   Sum          =  5, /* s    */
   Time         =  6, /* t    */
   UpdateTime   =  7, /* ut   */
   DataValue    =  8  /* vd   */
};

typedef void(*UpdateCallbackFunc)(void);

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

template <typename T>
class ArduinoCloudProperty {
public:

  ArduinoCloudProperty(T & property, String const & name, Permission const permission);

  void writeByCloud(T const val);

  /* Composable configuration of the ArduinoCloudProperty class */
  ArduinoCloudProperty<T> & onUpdate       (UpdateCallbackFunc func);
  ArduinoCloudProperty<T> & publishOnChange(T const min_delta_property, unsigned long const min_time_between_updates_millis = 0);
  ArduinoCloudProperty<T> & publishEvery   (unsigned long const seconds);

  inline String name              () const { return _name; }
  inline bool   isReadableByCloud () const { return (_permission == Permission::Read ) || (_permission == Permission::ReadWrite); }
  inline bool   isWriteableByCloud() const { return (_permission == Permission::Write) || (_permission == Permission::ReadWrite); }

  bool shouldBeUpdated        ();
  void execCallbackOnChange   ();
  void forceCallbackOnChange  ();
  void setLastChangeTime      (unsigned long cloudChangeTime);
  bool isAfterLastChange      (unsigned long cloudChangeTime); 

  void append                 (CborEncoder * encoder, CloudProtocol const cloud_protocol);
  void setShadowValue         (T const val);         
  void updateTime             (unsigned long );
  bool isChangedLocally       ();

private:

  T                & _property,
                     _shadow_property,
                     _local_shadow_property;
  String             _name;
  Permission         _permission;
  UpdateCallbackFunc _update_callback_func;

  UpdatePolicy       _update_policy;
  bool               _has_been_updated_once,
                     _has_been_modified_in_callback;  
  /* Variables used for UpdatePolicy::OnChange */
  T                  _min_delta_property;
  unsigned long      _min_time_between_updates_millis;
  /* Variables used for UpdatePolicy::TimeInterval */
  unsigned long      _last_updated_millis,
                     _update_interval_millis;
  /* Variables used for reconnection sync*/
  unsigned long      _local_change_timestamp;
  unsigned long      _last_change_timestamp;
        

  void appendValue(CborEncoder * mapEncoder, CloudProtocol const cloud_protocol) const;
  bool isValueDifferent(T const lhs, T const rhs) const;

  T getInitialMinDeltaPropertyValue() const;

};

/******************************************************************************
 * PROTOTYPE FREE FUNCTIONs
 ******************************************************************************/

template <typename T>
inline bool operator == (ArduinoCloudProperty<T> const & lhs, ArduinoCloudProperty<T> const & rhs) { return (lhs.name() == rhs.name()); }

/******************************************************************************
 * TEMPLATE IMPLEMENTATION
 ******************************************************************************/

#include "ArduinoCloudProperty.ipp"

#endif /* ARDUINO_CLOUD_PROPERTY_HPP_ */
