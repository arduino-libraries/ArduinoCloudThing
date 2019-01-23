/*
 * \authors A. Entinger
 */

#ifndef TEST_ARDUINO_H_
#define TEST_ARDUINO_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <string.h>

#include <string>

/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class String
{
public:

  String();
  String(char const * str);
  String(String const & str);

  inline char const * c_str() const { return _str.c_str(); }

private:

  std::string _str;

};

bool operator == (String const & lhs, String const & rhs);
bool operator != (String const & lhs, String const & rhs);

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

void          set_millis(unsigned long const millis);
unsigned long millis();

#endif /* TEST_ARDUINO_H_ */
