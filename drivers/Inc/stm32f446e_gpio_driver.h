/*
 * stm32f446e_gpio_driver.h
 *
 *  Created on: 24-Jun-2026
 *      Author: Malhar
 */

#include "stm32f446e.h"
#include <stdint.h>

#ifndef INC_STM32F446E_GPIO_DRIVER_H_
#define INC_STM32F446E_GPIO_DRIVER_H_

/*
 * Configuration structure for GPIO pin
 */

typedef struct{

	uint8_t GPIO_PinNumber;			//Possible values @GPIO_PIN_NUMBERS
	uint8_t GPIO_PinMode;			//Possible values @GPIO_MODES
	uint8_t GPIO_Speed;				//Possible values @GPIO_OUTPUT_SPEED
	uint8_t GPIO_OPType;			//Possible values @GPIO_OUTPUT_TYPE
	uint8_t GPIO_PUPDControl;		//Possible values @GPIO_PUPD
	uint8_t GPIO_AltFnMode;			//Possible values

} GPIO_PinConfig_t;

/*
 * Handle structure for GPIO peripheral
 */

typedef struct{

	GPIO_Regdef_t *pGPIOx ;
	GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t	;


/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin Numbers
 */
#define GPIO_PIN_0 			0
#define GPIO_PIN_1 			1
#define GPIO_PIN_2 			2
#define GPIO_PIN_3 			3
#define GPIO_PIN_4 			4
#define GPIO_PIN_5 			5
#define GPIO_PIN_6 			6
#define GPIO_PIN_7 			7
#define GPIO_PIN_8 			8
#define GPIO_PIN_9 			9
#define GPIO_PIN_10 		10
#define GPIO_PIN_11			11
#define GPIO_PIN_12			12
#define GPIO_PIN_13			13
#define GPIO_PIN_14			14
#define GPIO_PIN_15			15

/*
 * @GPIO_MODES
 * GPIO possible modes
 */
#define GPIO_MODE_IN 		0
#define GPIO_MODE_OUT 		1
#define GPIO_MODE_ALTFN		2
#define GPIO_MODE_ANALOG 	3
#define GPIO_MODE_IT_FT		4
#define GPIO_MODE_IT_RT		5
#define GPIO_MODE_IT_FRT	6

/*
 * @GPIO_OUTPUT_TYPE
 * GPIO output type
 */
#define GPIO_OP_TYPE_PP			0
#define GPIO_OP_TYPE_OD			1


/*
 * @GPIO_OUTPUT_SPEED
 * GPIO output speed
 */
#define GPIO_OP_SPEED_LOW		0
#define GPIO_OP_SPEED_MEDIUM	1
#define GPIO_OP_SPEED_FAST		2
#define GPIO_OP_SPEED_HIGH		3

/*
 * @GPIO_PUPD
 * GPIO	PUPD modes
 */
#define GPIO_NO_PUPD			0
#define GPIO_PU					1
#define GPIO_PD					2





/*********************************************************************************************
 *
 * 									APIs supported by this driver
 * 							for more info check the function definitions
 *
 *********************************************************************************************/


//Peripheral Clock Setup
void GPIO_PClkControl(GPIO_Regdef_t *pGPIOx, uint8_t ENorDI);

//Init & De-init
void GPIO_init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_Regdef_t *pGPIOx);

//Data Read & Write
uint8_t GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOx);
void GPIO_WritetoOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WritetoOutputPort(GPIO_Regdef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOx, uint8_t PinNumber);

//IRQ Configuration & Handling
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandle(uint8_t PinNumber);





#endif /* INC_STM32F446E_GPIO_DRIVER_H_ */
