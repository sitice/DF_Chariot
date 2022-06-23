#ifndef __ICM20602_HPP__
#define __ICM20602_HPP__

#include "main.h"
class ICM20602
{
public:
	struct Acc_t //���ٶ����ݽṹ��
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
	struct Gyro_t //���ٶ����ݽṹ��
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
private: 
/* Internal variables -----------------------------------------------------------*/
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_GPIO;
	uint16_t _GPIOPin;
	Acc_t acc;
	Gyro_t gyro;

/* Internal function -----------------------------------------------------------*/
	void Write(uint8_t reg,uint8_t data);
	uint8_t Read(uint8_t reg);
	void ReadNbyte(uint8_t REG,uint8_t len,uint8_t *buf);
	inline void DelayMs(uint16_t time);

/* register address -----------------------------------------------------------*/
	/********************************************
	*��λ�����мĴ�����ַ��Ϊ0������
	*register 26  CONFIG				 =  0x80
	*register 107 Power Management 1 	 =  0x41
	*register 117 WHO_AM_I 				 =  0x12
	*********************************************/
	//�������¶Ȳ���
	const uint8_t XG_OFFS_TC_H = 0x04;
	const uint8_t XG_OFFS_TC_L = 0x05;
	const uint8_t YG_OFFS_TC_H = 0x07;
	const uint8_t YG_OFFS_TC_L = 0x08;
	const uint8_t ZG_OFFS_TC_H = 0x0A;
	const uint8_t ZG_OFFS_TC_L = 0x0B;
	//���ٶ��Լ����(����ʱ���ã��������û����Լ����ֵ�Ƚϣ�
	const uint8_t SELF_TEST_X_ACCEL = 0x0D;
	const uint8_t SELF_TEST_Y_ACCEL = 0x0E;
	const uint8_t SELF_TEST_Z_ACCEL = 0x0F;
	//�����Ǿ�̬ƫ��
	const uint8_t XG_OFFS_USRH = 0x13;
	const uint8_t XG_OFFS_USRL = 0x14;
	const uint8_t YG_OFFS_USRH = 0x15;
	const uint8_t YG_OFFS_USRL = 0x16;
	const uint8_t ZG_OFFS_USRH = 0x17;
	const uint8_t ZG_OFFS_USRL = 0x18;

	const uint8_t SMPLRT_DIV = 0x19;//�����ǲ����ʣ�����ֵ��0x07(125Hz)
	const uint8_t CONFIG = 0x1A;
	const uint8_t GYRO_CONFIG = 0x1B;
	const uint8_t ACCEL_CONFIG = 0x1C;
	const uint8_t ACCEL_CONFIG2 = 0x1D;
	const uint8_t LP_MODE_CFG = 0x1E;

	//�˶����Ѽ��ٶ���ֵ
	const uint8_t ACCEL_WOM_X_THR = 0x20;
	const uint8_t ACCEL_WOM_Y_THR = 0x21;
	const uint8_t ACCEL_WOM_Z_THR = 0x22;


	const uint8_t FIFO_EN = 0x23;
	const uint8_t FSYNC_INT = 0x36;
	const uint8_t INT_PIN_CFG = 0x37;
	//const uint8_t	INT_ENABLE = 0x38;
	const uint8_t FIFO_WM_INT_STATUS = 0x39;
	const uint8_t INT_STATUS = 0x3A;

	//���ٶ����
	const uint8_t ACCEL_XOUT_H = 0x3B;
	const uint8_t ACCEL_XOUT_L = 0x3C;
	const uint8_t ACCEL_YOUT_H = 0x3D;
	const uint8_t ACCEL_YOUT_L = 0x3E;
	const uint8_t ACCEL_ZOUT_H = 0x3F;
	const uint8_t ACCEL_ZOUT_L = 0x40;
	//�¶����
	const uint8_t TEMP_OUT_H = 0x41;
	const uint8_t TEMP_OUT_L = 0x42;
	//���ٶ����
	const uint8_t GYRO_XOUT_H = 0x43;
	const uint8_t GYRO_XOUT_L = 0x44;
	const uint8_t GYRO_YOUT_H = 0x45;
	const uint8_t GYRO_YOUT_L = 0x46;
	const uint8_t GYRO_ZOUT_H = 0x47;
	const uint8_t GYRO_ZOUT_L = 0x48;
	//�������Լ����
	const uint8_t SELF_TEST_X_GYRO = 0x50;
	const uint8_t SELF_TEST_Y_GYRO = 0x51;
	const uint8_t SELF_TEST_Z_GYRO = 0x52;

	const uint8_t FIFO_WM_TH1 = 0x60;
	const uint8_t FIFO_WM_TH2 = 0x61;
	const uint8_t SIGNAL_PATH_RESET = 0x68;
	const uint8_t ACCEL_INTEL_CTRL = 0x69;
	const uint8_t USER_CTRL = 0x6A;
	//��Դ����
	const uint8_t PWR_MGMT_1 = 0x6B; //��Դ��������ֵ��0x00(��������)
	const uint8_t PWR_MGMT_2 = 0x6C;

	const uint8_t I2C_IF = 0x70;
	const uint8_t FIFO_COUNTH = 0x72;
	const uint8_t FIFO_COUNTL = 0x73;
	const uint8_t FIFO_R_W = 0x74;

	const uint8_t	WHO_AM_I = 0x75;
	//���ٶȾ�̬ƫ��
	const uint8_t XA_OFFSET_H = 0x77;
	const uint8_t XA_OFFSET_L = 0x78;
	const uint8_t YA_OFFSET_H = 0x7A;
	const uint8_t YA_OFFSET_L = 0x7B;
	const uint8_t ZA_OFFSET_H = 0x7D;
	const uint8_t ZA_OFFSET_L = 0x7E;
	const uint8_t ADDRESS = 0xD2;

public:
/**
  * @brief ���캯��
  * @param hspi spi���
  * @param CS_GPIO ƬѡGPIO
  * @param CS_Pin Ƭѡ����
  * @retval none
  */
	ICM20602(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin);
/**
  * @brief ��ʼ������
  * @param none
  * @retval ����0��ʼ��ʧ�ܣ�����1��ʼ���ɹ�
  */
	uint8_t Init(void);
/**
  * @brief ��λ
  * @param none
  * @retval ����0��ʼ��ʧ�ܣ�����1��ʼ���ɹ�
  */
	void Rest(void);
/**
  * @brief ��������
  * @param none
  * @retval none
  */
	void Updata(void);
/**
  * @brief ��ȡ�¶�
  * @param none
  * @retval none
  */
	float GetTemperature(void);
/**
  * @brief �õ����ٶ�����
  * @note ��Ҫ��ִ��Updata������ȡ���µ�����
  * @param none
  * @retval ���ٶ�
  */
	Acc_t GetAccVal(){return acc;};
/**
  * @brief �õ����ٶ�����
  * @note ��Ҫ��ִ��Updata������ȡ���µ�����
  * @param none
  * @retval ���ٶ�
  */
	Gyro_t GetGyroVal(){return gyro;};
};

#endif