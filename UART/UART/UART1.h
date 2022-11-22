/*
 * UART1.h
 *
 * Created: 2022-11-14-월요일 오후 12:40:23
 *  Author: jaewon
 */ 

#ifndef UART1_H_
#define UART1_H_
#include <stdio.h>

void UART1_init(unsigned long int baudRate);
void UART1_transmit(char data);

unsigned char UART1_receive();

void UART1_print_string(char *str);
void UART1_print_1_byte_number(uint8_t n);

#endif /* UART1_H_ */