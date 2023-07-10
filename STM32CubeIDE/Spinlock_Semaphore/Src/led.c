/*
 * led.c
 *
 *  Created on: Jun 29, 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

#include "led.h"

#define GPIO_B	(1UL<<1)
#define LED_RED (1UL<<14)


void led_init(void)
{
	//Enable clock access to the GPIO  of the LED
    RCC->AHB1ENR |= (GPIO_B);

	//Set the direction to output
    GPIOB->MODER |= (1UL<<28); //set bit 28 to '1'
    GPIOB->MODER &= ~(1UL<<29); //set bit 29 to '0'
}

void led_on(void)
{
	//Set the PIN to HIGH
	GPIOB->ODR |= LED_RED;
}


void led_off(void)
{
	//Set the PIN to LOW
    GPIOB->ODR &= ~LED_RED;
}
