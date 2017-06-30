/*
 * led.h
 *
 *  Created on: 30 cze 2017
 *      Author: tomek
 */

#ifndef LED_H_
#define LED_H_
#include <avr/io.h>

#define LED_DDR 		DDRB
#define LED_PORT 		PORTB
#define LED_PIN_GREEN 	1
#define LED_PIN_RED		2

class Led
{
public:
	Led();
};

#endif /* LED_H_ */
