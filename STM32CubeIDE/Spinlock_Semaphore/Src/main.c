/**
 ******************************************************************************
 * @file           : main.c
 * @author         : George Calin
 * @brief          : Main program body
 ******************************************************************************
  A driver for the onboard LED on the NUCLEO F429ZI board.

User LD1: a green user LED is connected to the STM32 I/O PB0 (SB120 ON and SB119 OFF) or PA5 (SB119 ON and SB120 OFF) corresponding to the ST Zio D13.
User LD2: a blue user LED is connected to PB7.
User LD3: a red user LED is connected to PB14.

 ******************************************************************************
 */

#include <stdint.h>
#include "uart.h"
#include "osKernel.h"


#define QUANTA (2)
//#define QUANTA (10)

typedef uint32_t TaskProfiler;

TaskProfiler Task0_Profiler, Task1_Profiler, Task2_Profiler, pTask1_Profiler;

#define TIM2_SR_UIF (1UL<<0)

void motor_start(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

int32_t semaphore_1, semaphore_2;

void task3(void)
{
	pTask1_Profiler++;
}

void task0(void)
{
	for(;;)
	{
		osSemaphoreWait(&semaphore_1);
		motor_start();
		osSemaphoreSet(&semaphore_2);
	}
}

void task1(void)
{
	for(;;)
	{
		osSemaphoreWait(&semaphore_2);
		valve_open();
		osSemaphoreSet(&semaphore_1);
	}
}

void task2(void)
{
	for(;;)
	{
		Task2_Profiler++;
	}
}

int main(void)
{


	/* Initialize uart */
	uart_tx_init();
	/* Initialize hardware timer */
	tim2_1hz_interrupt_init();

	osSemaphoreInit(&semaphore_1, 1);
	osSemaphoreInit(&semaphore_2, 0);

	/* Initialize Kernel */
	osKernelInit();
	/* Add threads */
	osKernelAddThreads(&task0, &task1, &task2);
	/* Set the Round Robin time slice(quanta) */
    osKernelLaunch(QUANTA);
}

void TIM2_IRQHandler(void)
{
	/* Clear update interrupt flag */
    TIM2->SR &= ~(TIM2_SR_UIF);
	/* Do something */
	pTask1_Profiler++;
}

void motor_start(void)
{
	printf("The motor is starting... in STM32CubeIDE\n\r");
}

void motor_stop(void)
{
	printf("The motor is stopping... in STM32CubeIDE\n\r");
}

void valve_open(void)
{
	printf("The valve is opening... in STM32CubeIDE\n\r");
}

void valve_close(void)
{
	printf("The valve is closing... in STM32CubeIDE\n\r");
}
