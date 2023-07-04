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
#include "timebase.h"

void motor_start(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);

int motor_main(void)
{
	for(;;)
	{
		motor_start();
		delay(1);
		motor_stop();
		delay(1);
	}
}

int valve_main(void)
{
	for(;;)
	{
		valve_open();
		delay(1);
		valve_close();
		delay(1);
	}
}



int main(void)
{
    /* Loop forever */

	uart_tx_init();
	timebase_init();

	uint32_t start = 0UL;

	for(;;)
	{
		if(start)
		{
			motor_main();
		}
		else
		{
			valve_main();
		}
	}
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
