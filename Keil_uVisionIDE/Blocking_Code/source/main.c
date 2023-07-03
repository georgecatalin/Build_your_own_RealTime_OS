/**
 ******************************************************************************
 * @file           : main.c
 * @author         : George Calin
 * @brief          : Main program body
 ******************************************************************************
  A driver for the onboard LED on the NUCLEO F429ZI board.

 ******************************************************************************
 */

#include <stdint.h>
#include "uart.h"
#include "timebase.h"

void motor_start(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);


int main(void)
{
    /* Loop forever */

	uart_tx_init();
	timebase_init();

	for(;;)
	{
		motor_start();
		delay(1);
		motor_stop();
		delay(1);
	}
}

void motor_start(void)
{
	printf("The motor is starting... in Keil uVision\n\r");
}

void motor_stop(void)
{
	printf("The motor is stopping... in Keil uVision\n\r");
}

void valve_open(void)
{
	printf("The valve is opening... in Keil uVision\n\r");
}

void valve_close(void)
{
	printf("The valve is closing... in Keil uVision\n\r");
}
