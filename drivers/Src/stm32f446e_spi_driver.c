/*
 * stm32f446e_spi_driver.c
 *
 *  Created on: 06-Jul-2026
 *      Author: Malhar
 */


#include "stm32f446e_spi_driver.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);


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
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx.CR1 & (1<<SPI_CR1_DFF))
	{
		//16 bit DFF
		//1. load data in the register
		pSPIHandle->pSPIx.DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;

		//make the TxBuffer point to next data item
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		//8 bit DFF
		pSPIHandle->pSPIx.DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;

		//make the TxBuffer point to next data item
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen )
	{
		//TxLen is zero, close the communication and inform the application that
		//Tx is over.

		//Prevent interrupts from setting up of the TXE flag
		SPI_CloseTransmission(pSPIHandle);

		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);

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
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx.CR1 & (1<<SPI_CR1_DFF))
	{
		//16 bit DFF
		//1. load data in the register
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx.DR ;
		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--;

		//make the TxBuffer point to next data item
		(uint16_t*)pSPIHandle->pRxBuffer++;
	}else
	{
		//8 bit DFF
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx.DR ;
		pSPIHandle->RxLen--;
		(uint16_t*)pSPIHandle->pRxBuffer++;
	}

	if(!pSPIHandle->RxLen)
	{
		//Reception is complete
		//turn off rxne interrupt
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
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
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//clear the ovr flag
	SPI_ClearOVRFlag(pSPIHandle);

	//inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);

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
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI)
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
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
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
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1. Save Tx Buffer addr and Len  in Global Variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		//2. Mark SPI state as busy in Transmission so that
		//   no other code take of the SPI peripheral until Tx is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//Enable TXEIE control bit to get interrupt whenever the TXE flag is set in SR
		pSPIHandle->pSPIx.CR2 |= (1 << SPI_CR2_TXEIE);

		//Data Tx will be handled by the ISR code
	}

	return state;
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
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1. Save Rx Buffer addr and Len  in Global Variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		//2. Mark SPI state as busy in Receive so that
		//   no other code take of the SPI peripheral until Rx is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//Enable RXNEIE control bit to get interrupt whenever the RXNE flag is set in SR
		pSPIHandle->pSPIx.CR2 |= (1 << SPI_CR2_RXNEIE);

		//Data Rx will be handled by the ISR code
	}

	return state;
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
void SPI_IRQHandle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1, temp2;

	// Check for TXE
	temp1 = pSPIHandle->pSPIx.SR & (1 << SPI_SR_TXE);
	temp2 = pSPIHandle->pSPIx.CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 & temp2)
	{
		//handle TXE
		spi_txe_interrupt_handle(pSPIHandle);
	}

	// Check for RXNE
	temp1 = pSPIHandle->pSPIx.SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx.CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 & temp2)
	{
		//handle RXNE
		spi_rxne_interrupt_handle(pSPIHandle);
	}

	//Check for OVR flag
	temp1 = pSPIHandle->pSPIx.SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx.CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 & temp2)
	{
		//handle OVR flag
		spi_ovr_err_interrupt_handle(pSPIHandle	);
	}

}


/*
 * other Peripheral Control APIs
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
void SPI_PeripheralCtrl(SPI_Regdef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR1 |= (1<<SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SPE);
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
void SPI_SSIConfig(SPI_Regdef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR1 |= (1<<SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SSI);
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
void SPI_SSOEConfig(SPI_Regdef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR2 |= (1<<SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &= ~(1<<SPI_CR2_SSOE);
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
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx.CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;


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
void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx.CR2 &= ~(1<<SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
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
void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;

	if(pSPIHandle->TxState == SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx.DR;
		temp = pSPIHandle->pSPIx.SR;
	}
	(void)temp;
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
_weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	//this is weak implementation of the function, it can be overridden
}


