/*
 * stm32f446e_gpio_driver.c
 *
 *  Created on: 24-Jun-2026
 *      Author: Malhar
 */


#include "stm32f446e_gpio_driver.h"


//Peripheral Clock Setup

/**************************************************************************************
 * @fn 				- GPIO_PClkControl
 *
 * @brief			- enables or disables the peripheral clock for the given GPIO port
 *
 * @param[in]		- base address of the GPIO peripheral
 * @param[in]		- ENABLE or DISABLE macros
 * @param[in]		-
 *
 * @return			- none
 *
 * @notes			- none
 */
void GPIO_PClkControl(GPIO_Regdef_t *pGPIOx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
//	}else
//	{
//		if(pGPIOx == GPIOA)
//		{
//			GPIOA_PCLK_DIS();
//		}else if (pGPIOx == GPIOB)
//		{
//			GPIOB_PCLK_DIS();
//		}else if (pGPIOx == GPIOC)
//		{
//			GPIOC_PCLK_DIS();
//		}else if (pGPIOx == GPIOD)
//		{
//			GPIOD_PCLK_DIS();
//		}else if (pGPIOx == GPIOE)
//		{
//			GPIOE_PCLK_DIS();
//		}else if (pGPIOx == GPIOF)
//		{
//			GPIOF_PCLK_DIS();
//		}else if (pGPIOx == GPIOG)
//		{
//			GPIOG_PCLK_DIS();
//		}else if (pGPIOx == GPIOH)
//		{
//			GPIOH_PCLK_DIS();
//		}
	}
}

//Init & De-init

/**************************************************************************************
 * @fn 				- GPIO_init
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp, temp1, temp2;
	//1. Configure the mode of the gpio pin
	if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//Non Interrupt Modes
		temp = pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle -> pGPIOx -> MODER &= ~(0x3 << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle -> pGPIOx -> MODER |= temp;

	}else
	{
		//interrupt mode
		if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1. configure the FTSR
			EXTI -> FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clear corresponding RSTR bit
			EXTI -> RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//2. configure the RTSR
			EXTI -> RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clear corresponding FTSR register
			EXTI -> FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FRT)
		{
			//3. configure the FTSR and RTSR
			EXTI -> FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI -> RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. configure the exti interrupt selection in SYSCFR_EXTICR
		uint8_t temp1, temp2;

		temp1 = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber)/4;
		temp2 = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber)%4;

		uint8_t portcode = GPIO_PORT_TO_CODE(pGPIOHandle -> pGPIOx);

		SYSCFG_PCLK_EN();
		SYSCFR ->EXTICR[temp1] |= portcode << (4*temp2);


		//3. enable the exti interrupt delivery using IMR
		EXTI ->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}

	temp = 0;

	//2. Configure the Output type of the pin
	temp = pGPIOHandle -> GPIO_PinConfig.GPIO_OPType << (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> OTYPER &= ~(0x1 << pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> OTYPER |= temp;

	temp = 0;

	//3. Configure the Speed the output
	temp = pGPIOHandle -> GPIO_PinConfig.GPIO_Speed << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> OSPEEDER &= ~(0x3 << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle -> pGPIOx -> OSPEEDER |= temp;

	temp = 0;

	//4. Configure the Pull-up Pull-down mode
	temp = pGPIOHandle -> GPIO_PinConfig.GPIO_PUPDControl << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle -> pGPIOx -> PUPDR &= ~(0x3 << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle -> pGPIOx -> PUPDR |= temp;

	temp = 0;

	//5. Configure the ALT functionality
	if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{

		temp1 = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber) / 8;
		temp2 = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber) % 8;

		pGPIOHandle -> pGPIOx -> AFR[temp1] &= ~(0xF << (4*temp2));
		pGPIOHandle -> pGPIOx ->AFR[temp1] |= (pGPIOHandle -> GPIO_PinConfig.GPIO_AltFnMode << (4*temp2));
	}
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_DeInit(GPIO_Regdef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
	 GPIOA_REG_RESET();
	}else if (pGPIOx == GPIOB)
	{
	 GPIOB_REG_RESET();
	}else if (pGPIOx == GPIOC)
	{
	 GPIOC_REG_RESET();
	}else if (pGPIOx == GPIOD)
	{
	 GPIOD_REG_RESET();
	}else if (pGPIOx == GPIOE)
	{
	 GPIOE_REG_RESET();
	}else if (pGPIOx == GPIOF)
	{
	 GPIOF_REG_RESET();
	}else if (pGPIOx == GPIOG)
	{
	 GPIOG_REG_RESET();
	}else if (pGPIOx == GPIOH)
	{
	 GPIOH_REG_RESET();
	}
}

//Data Read & Write

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
uint8_t GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx -> IDR >> PinNumber) & 0x00000001);
	return value;
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx -> IDR) ;
	return value;
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_WritetoOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1 << PinNumber);
	}else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_WritetoOutputPort(GPIO_Regdef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1<<PinNumber);
}

//IRQ Configuration & Handling

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ISER0 |= (1<<IRQNumber);

		}else if((31< IRQNumber) & (IRQNumber <= 63))
		{
			*NVIC_ISER1 |= (1<<IRQNumber%32);
		}
		else if((63< IRQNumber) & (IRQNumber <= 95))
		{
			*NVIC_ISER2 |= (1<<IRQNumber%32);
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ICER0 |= (1<<IRQNumber);

		}else if((31< IRQNumber) & (IRQNumber<= 63))
		{
			*NVIC_ICER1 |= (1<<IRQNumber%32);
		}
		else if((63< IRQNumber) & (IRQNumber <= 95))
		{
			*NVIC_ICER2 |= (1<<IRQNumber%64);
		}
	}
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	//Finding IPRx register
	uint8_t iprx = (IRQNumber / 4);

	//Finding the section
	uint8_t iprx_section = (IRQNumber % 4);

	uint8_t shift_amount = (8*iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_IPR_BASEADDR + (iprx)) |= (IRQPriority << shift_amount);
}

/**************************************************************************************
 * @fn 				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @notes			-
 */
void GPIO_IRQHandle(uint8_t PinNumber)
{
	//clear the Pending bit of the EXTI corresponding to PinNumber

	if(EXTI->PR & (1<<PinNumber))
	{
		EXTI->PR |= (1<<PinNumber);
	}
}
