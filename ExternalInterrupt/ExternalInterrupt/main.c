/*
 * ExternalInterrupt.c
 *
 * Created: 2022-11-27-일요일 오전 3:58:51
 * Author : jaewon
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

volatile int state = 0;

void UART1_init(unsigned long int baudrate);
void UART1_transmit(char data);
unsigned char UART1_receive();
void PORT_init();
void INIT_init();

FILE INOUT \
= FDEV_SETUP_STREAM(UART1_transmit, UART1_receive, _FDEV_SETUP_RW);

void UART1_init(unsigned long int baudrate) {
	
	stdout = &INOUT;
	unsigned long int UBBR = (F_CPU / (8 * baudrate)) - 1; 
	UBRR1L = UBBR % 0x100;
	UBRR1H = UBBR / 0x001;
	
	UCSR1A |= (1 << U2X1); // Double transmission speed 
	UCSR1B |= (1 << RXEN1); // Receiver Enable
	UCSR1B |= (1 << TXEN1); // Transmitter Enable
	UCSR1C |= 0x06; // Asynchronous mode, No parity, 1 stop bit, 8 bit
}

void UART1_transmit(char data) {
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}	

unsigned char UART1_receive() {
	while (!(UCSR1A & (1 << RXC1)));
	return UDR1;
}

void PORT_init() {
	DDRB |= 0x01; // led
	DDRD |= 0x00; // button
	PORTD |= 0x01; // set internal pull-up resister
}

void INIT_init() {
	EIMSK |= (1 << INT0); // INT0 pin(PD0) enable
	EICRA |= (1 << ISC01); // interrupt when the falling edge
	sei();
}

ISR(INT0_vect) {
	state = (state + 1) % 2;
	if (state == 1) {
		PORTB = 0x01;
	}
	else {
		PORTB = 0x00;
	}
}
int main(void)
{	
	PORT_init();
    UART1_init(115200);
	INIT_init();
    while (1) 
    {
		printf("%d\n\r", state);
		_delay_ms(1000);
    }
}

