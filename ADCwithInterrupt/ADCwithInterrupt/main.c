/*
 * ADCwithInterrupt.c
 * Created: 2022-11-23-수요일 오전 5:04:01
 * Author : jaewon
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART1_init(unsigned long int baudRate);
void UART1_transmit(char data);
void ADC_init();
unsigned char UART1_receive();

FILE INOUT \
= FDEV_SETUP_STREAM(UART1_transmit, UART1_receive, _FDEV_SETUP_RW);

volatile int adcValue;

int main(void) {
	int localACD;
	UART1_init(9600);
	ADC_init();
	_delay_ms(1000);
	
    DDRB = 0x01;
    while (1) {
		cli();
		localACD = adcValue;
		printf("%d\n\r", localACD);
		sei();
		_delay_ms(1000);
    }
}

ISR(ADC_vect) {
	ADCSRA |= ( 1 << ADSC);
	adcValue = ADC;
	
	// 5k 저항 사용 
	if (adcValue > 700) {
		PORTB = 0x01;
	}
	else {
		PORTB = 0x00;
	}
}

void UART1_init(unsigned long int baudRate) {
	stdout = &INOUT;
	stdin = &INOUT;
	int UBRR = (F_CPU / (8 * baudRate)) - 1;
	UBRR1L |= UBRR % 0x100;
	UBRR1H |= UBRR / 0x100;
	
	UCSR1A |= ( 1 << U2X1); // 2배속 모드
	
	UCSR1B |= ( 1 << RXEN1); // Receiver Enable
	UCSR1B |= ( 1 << TXEN1); // Transmitter Enable
	
	UCSR1C |= 0x06;
}

void UART1_transmit(char data) {
	while(!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

unsigned char UART1_receive() {
	while(!(UCSR1A  & (1 << RXC1 )));
	return UDR1;
}

void ADC_init() {
	ADCSRA |= (1 << ADEN); // ADC enable
	
	ADMUX |= (1 << REFS1); // set up external Vcc as reference voltage
	ADMUX |= 0x00; // set up ADC0 -> PF0
	
	ADCSRA |= 0x07; // 분주비 128Hz
	ADCSRA |= (1 << ADSC); // ADC start converting
	ADCSRA |= (1 << ADIE); // ADC interrupt Enable
}
