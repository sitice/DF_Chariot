#include "motor.hpp"
#include "pwm.h"

void Motor::SetRpm(int16_t rpm)
{
	if(rpm < 0)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_B_GPIO,_B_Pin,GPIO_PIN_RESET);
	}
	else if(rpm > 0)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(_B_GPIO,_B_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_B_GPIO,_B_Pin,GPIO_PIN_SET);
	}
	SetPWM(_tim_pwmHandle,_channel,ABS(rpm));
}
void Motor::SetRpm(Dir_e dir,uint16_t rpm)
{
	if(dir == Forward)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_B_GPIO,_B_Pin,GPIO_PIN_RESET);
	}
	else if(dir == Back)
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(_B_GPIO,_B_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(_A_GPIO,_A_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(_B_GPIO,_B_Pin,GPIO_PIN_SET);
	}
	SetPWM(_tim_pwmHandle,_channel,rpm);
}

void Motor::Init()
{
	HAL_GPIO_WritePin(_A_GPIO,_A_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(_B_GPIO,_B_Pin, GPIO_PIN_SET);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = _A_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_A_GPIO, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = _B_Pin;
	HAL_GPIO_Init(_B_GPIO, &GPIO_InitStruct);
	
	HAL_TIM_PWM_Start(_tim_pwmHandle,_channel);
	
	HAL_TIM_Encoder_Start(_tim_encoderHandle,TIM_CHANNEL_ALL);
	
	SetPWM(_tim_pwmHandle,_channel,0);

}
