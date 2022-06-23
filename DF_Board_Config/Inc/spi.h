/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


extern SPI_HandleTypeDef hspi1;

extern SPI_HandleTypeDef hspi2;


void SPI1Init(void);
void SPI2Init(void);

void SPI_SetSpeed(SPI_HandleTypeDef* spiHandle , uint8_t SPI_BaudRatePrescaler);
uint8_t SPI_Write_Read_Byte(SPI_HandleTypeDef* spiHandle , uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

