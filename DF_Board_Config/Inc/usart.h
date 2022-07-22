/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define USART1_BAUD		115200
#define USART2_BAUD		115200
#define USART3_BAUD		115200
#define UART4_BAUD		115200
#define UART5_BAUD		115200

#define UART1_RX_LENGTH 128
#define UART2_RX_LENGTH 128
#define UART3_RX_LENGTH 128
#define UART4_RX_LENGTH 128
#define UART5_RX_LENGTH 128
	
#define USART1_TX_GPIO	GPIOA
#define USART1_TX_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define USART1_TX_PIN	GPIO_PIN_9
#define USART1_RX_GPIO	GPIOA
#define USART1_RX_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define USART1_RX_PIN	GPIO_PIN_10

#define USART2_TX_GPIO	GPIOA
#define USART2_TX_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define USART2_TX_PIN	GPIO_PIN_2
#define USART2_RX_GPIO	GPIOA
#define USART2_RX_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define USART2_RX_PIN	GPIO_PIN_3

#define USART3_TX_GPIO	GPIOB
#define USART3_TX_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE();
#define USART3_TX_PIN	GPIO_PIN_10
#define USART3_RX_GPIO	GPIOB
#define USART3_RX_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE();
#define USART3_RX_PIN	GPIO_PIN_11

#define UART4_TX_GPIO	GPIOC
#define UART4_TX_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE();
#define UART4_TX_PIN	GPIO_PIN_10
#define UART4_RX_GPIO	GPIOC
#define UART4_RX_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE();
#define UART4_RX_PIN	GPIO_PIN_11

#define UART5_TX_GPIO	GPIOC
#define UART5_TX_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE();
#define UART5_TX_PIN	GPIO_PIN_12
#define UART5_RX_GPIO	GPIOD
#define UART5_RX_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE();
#define UART5_RX_PIN	GPIO_PIN_2

#define PRINT_UART huart1

extern UART_HandleTypeDef huart4;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart5;

typedef struct{
	UART_HandleTypeDef* uart;
	uint8_t *dataAddr;
	uint16_t dataLength;
	uint16_t index;
	uint16_t lastIndex;
}UARTInfo_t;

inline void UART1_SendData(uint8_t *data,uint16_t length);
inline void UART2_SendData(uint8_t *data,uint16_t length);
inline void UART3_SendData(uint8_t *data,uint16_t length);
inline void UART4_SendData(uint8_t *data,uint16_t length);
inline void UART5_SendData(uint8_t *data,uint16_t length);

void UART4Init(void);
void UART5Init(void);
void USART1Init(void);
void USART2Init(void);
void USART3Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

