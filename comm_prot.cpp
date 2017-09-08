/*
 * modbus_rtu.cpp
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
	slave_addr = 7;
}

void Comm_prot::Parse(uint8_t* frame)
{
	uint8_t crc = Crc8(frame, 2);
	if((frame[0] == slave_addr + 100) && (frame[2] == crc))
	{
		queued_command = frame[1];
	}

	if((frame[0] == 0xFF) && (frame[2] == crc))
	{
		timer.Disable(TIMER_LED_BLINK);
		LED_GREEN_OFF;
		LED_RED_OFF;

		switch(queued_command)
		{
		case 0x01:
			LED_GREEN_ON;
		break;
		case 0x02:
			LED_RED_ON;
		break;
		case 0x03:
			timer.Assign(TIMER_LED_BLINK, 200, LedBlinkGreen);
		break;
		case 0x04:
			timer.Assign(TIMER_LED_BLINK, 200, LedBlinkRed);
		break;
		//case 0x05:
		//	timer.Assign(TIMER_LED_BLINK, 800,
		default:
		break;
		}
	}
}

void Comm_prot::Prepare(uint8_t res)
{
	usart_data.frame[0] = slave_addr;
	usart_data.frame[1] = res;
	usart_data.frame[2] = Crc8(usart_data.frame, 2);
	usart_data.frame[3] = 0x0A;
	usart_data.len = FRAME_LENGTH;
	usart.SendFrame(&usart_data);
}

void Comm_prot::EnqueueCommand(uint8_t command)
{
	queued_command = command;
}

uint8_t Comm_prot::Crc8(uint8_t *frame, uint8_t len)
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
