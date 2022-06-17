#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include "main.h"

#define TIM1_PWM_PIN				GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14
#define TIM1_PWM_GPIO				GPIOE

#define TIM8_PWM_PIN				GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9
#define TIM8_PWM_GPIO				GPIOC

#define TIM9_PWM_PIN				GPIO_PIN_5 | GPIO_PIN_6
#define TIM9_PWM_GPIO				GPIOE

#define TIM1_PWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOE_CLK_ENABLE()

#define TIM8_PWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

#define TIM9_PWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOE_CLK_ENABLE()

														
#define TIM1_PWM_PSR				84 - 1			// 168M / 84 = 2M
#define TIM1_PWM_ARR				5000 - 1		// 2M / 5000 = 400 Hz 5000 / 2M = 2.5ms

#define TIM8_PWM_PSR				84 - 1			// 168M / 84 = 2M
#define TIM8_PWM_ARR				5000 - 1		// 2M / 5000 = 400 Hz 5000 / 2M = 2.5ms

#define TIM9_PWM_PSR				84 - 1			// 168M / 84 = 2M
#define TIM9_PWM_ARR				5000 - 1		// 2M / 5000 = 400 Hz 5000 / 2M = 2.5ms


extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim8;

extern TIM_HandleTypeDef htim9;

/**
  * @brief Timer1 pwm mode init
  * @note  PE9     ------> TIM1_CH1
  *        PE11     ------> TIM1_CH2
  *        PE13     ------> TIM1_CH3
  *        PE14     ------> TIM1_CH4
  * @param none.
  * @retval none
  */
void TIM1PWMInit(void);

/**
  * @brief Timer8 pwm mode init
  * @note  PC6     ------> TIM8_CH1
  *        PC7     ------> TIM8_CH2
  *        PC8     ------> TIM8_CH3
  *        PC9     ------> TIM8_CH4
  * @param none.
  * @retval none
  */
void TIM8PWMInit(void);

/**
  * @brief Timer9 pwm mode init
  * @note  PE5     ------> TIM9_CH1
  *        PE6     ------> TIM9_CH2
  * @param none.
  * @retval none
  */
void TIM9PWMInit(void);

/**
  * @brief set pwm
  * @param timer 
  * @param channel 
  * @param value 
  * @retval none
  */
#define SetPWM(tim,channel,value) __HAL_TIM_SetCompare(tim,channel,value)

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*__PWM_H__*/