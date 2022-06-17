#ifndef __ENCODER_H__
#define __ENCODER_H__

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include "main.h"

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim5;

/**
  * @brief 定时器2编码器模式初始化
  * @note  PA15    ------> TIM2_CH1
  *        PB3     ------> TIM2_CH2
  * @param none.
  * @retval none
  */
void TIM2EncoderInit(void);

/**
  * @brief 定时器4编码器模式初始化
  * @note  PB4    ------> TIM3_CH1
  *        PB5    ------> TIM3_CH2
  * @param none.
  * @retval none
  */
void TIM3EncoderInit(void);

/**
  * @brief 定时器5编码器模式初始化
  * @note  PB6    ------> TIM4_CH1
  *        PB7    ------> TIM4_CH2
  * @param none.
  * @retval none
  */
void TIM4EncoderInit(void);

/**
  * @brief 定时器5编码器模式初始化
  * @note  PA0-WKUP ------> TIM5_CH1
  *        PA1      ------> TIM5_CH2
  * @param none.
  * @retval none
  */
void TIM5EncoderInit(void);

/**
  * @brief 读取编码器值
  * @param 定时器句柄
  * @retval 编码器值
  */
#define GetEncoderValue(tim_encoderHandle) __HAL_TIM_GET_COUNTER(tim_encoderHandle)

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*__ENCODER_H__*/