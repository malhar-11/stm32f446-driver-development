/*
 * stm32f446e_spi_driver.h
 *
 *  Created on: 05-Jul-2026
 *      Author: Malhar
 */

#ifndef INC_STM32F446E_SPI_DRIVER_H_
#define INC_STM32F446E_SPI_DRIVER_H_

#include "stm32f446e.h"

/*
 * Configuration structure for SPIx Peripheral
 */

typedef struct
{
	uint8_t SPI_DeviceMode;			//Slave or Master
	uint8_t SPI_BusConfig;			//Full-Duplex, Half-Duplex, Simplex
	uint8_t SPI_SClkSpeed;			//
	uint8_t SPI_DFF;				//Data Format - 8 or 16 bits
	uint8_t SPI_CPOL;				//Polarity
	uint8_t SPI_CPHA;				//Phase
	uint8_t SPI_SSM;				//Slave Management - HW or SW

} SPI_Config_t;

/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
	SPI_Regdef_t 	pSPIx;			//Base address of SPIx peripherals
	SPI_Config_t	SPIConfig		//
};

#endif /* INC_STM32F446E_SPI_DRIVER_H_ */

