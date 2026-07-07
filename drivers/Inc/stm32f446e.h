/*
 * stm32f446e.h
 *
 *  Created on: 22-Jun-2026
 *      Author: Malhar
 */

#ifndef STM32F446E_H_
#define STM32F446E_H_

#include <stdint.h>

#define _vo volatile

/*****************************************START: Processor Specific Details **********************************************
 *
 * ARM Cortex M4 NVIC ISERx register Addresses
 */

#define NVIC_ISER0				((_vo uint32_t*)0xE000E100)
#define NVIC_ISER1				((_vo uint32_t*)0xE000E104)
#define NVIC_ISER2				((_vo uint32_t*)0xE000E108)
#define NVIC_ISER3				((_vo uint32_t*)0xE000E10C)
#define NVIC_ISER4				((_vo uint32_t*)0xE000E110)
#define NVIC_ISER5				((_vo uint32_t*)0xE000E114)
#define NVIC_ISER6				((_vo uint32_t*)0xE000E118)
#define NVIC_ISER7				((_vo uint32_t*)0xE000E11C)

/*
 * ARM Cortex M4 NVIC ICERx register Addresses
 */

#define NVIC_ICER0				((_vo uint32_t*)0XE000E180)
#define NVIC_ICER1				((_vo uint32_t*)0XE000E184)
#define NVIC_ICER2				((_vo uint32_t*)0XE000E188)
#define NVIC_ICER3				((_vo uint32_t*)0XE000E18C)
#define NVIC_ICER4				((_vo uint32_t*)0XE000E190)
#define NVIC_ICER5				((_vo uint32_t*)0XE000E194)
#define NVIC_ICER6				((_vo uint32_t*)0XE000E198)
#define NVIC_ICER7				((_vo uint32_t*)0XE000E19C)

/*
 * ARM Cortex M4 NVIC Priority register Base Address
 */
#define NVIC_IPR_BASEADDR		((_vo uint32_t*)0xE000E400)

/*
 * ARM Cortex M4 number of priority bits implemented by the priority register
 */

#define NO_PR_BITS_IMPLEMENTED  4

//defining addresses of registers
#define FLASH_BASEADDR			0x08000000U
#define SRAM1_BASEADDR			0x20000000U
#define SRAM1_SIZE				0x00001C00U
#define SRAM2_BASEADDR			(SRAM1_BASEADDR + SRAM1_SIZE)
#define SRAM 					SRAM1_BASEADDR
#define ROM_BASEADDR			0x1FFF0000


//Base Addresses of peripheral buses
#define PERIPH_BASEADDR				0X40000000U
#define APB1PERIPH_BASEADDR			PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR			0x40010000U
#define AHB1PERIPH_BASEADDR			0x40020000U
#define AHB2PERIPH_BASEADDR			0x50000000U

//Addresses of AHB1 peripherals
#define GPIOA_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1C00)
#define RCC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x3800)

//Addresses of APB1 peripherals
#define I2C1_BASEADDR 				(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR 				(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR 				(APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR 				(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR				(APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADRR				(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR				(APB1PERIPH_BASEADDR + 0x4800)

#define UART4_BASEADDR				(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR				(APB1PERIPH_BASEADDR + 0x5000)

//Addresses of APB2 peripherals
#define USART1_BASEADDR				(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR				(APB2PERIPH_BASEADDR + 0x1400)

#define SPI1_BASEADDR				(APB2PERIPH_BASEADDR + 0X3000)
#define SPI4_BASEADDR				(APB2PERIPH_BASEADDR + 0X3400)
#define EXTI_BASEADDR				(APB2PERIPH_BASEADDR + 0x3C00)
#define SYSCFG_BASEADDR				(APB2PERIPH_BASEADDR + 0x3800)

/********************************************************************/

