/*
 * led.cpp
 *
 *  Created on: 30 cze 2017
 *      Author: tomek
 */

#include "led.h"
#include "timer.h"

Led::Led()
{
	LED_DDR |= (1 << LED_GREEN_PIN) | (1 << LED_RED_PIN);
	LED_PORT &= ~((1 << LED_GREEN_PIN) | (1 << LED_RED_PIN));
}

void Led::SetParams(uint8_t led_color, uint8_t led_pulses)
{
	color = led_color;
	pulses = led_pulses;
	timer.Assign(TIMER_LED_PULSE, LED_PULSE_PERIOD, Pulse);
	timer.Assign(TIMER_LED_DELAY, LED_PULSE_DELAY_PERIOD, Delay);
}


