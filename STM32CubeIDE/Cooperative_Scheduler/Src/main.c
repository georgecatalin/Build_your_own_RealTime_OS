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


#define QUANTA (10)

typedef uint32_t TaskProfiler;

TaskProfiler Task0_Profiler, Task1_Profiler, Task2_Profiler;

void motor_start(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

void task0(void)
{
	for(;;)
	{
		Task0_Profiler++;
		osThreadYield();
	}
}

void task1(void)
{
	for(;;)
	{
		Task1_Profiler++;
	}
}

void task2(void)
{
	for(;;)
	{
		Task2_Profiler++;
		valve_open();
	}
}

int main(void)
{
	uart_tx_init();
	/* Initialize Kernel */
	osKernelInit();
	/* Add threads */
	osKernelAddThreads(&task0, &task1, &task2);
	/* Set the Round Robin time slice(quanta) */
    osKernelLaunch(QUANTA);
}


void motor_start(void)
{
	printf("The motor is starting...\n\r");
}

void motor_stop(void)
{
	printf("The motor is stopping...\n\r");
}

void valve_open(void)
{
	printf("The valve is opening...\n\r");
}

void valve_close(void)
{
	printf("The valve is closing...\n\r");
}
