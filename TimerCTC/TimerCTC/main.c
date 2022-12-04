/*
 * TimerCTC.c
 *
 * Created: 2022-12-04-일요일 오후 11:14:26
 * Author : jaewon
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int count = 0;
ISR(TIMER0_COMP_vect) {
	count++;
	if (count >= 125) {
		count = 0;
		PORTB = PORTB ^ 0x01;
	}
}

int main(void)
{
	DDRB = 0x01;
	PORTB = 0x00;

	TCCR0 |= (1 << WGM01); // CTC mode
	TCCR0 |= (1 << CS02) | (1 << CS01); // 256 clock division
	
	TIMSK |= (1 << OCIE0); // Compare match interrupt enable
	
	OCR0 = 250;
	
	sei(); // global interrupt enable
	
    while (1) 
    {
    }
}

