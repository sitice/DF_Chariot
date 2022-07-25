#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "spi.h"
#include "ICM20602.hpp"
#include "DF_Communicate.hpp"

inline void ICM20602::DelayMs(uint16_t time)
{
	vTaskDelay(time);
}

ICM20602::ICM20602(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin) : _hspi(hspi) , _GPIO(GPIO), _GPIOPin(GPIOPin){}


//读写配置
void ICM20602::Write(uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);//CS拉低则为SPI模式
	SPI_Write_Read_Byte(_hspi,reg&0x7f);//发送寄存器地址+写命令
	SPI_Write_Read_Byte(_hspi,data);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
}

uint8_t ICM20602::Read(uint8_t reg)
{
	uint8_t data;
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);
	SPI_Write_Read_Byte(_hspi,reg|0x80);//发送寄存器地址+读命令
	data = SPI_Write_Read_Byte(_hspi,0xff);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
	return data;
}

void ICM20602::ReadNbyte(uint8_t reg,uint8_t len,uint8_t *buf)
{
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);
	SPI_Write_Read_Byte(_hspi,reg|0x80);//发送寄存器地址+读命令
	while(len--)
		*buf++ = SPI_Write_Read_Byte(_hspi,0xff);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
}

void ICM20602::Rest(void)
{
	Write(PWR_MGMT_1, 0x80);
	DelayMs(20);
}

uint8_t ICM20602::Init(void) //初始化
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _GPIOPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_GPIO, &GPIO_InitStruct);
	
	Write(PWR_MGMT_1, 0x80);	//复位传感器
	DelayMs(100);//等待复位完成
	Write(SMPLRT_DIV, 0x00);	//陀螺仪采样率，最快1kHZ
	DelayMs(10);
	Write(PWR_MGMT_1, 0x00);	//设置设备时钟源，陀螺仪Z轴  0x01
	//新加入的
	Write(PWR_MGMT_2, 0x00);//输出3轴数据//默认也输出
	DelayMs(10);
	Write(CONFIG, 0x04);   //低通滤波频率，0x03(42Hz) 0x04(20HZ)
	DelayMs(10);//
	//设置量程
	Write(GYRO_CONFIG, (3 << 3));//+-2000deg/s 0x1B
	DelayMs(10);
	Write(ACCEL_CONFIG, (2 << 3));//+-8G  0x1C
	//加速度计滤波配置 20hz
	//新增
	Write(ACCEL_CONFIG2,0x04);
	//因该深入研究具体多多少频率合适
	DelayMs(10);
    //FIFO 电源
	Write(0x1E, 0x00);//关闭低功耗
	DelayMs(10);
	//新增
	Write(0x23, 0x00);//关闭FIFO DMP解算才打开 
    DelayMs(10);
	if(Read(WHO_AM_I)!= 0x12) //如果地址 不正确
		return 0;
	return 1;
}
//下面是单独获取
#define  Acc_Read() ReadNbyte(0X3B,6,buffer) //读取加速度
#define  Gyro_Read() ReadNbyte(0x43,6,&buffer[6])  //读取角速度
void ICM20602::Updata(void) //读取数据
{
	static uint16_t nowCailbAccTime = 0;
	static uint16_t nowCailbGyroTime = 0;
	static AccOrigin_t cailbAccSum{
		.x = 0,
		.y = 0,
		.z = 0
	};
	static GyroOrigin_t cailbGyroSum{
		.x = 0,
		.y = 0,
		.z = 0
	};
    uint8_t buffer[14];
    ReadNbyte(ACCEL_XOUT_H,14,buffer);
    originAcc.x = ((buffer[0] << 8) | buffer[1] );
    originAcc.y = ((buffer[2] << 8) | buffer[3] );
    originAcc.z = ((buffer[4] << 8) | buffer[5] );
    originGyro.x =((buffer[8] << 8) | buffer[9]) ;
    originGyro.y =((buffer[10] << 8)| buffer[11] );
    originGyro.z = ((buffer[12] << 8) | buffer[13]);
	if(isCailbAcc)
	{
		cailbAccSum.x += originAcc.x;
		cailbAccSum.y += originAcc.y;
		cailbAccSum.z += originAcc.z;
		nowCailbAccTime++;
		if(nowCailbAccTime >= calibrationAccTime)
		{
			isCailbAcc = false;
			nowCailbAccTime = 0;
			calibrationAccData.x = cailbAccSum.x / 1000.0f;
			calibrationAccData.y = cailbAccSum.y / 1000.0f;
			calibrationAccData.z = cailbAccSum.z / 1000.0f;
		}
	}
	if(isCailbGyro)
	{
		cailbGyroSum.x += originGyro.x;
		cailbGyroSum.y += originGyro.y;
		cailbGyroSum.z += originGyro.z;
		nowCailbGyroTime++;
		if(nowCailbGyroTime >= calibrationGyroTime)
		{
			isCailbGyro = false;
			nowCailbGyroTime = 0;
			calibrationGyroData.x = cailbGyroSum.x / 1000.0f;
			calibrationGyroData.y = cailbGyroSum.y / 1000.0f;
			calibrationGyroData.z = cailbGyroSum.z / 1000.0f;
		}
	}
//	acc.x = originAcc.x - calibrationAccData.x;
//    acc.y = originAcc.y - calibrationAccData.y;
//    acc.z = originAcc.z - calibrationAccData.z - 4096;
//    gyro.x = originGyro.x - calibrationGyroData.x;
//    gyro.y = originGyro.y - calibrationGyroData.y;
//    gyro.z = originGyro.z - calibrationGyroData.z;
		acc.x = originAcc.x;
    acc.y = originAcc.y;
    acc.z = originAcc.z ;
    gyro.x = originGyro.x;
    gyro.y = originGyro.y;
    gyro.z = originGyro.z;
	
	//sendSenser(calibrationGyroData.x*100, calibrationGyroData.y*100, calibrationGyroData.z*100 , 1); 
}

float ICM20602::GetTemperature(void)
{
	int16_t temp_adc;
	uint8_t buf[2];
	ReadNbyte(TEMP_OUT_H,2,buf);
	temp_adc = (buf[0]<<8)+buf[1];
	return (25.0f + (float)temp_adc/326.8f);
}