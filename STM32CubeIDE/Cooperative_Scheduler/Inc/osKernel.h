/*
 * osKernel.h
 *
 *  Created on: 7 Jul 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

#ifndef OSKERNEL_H_
#define OSKERNEL_H_

#include "stm32f4xx.h"
#include <stdint.h>

void osKernelInit();
void osKernelLaunch(uint32_t quanta);
void osSchedulerLaunch(void);
void osThreadYield(void);


uint8_t osKernelAddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));


#endif /* OSKERNEL_H_ */
