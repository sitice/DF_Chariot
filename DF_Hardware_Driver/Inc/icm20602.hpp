#ifndef __ICM20602_HPP__
#define __ICM20602_HPP__

#include "main.h"
class ICM20602
{
private:                             
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_GPIO;
	uint16_t _GPIOPin;
	inline uint8_t SPI_RW(uint8_t data);
	void Write(uint8_t reg,uint8_t data);
	uint8_t Read(uint8_t reg);
	void ReadNbyte(uint8_t REG,uint8_t len,uint8_t *buf);
	uint8_t Init(void);
	inline void DelayMs(uint16_t time);
public:
	ICM20602(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin); 
	struct Acc_t
	{
		int16_t x;
		int16_t y;
		int16_t z;
	}acc;
	struct Gyro_t
	{
		int16_t x;
		int16_t y;
		int16_t z;
	}gyro;
	uint8_t state;
	void Rest(void);
	void Updata(void);
	float GetTemperature(void);
};

#endif