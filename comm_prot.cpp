/*
 * comm_prot.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include "comm_prot.h"
#include "usart.h"
#include "timer.h"
#include "led.h"

Comm_prot::Comm_prot()
{
	address = 6;
	queued_command = 0;
}

void Comm_prot::Parse(uint8_t* frame)
{
	uint8_t crc = Crc8(frame, 2);
	// diag
	if((frame[0] == address + ADDRESS_OFFSET) && (frame[1] == COMM_DIAG) && (frame[2] == crc))
	{
		comm.Prepare(0x80);
		return;
	}
	// queue or run immediately
	if((frame[0] == address + ADDRESS_OFFSET) && (frame[2] == crc))
	{
		if(NeedQueue())
			queued_command = frame[1] - (1 << COMM_NEED_QUEUE_BIT);
		else
			RunCommand(frame[1]);
	}
	// run queued command
	if((frame[0] == 0xFF) && (frame[2] == crc))
	{
		RunCommand(queued_command);
	}
}

void Comm_prot::Prepare(uint8_t res)
{
	usart_data.frame[0] = address + ADDRESS_OFFSET;
	usart_data.frame[1] = res;
	usart_data.frame[2] = Crc8(usart_data.frame, 2);
	usart_data.frame[3] = 0x0A;
	usart_data.len = FRAME_LENGTH;
	usart.SendFrame(&usart_data);
}

bool Comm_prot::NeedQueue()
{
	if(usart_data.frame[1] & (1 << COMM_NEED_QUEUE_BIT)) return true;
	return false;
}

void Comm_prot::EnqueueCommand(uint8_t command)
{
	queued_command = command;
}

void Comm_prot::RunCommand(uint8_t command)
{
	LED_GREEN_OFF;
	LED_RED_OFF;
	timer.Disable(TIMER_LED_BLINK);
	timer.Disable(TIMER_LED_PULSE);
	timer.Disable(TIMER_LED_DELAY);
	led.pulses_cnt = 0;
	switch(command)
	{
	case COMM_GREEN_ON:
		LED_GREEN_ON;
	break;
	case COMM_RED_ON:
		LED_RED_ON;
	break;
	case COMM_GREEN_BLINK:
		timer.Assign(TIMER_LED_BLINK, LED_BLINK_PERIOD, GreenBlink);
	break;
	case COMM_RED_BLINK:
		timer.Assign(TIMER_LED_BLINK, LED_BLINK_PERIOD, RedBlink);
	break;
	case COMM_GREEN_RED_BLINK:
		timer.Assign(TIMER_LED_BLINK, LED_BLINK_PERIOD, GreenRedBlink);
	break;
	case COMM_GREEN_1PULSE:
		led.SetParams(LED_GREEN_PIN, 1);
	break;
	case COMM_RED_1PULSE:
		led.SetParams(LED_RED_PIN, 1);
	break;
	case COMM_GREEN_2PULSES:
		led.SetParams(LED_GREEN_PIN, 2);
	break;
	case COMM_RED_2PULSES:
		led.SetParams(LED_RED_PIN, 2);
	break;
	case COMM_GREEN_3PULSES:
		led.SetParams(LED_GREEN_PIN, 3);
	break;
	case COMM_RED_3PULSES:
		led.SetParams(LED_RED_PIN, 3);
	break;
	case COMM_GREEN_ON_FOR_TIME:
		LED_GREEN_ON;
		timer.Assign(TIMER_LED_FOR_TIME, LED_FOR_TIME, GreenForTime);
	break;
	default:
	break;
	}
}

uint8_t Comm_prot::Crc8(uint8_t* frame, uint8_t len)
{
	uint8_t crc = 0x00;
	for (uint8_t pos = 0; pos < len; pos++)
	{
		crc ^= frame[pos];
		for (uint8_t i = 8; i != 0; i--)
		{
			if ((crc & 0x80) != 0)
			{
				crc <<= 1;
				crc ^= 0x07;
			}
			else
				crc <<= 1;
		}
	}
	return crc;
}
