/*
 * 001Led_Toggle.c
 *
 *  Created on: 30-Jun-2026
 *      Author: Malhar
 */

#include "stm32f446e.h"
#include "stm32f446e_gpio_driver.h"


void delay(void)
{
	for(volatile uint32_t i=0; i<500000/2; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_OPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_Speed = GPIO_OP_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PUPDControl = GPIO_NO_PUPD;

	GPIO_PClkControl(GPIOA, ENABLE);
	GPIO_init(&GpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_5);
		delay();
	}

	return 0;
}
