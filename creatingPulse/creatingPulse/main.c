/*
 * creatingPulse.c
 *
 * Created: 2022-12-07-수요일 오전 1:02:48
 * Author : jaewon
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

volatile int count = 0;

ISR(TIMER0_COMP_vect) {
	count++; 
	TCNT0 = 0;
}

int main(void)
{
	DDRB = 0x10;
	PORTB = 0x00;
	    
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00); // 1024 prescaler
			
	OCR0 = 255;
			
	TCCR0 |= (1 << COM00); // toggle OC(PB4) when compare match interrupt
			
	TIMSK |= (1 << OCIE0);
			
	sei();
    while (1) 
    {

    }
}

