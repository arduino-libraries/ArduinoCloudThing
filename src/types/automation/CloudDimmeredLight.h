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

#ifndef CLOUDDIMMEREDLIGHT_H_
#define CLOUDDIMMEREDLIGHT_H_

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include <math.h>
#include <Arduino.h>
#include "../../ArduinoCloudProperty.h"

/******************************************************************************
   CLASS DECLARATION
 ******************************************************************************/
class DimmeredLight {
  public:
    bool swi;
    float bri;
    DimmeredLight(bool swi, float bri): swi(swi), bri(bri) {
    }

    bool operator==(DimmeredLight & aLight) {
      return aLight.swi == swi && aLight.bri == bri;
    }

    bool operator!=(DimmeredLight & aLight) {
      return !(operator==(aLight));
    }

};

class CloudDimmeredLight : public ArduinoCloudProperty {
  private:
    DimmeredLight _value,
                  _cloud_value;

  public:
    CloudDimmeredLight() : _value(false, 0), _cloud_value(false, 0) {}
    CloudDimmeredLight(bool swi, float brightness) : _value(swi, brightness), _cloud_value(swi, brightness) {}

    virtual bool isDifferentFromCloud() {

      return _value != _cloud_value;
    }

    CloudDimmeredLight& operator=(DimmeredLight aLight) {
      _value.swi = aLight.swi;
      _value.bri = aLight.bri;
      updateLocalTimestamp();
      return *this;
    }

    DimmeredLight getCloudValue() {
      return _cloud_value;
    }

    DimmeredLight getValue() {
      return _value;
    }
    
    virtual void fromCloudToLocal() {
      _value = _cloud_value;
    }
    virtual void fromLocalToCloud() {
      _cloud_value = _value;
    }

    virtual void appendAttributesToCloud() {
      appendAttribute(_value.swi);
      // To allow visualization through color widget
      // Start
      float hue = 0;
      float sat = 0;
      appendAttributeReal(hue, getAttributeName(".hue", '.'), encoder);
      appendAttributeReal(sat, getAttributeName(".sat", '.'), encoder);
      if (_value.swi) {
        appendAttribute(_value.bri);
      } else {
        float bri = 0;
        appendAttributeReal(bri, getAttributeName(".bri", '.'), encoder);
      }
      // should be only:
      // appendAttribute(_value.bri);
      // end
    }

    virtual void setAttributesFromCloud() {
      setAttribute(_cloud_value.swi);
      setAttribute(_cloud_value.bri);
    }
};

#endif /* CLOUDDIMMEREDLIGHT_H_ */