/*
 * stm32f446e_i2c_driver.h
 *
 *  Created on: 16-Jul-2026
 *      Author: Malhar
 */

#ifndef INC_STM32F446E_I2C_DRIVER_H_
#define INC_STM32F446E_I2C_DRIVER_H_

#include "stm32f446e.h"
#include <stdio.h>

/*
 * I2C peripheral configuration structure
 */
typedef struct
{
	uint8_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddr;
	uint8_t I2C_ACKCtrl;
	uint8_t I2C_FMDutyCycle;

}I2C_Config_t;

/*
 * I2C Handle structure
 */
typedef struct
{
	I2C_Regdef_t 	*pI2Cx;
	I2C_Config_t 	I2CConfig;
	uint8_t 		*pTxBuffer;		/* !< To store the app. Tx buffer address > */
	uint8_t			*pRxBuffer;		/* !< To store the app. Rx buffer address > */
	uint32_t		TxLen;			/* !< To store Tx len > */
	uint32_t		RxLen;			/* !< To store Rx len > */
	uint8_t			TxRxState;		/* !< To store Communication state > */
	uint8_t			DevAddr;		/* !< To store slave/device addres> */
	uint32_t		RxSize;			/* !< To store Rx size> */
	uint8_t			Sr;				/* !< To store count of repeated starts	> */

}I2C_Handle_t;

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 			100000
#define I2C_SCL_SPEED_FM2K			200000
#define I2C_SCL_SPEED_FM4K			400000

/*
 * @I2C_ACKCtrl
 */
#define I2C_ACK_EN					1
#define I2C_ACK_DI					0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2 				0
#define I2C_FM_DUTY_16_9 			1

/*
 * 	I2C Application States
 */
#define I2C_READY					0
#define I2C_BUSY_IN_RX				1
#define I2C_BUSY_IN_TX				2

/*
 * Repeated start macros
 */
#define I2C_SR_ENABLE				1
#define I2C_SR_DISABLE				0


/*
 * I2C Flag definitions
 */
#define I2C_FLAG_SB					(1<<I2C_SR1_SB)
#define I2C_FLAG_ADDR				(1<<I2C_SR1_ADDR)
#define I2C_FLAG_BTF				(1<<I2C_SR1_BTF)
#define I2C_FLAG_ADD10				(1<<I2C_SR1_ADD10)
#define I2C_FLAG_STOPF				(1<<I2C_SR1_STOPF)
#define I2C_FLAG_RXNE				(1<<I2C_SR1_RXNE)
#define I2C_FLAG_TXE				(1<<I2C_SR1_TXE)
#define I2C_FLAG_BERR				(1<<I2C_SR1_BERR)
#define I2C_FLAG_ARLO				(1<<I2C_SR1_ARLO)
#define I2C_FLAG_AF 				(1<<I2C_SR1_AF)
#define I2C_FLAG_OVR				(1<<I2C_SR1_OVR)
#define I2C_FLAG_PECERR				(1<<I2C_SR1_PECERR)
#define I2C_FLAG_TIMEOUT			(1<<I2C_SR1_TIMEOUT)
#define I2C_FLAG_SMBALERT			(1<<I2C_SR1_SMBALERT)

/*
 * Possible Event and Errors definition Macros
 */
#define I2C_EVT_TX_CMPLT			0
#define I2C_EVT_RX_CMPLT			1
#define I2C_EVT_STOP				2



/*********************************************************************************************
 *
 * 									APIs supported by this driver
 * 							for more info check the function definitions
 *
 *********************************************************************************************/

/*
 *
 */
void I2C_PClkControl(I2C_Regdef_t *pI2Cx, uint8_t ENorDI);

/*
 * Init & De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_Regdef_t *pI2Cx);

/*
 * Data send and Receive
 */
void I2CMasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2CMasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/*
 * IRQ Configuration & ISR Handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

void I2C_EV_IRQHandle(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandle(I2C_Handle_t *pI2CHandle);

/*
 * other Peripheral Control APIs
 */
void I2C_PeripheralCtrl(I2C_Regdef_t *pI2Cx, uint8_t ENorDI);
uint8_t I2C_GetFlagStatus(I2C_Regdef_t *pI2Cx, uint32_t FlagName);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_ACKConfig(I2C_Regdef_t *pI2Cx, uint8_t ENorDI);

/*
 * Application CallBack
 */
_weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);


#endif /* INC_STM32F446E_I2C_DRIVER_H_ */
