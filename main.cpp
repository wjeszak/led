/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "comm_prot.h"
#include "usart.h"
#include "timer.h"
#include "led.h"

Timer timer(T0_PS_64, 115);
Led led;
Usart usart;
UsartData usart_data;
Comm_prot comm;

int main()
{
	sei();
	while(1);
}
