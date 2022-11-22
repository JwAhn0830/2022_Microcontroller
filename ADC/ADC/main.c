/*
 * Potentiometer로 LED 제어
 * Created: 2022-11-19-토요일 오후 7:42:41
 * Author : jaewon
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void UART1_init();
void UART1_transmit(char data);
unsigned char UART1_receive();

FILE INOUT \
= FDEV_SETUP_STREAM(UART1_transmit, UART1_receive,  _FDEV_SETUP_RW);

int main(void)
{
	int read;
	
    UART1_init();
	ADC_init();
	DDRC |= 0x0F;
	printf("hello\n\r");
    while (1) 
    {
		PORTC ^= 0x0F;
		
		read = read_ADC();
		
		if (read < 200){
			PORTC = 0x01; // 0000 0001
		}
		else if (read > 200 && read < 400 ){
			PORTC = 0x03; // 0000 0011
		}
		else if (read > 400 && read < 600){
			PORTC = 0x07; // 0000 0111
		}
		else if (read > 600) {
			PORTC = 0x0F; // 0000 1111
		}
		printf("%d\r\n", read);
		

	 }
}

void UART1_init(){
	stdout = &INOUT;
	stdin = &INOUT;
		
	UBRR1H |= 0x00;
	UBRR1L |= 207; // if U2X == 1
	
	UCSR1A |= 0x02; // set U2X 1
	
	UCSR1B |= 0x10; // receiver enable
	UCSR1B |= 0x08; // transmitter enable
	
	UCSR1C |= 0x06; // Asynchronous, no parity bit, 1 stop bit, 8 bit
}

void UART1_transmit(char data) {
	while(!(UCSR1A & ( 1 << UDRE1)));
	UDR1 = data;
}

unsigned char UART1_receive() {
	while( !(UCSR1A & (1 << RXC1)) );
	
	return UDR1;
}

void ADC_init() {
	ADMUX |= ( 1 << REFS0); // set up AVCC as reference voltage
	ADMUX |= 0x01; // select ADC1 
	
	ADCSRA |= ( 1 << ADEN); // ADC Enable
	ADCSRA |= ( 1 << ADFR); // ADC free running mode
	ADCSRA |= 0x07; // set up division factor 128
	
	ADCSRA |= ( 1 << ADSC); // start conversion
	 
}

void read_ADC() {
	while(!(ADCSRA & (1 << ADIF)));
	
	return ADC;
}