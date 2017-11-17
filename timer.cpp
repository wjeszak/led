/*
 * timer.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/interrupt.h>
#include "timer.h"
#include "led.h"

Timer::Timer(T0Prescallers Prescaller, uint8_t Tick)
{
	for(uint8_t n = 0; n < NUMBER_OF_TIMERS; n++)
	{
		timer_handlers[n].active = false;
		timer_handlers[n].counter = 0;
		timer_handlers[n].interval = 0;
		timer_handlers[n].fp = NULL;
	}

	TCCR0A |= (1 << WGM01); 		// CTC
	TCCR0B |= Prescaller;
	OCR0A = Tick; 					// :		F_CPU / preskaler / 200 Hz = OCR
	TIMSK0 |= (1 << OCIE0A);
}

void Timer::Irq()
{
	for(uint8_t n = 0; n < NUMBER_OF_TIMERS; n++)
	{
		if ((timer_handlers[n].active) && (timer_handlers[n].fp != NULL))
		{
			if ((timer_handlers[n].counter == timer_handlers[n].interval))
			{
				timer_handlers[n].counter = 0;
				timer_handlers[n].fp();
			}
			else
			{
				timer_handlers[n].counter++;
			}
		}
	}
}

void Timer::Assign(uint8_t handler_id, uint16_t interval, void(*fp)())
{
	timer_handlers[handler_id].interval = interval;
	timer_handlers[handler_id].counter = 0;
	timer_handlers[handler_id].active = true;
	timer_handlers[handler_id].fp = fp;
	TCNT0 = 0;
}

void Timer::Enable(uint8_t handler_id)
{
	timer_handlers[handler_id].active = true;
	timer_handlers[handler_id].counter = 0;
}

void Timer::Disable(uint8_t handler_id)
{
	timer_handlers[handler_id].active = false;
}

ISR(TIMER0_COMPA_vect)
{
	timer.Irq();
}
// -----------------------------------------------------------------
void GreenBlink()
{
	LED_GREEN_SW;
}

void RedBlink()
{
	LED_RED_SW;
}

void GreenRedBlink1()
{
	timer.Disable(TIMER_LED_BLINK);
	if(led.current_led == LED_GREEN_PIN)
	{
		LED_GREEN_ON;
		led.current_led = LED_GREEN_PIN;
		timer.Assign(TIMER_LED_BLINK, LED_PULSE_DELAY_PERIOD1, GreenRedBlink);
		return;
	}
	if(led.current_led == LED_RED_PIN)
	{
		LED_RED_ON;
		led.current_led = LED_RED_PIN;
		timer.Assign(TIMER_LED_BLINK, LED_PULSE_DELAY_PERIOD1, GreenRedBlink);
		return;
	}
}

void GreenRedBlink()
{
	timer.Disable(TIMER_LED_BLINK);
	if(led.current_led == LED_GREEN_PIN)
	{
		LED_GREEN_OFF;
		led.current_led = LED_RED_PIN;
		timer.Assign(TIMER_LED_BLINK, LED_PULSE_PERIOD, GreenRedBlink1);
		return;
	}
	if(led.current_led == LED_RED_PIN)
	{
		LED_RED_OFF;
		led.current_led = LED_GREEN_PIN;
		timer.Assign(TIMER_LED_BLINK, LED_PULSE_PERIOD, GreenRedBlink1);
		return;
	}
}

void Pulse()
{
	led.pulses_cnt++;
	if(led.pulses_cnt == led.pulses * 2)
	{
		led.pulses_cnt = 0;
		timer.Disable(TIMER_LED_PULSE);
	}
	LED_PORT ^= (1 << led.color);
}

void Delay()
{
	//timer.Disable(TIMER_LED_DELAY);
	//LED_PORT &= ~(1 << led.color);
	timer.Assign(TIMER_LED_PULSE, LED_PULSE_PERIOD, Pulse);
}

void GreenForTime()
{
	LED_GREEN_OFF;
	timer.Disable(TIMER_LED_FOR_TIME);
}
