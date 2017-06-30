/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "led.h"

#if defined (__AVR_ATmega88PA__)
Timer timer(T0_PS_64, 229);
#endif
#if defined (__AVR_ATmega8__)
Timer timer(T1_PS_64, 229);
#endif

Led led;
Usart usart;
UsartData usart_data;
ModbusRTU modbus_rtu;

int main()
{
	sei();
	while(1);
}



