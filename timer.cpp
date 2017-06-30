/*
 * timer.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>			// NULL
#include "timer.h"
#include "usart.h"
#include "led.h"

volatile TimerHandler STHandlers[8];

#if defined (__AVR_ATmega88PA__)
Timer::Timer(T0Prescallers Prescaller, uint8_t Tick)
{
	for(uint8_t n = 0; n < 8; n++)
	{
		STHandlers[n].Active = false;
		STHandlers[n].Counter = 0;
		STHandlers[n].Interval = 0;
		STHandlers[n].fp = NULL;
	}

	TCCR0A |= (1 << WGM01); 		// CTC
	TCCR0B |= Prescaller;
	OCR0A = Tick; 					// :		F_CPU / preskaler / 200 Hz = OCR
	TIMSK0 |= (1 << OCIE0A);
}
#endif

#if defined (__AVR_ATmega8__)
Timer::Timer(T1Prescallers Prescaller, uint8_t Tick)
{
	for(uint8_t n = 0; n < 8; n++)
	{
		STHandlers[n].Active = false;
		STHandlers[n].Counter = 0;
		STHandlers[n].Interval = 0;
		STHandlers[n].fp = NULL;
	}

	TCCR1B |= (1 << WGM12); 		// CTC
	TCCR1B |= Prescaller;
	OCR1A = Tick; 					// :		F_CPU / preskaler / 200 Hz = OCR
	TIMSK |= (1 << OCIE1A);
}
#endif

void Timer::Assign(uint8_t HandlerNumber, uint64_t Interval, void(*fp)())
{
	STHandlers [HandlerNumber].Interval = Interval;
	STHandlers [HandlerNumber].Counter = 0;
	STHandlers [HandlerNumber].Active = true;
	STHandlers [HandlerNumber].fp = fp;
}

void Timer::Enable(uint8_t HandlerNumber)
{
	STHandlers [HandlerNumber].Active = true;
	STHandlers [HandlerNumber].Counter = 0;
}

void Timer::Disable(uint8_t HandlerNumber)
{
	STHandlers [HandlerNumber].Active = false;
}

void ModbusRTU35T()
{
	usart.RTU35T();
}

void LedSW()
{
	static uint8_t i;
	LED_PORT ^= (1 << LED_PIN_RED);
	i++;
	if(i == 10)
	{
		i = 0;
		LED_PORT &= ~(1 << LED_PIN_RED);
		timer.Disable(7);
	}
}

#if defined (__AVR_ATmega88PA__)
ISR(TIMER0_COMPA_vect)
#endif

#if defined (__AVR_ATmega8__)
ISR(TIMER1_COMPA_vect)
#endif
{
	for(uint8_t n = 0; n < 8; n++)
	{
		if ((STHandlers[n].Active) && (STHandlers[n].fp != NULL))
		{
			if ((STHandlers[n].Counter == STHandlers[n].Interval))
			{
				STHandlers[n].Counter = 0;
				STHandlers [n].fp();
			}
			else
			{
				STHandlers[n].Counter++;
			}
		}
	}
}


