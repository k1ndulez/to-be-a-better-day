#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

void uart1_init(void);
void uart1_send_string(char *str);

#endif
