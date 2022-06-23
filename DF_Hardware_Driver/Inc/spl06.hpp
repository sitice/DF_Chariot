#ifndef __SPL06_HPP__
#define __SPL06_HPP__

#include "main.h"

class SPL06
{
public:
/**
  * @brief ���캯��
  * @param hspi spi���
  * @param CS_GPIO ƬѡGPIO
  * @param CS_Pin Ƭѡ����
  * @retval none
  */
	SPL06(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *CS_GPIO,
	uint16_t CS_Pin): _hspi(hspi),_CS_GPIO(CS_GPIO),_CS_Pin(CS_Pin){};
/**
  * @brief ��ʼ��
  * @retval ����0ʧ�ܣ�����1�ɹ�
  */
	uint8_t Init(void);
/**
  * @brief ��ȡSPL06�¶�
  * @retval �¶�
  */
	float GetRealTemperature(void);
	
/**
  * @brief ��ȡSPL06��ѹ
  * @retval ��ѹ
  */
	float GetRealPresure(void);

	struct Baro_t{
		float temperature;//�¶�
		float pressure;//��ѹ
		float alti;//�߶�
	};
/**
  * @brief ��������
  * @retval Baro_t SPL06����
  */
	Baro_t Updata();

private:
/* register address -----------------------------------------------------------*/
	const uint8_t PRESSURE_REG = 0X00;
	const uint8_t TEMP_REG = 0X03;
	const uint8_t PRS_CFG_REG = 0x06; //��ѹ������������
	const uint8_t TMP_CFG_REG = 0x07; //�¶Ȳ����ٶ�����
	const uint8_t MEAS_CFG_REG = 0x08; //���������봫��������
	const uint8_t CFG_REG = 0x09; //�ж�/FIFO/SPI����������
	const uint8_t INT_STS_REG = 0X0A; //�ж�״̬��־λ
	const uint8_t FIFO_STS_REG = 0X0B; //FIFO״̬
	const uint8_t RESET_REG = 0X0C;
	const uint8_t ID_REG = 0x0D;
	const uint8_t COEF_REG = 0x10;

	const uint8_t PRESSURE_RATE_1_TIMES = 0; //������ 
	const uint8_t PRESSURE_RATE_2_TIMES = 1;
	const uint8_t PRESSURE_RATE_4_TIMES = 2;
	const uint8_t PRESSURE_RATE_8_TIMES = 3;
	const uint8_t PRESSURE_RATE_16_TIMES = 4;
	const uint8_t PRESSURE_RATE_32_TIMES = 5;
	const uint8_t PRESSURE_RATE_64_TIMES = 6;
	const uint8_t PRESSURE_RATE_128_TIMES = 7;

	const uint8_t TEMPERATURE_RATE_1_TIMES = 0; //������
	const uint8_t TEMPERATURE_RATE_2_TIMES = 1;
	const uint8_t TEMPERATURE_RATE_4_TIMES = 2;
	const uint8_t TEMPERATURE_RATE_8_TIMES = 3;
	const uint8_t TEMPERATURE_RATE_16_TIMES = 4;
	const uint8_t TEMPERATURE_RATE_32_TIMES = 5;
	const uint8_t TEMPERATURE_RATE_64_TIMES = 6;
	const uint8_t TEMPERATURE_RATE_128_TIMES = 7;
	const uint8_t TEMPERATURE_RATE_TMP_EXT_INTERNAL = 0;//���ɵ�·�ϵ��¶ȼ�
	const uint8_t TEMPERATURE_RATE_TMP_EXT_EXTERNAL = 1;//������MEMS��ѹоƬ���¶�

	const uint8_t MEAS_CFG_COEF_RDY = 0X80;// �������ڲ�У׼ֵ�ɶ�����������
	const uint8_t MEAS_CFG_SENSOR_RDY = 0X40;// �������ѳ�ʼ����ɣ���������
	const uint8_t MEAS_CFG_TMP_RDY = 0x20;//�¶�ֵ�Ѿ�׼�����������Խ��ж�ȡ���ñ�־λ��ȡ���Զ���0
	const uint8_t MEAS_CFG_PRS_RDY = 0x10;//��ѹֵ�Ѿ�׼�����������Խ��ж�ȡ���ñ�־λ
	const uint8_t MEAS_CFG_MEAS_CTR_STANDBY = 0;//ģʽ���� ����ģʽ
	const uint8_t MEAS_CFG_MEAS_CTR_COMMAND_PRS = 0x01;//ģʽ���� ����ģʽ��������ѹ�ɼ�
	const uint8_t MEAS_CFG_MEAS_CTR_COMMAND_TMP = 0x02;//ģʽ���� ����ģʽ�������¶Ȳɼ�
	const uint8_t MEAS_CFG_MEAS_CTR_BACKGROUND_PRS = 0x05;//ģʽ���� ��̨ģʽֻ��ȡ��ѹֵ
	const uint8_t MEAS_CFG_MEAS_CTR_BACKGROUND_TMP = 0X06;//ģʽ���� ��̨ģʽֻ��ȡ�¶�ֵ
	const uint8_t MEAS_CFG_MEAS_CTR_BACKGROUND_PSR_TMP = 0X07;//ģʽ���� ��̨ģʽͬʱ��ȡ�¶�ֵ����ѹֵ

