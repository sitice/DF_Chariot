#include "motor.hpp"
#include "pwm.h"


Motor & Motor::SetGPIO(GPIO_TypeDef * A_GPIO,uint16_t A_Pin,GPIO_TypeDef * B_GPIO,uint16_t B_Pin)
{
	_A_GPIO = A_GPIO, _A_Pin = A_Pin,_B_GPIO = B_GPIO,_B_Pin = B_Pin;
	return *this;
}
Motor & Motor::SetPWMTimerAndChannel(TIM_HandleTypeDef* tim_pwmHandle,uint32_t channel)
{
	_tim_pwmHandle = tim_pwmHandle;
	_channel = channel;
	return *this;
}
Motor & Motor::SetEncoderTimer(TIM_HandleTypeDef* tim_encoderHandle)
{
	_tim_encoderHandle = tim_encoderHandle;
	return *this;
}
void Motor::SetRpm(int16_t rpm)
{
	if(rpm < 0)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_RESET);
	}
	else if(rpm > 0)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
	}
	SetPWM(_tim_pwmHandle,_channel,ABS(rpm));
}
void Motor::SetRpm(Dir_e dir,uint16_t rpm)
{
	if(dir == Forward)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_RESET);
	}
	else if(dir == Back)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
	}
	SetPWM(_tim_pwmHandle,_channel,rpm);
}

void Motor::Init()
{
	GPIO_TypeDef *GPIOs[]{_A_GPIO,_B_GPIO};
	uint16_t pins[]{_A_Pin,_B_Pin};
	for(size_t i = 0 ; i < 2 ; i++)
	{
		if(GPIOs[i] == GPIOA)
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOB)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOC)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOD)
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOE)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOs[i], &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOs[i],pins[i], GPIO_PIN_SET);
	}
	HAL_TIM_PWM_Start(_tim_pwmHandle,_channel);
	
	HAL_TIM_Encoder_Start(_tim_encoderHandle,TIM_CHANNEL_ALL);
	
	SetPWM(_tim_pwmHandle,_channel,0);

}
