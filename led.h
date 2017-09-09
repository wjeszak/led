/*
 * led.h
 *
 *  Created on: 30 cze 2017
 *      Author: tomek
 */

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define LED_DDR 				DDRB
#define LED_PORT 				PORTB
#define LED_GREEN_PIN 			1
#define LED_RED_PIN				2

#define LED_GREEN_OFF 			LED_PORT &= ~(1 << LED_GREEN_PIN)
#define LED_GREEN_ON 			LED_PORT |=  (1 << LED_GREEN_PIN)
#define LED_RED_OFF 			LED_PORT &= ~(1 << LED_RED_PIN)
#define LED_RED_ON 				LED_PORT |=  (1 << LED_RED_PIN)
#define LED_GREEN_SW 			LED_PORT ^=  (1 << LED_GREEN_PIN)
#define LED_RED_SW 				LED_PORT ^=  (1 << LED_RED_PIN)

#define LED_BLINK_PERIOD 		200 		// [ms]
#define LED_PULSE_PERIOD		50
#define LED_PULSE_DELAY_PERIOD	1000

class Led
{
public:
	Led();
	void SetParams(uint8_t led_color, uint8_t led_pulses);
	uint8_t color, pulses_cnt, pulses;
};

extern Led led;

#endif /* LED_H_ */
