/*
 * stm32_cc1101.h
 *
 *  Created on: Jun 26, 2025
 *      Author: bkendinibilir
 */

#ifndef STM32_CC1101_INC_STM32_CC1101_H_
#define STM32_CC1101_INC_STM32_CC1101_H_

/* -- Includes -----------------------------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"

/* -- Options ------------------------------------------------------------------*/

#define CC1101_LOGLEVEL			2		// 0 = No debug, 1 = errors, 2 = debug

/* -- Constants ----------------------------------------------------------------*/

#define CC1101_VERSION_DEFAULT 	0x14	// Default version of CC1101
#define CC1101_WAKEUP_TIMEOUT	5		// Wakeup time in ms

/* CC1101 MODES	*/
// header byte: first bit R/^W, second bit burst access
#define CC1101_WRITE_BURST  	0x40	// 01000000
#define CC1101_READ_SINGLE  	0x80	// 10000000
#define CC1101_READ_BURST   	0xC0	// 11000000

/* CC1101 STATUS REGSITER */
#define CC1101_PARTNUM      	0x30
#define CC1101_VERSION      	0x31
#define CC1101_FREQEST      	0x32
#define CC1101_LQI          	0x33
#define CC1101_RSSI         	0x34
#define CC1101_MARCSTATE    	0x35
#define CC1101_WORTIME1     	0x36
#define CC1101_WORTIME0     	0x37
#define CC1101_PKTSTATUS    	0x38
#define CC1101_VCO_VC_DAC   	0x39
#define CC1101_TXBYTES      	0x3A
#define CC1101_RXBYTES      	0x3B

/* -- Typedefs -----------------------------------------------------------------*/

typedef struct {
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* CS_GPIO_Port;
    uint16_t CS_Pin;
    GPIO_TypeDef* MISO_GPIO_Port;
    uint16_t MISO_Pin;
} CC1101_HandleTypeDef;

typedef enum {
    CC1101_SPI_OP_READ,
    CC1101_SPI_OP_WRITE
} CC1101_SPI_OpType;

/* -- Macros -------------------------------------------------------------------*/

#define ERROR_LOG(...) \
	do { \
		if (CC1101_LOGLEVEL >= 1) { \
			printf("ERROR: " __VA_ARGS__); \
		} \
	} while (0)

#define DEBUG_LOG(...) \
	do { \
		if (CC1101_LOGLEVEL >= 2) { \
			printf("DEBUG: "__VA_ARGS__); \
		} \
	} while (0)

/* -- Public Functions ---------------------------------------------------------*/

bool CC1101_Init(CC1101_HandleTypeDef* cc1101_handle);
bool CC1101_CheckVersion(void);
bool CC1101_GetVersion(uint8_t *pVersion);

bool CC1101_ReadStatusRegister(uint8_t reg, uint8_t *pData);
bool CC1101_ReadRegister(uint8_t reg, uint8_t *pData);

/* -- Private Functions ---------------------------------------------------------*/

HAL_StatusTypeDef __CC1101_SPI_Read(uint8_t *addr, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef __CC1101_SPI_Write(uint8_t *addr, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef __CC1101_SPI_Transfer(uint8_t *addr, uint8_t *pData, uint16_t size, CC1101_SPI_OpType op);
bool __CC1101_MISO_Wait(void);

#endif /* STM32_CC1101_INC_STM32_CC1101_H_ */

