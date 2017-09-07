/*
 * led.cpp
 *
 *  Created on: 30 cze 2017
 *      Author: tomek
 */

#include "led.h"

Led::Led()
{
	LED_DDR |= (1 << LED_PIN_GREEN) | (1 << LED_PIN_RED);
	LED_PORT &= ~((1 << LED_PIN_GREEN) | (1 << LED_PIN_RED));
}


