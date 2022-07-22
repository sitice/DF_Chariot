#ifndef __ICM20602_HPP__
#define __ICM20602_HPP__

#define RAWDATA_TO_ANGLE	0.0610351f
#define RAWDATA_TO_RADIAN	0.0010653f

#include "main.h"
class ICM20602
{
public:
	struct AccOrigin_t //加速度数据结构体
	{
		uint16_t x;
		uint16_t y;
		uint16_t z;
	};
	struct GyroOrigin_t //角速度数据结构体
	{
		uint16_t x;
		uint16_t y;
		uint16_t z;
	};
	struct Acc_t //加速度数据结构体
	{
		float x;
		float y;
		float z;
	};
	struct Gyro_t //角速度数据结构体
	{
		float x;
		float y;
		float z;
	};
private: 
/* Internal variables -----------------------------------------------------------*/
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_GPIO;
	uint16_t _GPIOPin;
	AccOrigin_t originAcc;
	GyroOrigin_t originGyro;
	Acc_t acc;
	Gyro_t gyro;
	uint16_t calibrationAllTime;
	uint16_t calibrationAccTime;
	uint16_t calibrationGyroTime;
	bool isCailbGyro = false;
	bool isCailbAcc = false;
	Acc_t calibrationAccData;
	Gyro_t calibrationGyroData;

/* Internal function -----------------------------------------------------------*/
	void Write(uint8_t reg,uint8_t data);
	uint8_t Read(uint8_t reg);
	void ReadNbyte(uint8_t REG,uint8_t len,uint8_t *buf);
	inline void DelayMs(uint16_t time);

/* register address -----------------------------------------------------------*/
	/********************************************
	*复位后所有寄存器地址都为0，除了
	*register 26  CONFIG				 =  0x80
	*register 107 Power Management 1 	 =  0x41
	*register 117 WHO_AM_I 				 =  0x12
	*********************************************/
	//陀螺仪温度补偿
	const uint8_t XG_OFFS_TC_H = 0x04;
	const uint8_t XG_OFFS_TC_L = 0x05;
	const uint8_t YG_OFFS_TC_H = 0x07;
	const uint8_t YG_OFFS_TC_L = 0x08;
	const uint8_t ZG_OFFS_TC_H = 0x0A;
	const uint8_t ZG_OFFS_TC_L = 0x0B;
	//加速度自检输出(出产时设置，用于与用户的自检输出值比较）
	const uint8_t SELF_TEST_X_ACCEL = 0x0D;
	const uint8_t SELF_TEST_Y_ACCEL = 0x0E;
	const uint8_t SELF_TEST_Z_ACCEL = 0x0F;
	//陀螺仪静态偏移
	const uint8_t XG_OFFS_USRH = 0x13;
	const uint8_t XG_OFFS_USRL = 0x14;
	const uint8_t YG_OFFS_USRH = 0x15;
	const uint8_t YG_OFFS_USRL = 0x16;
	const uint8_t ZG_OFFS_USRH = 0x17;
	const uint8_t ZG_OFFS_USRL = 0x18;

	const uint8_t SMPLRT_DIV = 0x19;//陀螺仪采样率，典型值：0x07(125Hz)
	const uint8_t CONFIG = 0x1A;
	const uint8_t GYRO_CONFIG = 0x1B;
	const uint8_t ACCEL_CONFIG = 0x1C;
	const uint8_t ACCEL_CONFIG2 = 0x1D;
	const uint8_t LP_MODE_CFG = 0x1E;

	//运动唤醒加速度阈值
	const uint8_t ACCEL_WOM_X_THR = 0x20;
	const uint8_t ACCEL_WOM_Y_THR = 0x21;
	const uint8_t ACCEL_WOM_Z_THR = 0x22;


	const uint8_t FIFO_EN = 0x23;
	const uint8_t FSYNC_INT = 0x36;
	const uint8_t INT_PIN_CFG = 0x37;
	//const uint8_t	INT_ENABLE = 0x38;
	const uint8_t FIFO_WM_INT_STATUS = 0x39;
	const uint8_t INT_STATUS = 0x3A;

	//加速度输出
	const uint8_t ACCEL_XOUT_H = 0x3B;
	const uint8_t ACCEL_XOUT_L = 0x3C;
	const uint8_t ACCEL_YOUT_H = 0x3D;
	const uint8_t ACCEL_YOUT_L = 0x3E;
	const uint8_t ACCEL_ZOUT_H = 0x3F;
	const uint8_t ACCEL_ZOUT_L = 0x40;
	//温度输出
	const uint8_t TEMP_OUT_H = 0x41;
	const uint8_t TEMP_OUT_L = 0x42;
	//角速度输出
	const uint8_t GYRO_XOUT_H = 0x43;
	const uint8_t GYRO_XOUT_L = 0x44;
	const uint8_t GYRO_YOUT_H = 0x45;
	const uint8_t GYRO_YOUT_L = 0x46;
	const uint8_t GYRO_ZOUT_H = 0x47;
	const uint8_t GYRO_ZOUT_L = 0x48;
	//陀螺仪自检输出
	const uint8_t SELF_TEST_X_GYRO = 0x50;
	const uint8_t SELF_TEST_Y_GYRO = 0x51;
	const uint8_t SELF_TEST_Z_GYRO = 0x52;

