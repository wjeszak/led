/*
 * modbus_rtu.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include "modbus_rtu.h"
#include "usart.h"
#include "led.h"
#include "timer.h"

ModbusRTU::ModbusRTU()
{
	slave_addr = 101;
	starting_address = 0;
	quantity = 0;
	byte_count = 0;
}

void ModbusRTU::ParseFrame(uint8_t* frame, uint8_t len)
{
	uint16_t crc = Checksum(frame, len - 2);
	if((frame[0] == slave_addr) && ((uint8_t) crc == frame[len - 2]) && ((uint8_t) (crc >> 8)) == frame[len - 1])
	{
		switch(frame[1])
		{
			case 6:
				if(WriteSingleRegister(frame) == 0)
				{
					//LED_PORT |= (1 << LED_PIN_RED);
					timer.Assign(7, 100, LedSW);
				}
					//PrepareFrame(usart_data.frame);
				//electromagnet.TestCoil();
			break;
			default:
				FunctionNotSupported(frame);
		}
	}
}

uint8_t ModbusRTU::WriteSingleRegister(uint8_t* frame)
{
	if(frame[5] == 25) return 0;
		return 1;
}
/*
uint8_t ModbusRTU::ReadHoldingRegisters(uint8_t* frame)
{
	starting_address = frame[2] << 8 | frame[3];
	quantity		 = frame[4] << 8 | frame[5];
	uint8_t error_code = 0;

	if((quantity > 125) || (quantity < 1)) error_code = 3;
	if((starting_address + quantity) > NUMBER_OF_HOLDING_REGISTERS) error_code = 2;

	if(error_code)
	{
		frame[0] = slave_addr;
		frame[1] = frame[1] + 0x80;
		frame[2] = error_code;
		uint16_t crc = Checksum(frame, 3);
		frame[3] = (uint8_t) crc;
		frame[4] = (uint8_t) (crc >> 8);
		usart_data.len = 5;
		usart.SendFrame(&usart_data);
		return error_code;
	}
	return 0;
}
*/
void ModbusRTU::PrepareFrame(uint8_t* frame)
{
	byte_count = (uint8_t)(quantity * 2);
	frame[0] = slave_addr;
	//frame[1] =
	frame[2] = byte_count;

	for(uint8_t i = 0; i < quantity; i++)
	{
		frame[3 + 2 * i] 	 = (uint8_t)(HoldingRegisters[starting_address + i] >> 8);
		frame[3 + 2 * i + 1] = (uint8_t)(HoldingRegisters[starting_address + i]);
	}
	uint16_t crc = Checksum(frame, (3 + byte_count));
	frame[3 + byte_count] = (uint8_t) crc;
	frame[4 + byte_count] = (uint8_t) (crc >> 8);
	usart_data.len = 5 + byte_count;
	usart.SendFrame(&usart_data);
}

void ModbusRTU::FunctionNotSupported(uint8_t *frame)
{
	frame[0] = slave_addr;
	frame[1] = frame[1] + 0x80;
	frame[2] = 1; 									// Illegal function
	uint16_t crc = Checksum(frame, 3);
	frame[3] = (uint8_t) crc;
	frame[4] = (uint8_t) (crc >> 8);
	usart_data.len = 5;
	usart.SendFrame(&usart_data);
}

uint16_t ModbusRTU::Checksum(uint8_t *frame, uint8_t len)
{
	uint16_t crc = 0xFFFF;
	int pos = 0;
	for (pos = 0; pos < len; pos++)
	{
		crc ^= (uint16_t)frame[pos];
		int i = 0;
		for (i = 8; i != 0; i--)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			crc >>= 1;
		}
	}
	return crc;
}
