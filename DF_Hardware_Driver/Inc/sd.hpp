#ifndef __SD_HPP__
#define __SD_HPP__

#include "main.h"

class SD
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
	SD(SPI_HandleTypeDef *hspi,
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
const uint8_t TYPE_ERR = 0X00;
const uint8_t TYPE_MMC = 0X01;
const uint8_t TYPE_V1 = 0X02;
const uint8_t TYPE_V2 = 0X04;
const uint8_t TYPE_V2HC = 0X06;

const uint8_t CMD0 = 0;      
const uint8_t CMD1 = 1;
const uint8_t CMD8 = 8;      
const uint8_t CMD9 = 9;      
const uint8_t CMD10 = 10;     
const uint8_t CMD12 = 12;      
const uint8_t CMD16 = 16;      
const uint8_t CMD17 = 17;     
const uint8_t CMD18 = 18;     
const uint8_t CMD23 = 23;     
const uint8_t CMD24 = 24;     
const uint8_t CMD25 = 25;      
const uint8_t CMD41 = 41;      
const uint8_t CMD55 = 55;    
const uint8_t CMD58 = 58;     
const uint8_t CMD59 = 59;     

const uint8_t MSD_DATA_OK = 0x05;
const uint8_t MSD_DATA_CRC_ERROR = 0x0B;
const uint8_t MSD_DATA_WRITE_ERROR = 0x0D;
const uint8_t MSD_DATA_OTHER_ERROR = 0xFF;

const uint8_t MSD_RESPONSE_NO_ERROR = 0x00;
const uint8_t MSD_IN_IDLE_STATE = 0x01;
const uint8_t MSD_ERASE_RESET = 0x02;
const uint8_t MSD_ILLEGAL_COMMAND = 0x04;
const uint8_t MSD_COM_CRC_ERROR = 0x08;
const uint8_t MSD_ERASE_SEQUENCE_ERROR = 0x10;
const uint8_t MSD_ADDRESS_ERROR = 0x20;
const uint8_t MSD_PARAMETER_ERROR = 0x40;
const uint8_t MSD_RESPONSE_FAILURE = 0xFF;

/* Internal variables -----------------------------------------------------------*/
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_GPIO;
	uint16_t _GPIOPin;
/* Internal function -----------------------------------------------------------*/
	void Write(uint8_t reg,uint8_t data);
	uint8_t Read(uint8_t reg);
};


#endif