/*
 * \authors A. Entinger
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino.h>

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

static unsigned long current_millis = 0;

/******************************************************************************
 * CTOR/DTOR
 ******************************************************************************/

String::String()
{

}

String::String(char const * str)
{
  if(str) _str = std::string(str);
}

String::String(String const & str) : String(str.c_str())
{

}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

bool operator == (String const & lhs, String const & rhs)
{
  return (strcmp(lhs.c_str(), rhs.c_str()) == 0);
}

bool operator != (String const & lhs, String const & rhs)
{
  return !(lhs == rhs);
}

void set_millis(unsigned long const millis)
{
  current_millis = millis;
}

unsigned long millis()
{
  return current_millis;
}
