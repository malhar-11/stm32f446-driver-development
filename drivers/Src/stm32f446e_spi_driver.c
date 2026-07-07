/*
 * stm32f446e_spi_driver.c
 *
 *  Created on: 06-Jul-2026
 *      Author: Malhar
 */


#include "stm32f446e_spi_driver.h"


/*
 * Peripheral Clock Setup
 */

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
void SPI_PClkControl(SPI_Regdef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}else if(pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}else if(pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
	}
}

/*
 * Init & De-init
 */

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
void SPI_init(SPI_Handle_t *pSPIHandle)
{
	/*
	 * Configuring CR1 Register
	 */

	//Defining a Temp register and copying it to CR1 at the end
	uint32_t tempreg = 0;

	//Device mode
	tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	//BUS Configuration
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//clear bidimode bit
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//set bidimode bit
		tempreg |= (1<<SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX)
	{
		//clear bidimode bit
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);

		//set RXONLY bit
		tempreg |= (1<<SPI_CR1_RXONLY);
	}

	//Baud Rate
	tempreg |= (pSPIHandle->SPIConfig.SPI_SClkSpeed << SPI_CR1_BR);

	//DFF
	tempreg |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

	//CPHA
	tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

	//CPOL
	tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

	//SSM
	tempreg |= (pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);


	pSPIHandle->pSPIx.CR1 = tempreg;
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
void SPI_DeInit(SPI_Regdef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}else if(pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}



/*
 * Data send and Receive
 */

uint8_t SPI_GetFlagStatus(SPI_Regdef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}else
	{
		return FLAG_RESET;
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
void SPI_SendData(SPI_Regdef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait till TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//2. Check DFF bit
		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
			//16 bit DFF
			//1. load data in the register
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;

			//make the TxBuffer point to next data item
			(uint16_t*)pTxBuffer++;
		}else
		{
			//8 bit DFF
			pSPIx->DR = *pTxBuffer;
			Len--;
			(uint16_t*)pTxBuffer++;
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
void SPI_ReceiveData(SPI_Regdef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait till TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		//2. Check DFF bit
		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
			//16 bit DFF
			//1. load data in the register
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;
			Len--;
			Len--;

			//make the TxBuffer point to next data item
			(uint16_t*)pRxBuffer++;
		}else
		{
			//8 bit DFF
			*pRxBuffer = pSPIx->DR ;
			Len--;
			(uint16_t*)pRxBuffer++;
		}
	}
}

/*
 * IRQ Configuration & ISR Handling
 */

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
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);

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
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

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
void SPI_IRQHandle(SPI_Handle_t *pSPIHandle);

/*
 * other Peripheral Control APIs
 */

