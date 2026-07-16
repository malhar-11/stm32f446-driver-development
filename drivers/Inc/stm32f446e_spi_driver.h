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
	uint8_t SPI_DeviceMode;			//Slave or Master @SPIDeviceModes
	uint8_t SPI_BusConfig;			//Full-Duplex, Half-Duplex, Simplex @SPI_BusConfig
	uint8_t SPI_SClkSpeed;			//@SClkSpeed
	uint8_t SPI_DFF;				//Data Format - 8 or 16 bits @DFF
	uint8_t SPI_CPOL;				//Polarity @CPOL
	uint8_t SPI_CPHA;				//Phase @CPHA
	uint8_t SPI_SSM;				//Slave Management - HW or SW @SSM

} SPI_Config_t;

/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
	SPI_Regdef_t 	pSPIx;			//Base address of SPIx peripherals
	SPI_Config_t	SPIConfig;		//
	uint8_t 		*pTxBuffer;		//To store applications Tx Buffer address
	uint8_t 		*pRxBuffer;		//To store applications Rx Buffer address
	uint8_t 		TxLen;			//To store Tx data Length
	uint8_t 		RxLen;			//To store Rx data Length
	uint8_t 		TxState;		//To store the current Tx state
	uint8_t 		RxState;		//To store the current Rx state

}SPI_Handle_t;


/*
 * @SPIDeviceModes
 */
#define SPI_DEVICE_MODE_MASTER		1
#define SPI_DEVICE_MODE_SLAVE		0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD			0
#define SPI_BUS_CONFIG_HD			1
#define SPI_BUS_CONFIG_SIMPLEX_RX	2
//#define SPI_BUS_CONFIG_SIMPLEX_TX	3

/*
 * @SClkSpeed
 */
#define SPI_SCLK_PRESCALAR_2		0
#define SPI_SCLK_PRESCALAR_4		1
#define SPI_SCLK_PRESCALAR_8		2
#define SPI_SCLK_PRESCALAR_16		3
#define SPI_SCLK_PRESCALAR_32		4
#define SPI_SCLK_PRESCALAR_64		5
#define SPI_SCLK_PRESCALAR_128		6
#define SPI_SCLK_PRESCALAR_256		7

/*
 * @DFF
 */
#define SPI_DFF_BITS_8				0
#define SPI_DFF_BITS_16				1

/*
 * @CPOL
 */
#define SPI_CPOL_LOW				0
#define SPI_CPOL_HIGH				1

/*
 * @CPHA
 */
#define SPI_CPHA_LOW				0
#define SPI_CPHA_HIGH				1

/*
 * @SSM
 */
#define SPI_SSM_DI					0
#define SPI_SSM_EN					1


//SPI related status flag definitions
#define SPI_RXNE_FLAG				(1<< SPI_SR_RXNE)
#define SPI_TXE_FLAG				(1<< SPI_SR_TXE)
#define SPI_BUSY_FLAG				(1<< SPI_SR_BSY)

//Possible SPI Application states
#define SPI_READY					0
#define SPI_BUSY_IN_RX				1
#define SPI_BUSY_IN_TX				2

//Possible application events
#define SPI_EVENT_TX_CMPLT			1
#define SPI_EVENT_RX_CMPLT			2
#define SPI_EVENT_OVR_ERR			3
#define SPI_EVENT_CRC_ERR			4



/*********************************************************************************************
 *
 * 									APIs supported by this driver
 * 							for more info check the function definitions
 *
 *********************************************************************************************/

/*
 * Peripheral Clock Setup
 */
void SPI_PClkControl(SPI_Regdef_t *pSPIx, uint8_t ENorDI);

/*
 * Init & De-init
 */
void SPI_init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_Regdef_t *pSPIx);

/*
 * Data send and Receive
 */
void SPI_SendData(SPI_Regdef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_Regdef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration & ISR Handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandle(SPI_Handle_t *pSPIHandle);

/*
 * other Peripheral Control APIs
 */
void SPI_PeripheralCtrl(SPI_Regdef_t *pSPIx, uint8_t ENorDI);
void SPI_SSIConfig(SPI_Regdef_t *pSPIx, uint8_t ENorDI);
void SPI_SSOEConfig(SPI_Regdef_t *pSPIx, uint8_t ENorDI);
uint8_t SPI_GetFlagStatus(SPI_Regdef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
_weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif /* INC_STM32F446E_SPI_DRIVER_H_ */



