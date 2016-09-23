#ifndef __UART_H
#define __UART_H

#include <stdint.h>

extern uint8_t uart_result;
extern uint8_t uart_status;

#define UART_STATUS_OK  0x00
#define UART_STATUS_NOK 0xFF

void    uart_getc();
uint8_t uart_get_status();
void    uart_putc(char c);

#endif
