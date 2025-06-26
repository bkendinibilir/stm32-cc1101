/*
 * stm32_cc1101.c
 *
 *  Created on: Jun 26, 2025
 *      Author: bkendinibilir
 */

#include "stm32_cc1101.h"

CC1101_HandleTypeDef* cc1101;


bool CC1101_Init(CC1101_HandleTypeDef* cc1101_handle) {
	if (cc1101_handle == NULL) {
		ERROR_LOG("CC1101 handle is NULL.\n");
		return false;
	}
	cc1101 = cc1101_handle;

	// Check if the CC1101 is working correctly by checking the version
	if (!CC1101_CheckVersion()) {
		ERROR_LOG("CC1101 version check failed.\n");
		return false;
	}

	return true;
}

bool CC1101_CheckVersion(void) {
	uint8_t version;

	if (CC1101_GetVersion(&version) && version == CC1101_VERSION_DEFAULT) {
		return true;
	} else {
		DEBUG_LOG("CC1101 not responding or version not 0x%02X\n", CC1101_VERSION_DEFAULT);
		return false;
	}
}

bool CC1101_GetVersion(uint8_t *pVersion) {
	return CC1101_ReadStatusRegister(CC1101_VERSION, pVersion);
}

/* -- Read and Write Functions --------------------------------------------------*/

bool CC1101_ReadStatusRegister(uint8_t reg, uint8_t *pData) {
	// Set bit for read burst access to read status register
	reg |= CC1101_READ_BURST;

	return CC1101_ReadRegister(reg, pData);
}

bool CC1101_ReadRegister(uint8_t reg, uint8_t *pData) {
	reg |= CC1101_READ_SINGLE; // Set the bit for read single byte

	HAL_StatusTypeDef status = __CC1101_SPI_Read(&reg, pData, 1);
	if (status != HAL_OK) {
		ERROR_LOG("Failed to read register 0x%02X, status: %d\n", reg, status);
		return false;
	}

	return true;
}

/* -- SPI Read and Write Functions -----------------------------------------------*/

HAL_StatusTypeDef __CC1101_SPI_Read(uint8_t *addr, uint8_t *pData, uint16_t size) {
    return __CC1101_SPI_Transfer(addr, pData, size, CC1101_SPI_OP_READ);
}

HAL_StatusTypeDef __CC1101_SPI_Write(uint8_t *addr, uint8_t *pData, uint16_t size) {
    return __CC1101_SPI_Transfer(addr, pData, size, CC1101_SPI_OP_WRITE);
}

HAL_StatusTypeDef __CC1101_SPI_Transfer(uint8_t *addr, uint8_t *pData, uint16_t size, CC1101_SPI_OpType op) {
	HAL_StatusTypeDef status;

	// Set the CS pin low to select the CC1101
	HAL_GPIO_WritePin(cc1101->CS_GPIO_Port, cc1101->CS_Pin, GPIO_PIN_RESET);

	// Wait for MISO to be low before sending the command
	if(!__CC1101_MISO_Wait()) {
		status = HAL_ERROR;

	} else {
		// Send the address, which we want to read or write
		status = HAL_SPI_Transmit(cc1101->hspi, addr, 1, HAL_MAX_DELAY);

		if(status == HAL_OK) {
			// read or write data based on the operation type
			if(op == CC1101_SPI_OP_READ) {
				status = HAL_SPI_Receive(cc1101->hspi, pData, size, HAL_MAX_DELAY);
			} else if(op == CC1101_SPI_OP_WRITE && pData != NULL) {
				status = HAL_SPI_Transmit(cc1101->hspi, pData, size, HAL_MAX_DELAY);
			}
		}
	}

	// Set the CS pin to high to deselect the CC1101
	HAL_GPIO_WritePin(cc1101->CS_GPIO_Port, cc1101->CS_Pin, GPIO_PIN_SET);

	return status;
}

bool __CC1101_MISO_Wait(void) {
	uint32_t startTime = HAL_GetTick();

	// Wait for MISO to be low before sending the command
	while(HAL_GPIO_ReadPin(cc1101->MISO_GPIO_Port, cc1101->MISO_Pin) == GPIO_PIN_SET) {
		if (HAL_GetTick() - startTime > CC1101_WAKEUP_TIMEOUT) {
			DEBUG_LOG("Timeout in waiting for MISO going low, CC1101 not responding.\n");
			return false;
		}
	}
	return true;
}