#define GPIOA						((GPIO_Regdef_t*)GPIOA_BASEADDR)
#define GPIOB						((GPIO_Regdef_t*)GPIOB_BASEADDR)
#define GPIOC						((GPIO_Regdef_t*)GPIOC_BASEADDR)
#define GPIOD						((GPIO_Regdef_t*)GPIOD_BASEADDR)
#define GPIOE						((GPIO_Regdef_t*)GPIOE_BASEADDR)
#define GPIOF						((GPIO_Regdef_t*)GPIOF_BASEADDR)
#define GPIOG						((GPIO_Regdef_t*)GPIOG_BASEADDR)
#define GPIOH						((GPIO_Regdef_t*)GPIOH_BASEADDR)

#define RCC							((RCC_Regdef_t*)RCC_BASEADDR)

#define EXTI						((EXTI_Regdef_t*)EXTI_BASEADDR)

#define SYSCFR						((SYSCFR_Regdef_t*)SYSCFG_BASEADDR)

#define SPI1						((SPI_Regdef_t*)SPI1_BASEADDR)
#define SPI2						((SPI_Regdef_t*)SPI2_BASEADDR)
#define SPI3						((SPI_Regdef_t*)SPI3_BASEADDR)
#define SPI4						((SPI_Regdef_t*)SPI4_BASEADDR)

/*
 * GPIO peripheral register structure
 */

typedef struct{

	_vo uint32_t MODER; 		//port mode register
	_vo uint32_t OTYPER;		//port output type register
	_vo uint32_t OSPEEDER;		//port output speed register
	_vo uint32_t PUPDR;			//port pull-up/pull-down register
	_vo uint32_t IDR;			//port input data register
	_vo uint32_t ODR;			//port output data register
	_vo uint32_t BSRR;			//port bit set/reset register
	_vo uint32_t LCKR;			//port configuration lock register
	_vo uint32_t AFR[2];		//alternate function low-high register

} GPIO_Regdef_t;


/*
 * RCC peripheral register structure
 */


typedef struct{

	_vo uint32_t CR; 			//clock control register
	_vo uint32_t PLLCFGR; 		//PLL configuration register
	_vo uint32_t CFGR; 			//clock configuration register
	_vo uint32_t CIR; 			//clock interrupt register
	_vo uint32_t AHB1RSTR; 		//AHB1 peripheral reset register
	_vo uint32_t AHB2RSTR; 		//AHB2 peripheral reset register
	_vo uint32_t AHB3RSTR; 		//AHB3 peripheral reset register
	_vo uint32_t RESERVED0; 	//
	_vo uint32_t APB1RSTR; 		//APB1 peripheral reset register
	_vo uint32_t APB2RSTR; 		//APB2 peripheral reset register
	_vo uint32_t RESERVED1[2]; 	//
	_vo uint32_t AHB1ENR; 		//AHB1 peripheral clock enable register
	_vo uint32_t AHB2ENR; 		//AHB2 peripheral clock enable register
	_vo uint32_t AHB3ENR; 		//AHB3 peripheral clock enable register
	_vo uint32_t RESERVED2; 		//
	_vo uint32_t APB1ENR; 		//APB1 peripheral clock enable register
	_vo uint32_t APB2ENR; 		//APB2 peripheral clock enable register
	_vo uint32_t RESERVED3[2]; 		//
	_vo uint32_t AHB1LPENR; 	//AHB1 peripheral clock enable in low power mode register
	_vo uint32_t AHB2LPENR; 	//AHB2 peripheral clock enable in low power mode register
	_vo uint32_t AHB3LPENR; 	//AHB3 peripheral clock enable in low power mode register
	_vo uint32_t RESERVED4; 	//
	_vo uint32_t APB1LPENR; 	//APB1 peripheral clock enable in low power mode register
	_vo uint32_t APB2LPENR; 	//APB2 peripheral clock enable in low power mode register
	_vo uint32_t RESERVED5[2]; 	//
	_vo uint32_t BDCR; 			//Backup domain control register
	_vo uint32_t CSR;			//clock control and status register
	_vo uint32_t RESERVED6[2]; 	//
	_vo uint32_t SSCGR; 		//spread spectrum clock generation register
	_vo uint32_t PLLI2SCFGR; 	//PLLI2S configuration register
	_vo uint32_t PLLSAICFGR; 	//PLL configuration register
	_vo uint32_t DCKCFGR; 		//dedicated clock configuration register
	_vo uint32_t CKGATENR; 		//clocks gated enable register
	_vo uint32_t DCKCFGR2; 		//dedicated clocks configuration register 2


} RCC_Regdef_t;


