/*
 * main.c
 *
 *  Created on: 02-Jul-2026
 *      Author: Malhar
 */


#include <stdio.h>
#include <stdint.h>
#include "stm32f446e.h"
#include "stm32f446e_gpio_driver.h"

int main(void)
{
	return 0;
}

void EXTI0_IRQHandler(void)
{
	GPIO_IRQHandle(0);
}
