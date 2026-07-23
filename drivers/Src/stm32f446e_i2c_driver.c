/*
 * stm32f446e_i2c_driver.c
 *
 *  Created on: 16-Jul-2026
 *      Author: Malhar
 */

#include "stm32f446e_i2c_driver.h"

uint16_t AHB_Prescalar[9] = {2, 4, 8, 16, 32, 64, 128, 256, 512};
uint8_t APB1_Prescalar[4] ={2, 4, 8, 16};

static void I2C_GenerateStartCondition(I2C_Regdef_t *pI2Cx);
static void I2C_ExecuteAddrPhase_Write(I2C_Regdef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddrPhase_Read(I2C_Regdef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_GenerateStopCondition(I2C_Regdef_t *pI2Cx);

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);

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
void I2C_PClkControl(I2C_Regdef_t *pI2Cx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if(pI2Cx == I2C2)
		{
			I2C3_PCLK_EN();
		}else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}

	}else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}else if(pI2Cx == I2C2)
		{
			I2C3_PCLK_DI();
		}else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
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
uint32_t RCC_PLLOutputClk()
{
	return 0;
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
uint32_t RCC_GetPClkValue(void)
{
	uint32_t pclk1, ahb, apb1, temp, SystemClk;


	//Identification of Clock source
	uint8_t clksrc = ((RCC->CFGR >> 2) & (0x3));

	if(clksrc == 0)
	{
		SystemClk = 16000000;

	}else if(clksrc == 1)
	{
		SystemClk = 8000000;

	}else if(clksrc == 2)
	{
		SystemClk = RCC_PLLOutputClk();
	}

	//Determining Prescalar for AHB bus
	temp = ((RCC->CFGR >> 4) & (0xF));

	if(temp < 8)
	{
		ahb = 1;
	}else
	{
		ahb = AHB_Prescalar[temp-8];
	}

	//Determining Prescalar for APB1 bus
	temp = ((RCC->CFGR >> 10) & (0x7));

	if(temp < 4)
	{
		apb1 = 1;
	}else
	{
		apb1 = APB1_Prescalar[temp-4];
	}

	//Resulting Peripheral Clock
	pclk1 = (SystemClk / ahb)/apb1 ;

	return pclk1;
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
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;

	//ACKing init
	tempreg |= (pI2CHandle->I2CConfig.I2C_ACKCtrl << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//Configuring FREQ bit of the CR2 register
	tempreg = 0;
	tempreg |= RCC_GetPClkValue() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & (0x3F));

	//program the device own address
	tempreg = 0;
	tempreg |= (pI2CHandle->I2CConfig.I2C_DeviceAddr << 1);
	tempreg |= (1<<14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//standard mode
		ccr_value = RCC_GetPClkValue() / (2 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		tempreg |= (ccr_value & 0xFFF);
	}else
	{
		//fast mode
		tempreg |= (1<<I2C_CCR_FS);
		tempreg |= (pI2CHandle->I2CConfig.I2C_FMDutyCycle << I2C_CCR_DUTY);

		if( pI2CHandle->I2CConfig.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = RCC_GetPClkValue() / (3 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		}else
		{
			ccr_value = RCC_GetPClkValue() / (25 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		}

		tempreg |= (ccr_value & 0xFFF);
	}

	pI2CHandle->pI2Cx->CCR = tempreg;

	tempreg = 0;

	//TRISE configuration
	if(pI2CHandle->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//standard mode
		tempreg = (RCC_GetPClkValue() / 1000000U) + 1;

	}else
	{
		//fast mode
		tempreg = ((RCC_GetPClkValue() * 300)/1000000000) + 1;

	}
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
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
void I2C_DeInit(I2C_Regdef_t *pI2Cx);

/*
 * Data send and Receive
 */
static void I2C_GenerateStartCondition(I2C_Regdef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_START);
}

static void I2C_GenerateStopCondition(I2C_Regdef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_STOP);
}


static void I2C_ExecuteAddrPhase_Write(I2C_Regdef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ExecuteAddrPhase_Read(I2C_Regdef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= (1);
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint16_t dummyread;

	if(pI2CHandle->pI2Cx->SR2 & (1<<I2C_SR2_MSL))
	{
		//device is in master
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize == 1)
			{
				//disable ACKing
				I2C_ACKConfig(pI2CHandle->pI2Cx, DISABLE);

				//clear ADDR flag
				dummyread = pI2CHandle->pI2Cx->SR1;
				dummyread = pI2CHandle->pI2Cx->SR1;
				(void)dummyread;
			}
		}else
		{
			//clear ADDR flag
			dummyread = pI2CHandle->pI2Cx->SR1;
			dummyread = pI2CHandle->pI2Cx->SR1;
			(void)dummyread;
		}
	}else
	{
		//clear ADDR flag
		dummyread = pI2CHandle->pI2Cx->SR1;
		dummyread = pI2CHandle->pI2Cx->SR1;
		(void)dummyread;
	}
}

void I2CMasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{

	//1. Generate START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB bit in SR1 register
	// NOTE: Until SB bit is cleared, SCL will is stretched LOW
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to w(0)
	I2C_ExecuteAddrPhase_Write(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag is SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Clear ADDR flag
	I2C_ClearADDRFlag(pI2CHandle);

	//6. Send Data Until Len = 0
	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	//7. When Len=0, wait for BTF and TXE flag to be set before generating stop condition
	//   Note: when BTF=1, TXE=1, it means both SR and DR are empty and next transmission should begin
	//	 when BTF=1 SCL will be pull low

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Genrate STOP condition and master need not to wait for the completion of stop conditon
	//	 Genrating stop condition automatically clears BTF

	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}

void I2CMasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{

	//Generate START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB bit in SR1 register
	// NOTE: Until SB bit is cleared, SCL will is stretched LOW
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the slave with the r/nw bit set to R(1)
	I2C_ExecuteAddrPhase_Read(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Wait till the address phase is completed by checking the ADDR Flag
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Procedure to read 1 byte from the slave at a time
	if(Len == 1)
	{
		//disable ACKing
		I2C_ACKConfig(pI2CHandle->pI2Cx, DISABLE);

		//clear ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//wait until RXNE flag is set
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		//generate stop condition
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//read data to the buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;
		pRxBuffer++;
		Len--;

		return;
	}

	if(Len > 1)
	{
		//Clear ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read data until Len becomes 0
		for( uint32_t i = Len; i>0; i--)
		{
			//Wait till RXNE is set
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			if(i == 2) //last 2 bytes
			{
				//disable ACKing
				I2C_ACKConfig(pI2CHandle->pI2Cx, DISABLE);

				//generate stop condition
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			}
			*pRxBuffer = pI2CHandle->pI2Cx->DR;
			pRxBuffer++;
			Len--;
		}
	}

	//re-enable ACKing
	I2C_ACKConfig(pI2CHandle->pI2Cx, ENABLE);
}

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;

}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);
	}

	return busystate;
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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI)
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	//Finding IPRx register
	uint8_t iprx = (IRQNumber / 4);

	//Finding the section
	uint8_t iprx_section = (IRQNumber % 4);

	uint8_t shift_amount = (8*iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_IPR_BASEADDR + (iprx)) |= (IRQPriority << shift_amount);
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{

	//One byte
	if(pI2CHandle->RxSize == 1)
	{
		*(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen --;
	}

	//More than one byte
	else if(pI2CHandle->RxSize > 1)
	{
		if(pI2CHandle->RxLen == 2)
		{
			//clear ACK bit
			I2C_ACKConfig(pI2CHandle->pI2Cx, DISABLE);
		}

		*(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen --;
		pI2CHandle->pRxBuffer++;
	}

	else if(pI2CHandle->RxLen == 0)
	{
		//close I2C reception  and notify the application

		//1. Generate Stop condition
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//2. Close i2c Rx
		I2C_CloseReceiveData(pI2CHandle);

		//3. Notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EVT_RX_CMPLT);
	}
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen > 0)
	{
		//Load data in DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		//Decrement TX Len
		pI2CHandle->TxLen --;

		//Increment buffer address
		pI2CHandle->pTxBuffer ++;
	}
}

void I2C_EV_IRQHandle(I2C_Handle_t *pI2CHandle)
{

	//Interrupt handling for both master and slave mode of a device
	uint32_t temp1, temp2, temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	temp3 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_SR1_SB);

	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
	if(temp1 && temp2)
	{
		//Interrupt due to SB flag
		//Block wont be executed in slave mode (SB=0) for slave mode

		//Now handling the Address phase
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddrPhase_Write(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddrPhase_Read(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}

	}


	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);

	if(temp1 && temp2)
	{
		//Interrupt due to ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

	}
	temp3 = 0;

	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);

	if(temp1 && temp2)
	{
		//BTF flag is set
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			//Check TXE flag
			if(pI2CHandle->pI2Cx->SR1 & 1 << I2C_SR1_TXE)
			{
				//Check if Len = 0;
				if(pI2CHandle->TxLen == 0)
				{
					//1. Stop Transmission
					//Check is SR disable
					if(pI2CHandle->Sr == I2C_SR_DISABLE)
					{
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}

					//2. Reset all the meneber elements of the handle structure
					I2C_CloseSendData(pI2CHandle);


					//3. Notify the application that the Tx is over
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EVT_TX_CMPLT);
				}
			}
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}
	}
	temp3 = 0;

	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);

	if(temp1 && temp2)
	{
		//STOPF flag is set
		//CLear STOPF
		//Read SR1 followed by writing to CR1;
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//Notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EVT_STOP);

	}
	temp3 = 0;

	//5. Handle For interrupt generated by TXE event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);

	if((temp1 && temp2) && temp3)
	{
		//TXE flag is set

		//Check Device mode is Master or not
		if(pI2CHandle->pI2Cx->SR2 & (1<<I2C_SR2_MSL))
		{
			//Data Transmission
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}


	}


	//6. Handle For interrupt generated by RXNE event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);

	if((temp1 && temp2) && temp3)
	{
		//RXNE flag is set

		//Check Device mode
		if(pI2CHandle->pI2Cx->SR2 & (1<<I2C_SR2_MSL))
		{
			//Device is master
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				//Data Reception
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}
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
void I2C_PeripheralCtrl(I2C_Regdef_t *pI2Cx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pI2Cx->CR1 |= (1<<I2C_CR1_PE);
	}else
	{
		pI2Cx->CR1 &= ~(1<<I2C_CR1_PE);
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
uint8_t I2C_GetFlagStatus(I2C_Regdef_t *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}else
	{
		return FLAG_RESET;
	}
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Disable ITEVTEN
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITEVTEN);

	//Disable ITBUFEN
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITBUFEN);

	pI2CHandle->TxLen = 0;
	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Disable ITEVTEN
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITEVTEN);

	//Disable ITBUFEN
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITBUFEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;
	pI2CHandle->pRxBuffer = NULL;

	if(pI2CHandle->I2CConfig.I2C_ACKCtrl == I2C_ACK_EN)
	{
		I2C_ACKConfig(pI2CHandle->pI2Cx, ENABLE);
	}
}

void I2C_ACKConfig(I2C_Regdef_t *pI2Cx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pI2Cx->CR1 |= (1<<I2C_CR1_ACK);
	}else
	{
		pI2Cx->CR1 &= ~(1<<I2C_CR1_ACK);
	}
}

/*
 * Application CallBack
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
_weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);
