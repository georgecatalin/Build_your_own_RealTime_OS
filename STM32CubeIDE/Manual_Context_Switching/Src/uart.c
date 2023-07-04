/*
 * uart.c
 *
 *  Created on: 30 Jun 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

/* ****************************************************************************************************************************************************************************
 * What USART can be used on the STM32 Nucleo F429ZI board to avoid using a supplementary USB to serial convertor?
 *
 * Board User Manual:
 * The USART3 interface available on PD8 and PD9 of the STM32 can be connected either to ST-LINK or to ST morpho connector. The choice is changed by setting the related solder bridges.
 * By default the USART3 communication between the target STM32 and the ST-LINK is enabled, to support the Virtual COM port for the mbed (SB5 and SB6 ON).
 *
 * name 		Function		 Virtual COM port			ST morpho connection (default configuration)
	PD8 		USART3 TX 		SB5 ON and SB7 OFF 			SB5 OFF and SB7 ON
	PD9 		USART3 RX 		SB6 ON and SB4 OFF 			SB6 OFF and SB4 ON


 * What is the default clock frequency of this STM32 Nucleo F429ZI board?
 * Board User Manual:
 * Clocks and startup
 * On reset the 16 MHz internal RC oscillator is selected as the default CPU clock. The 16 MHz internal RC oscillator is factory-trimmed to offer 1% accuracy over the full
 * temperature range. The application can then select as system clock either the RC oscillator or an external 4-26 MHz clock source. This clock can be monitored for failure.

 * USART3 is connected to APB1 bus interface as per the datasheet document
 * ******************************************************************************************************************************************************************************** */


#include "uart.h"
#include <stdint.h>

#define GPIOD_ENR (1UL<<3)
#define USART3_DIRECTION (1UL<<3)  // Bit for 'TE'
#define USART3_ENABLE (1UL<<13)  // Bit for 'UE'

#define DEFAULT_CLOCK (16000000)
#define APB1_CLOCK DEFAULT_CLOCK
#define BAUDRATE (115200)

#define USART_EMPTY  (1UL<<7)  // bit 7 in USART->SR  TXE: Transmit data register empty

static void set_the_baudrate(uint32_t periph_clock, uint32_t baudrate);
static uint16_t compute_baudrate(uint32_t periph_clock, uint32_t baudrate);
static void usart_write(int myChar);

int __io_putchar(int myChar)
{
	usart_write(myChar);
	return myChar;
}


void uart_tx_init(void)
{
	/* Set clock access to the GPIOD */
	RCC->AHB1ENR |= (GPIOD_ENR);

	/* Set the PD8 (USART3_TX) to Analog Function mode */
    GPIOD->MODER &= ~(1UL<<16); // Set bit no.16 to '0'
    GPIOD->MODER |= (1UL<<17); // Set bit no.17 to '1'

	/* Set the Analog function to AF7 USART3_TX */
    GPIOD->AFR[1] |= (1UL<<0); // Set bit no.0 in AFRH to '1'
    GPIOD->AFR[1] |= (1UL<<1); // Set bit no.1 in AFRH to '1'
    GPIOD->AFR[1] |= (1UL<<2); // Set bit no.2 in AFRH to '1'
    GPIOD->AFR[1] &= ~(1UL<<3); // Set bit no.3 in AFRH to '0'

	/* Set clock access to UART */
    RCC->APB1ENR |= (1UL<<18);

	/* Configure the baudrate */
    set_the_baudrate(APB1_CLOCK,BAUDRATE);

	/* Configure the transfer direction */
    USART3->CR1 = USART3_DIRECTION;

	/* Enable UART module */
    USART3->CR1 |= USART3_ENABLE;
}

static void set_the_baudrate(uint32_t periph_clock, uint32_t baudrate)
{
	USART3->BRR = compute_baudrate(periph_clock,baudrate);
}

static uint16_t compute_baudrate(uint32_t periph_clock, uint32_t baudrate)
{
	return ((periph_clock + (baudrate/2UL))/baudrate);
}

static void usart_write(int myChar)
{
	/* Verify the transmit data register is empty */
	while(!(USART3->SR & USART_EMPTY))
	{

	}

	/* Write data to the transmit data register */
	USART3->DR = (myChar & 0xFF);
}
