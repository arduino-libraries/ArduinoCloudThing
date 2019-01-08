/*
 * \authors A. Entinger
 */

#ifndef TEST_ARDUINO_H_
#define TEST_ARDUINO_H_

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <string>

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/

typedef std::string String;

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
