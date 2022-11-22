/*
 * UART.c
 *
 * Created: 2022-11-06-일요일 오후 5:37:12
 * Author : jaewon
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

int main(void)
{
	char str[] = "Hello World!";
	float floatNum = 3.14;
	uint8_t num = 128;
	
	UART1_init(115200);
	//_delay_ms(1000);
	printf("%d\n\r", 123);
	printf("test\n\r");
	printf("%f", floatNum);
	UART1_print_1_byte_number(num);
	UART1_print_string(str);

    while (1) 
    {
		
    }
	return 0;
}

