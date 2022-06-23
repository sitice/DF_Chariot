#ifndef __AK8975_HPP__
#define __AK8975_HPP__

#include "main.h"

class AK8975
{
public:
	struct Mag_t//���������ݽṹ��
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
/**
  * @brief ���캯��
  * @param hspi spi���
  * @param CS_GPIO ƬѡGPIO
  * @param CS_Pin Ƭѡ����
  * @retval none
  */
	AK8975(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin) : _hspi(hspi),_GPIO(GPIO),_GPIOPin(GPIOPin){};

/**
  * @brief ��ʼ������
  * @param none
  * @retval ����0��ʼ��ʧ�ܣ�����1��ʼ���ɹ�
  */
	uint8_t Init(void);
/**
  * @brief ��ȡ����������
  * @param none
  * @retval Mag_t ����������
  */
	Mag_t GetMagVal(void);
/**
  * @brief ��λ������
  * @param none
  * @retval none
  */
	void Restart(void);
private:
 /* register address -----------------------------------------------------------*/
	const uint8_t ID = 0X48;
	const uint8_t ID_REG = 0x00; //������ID�ļĴ�����ַ
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