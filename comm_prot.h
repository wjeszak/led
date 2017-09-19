/*
 * comm_prot.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef COMM_PROT_H_
#define COMM_PROT_H_

#include <inttypes.h>

#define FRAME_LENGTH 			4
#define ADDRESS_OFFSET 			100

#define COMM_GREEN_RED_OFF 		0x00
#define COMM_GREEN_ON 			0x01
#define COMM_RED_ON 			0x02
#define COMM_GREEN_BLINK 		0x03
#define COMM_RED_BLINK 			0x04
#define COMM_GREEN_RED_BLINK 	0x05
#define COMM_GREEN_1PULSE 		0x06
#define COMM_RED_1PULSE 		0x07
#define COMM_GREEN_2PULSES 		0x08
#define COMM_RED_2PULSES 		0x09
#define COMM_GREEN_3PULSES 		0x0A
#define COMM_RED_3PULSES 		0x0B
#define COMM_GREEN_ON_FOR_TIME 	0x0C
#define COMM_DIAG 				0x0D
#define COMM_NEED_QUEUE_BIT		7

class Comm_prot
{
public:
	Comm_prot();
	void Parse(uint8_t* frame);
	void Prepare(uint8_t res);
private:
	bool NeedQueue();
	void EnqueueCommand(uint8_t command);
	void RunCommand(uint8_t command);
	uint8_t Crc8(uint8_t* frame, uint8_t len);
	uint8_t address;
	uint8_t queued_command;
};

extern Comm_prot comm;

#endif /* COMM_PROT_H_ */
