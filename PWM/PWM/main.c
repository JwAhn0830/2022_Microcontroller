/*
 * PWM.c
 *
 * Created: 2022-12-07-수요일 오전 3:30:05
 * Author : jaewon
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

void UART1_init(unsigned long int baudrate);
void UART1_transmit(char data);
unsigned char UART1_receive();
void ADC_init(unsigned char channel);
void Timer0_init();

FILE INOUT \

= FDEV_SETUP_STREAM(UART1_transmit, UART1_receive, _FDEV_SETUP_RW);

volatile int adcValue;
	volatile int count;
int main(void) {
	
	UART1_init(9600);
	ADC_init(0);
	Timer0_init();
	DDRB = 0xFF;
	PORTB = 0x00;
	sei();

	int localADC;
    while (1) 
    {	
		cli();
		localADC = adcValue * 0.4464; 
		sei();
		if (localADC < 29) {
			OCR0 = 0x00;
		}
		else if (29 <= localADC && localADC < 31) {
			OCR0 = 12;
		} 
		else if (31 <= localADC && localADC < 33) {
			OCR0 = 38;
		}
		else if (33 <= localADC) {
			OCR0 = 51;
		}
		
		printf("Now temperature is %d Celsius Deg.\r\n", localADC);
	
		_delay_ms(500);			
    }
}

ISR(ADC_vect) {
	ADCSRA |= ( 1 << ADSC);
	adcValue = ADC; 
}

void UART1_init(unsigned long int baudrate) {
	
	stdout = &INOUT;
	stdin = &INOUT;
	
	int UBRR = (F_CPU / (8 * baudrate)) - 1;
	UBRR1H = UBRR / 0x100;
	UBRR1L = UBRR % 0x100;
	
	UCSR1A |= (1 << U2X1); // 2배속 모드
	
	UCSR1B |= (1 << RXEN1); // Reciver enable
	UCSR1B |= (1 << TXEN1); // Transmitter enable
	
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10); // 8 bit
}
void UART1_transmit(char data) {
	while(!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}
unsigned char UART1_receive() {
	while(!(UCSR1A & (1 << RXC1)));
	return UDR1;
}

void ADC_init(unsigned char channel) {
	ADCSRA |= (1 << ADEN); // ADC enable
	
	ADMUX |= (1 << REFS1); // set up external Vcc as reference voltage
	ADMUX |= (ADMUX & 0xE0)|channel; // set up ADC0 -> PF0
	
	ADCSRA |= 0x07; // 분주비 128Hz
	ADCSRA |= (1 << ADSC); // ADC start converting
	ADCSRA |= (1 << ADIE); // ADC interrupt Enable
}

void Timer0_init() {
	TCCR0 |= (1 << WGM01) | (1 << WGM00); // Fast PWM mode
	TCCR0 |= (1 << COM01); // Non-inverting mode
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00); // 1024 Clock divison
	//TIMSK |= (1 << OCIE0) | (1 << TOIE0);
}