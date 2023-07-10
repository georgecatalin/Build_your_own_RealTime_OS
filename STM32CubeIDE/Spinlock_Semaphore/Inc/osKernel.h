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

void tim2_1hz_interrupt_init(void);
void osKernelInit();
void osKernelLaunch(uint32_t quanta);
void osSchedulerLaunch(void);
void osThreadYield(void);
void task3(void);

uint8_t osKernelAddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));
void osSemaphoreInit(int32_t *semaphore, int32_t value);
void osSemaphoreSet(int32_t *semaphore);
void osSemaphoreWait(int32_t *semaphore);

#endif /* OSKERNEL_H_ */
