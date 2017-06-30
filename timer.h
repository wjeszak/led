/*
 * timer.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <avr/io.h>
#include "machine.h"

// PCB door: F_CPU = 73728000 Hz
// OCR = (F_CPU / 64 / 500) - 1 = 229		~2 ms
// UART baud = 19200 bps -> tchar = 1 / (192000 / 10 bits frame) = 0.5 ms

// poprawic kolejnosc bitow
#if defined (__AVR_ATmega88PA__)
enum T0Prescallers
{
	T0_PS_0 = 0,
	T0_PS_1 = (1 << CS00),
	T0_PS_8 = (1 << CS01),
	T0_PS_64 = (1 << CS00) | (1 << CS01),
	T0_PS_256 = (1 << CS02),
	T0_PS_1024 = (1 << CS00) | (1 << CS02)
};
#endif
// to jest timer 1, ale juz nie bede zmienial

#if defined (__AVR_ATmega8__)
enum T1Prescallers
{
	T1_PS_0 = 0,
	T1_PS_1 = (1 << CS10),
	T1_PS_8 = (1 << CS11),
	T1_PS_64 = (1 << CS11) | (1 << CS10),
	T1_PS_256 = (1 << CS12),
	T1_PS_1024 = (1 << CS12) | (1 << CS10)
};
#endif

struct TimerHandler
{
	void(*fp)();
	bool Active;
	uint64_t Interval;
	uint64_t Counter;
};

class Timer
{
public:
#if defined (__AVR_ATmega88PA__)
	Timer(T0Prescallers Prescaller, uint8_t Tick);
#endif
#if defined (__AVR_ATmega8__)
	Timer(T1Prescallers Prescaller, uint8_t Tick);
#endif
	void Assign(uint8_t HandlerNumber, uint64_t Interval, void(*fp)());
//	int8_t Assign (uint64_t Interval, void(*fp)());
	void UnAssign (uint8_t HandlerNumber);
	void Enable (uint8_t HandlerNumber);
	void Disable (uint8_t HandlerNumber);
	void Hold (uint8_t HandlerNumber);
	void UnHold (uint8_t HandlerNumber);
	bool Enabled (uint8_t HandlerNumber);
	void SetInterval (uint8_t HandlerNumber, uint64_t Interval);
	uint64_t GetInterval (uint8_t HandlerNumber);
//	void SetPrescaller (T0Prescallers Prescaller);
	void SetPeriod (uint8_t Tick);
};
extern void DisplayRefresh();
extern void ModbusRTU35T();
extern void LedSW();

extern Timer timer;

#endif /* TIMER_H_ */
