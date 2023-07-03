/*
 * timebase.c
 *
 *  Created on: 3 Jul 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

#include "timebase.h"

#define  CLOCK_FREQUENCY (16000000)

#define  CTRL_CLKSOURCE (1UL<<2)
#define CTRL_TICK_ENBL_INTERRUPT (1UL<<1)
#define CTRL_ENABLE (1UL<<0)

#define MAX_DELAY 0xFFFFFFFF

uint32_t current_counter;
uint32_t current_counter_tick;
uint32_t tick_frequency = 1;



void increment_counter(void)
{
	current_counter += tick_frequency;
}

uint32_t get_current_tick(void)
{
	__disable_irq();
	current_counter_tick = current_counter;
	__enable_irq();

	return current_counter_tick;
}


void delay(uint32_t delay) // delay is in seconds
{
   uint32_t current_tick = get_current_tick();
   uint32_t wait = delay;

   if(wait < MAX_DELAY)
   {
	   wait += (uint32_t) tick_frequency;
   }

   while(current_tick - get_current_tick() < wait)
   {

   }
}

void timebase_init(void)
{
	/* Step 1. Reload the SysTick Reload Value Register with the needed value */
	SysTick->LOAD = CLOCK_FREQUENCY;

	/* Step 2. Clear the current SysTick Value Register */
	SysTick->VAL = 0;

	/* Step 3. Set the Clock Source in the SysTick Control and Status Register */
	SysTick->CTRL = CTRL_CLKSOURCE;

	/* Step 4. Enable SysTick interruption in the SysTick Control and Status Register */
	SysTick->CTRL |= CTRL_TICK_ENBL_INTERRUPT;

	/* Step 5. Enable the counter in the SysTick Control and Status Register */
	SysTick->CTRL |= CTRL_ENABLE;

	/* Step 6. Enable the global interrupts */
	__enable_irq();
}

void SysTick_Handler(void) // every cycle defined int the timebase , this interrupt is executed
{
	increment_counter();
}
