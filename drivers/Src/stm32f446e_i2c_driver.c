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
static void I2C_ExecuteAddrPhase(I2C_Regdef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Regdef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_Regdef_t *pI2Cx);
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


static void I2C_ExecuteAddrPhase(I2C_Regdef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFlag(I2C_Regdef_t *pI2Cx)
{
	uint16_t dummyread;

	dummyread = pI2Cx->SR1;
	dummyread = pI2Cx->SR2;

	(void)dummyread;
}

void I2CMasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	uint16_t tempreg;
	//1. Generate START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB bit in SR1 register
	// NOTE: Until SB bit is cleared, SCL will is stretched LOW
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to w(0)
	I2C_ExecuteAddrPhase(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag is SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Clear ADDR flag
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);

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
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

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
