/*
 * usart.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef USART_H_
#define USART_H_

#include <inttypes.h>
#include "state_machine.h"

#if defined (__AVR_ATmega88P__)
	#define USART_DE_DDR 			DDRC
	#define USART_DE_PORT 			PORTC
	#define USART_DE_PIN	 		3
#endif

#if defined (__AVR_ATmega8__)
	#define USART_DE_DDR 			DDRD
	#define USART_DE_PORT 			PORTD
	#define USART_DE_PIN	 		2
#endif

#define USART_DE_RECEIVE 		USART_DE_PORT &= ~(1 << USART_DE_PIN)
#define USART_DE_SEND 			USART_DE_PORT |=  (1 << USART_DE_PIN)
#define USART_DE_INIT 			USART_DE_DDR  |=  (1 << USART_DE_PIN)

#define UART_BUF_SIZE 			16
#define UART_BUF_MASK 			(UART_BUF_SIZE - 1)

#if defined (__AVR_ATmega88P__)
#define US_UBRRH 	UBRR0H
#define US_UBRRL 	UBRR0L
#define US_UCSRB	UCSR0B
#define US_RXEN 	RXEN0
#define US_TXEN 	TXEN0
#define US_RXCIE	RXCIE0
#define US_TXCIE	TXCIE0
#define US_UDRIE 	UDRIE0
#define US_UDR 		UDR0
#define US_RX 		USART_RX_vect
#define US_UDRE 	USART_UDRE_vect
#define US_TX 		USART_TX_vect
#endif

#if defined (__AVR_ATmega8__)
#define US_UBRRH 	UBRRH
#define US_UBRRL 	UBRRL
#define US_UCSRB	UCSRB
#define US_RXEN 	RXEN
#define US_TXEN 	TXEN
#define US_RXCIE	RXCIE
#define US_TXCIE	TXCIE
#define US_UDRIE 	UDRIE
#define US_UDR 		UDR
#define US_RX 		USART_RXC_vect
#define US_UDRE 	USART_UDRE_vect
#define US_TX 		USART_TXC_vect
#endif

class UsartData : public EventData
{
public:
	uint8_t c;
	uint8_t frame[UART_BUF_SIZE];
	uint8_t len;
};

class Usart : public StateMachine
{
public:
	Usart(uint16_t baud = 19200);
	// Events
	void EV_NewByte(UsartData* pdata);							// RX_vect callback
	void EV_TXBufferEmpty(UsartData* pdata = NULL);				// UDRE_vect callback
	void EV_TXComplete(UsartData* pdata = NULL);				// TX_vect callback
	void SendFrame(UsartData* pdata);
private:
	void RxEnable();
	void RxDisable();
	void TxEnable();
	void TxDisable();
	// States functions
	void ST_Idle(UsartData* pdata);
	void ST_ByteReceived(UsartData* pdata);
	void ST_FrameReceived(UsartData* pdata);
	enum States {ST_IDLE = 0, ST_BYTE_RECEIVED, ST_FRAME_RECEIVED, ST_MAX_STATES};
	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(&Usart::ST_Idle)
		STATE_MAP_ENTRY(&Usart::ST_ByteReceived)
		STATE_MAP_ENTRY(&Usart::ST_FrameReceived)
	END_STATE_MAP

	volatile uint8_t rx_buf[UART_BUF_SIZE];
	volatile uint8_t tx_buf[UART_BUF_SIZE];
	volatile uint8_t rx_head, rx_tail, tx_head, tx_tail;
};

extern Usart usart;
extern UsartData usart_data;

#endif /* USART_H_ */
