/*
 * ADCwithSIngleConversion.c
 * use PF7 as ADC pin (ADC7)
 * In single conversion mode, ADSC will set to 0 after converting 
 * Created: 2022-11-23-수요일 오전 4:02:08
 * Author : jaewon
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "UART1.h"

void ADC_init();
int read_ADC();
int main(void)
{
	int value = 0;
	ADC_init();
	UART1_init(9600);
    /* Replace with your application code */
    while (1) 
    {
		value = read_ADC();
		printf("%d\n\r", value);
		_delay_ms(500);
    }
}

void ADC_init() {
	ADCSRA |= ( 1 << ADEN); // ADC enable
	
	ADMUX |= ( 1 << REFS0); // set VCC as reference voltage 
	ADMUX |= 0x07; // set ADC7

	ADCSRA |= ( 1 << ADSC); // start conversion
	ADCSRA |= 0x07;
}

int read_ADC() {
	ADCSRA |= ( 1 << ADSC); // set ADCS to 1
	while(!(ADCSRA & (1 << ADSC))); // ADCS will set 0 after converting 
	
	return ADC;
}