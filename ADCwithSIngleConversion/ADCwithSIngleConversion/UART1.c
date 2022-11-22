/*
 * UART1.c
 *
 * Created: 2022-11-14-월요일 오후 12:39:58
 *  Author: jaewon
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART1.h"
#include <stdio.h>

FILE INOUT \
= FDEV_SETUP_STREAM(UART1_transmit, UART1_receive, _FDEV_SETUP_RW);

void UART1_init(unsigned long int baudRate) {
	
	
	stdout = &INOUT;
	stdin = &INOUT;
		
	int UBRR = (F_CPU / (8 * baudRate)) - 1;
	
	UBRR1H |= UBRR / 0x100;
	UBRR1L |= UBRR % 0x100;
	UCSR1A |= _BV(U2X1); // 2배속 모드 ( f = 8)
	
	UCSR1B |= _BV(RXEN1); // Receiver Enable
	UCSR1B |= _BV(TXEN1); // Transmitter Enable
	
	UCSR1C |= 0x06;
}
void UART1_transmit(char data) {
	while(!(UCSR1A & (_BV(UDRE1))));
	UDR1 = data;
}

unsigned char UART1_receive() {
	while(!(UCSR1A & (_BV(RXC1))));
	return UDR1;
}

void UART1_print_string(char *str) {
	for (int i = 0; str[i]; i++) {
		UART1_transmit(str[i]);
	}
}
void UART1_print_1_byte_number(uint8_t n) {
	char numString[4] = "0";
	int i, index = 0;
	
	if (n > 0) {
		for (i = 0; n != 0; i++){
			numString[i] = n % 10 + '0';
		}
		numString[i] = '\0';
		index = i - 1;
	}
	
	for (i = index; i >= 0; i--) {
		UART1_transmit(numString[i]);
	}
}