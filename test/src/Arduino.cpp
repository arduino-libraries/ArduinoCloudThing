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
 * PUBLIC FUNCTIONS
 ******************************************************************************/

unsigned long millis()
{
  return static_cast<unsigned long>(duration_cast<microseconds>(steady_clock::now() - START).count());
}

void delay(unsigned long const delay_ms)
{
  std::this_thread::sleep_for(microseconds(delay_ms));
}
