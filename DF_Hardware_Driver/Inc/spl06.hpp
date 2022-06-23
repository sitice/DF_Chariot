#ifndef __SPL06_HPP__
#define __SPL06_HPP__

#include "main.h"

class SPL06
{
public:
/**
  * @brief 构造函数
  * @param hspi spi句柄
  * @param CS_GPIO 片选GPIO
  * @param CS_Pin 片选引脚
  * @retval none
  */
	SPL06(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *CS_GPIO,
	uint16_t CS_Pin): _hspi(hspi),_CS_GPIO(CS_GPIO),_CS_Pin(CS_Pin){};
/**
  * @brief 初始化
  * @retval 返回0失败，返回1成功
  */
	uint8_t Init(void);
/**
  * @brief 获取SPL06温度
  * @retval 温度
  */
	float GetRealTemperature(void);
	
/**
  * @brief 获取SPL06气压
  * @retval 气压
  */
	float GetRealPresure(void);

	struct Baro_t{
		float temperature;//温度
		float pressure;//气压
		float alti;//高度
	};
/**
  * @brief 更新数据
  * @retval Baro_t SPL06数据
  */
	Baro_t Updata();

private:
/* register address -----------------------------------------------------------*/
	const uint8_t PRESSURE_REG = 0X00;
	const uint8_t TEMP_REG = 0X03;
	const uint8_t PRS_CFG_REG = 0x06; //气压测量速率配置
	const uint8_t TMP_CFG_REG = 0x07; //温度测量速度配置
	const uint8_t MEAS_CFG_REG = 0x08; //测量配置与传感器配置
	const uint8_t CFG_REG = 0x09; //中断/FIFO/SPI线数等配置
	const uint8_t INT_STS_REG = 0X0A; //中断状态标志位
	const uint8_t FIFO_STS_REG = 0X0B; //FIFO状态
	const uint8_t RESET_REG = 0X0C;
	const uint8_t ID_REG = 0x0D;
	const uint8_t COEF_REG = 0x10;

	const uint8_t PRESSURE_RATE_1_TIMES = 0; //采样率 
	const uint8_t PRESSURE_RATE_2_TIMES = 1;
	const uint8_t PRESSURE_RATE_4_TIMES = 2;
	const uint8_t PRESSURE_RATE_8_TIMES = 3;
	const uint8_t PRESSURE_RATE_16_TIMES = 4;
	const uint8_t PRESSURE_RATE_32_TIMES = 5;
	const uint8_t PRESSURE_RATE_64_TIMES = 6;
	const uint8_t PRESSURE_RATE_128_TIMES = 7;

	const uint8_t TEMPERATURE_RATE_1_TIMES = 0; //采样率
	const uint8_t TEMPERATURE_RATE_2_TIMES = 1;
	const uint8_t TEMPERATURE_RATE_4_TIMES = 2;
	const uint8_t TEMPERATURE_RATE_8_TIMES = 3;
	const uint8_t TEMPERATURE_RATE_16_TIMES = 4;
	const uint8_t TEMPERATURE_RATE_32_TIMES = 5;
	const uint8_t TEMPERATURE_RATE_64_TIMES = 6;
	const uint8_t TEMPERATURE_RATE_128_TIMES = 7;
	const uint8_t TEMPERATURE_RATE_TMP_EXT_INTERNAL = 0;//集成电路上的温度计
	const uint8_t TEMPERATURE_RATE_TMP_EXT_EXTERNAL = 1;//传感器MEMS气压芯片上温度

	const uint8_t MEAS_CFG_COEF_RDY = 0X80;// 传感器内部校准值可读，在启动后
	const uint8_t MEAS_CFG_SENSOR_RDY = 0X40;// 传感器已初始化完成，在启动后
	const uint8_t MEAS_CFG_TMP_RDY = 0x20;//温度值已经准备就绪，可以进行读取，该标志位读取后自动清0
	const uint8_t MEAS_CFG_PRS_RDY = 0x10;//气压值已经准备就绪，可以进行读取，该标志位
	const uint8_t MEAS_CFG_MEAS_CTR_STANDBY = 0;//模式配置 挂起模式
	const uint8_t MEAS_CFG_MEAS_CTR_COMMAND_PRS = 0x01;//模式配置 命令模式下启动气压采集
	const uint8_t MEAS_CFG_MEAS_CTR_COMMAND_TMP = 0x02;//模式配置 命令模式下启动温度采集
	const uint8_t MEAS_CFG_MEAS_CTR_BACKGROUND_PRS = 0x05;//模式配置 后台模式只读取气压值
	const uint8_t MEAS_CFG_MEAS_CTR_BACKGROUND_TMP = 0X06;//模式配置 后台模式只读取温度值
	const uint8_t MEAS_CFG_MEAS_CTR_BACKGROUND_PSR_TMP = 0X07;//模式配置 后台模式同时读取温度值和气压值

	const uint8_t CFG_INT_LEVEL_ACTIVE_LOW = 0;//中断低电平有效
	const uint8_t CFG_INT_LEVEL_ACTIVE_HIGH = 1;//中断高电平有效
	const uint8_t CFG_INT_FIFO = 0X40;//当FIFO满使能中断 
	const uint8_t CFG_INT_PRS = 0X20;//当气压计读取完毕使能中断 
	const uint8_t CFG_INT_TMP = 0X10;//当温度读取完毕使能中断 
	const uint8_t CFG_T_SHIFT = 0X08;//允许数据被覆盖，可以进行下一笔采集
	const uint8_t CFG_P_SHIFT = 0X04;//允许数据被覆盖，可以进行下一笔采集
	const uint8_t CFG_FIF = 0X02;//使能FIFO
	const uint8_t CFG_SPI_3_WIRE = 1;//3线SPI
	const uint8_t CFG_SPI_4_WIRE = 0;//4线SPI

	const uint8_t INT_STS_FIFO_FULL = 0X04;//FIFO满中断状态
	const uint8_t INT_STS_FIFO_TMP = 0X02;//温度测量完成标志位
	const uint8_t INT_STS_FIFO_PRS = 0X01;//气压测量完成标志位

	const uint8_t FIFO_STS_FULL = 0X02;//FIFO满
	const uint8_t FIFO_STS_EMPTY = 0X01;//FIFO空满

	const uint8_t RESET_FIFO_FLUSH = 0X80; //FIFO清0
	const uint8_t RESET_SOFT = 0X09;//软件复位

	const uint8_t PRODUCT_ID = 0X10;//产品ID

	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_CS_GPIO;
	uint16_t _CS_Pin;
	
/* Internal variables -----------------------------------------------------------*/
	struct {	//内部出厂校准数据
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
		uint8_t chip_id;// 芯片ID
		int32_t i32rawPressure;// 原始气压数据
		int32_t i32rawTemperature;// 原始温度数据
		int32_t i32kP;// 气压补偿参数
		int32_t i32kT;// 温度补偿参数
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

