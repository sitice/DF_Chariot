#ifndef __AK8975_HPP__
#define __AK8975_HPP__

#include "main.h"

class AK8975
{
public:
	struct Mag_t//磁力计数据结构体
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
/**
  * @brief 构造函数
  * @param hspi spi句柄
  * @param CS_GPIO 片选GPIO
  * @param CS_Pin 片选引脚
  * @retval none
  */
	AK8975(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin) : _hspi(hspi),_GPIO(GPIO),_GPIOPin(GPIOPin){};

/**
  * @brief 初始化函数
  * @param none
  * @retval 返回0初始化失败，返回1初始化成功
  */
	uint8_t Init(void);
/**
  * @brief 获取磁力计数据
  * @param none
  * @retval Mag_t 磁力计数据
  */
	Mag_t GetMagVal(void);
/**
  * @brief 复位磁力计
  * @param none
  * @retval none
  */
	void Restart(void);
private:
 /* register address -----------------------------------------------------------*/
	const uint8_t ID = 0X48;
	const uint8_t ID_REG = 0x00; //磁力计ID的寄存器地址
	const uint8_t STATUS1 = 0x02;
	const uint8_t XOUT_L = 0x03;
	const uint8_t XOUT_H = 0x04;
	const uint8_t YOUT_L = 0x05;
	const uint8_t YOUT_H = 0x06;
	const uint8_t ZOUT_L = 0x07;
	const uint8_t ZOUT_H = 0x08;
	const uint8_t STATUS2 = 0x09;
	const uint8_t CONTROL = 0x0A;
	const uint8_t SELF_TEST = 0x0C;

/* Internal variables -----------------------------------------------------------*/
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_GPIO;
	uint16_t _GPIOPin;
/* Internal function -----------------------------------------------------------*/
	void Write(uint8_t reg,uint8_t data);
	uint8_t Read(uint8_t reg);
};

#endif