#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "encoder.h"

#define ABS(x) (x)>0 ? (x):-(x)

class Motor
{
public:
	enum Dir_e
	{
		Forward = 0,
		Back,
		Stop
	};
	Motor & SetGPIO(GPIO_TypeDef * A_GPIO,uint16_t A_Pin,GPIO_TypeDef * B_GPIO,uint16_t B_Pin);
	Motor & SetPWMTimerAndChannel(TIM_HandleTypeDef* tim_pwmHandle,uint32_t channel);
	Motor & SetEncoderTimer(TIM_HandleTypeDef* tim_encoderHandle);
	void Init(void);
	void SetRpm(int16_t rpm);
	void SetRpm(Dir_e dir,uint16_t rpm);
	int16_t GetRpm(){return GetEncoderValue(_tim_encoderHandle);};
	
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