	const uint8_t CFG_INT_LEVEL_ACTIVE_LOW = 0;//�жϵ͵�ƽ��Ч
	const uint8_t CFG_INT_LEVEL_ACTIVE_HIGH = 1;//�жϸߵ�ƽ��Ч
	const uint8_t CFG_INT_FIFO = 0X40;//��FIFO��ʹ���ж� 
	const uint8_t CFG_INT_PRS = 0X20;//����ѹ�ƶ�ȡ���ʹ���ж� 
	const uint8_t CFG_INT_TMP = 0X10;//���¶ȶ�ȡ���ʹ���ж� 
	const uint8_t CFG_T_SHIFT = 0X08;//�������ݱ����ǣ����Խ�����һ�ʲɼ�
	const uint8_t CFG_P_SHIFT = 0X04;//�������ݱ����ǣ����Խ�����һ�ʲɼ�
	const uint8_t CFG_FIF = 0X02;//ʹ��FIFO
	const uint8_t CFG_SPI_3_WIRE = 1;//3��SPI
	const uint8_t CFG_SPI_4_WIRE = 0;//4��SPI

	const uint8_t INT_STS_FIFO_FULL = 0X04;//FIFO���ж�״̬
	const uint8_t INT_STS_FIFO_TMP = 0X02;//�¶Ȳ�����ɱ�־λ
	const uint8_t INT_STS_FIFO_PRS = 0X01;//��ѹ������ɱ�־λ

	const uint8_t FIFO_STS_FULL = 0X02;//FIFO��
	const uint8_t FIFO_STS_EMPTY = 0X01;//FIFO����

	const uint8_t RESET_FIFO_FLUSH = 0X80; //FIFO��0
	const uint8_t RESET_SOFT = 0X09;//�����λ

	const uint8_t PRODUCT_ID = 0X10;//��ƷID

	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_CS_GPIO;
	uint16_t _CS_Pin;
	
/* Internal variables -----------------------------------------------------------*/
	struct {	//�ڲ�����У׼����
		int16_t c0;
		int16_t c1;
		int32_t c00;
		int32_t c10;
		int16_t c01;
		int16_t c11;
		int16_t c20;
		int16_t c21;
		int16_t c30;       
	}spl06_calib_param;

	struct{	
		uint8_t chip_id;// оƬID
		int32_t i32rawPressure;// ԭʼ��ѹ����
		int32_t i32rawTemperature;// ԭʼ�¶�����
		int32_t i32kP;// ��ѹ��������
		int32_t i32kT;// �¶Ȳ�������
	}spl06;
/* Internal function -----------------------------------------------------------*/
	void Write(uint8_t reg,uint8_t data);
	uint8_t Read(uint8_t reg);
	void PressureRateConfig(uint8_t background_rate,uint8_t oversamply);
	void TemperatureRateConfig(uint8_t background_rate,uint8_t oversamply,uint8_t ext);
	inline uint8_t GetMeasureStatus(void);
	inline void SetMeasureMode(uint8_t mode);
	inline void StartTemperature(void);
	inline void StartPressure(void);
	inline void EnterStandby(void);
	inline void SetInterrupt(uint8_t interrupt,uint8_t type);
	inline void SetSpiWire(uint8_t wire);
	inline void SetIntruptLevel(uint8_t level);
	inline uint8_t GetIntStatus(void);
	inline uint8_t GetFifoStatus(void);
	inline void SoftReset(void);
	inline void ResetFifo(void);
	inline uint8_t GetChipID(void);
	void GetCalibParam(void);
	void GetRawTemp(void);
	void GetRawPressure(void);
	float GetTemperature(void);
	float GetPressure(void);
};

#endif

