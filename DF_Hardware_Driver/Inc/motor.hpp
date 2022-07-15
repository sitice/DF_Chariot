#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "encoder.h"
#include "tim.h"

#define ABS(x) (x)>0 ? (x):-(x)

class Motor
{
public:
	Motor(GPIO_TypeDef * A_GPIO,uint16_t A_Pin,GPIO_TypeDef * B_GPIO,uint16_t B_Pin,
	TIM_HandleTypeDef* tim_pwmHandle,uint32_t channel,
	TIM_HandleTypeDef* tim_encoderHandle):_A_GPIO(A_GPIO),_B_GPIO(B_GPIO),_A_Pin(A_Pin),_B_Pin(B_Pin),_tim_pwmHandle(tim_pwmHandle),
	_channel(channel),_tim_encoderHandle(tim_encoderHandle){};
	enum Dir_e
	{
		Forward = 0,
		Back,
		Stop
	};
	void Init(void);
	void SetRpm(int16_t rpm);
	void SetRpm(Dir_e dir,uint16_t rpm);
	int16_t GetRpm(){return GetEncoderValue(_tim_encoderHandle);};
	int32_t EncoderCntSum;
	
private:
	GPIO_TypeDef * _A_GPIO;
	uint16_t _A_Pin;
	GPIO_TypeDef * _B_GPIO;
	uint16_t _B_Pin;
	TIM_HandleTypeDef* _tim_pwmHandle;
	uint32_t _channel;
	TIM_HandleTypeDef* _tim_encoderHandle;
};


#endif