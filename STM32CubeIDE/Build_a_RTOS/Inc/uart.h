/*
 * uart.h
 *
 *  Created on: 30 Jun 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdio.h>

void uart_tx_init(void);
int __io_putchar(int myChar);

#endif /* UART_H_ */
