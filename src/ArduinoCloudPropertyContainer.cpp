/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "ArduinoCloudPropertyContainer.hpp"

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool ArduinoCloudPropertyContainer::isPropertyInContainer(Type const type, String const & name) {
  if      (type == Type::Bool  ) return isPropertyInList(_bool_property_list,   name);
  else if (type == Type::Int   ) return isPropertyInList(_int_property_list,    name);
  else if (type == Type::Float ) return isPropertyInList(_float_property_list,  name);
  else if (type == Type::String) return isPropertyInList(_string_property_list, name);
  else                           return false;
}

int ArduinoCloudPropertyContainer::getNumOfChangedProperties() {
  int num_changes_properties = 0;

  num_changes_properties += getNumOfChangedProperties(_bool_property_list  );
  num_changes_properties += getNumOfChangedProperties(_int_property_list   );
  num_changes_properties += getNumOfChangedProperties(_float_property_list );
  num_changes_properties += getNumOfChangedProperties(_string_property_list);

  return num_changes_properties;
}

void ArduinoCloudPropertyContainer::appendChangedProperties(CborEncoder * arrayEncoder) {
  appendChangedProperties<bool>  (_bool_property_list,   arrayEncoder);
  appendChangedProperties<int>   (_int_property_list,    arrayEncoder);
  appendChangedProperties<float> (_float_property_list,  arrayEncoder);
  appendChangedProperties<String>(_string_property_list, arrayEncoder);
}
