/*
 * modbus_rtu.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef MODBUS_RTU_H_
#define MODBUS_RTU_H_

#include <inttypes.h>

#define NUMBER_OF_HOLDING_REGISTERS 	20

class ModbusRTU
{
public:
	ModbusRTU();
	void ParseFrame(uint8_t* frame, uint8_t len);
	void PrepareFrame(uint8_t* frame);
	uint16_t HoldingRegisters[NUMBER_OF_HOLDING_REGISTERS];
private:
	//uint8_t ReadHoldingRegisters(uint8_t *frame);
	uint8_t WriteSingleRegister(uint8_t* frame);
	void FunctionNotSupported(uint8_t *frame);
	uint16_t Checksum(uint8_t *frame, uint8_t len);
	uint8_t slave_addr;
	uint16_t starting_address;
	uint16_t quantity;
	uint8_t byte_count;
};

extern ModbusRTU modbus_rtu;

#endif /* MODBUS_RTU_H_ */