/*
 * EXTI peripheral register structure
 */


typedef struct
{
	_vo uint32_t IMR;			//Interrupt mask register
	_vo uint32_t EMR;			//Event mask register
	_vo uint32_t RTSR;			//Rising trigger selection register
	_vo uint32_t FTSR;			//Falling trigger selection register
	_vo uint32_t SWIER;			//Software interrupt event register
	_vo uint32_t PR;			//Pending register


} EXTI_Regdef_t;


/*
 * SYSCFR peripheral register structure
 */


typedef struct
{
	_vo uint32_t MEMRMP; 		//SYSCFG memory remap register
	_vo uint32_t PMC; 			//SYSCFG peripheral mode configuration register
	_vo uint32_t EXTICR[4]; 	//SYSCFG external interrupt configuration registers
	uint32_t RESERVED1[2];
	_vo uint32_t CMPCR; 		//Compensation cell control register
	uint32_t RESERVED2[2];
	_vo uint32_t CFGR; 			//SYSCFG configuration register


}SYSCFR_Regdef_t;


/*
 * SPI peripheral register structure
 */
typedef struct
{
	_vo uint32_t CR1;			//SPI control register 1
	_vo uint32_t CR2;			//SPI control register 2
	_vo uint32_t SR;			//SPI status register
	_vo uint32_t DR;			//SPI data register
	_vo uint32_t CRCPR;			//SPI CRC polynomial register
	_vo uint32_t RXCRCR;		//SPI RX CRC register
	_vo uint32_t TXCRCR;		//SPI TX CRC register
	_vo uint32_t I2SCFGR;		//SPI_I2S configuration register
	_vo uint32_t I2SPR;			//SPI_I2S prescaler register


}SPI_Regdef_t;


//Clock	enable macros of GPIOx peripherals
#define GPIOA_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<0) )
#define GPIOB_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<1) )
#define GPIOC_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<2) )
#define GPIOD_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<3) )
#define GPIOE_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<4) )
#define GPIOF_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<5) )
#define GPIOG_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<6) )
#define GPIOH_PCLK_EN() 		( RCC -> AHB1ENR |= (1<<7) )

//Clock enable macros of I2Cx peripherals
#define I2C1_PCLK_EN()			( RCC -> APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()			( RCC -> APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()			( RCC -> APB1ENR |= (1<<23))

//Clock enable macros of SPIx peripherals
#define SPI1_PCLK_EN()			( RCC -> APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()			( RCC -> APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()			( RCC -> APB1ENR |= (1<<15))
#define SPI4_PCLK_EN()			( RCC -> APB2ENR |= (1<<13))

//Clock enable Macros of USARTx peripherals
#define USART1_PCLK_EN()		( RCC -> APB2ENR |= (1<<4))
#define USART2_PCLK_EN()		( RCC -> APB1ENR |= (1<<17))
#define USART3_PCLK_EN()		( RCC -> APB1ENR |= (1<<18))
#define UART4_PCLK_EN()			( RCC -> APB1ENR |= (1<<19))
#define UART5_PCLK_EN()			( RCC -> APB1ENR |= (1<<20))
#define USART6_PCLK_EN()		( RCC -> APB2ENR |= (1<<5))

//Clock enable Macros of SYSCFG peripherals
#define SYSCFG_PCLK_EN()		( RCC -> APB2ENR |= (1<<14))

/**********************************************************/

//Clock disable Macros of GPIOx peripherals

//Clock disable Macros of I2Cx peripherals

//Clock disable Macros of SPIx peripherals
#define SPI1_PCLK_DI()			( RCC -> APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI()			( RCC -> APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI()			( RCC -> APB1ENR &= ~(1<<15))
#define SPI4_PCLK_DI()			( RCC -> APB2ENR &= ~(1<<13))

