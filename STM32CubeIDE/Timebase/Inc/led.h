/*
 * led.h
 *
 *  Created on: Jun 29, 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

void led_init(void);
void led_on(void);
void led_off(void);

#endif

