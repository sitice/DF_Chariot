/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


extern TIM_HandleTypeDef htim6;

void TIM6Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

volatile uint64_t GetSysTime(void);
#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

