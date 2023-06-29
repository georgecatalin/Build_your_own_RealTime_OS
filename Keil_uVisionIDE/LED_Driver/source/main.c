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
#include "led.h"


int main(void)
{
    /* Loop forever */

	 int counter1, counter2;
	led_init();

	for(;;)
	{
		int i=0;
		led_on();

		for(i=0; i<=100000;i++)
		{
			counter1++;
		}

		led_off();

	   for(i=0;i<=100000;i++)
	   {
				counter2++;
	   }
	}
}
