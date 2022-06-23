#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "spi.h"
#include "ICM20602.hpp"


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
    uint8_t buffer[14];
    ReadNbyte(ACCEL_XOUT_H,14,buffer);
    acc.x = ((buffer[0] << 8) | buffer[1] );
    acc.y = ((buffer[2] << 8) | buffer[3] );
    acc.z = ((buffer[4] << 8) | buffer[5] );
    gyro.x =((buffer[8] << 8) | buffer[9]) ;
    gyro.y =((buffer[10] << 8)| buffer[11] );
    gyro.z = ((buffer[12] << 8) | buffer[13]);
}

float ICM20602::GetTemperature(void)
{
	int16_t temp_adc;
	uint8_t buf[2];
	ReadNbyte(TEMP_OUT_H,2,buf);
	temp_adc = (buf[0]<<8)+buf[1];
	return (25.0f + (float)temp_adc/326.8f);
}