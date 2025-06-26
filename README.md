# stm32-cc1101

This repository contains initial experiments with an STM32 microcontroller and SPI communication with a CC1101 transceiver.

The project demonstrates basic setup, initialization, and register access for the CC1101 using STM32 HAL libraries. It is intended as a starting point for further development and learning about SPI-based RF communication on STM32 platforms.

Use in main.c:

    // Initialize CC1101
    CC1101_HandleTypeDef cc1101 = {
        .hspi = &hspi1,
        .CS_GPIO_Port = CC1101_CS_GPIO_Port,
        .CS_Pin = CC1101_CS_Pin,
        .MISO_GPIO_Port = CC1101_MISO_GPIO_Port,
        .MISO_Pin = CC1101_MISO_Pin
    };
    if (CC1101_Init(&cc1101)) {
      printf("CC1101 initialized successfully.\n");
    } else {
      printf("CC1101 initialization failed.\n");
    }

Note:
This code is experimental and for educational purposes. Use at your own risk.
