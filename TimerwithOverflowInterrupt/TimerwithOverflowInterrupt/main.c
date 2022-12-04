/*
 * TimerwithOverflowInterrupt.c
 *
 * Created: 2022-12-04-일요일 오전 4:11:17
 * Author : jaewon
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int count = 0;

ISR(TIMER0_OVF_vect) {
	count++;
	
	if (count >= 31250) { // 0.5s
		count = 0;
		PORTB = PORTB ^ 0x01;	
	}
}
int main(void)
{
	DDRB = 0x01;
    PORTB |= 0x00;
	
	TCCR0 |= (1 << CS00); // clock soucre == 1	
	TIMSK |= (1 << TOIE0); // Overflow interrupt enable	
	sei(); // global interrupt enable
    while (1) 
	{
    }
}