//Clock disable Macros of USARTx peripherals

//Clock disable Macros of SYSCFG peripherals

//Macros to reset the GPIOx peripherals
#define GPIOA_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<0) ) ; ( RCC -> AHB1RSTR &= ~(1<<0) ); } while(0)
#define GPIOB_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<1) ) ; ( RCC -> AHB1RSTR &= ~(1<<1) ); } while(0)
#define GPIOC_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<2) ) ; ( RCC -> AHB1RSTR &= ~(1<<2) ); } while(0)
#define GPIOD_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<3) ) ; ( RCC -> AHB1RSTR &= ~(1<<3) ); } while(0)
#define GPIOE_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<4) ) ; ( RCC -> AHB1RSTR &= ~(1<<4) ); } while(0)
#define GPIOF_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<5) ) ; ( RCC -> AHB1RSTR &= ~(1<<5) ); } while(0)
#define GPIOG_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<6) ) ; ( RCC -> AHB1RSTR &= ~(1<<6) ); } while(0)
#define GPIOH_REG_RESET()		do{( RCC -> AHB1RSTR |= (1<<7) ) ; ( RCC -> AHB1RSTR &= ~(1<<7) ); } while(0)

//Macros to reset SPIx peripherals
#define SPI1_REG_RESET()		do{( RCC -> APB2RSTR |= (1<<12)) ; ( RCC -> APB2RSTR &= ~(1<<12)); } while(0)
#define SPI2_REG_RESET()		do{( RCC -> APB1RSTR |= (1<<14)) ; ( RCC -> APB1RSTR &= ~(1<<14)); } while(0)
#define SPI3_REG_RESET()		do{( RCC -> APB1RSTR |= (1<<15)) ; ( RCC -> APB1RSTR &= ~(1<<15)); } while(0)
#define SPI4_REG_RESET()		do{( RCC -> APB2RSTR |= (1<<13)) ; ( RCC -> APB2RSTR &= ~(1<<13)); } while(0)


#define GPIO_PORT_TO_CODE(x)	((x == GPIOA)?0:\
								 (x == GPIOB)?1:\
								 (x == GPIOC)?2:\
								 (x == GPIOD)?3:\
								 (x == GPIOE)?4:\
								 (x == GPIOF)?5:\
								 (x == GPIOG)?6:\
								 (x == GPIOH)?7:0)

/*
 * IRQ Numbers of stm32f446re
 */

#define IRQ_NO_EXTI0			6
#define IRQ_NO_EXTI1			7
#define IRQ_NO_EXTI2			8
#define IRQ_NO_EXTI3			9
#define IRQ_NO_EXTI4			10
#define IRQ_NO_EXTI9_5			23
#define IRQ_NO_EXTI15_10		40

/***************************************************************************************************************
 * Bit position definition of SPI peripheral
 *
 ***************************************************************************************************************/
#define SPI_CR1_CPHA			0
#define SPI_CR1_CPOL			1
#define SPI_CR1_MSTR			2
#define SPI_CR1_BR				3
#define SPI_CR1_SPE				6
#define SPI_CR1_LSBFIRST		7
#define SPI_CR1_SSI 			8
#define SPI_CR1_SSM  			9
#define SPI_CR1_RXONLY			10
#define SPI_CR1_DFF				11
#define SPI_CR1_CRCNEXT			12
#define SPI_CR1_CRCEN			13
#define SPI_CR1_BIDIOE			14
#define SPI_CR1_BIDIMODE		15

#define SPI_CR2_RXDMAEN			0
#define SPI_CR2_TXDMAEN			1
#define SPI_CR2_SSOE			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE			5
#define SPI_CR2_RXNEIE 			6
#define SPI_CR2_TXEIE			7

#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8


//Some Generic Macros
#define ENABLE 					1
#define DISABLE					0
#define SET						ENABLE
#define RESET					DISABLE
#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET
#define FLAG_RESET				RESET
#define FLAG_SET				SET



#include "stm32f446e_gpio_driver.h"
#include "stm32f446e_spi_driver.h"

#endif /* STM32F446E_H_ */
