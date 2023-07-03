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

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
    /* Loop forever */

	uart_tx_init();
	timebase_init();

	for(;;)
	{
		delay(1);
		printf("A second has passed in  Keil uVision!!. \n\r");
	}
}