	const uint8_t FIFO_WM_TH1 = 0x60;
	const uint8_t FIFO_WM_TH2 = 0x61;
	const uint8_t SIGNAL_PATH_RESET = 0x68;
	const uint8_t ACCEL_INTEL_CTRL = 0x69;
	const uint8_t USER_CTRL = 0x6A;
	//电源控制
	const uint8_t PWR_MGMT_1 = 0x6B; //电源管理，典型值：0x00(正常启用)
	const uint8_t PWR_MGMT_2 = 0x6C;

	const uint8_t I2C_IF = 0x70;
	const uint8_t FIFO_COUNTH = 0x72;
	const uint8_t FIFO_COUNTL = 0x73;
	const uint8_t FIFO_R_W = 0x74;

	const uint8_t	WHO_AM_I = 0x75;
	//加速度静态偏移
	const uint8_t XA_OFFSET_H = 0x77;
	const uint8_t XA_OFFSET_L = 0x78;
	const uint8_t YA_OFFSET_H = 0x7A;
	const uint8_t YA_OFFSET_L = 0x7B;
	const uint8_t ZA_OFFSET_H = 0x7D;
	const uint8_t ZA_OFFSET_L = 0x7E;
	const uint8_t ADDRESS = 0xD2;

public:
/**
  * @brief 构造函数
  * @param hspi spi句柄
  * @param CS_GPIO 片选GPIO
  * @param CS_Pin 片选引脚
  * @retval none
  */
	ICM20602(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin);
/**
  * @brief 初始化函数
  * @param none
  * @retval 返回0初始化失败，返回1初始化成功
  */
	uint8_t Init(void);
/**
  * @brief 复位
  * @param none
  * @retval 返回0初始化失败，返回1初始化成功
  */
	void Rest(void);
/**
  * @brief 更新数据
  * @param none
  * @retval none
  */
	void Updata(void);
/**
  * @brief 获取温度
  * @param none
  * @retval none
  */
	float GetTemperature(void);
/**
  * @brief 得到原始加速度数据
  * @note 需要先执行Updata函数获取最新的数据
  * @param none
  * @retval 加速度
  */
	AccOrigin_t GetOriginAccVal(){return originAcc;};
/**
  * @brief 得到原始角速度数据
  * @note 需要先执行Updata函数获取最新的数据
  * @param none
  * @retval 角速度
  */
	Gyro_t GetGyroVal(){return gyro;};
/**
  * @brief 得到校准后加速度数据
  * @note 需要先执行Updata函数获取最新的数据
  * @param none
  * @retval 加速度
  */
	Acc_t GetAccVal(){return acc;};
/**
  * @brief 得到校准后角速度数据
  * @note 需要先执行Updata函数获取最新的数据
  * @param none
  * @retval 角速度
  */
	GyroOrigin_t GetOrgionGyroVal(){return originGyro;};
/**
  * @brief 校准所有传感器
  * @note 在updata内获取数据，默认1000次
  * @param time 获取数据次数
  * @retval none
  */
	void CalibrationAll(uint16_t time = 1000)
	{
		calibrationAllTime = time;
		isCailbAcc = true;
		isCailbGyro = true;
	}
/**
  * @brief 校准陀螺仪
  * @note 在updata内获取数据，默认1000次
  * @param time 获取数据次数
  * @retval none
  */
	void CalibrationGyro(uint16_t time = 1000)
	{
		calibrationGyroTime = time;
		isCailbGyro = true;
	}
/**
  * @brief 校准加速度计
  * @note 在updata内获取数据，默认1000次
  * @param time 获取数据次数
  * @retval none
  */
	void CalibrationAcc(uint16_t time = 1000)
	{
		calibrationAccTime = time;
		isCailbAcc = true;
	}
/**
  * @brief 得到角速度
  * @note 需要先执行Updata函数获取最新的数据，单位弧度
  * @retval 陀螺仪数据
  */
	Gyro_t GetRadian(void)
	{
		Gyro_t gyro_;
		gyro_.x = gyro.x * RAWDATA_TO_RADIAN;
		gyro_.y = gyro.y * RAWDATA_TO_RADIAN;
		gyro_.z = gyro.z * RAWDATA_TO_RADIAN;
		return gyro_;
	}

/**
  * @brief 得到陀螺仪校准数据
  * @retval 校准数据
  */
	Gyro_t GetCalibrationGyroData(){return calibrationGyroData;}

/**
  * @brief 得到加速度校准数据
  * @retval 校准数据
  */
	Acc_t GetCalibrationAccData(){return calibrationAccData;}
/**
  * @brief 设置陀螺仪校准数据
  * @retval none
  */
	void SetCalibrationGyroData(Gyro_t &data){calibrationGyroData = data;}
/**
  * @brief 设置加速度校准数据
  * @retval none
  */
	void SetCalibrationAccData(Acc_t &data){calibrationAccData = data;}
};

#endif