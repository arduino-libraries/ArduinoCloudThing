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

/* In order to speed up testing the following two functions are not
 * honoring the contract set up by their name.
 *  * millis() is returning the microseconds since system start
 *  * delay() is delaying by 'delay_ms' microseconds - not milliseconds
 */
unsigned long millis();
void delay(unsigned long const delay_ms);

#endif /* TEST_ARDUINO_H_ */
