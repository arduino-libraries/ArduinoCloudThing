/*
 * \authors A. Entinger
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino.h>

#include <thread>
#include <chrono>

/******************************************************************************
 * NAMESPACES
 ******************************************************************************/

using namespace std::chrono;

/******************************************************************************
 * GLOBAL CONSTANTS
 ******************************************************************************/

time_point<steady_clock> const START = steady_clock::now();

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

unsigned long millis()
{
  return static_cast<unsigned long>(duration_cast<microseconds>(steady_clock::now() - START).count());
}

void delay(unsigned long const delay_ms)
{
  std::this_thread::sleep_for(microseconds(delay_ms));
}
