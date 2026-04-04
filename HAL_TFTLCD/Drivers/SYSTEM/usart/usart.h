#ifndef __USART_H
#define __USART_H

#include "./SYSTEM/sys/sys.h"
#include <stdio.h>
#include <stdarg.h>

extern  uint8_t g_rx_buf[1];

void usart_init(uint32_t Baud);
void uart_sendbyte(uint8_t byte);

//发送数组
void uart_sendarray(uint8_t *array,uint16_t length);

//发送字符串
void uart_sendstring(char *String);

void uart_sendnum(uint32_t number,uint8_t length);

void uart_printf(char *format,...);

uint8_t uart_getRxflag(void);

uint8_t uart_getdata(void);
#endif


