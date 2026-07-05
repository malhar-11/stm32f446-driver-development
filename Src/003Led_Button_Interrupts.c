/*
 * 003Led_Button_Interrupts.c
 *
 *  Created on: 03-Jul-2026
 *      Author: Malhar
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f446e.h"
#include "stm32f446e_gpio_driver.h"

int main(void)
{
	GPIO_Handle_t GpioLed, GpioButton;
	memset(&GpioLed, 0, sizeof(GpioLed));
	memset(&GpioButton, 0, sizeof(GpioLed));

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_OPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_Speed = GPIO_OP_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PUPDControl = GPIO_NO_PUPD;

	GpioButton.pGPIOx = GPIOC;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioButton.GPIO_PinConfig.GPIO_OPType = GPIO_OP_TYPE_PP;
	GpioButton.GPIO_PinConfig.GPIO_Speed = GPIO_OP_SPEED_FAST;
	GpioButton.GPIO_PinConfig.GPIO_PUPDControl = GPIO_PU;


	GPIO_PClkControl(GPIOA, ENABLE);
	GPIO_PClkControl(GPIOC, ENABLE);

	GPIO_init(&GpioLed);
	GPIO_init(&GpioButton);

	//IRQ Configurations
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, 15);

	while(1);


	return 0;

}

void EXTI15_10_IRQHandler(void)
{
	GPIO_IRQHandle(GPIO_PIN_13);
	GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_5);
}
