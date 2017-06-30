/*
 * usart.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "machine.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "led.h"

Usart::Usart(uint16_t baud) : Machine(ST_MAX_STATES)
{
	uint8_t ubrr = F_CPU / 16 / baud - 1;
	US_UBRRH = (uint8_t)(ubrr >> 8);
	US_UBRRL = (uint8_t)ubrr;

	US_UCSRB |= (1 << US_RXEN) | (1 << US_TXEN) | (1 << US_TXCIE);
#if defined (__AVR_ATmega88PA__)
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
#endif

#if defined (__AVR_ATmega8__)
	UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
#endif
	USART_DE_INIT;
	rx_head = 0;
	rx_tail = 0;
	tx_head = 0;
	tx_tail = 0;
	RxEnable();
	timer.Assign(1, 1, ModbusRTU35T);	// 2ms
}

void Usart::RxEnable()
{
	USART_DE_RECEIVE;
	US_UCSRB |= (1 << US_RXEN) | (1 << US_RXCIE);
}

void Usart::RxDisable()
{
	US_UCSRB &= ~((1 << US_RXEN) | (1 << US_RXCIE));
}

void Usart::TxEnable()
{
	USART_DE_SEND;
	US_UCSRB |= (1 << US_UDRIE);
}

void Usart::TxDisable()
{
	US_UCSRB &= ~(1 << US_UDRIE);
}

void Usart::ST_Init(UsartData* pdata) {}

void Usart::ST_Idle(UsartData* pdata)
{
	timer.Disable(1);
}

void Usart::ST_ByteReceived(UsartData* pdata)
{
	//LED_PORT |= (1 << LED_PIN_RED);
	uint8_t tmp_head;
	tmp_head = (rx_head + 1) & UART_RX_BUF_MASK;
	if(tmp_head == rx_tail)
	{
		// nadpisanie bufora
	}
	else
	{
		rx_head = tmp_head;
		buf_rx[tmp_head] = pdata->c;
	}
	timer.Enable(1);
}

void Usart::ST_FrameReceived(UsartData* pdata)
{
	//LED_PORT |= (1 << LED_PIN_RED);
	usart_data.len = 8;
	uint8_t i = 0;
	while(rx_tail != rx_head)
	{
		rx_tail = (rx_tail + 1) & UART_RX_BUF_MASK;
		usart_data.frame[i] = buf_rx[rx_tail];
		i++;
	}
	modbus_rtu.ParseFrame(usart_data.frame, 8);
	timer.Disable(1);
}

void Usart::CharReceived(UsartData* pdata)
{
	const uint8_t Transitions[] =
	{
		// musi byc obsluga jesli znak przyjdzie w stanie INIT
		ST_BYTE_RECEIVED,			// ST_INIT
		ST_BYTE_RECEIVED, 			// ST_IDLE
		ST_BYTE_RECEIVED,			// ST_BYTE_RECEIVED
		ST_BYTE_RECEIVED			// ST_FRAME_RECEIVED
	};
	Event(Transitions[current_state], pdata);
}

void Usart::RTU35T(UsartData* pdata)
{
	const uint8_t Transitions[] =
	{
		ST_IDLE,						// ST_INIT
		ST_NOT_ALLOWED, 				// ST_IDLE
		ST_FRAME_RECEIVED				// ST_BYTE_RECEIVED
	};
	Event(Transitions[current_state], pdata);
}

void Usart::TXBufferEmpty(UsartData* pdata)
{
	if(tx_head != tx_tail)
	{
		tx_tail = (tx_tail + 1) & UART_TX_BUF_MASK;
		US_UDR = buf_tx[tx_tail];
	}
	else
	{
		TxDisable();
	}
}

void Usart::TXComplete(UsartData* pdata)
{
	RxEnable();
}

void Usart::SendFrame(UsartData* pdata)
{
	RxDisable();
	uint8_t tmp_tx_head;
	uint8_t *w = pdata->frame;
	uint16_t len = pdata->len;
	while(len)
	{
		tmp_tx_head = (tx_head  + 1) & UART_TX_BUF_MASK;
		while(tmp_tx_head == tx_tail) {}
		buf_tx[tmp_tx_head] = *w++;
		tx_head = tmp_tx_head;
		len--;
	}

	TxEnable();
}

void Usart::SendInt(UsartData *pdata)
{
	char buf[10];
	itoa(pdata->c, buf, 10);
	//pdata->frame = buf;
	SendFrame(pdata);
}

// --------- Debugowanie
// http://mckmragowo.pl/mck/pliki/programming/clib/?f=va_start

ISR(US_RX)
{
	usart_data.c = US_UDR;
	usart.CharReceived(&usart_data);
}

ISR(USART_UDRE_vect)
{
	usart.TXBufferEmpty();
}

ISR(US_TX)
{
	usart.TXComplete();
}


