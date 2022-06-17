#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "spi.h"
#include "ICM20602.hpp"
//========ICM20602寄存器地址========================
/********************************************
*复位后所有寄存器地址都为0，除了
*register 26  CONFIG				= 0x80
*register 107 Power Management 1 	= 0x41
*register 117 WHO_AM_I 				= 0x12
*********************************************/
//陀螺仪温度补偿
#define	ICM20_XG_OFFS_TC_H				0x04
#define	ICM20_XG_OFFS_TC_L				0x05
#define	ICM20_YG_OFFS_TC_H				0x07
#define	ICM20_YG_OFFS_TC_L				0x08
#define	ICM20_ZG_OFFS_TC_H				0x0A
#define	ICM20_ZG_OFFS_TC_L				0x0B
//加速度自检输出(出产时设置，用于与用户的自检输出值比较）
#define	ICM20_SELF_TEST_X_ACCEL			0x0D
#define	ICM20_SELF_TEST_Y_ACCEL			0x0E
#define	ICM20_SELF_TEST_Z_ACCEL			0x0F
//陀螺仪静态偏移
#define	ICM20_XG_OFFS_USRH				0x13
#define	ICM20_XG_OFFS_USRL				0x14
#define	ICM20_YG_OFFS_USRH				0x15
#define	ICM20_YG_OFFS_USRL				0x16
#define	ICM20_ZG_OFFS_USRH				0x17
#define	ICM20_ZG_OFFS_USRL				0x18

#define	ICM20_SMPLRT_DIV				0x19//陀螺仪采样率，典型值：0x07(125Hz)
#define	ICM20_CONFIG					0x1A
#define	ICM20_GYRO_CONFIG				0x1B
#define	ICM20_ACCEL_CONFIG				0x1C
#define	ICM20_ACCEL_CONFIG2				0x1D
#define	ICM20_LP_MODE_CFG				0x1E

//运动唤醒加速度阈值
#define	ICM20_ACCEL_WOM_X_THR			0x20
#define	ICM20_ACCEL_WOM_Y_THR			0x21
#define	ICM20_ACCEL_WOM_Z_THR			0x22


#define	ICM20_FIFO_EN					0x23
#define	ICM20_FSYNC_INT					0x36
#define	ICM20_INT_PIN_CFG				0x37
//#define	ICM20_INT_ENABLE				0x38
#define	ICM20_FIFO_WM_INT_STATUS		0x39
#define	ICM20_INT_STATUS				0x3A

//加速度输出
#define	ICM20_ACCEL_XOUT_H				0x3B
#define	ICM20_ACCEL_XOUT_L				0x3C
#define	ICM20_ACCEL_YOUT_H				0x3D
#define	ICM20_ACCEL_YOUT_L				0x3E
#define	ICM20_ACCEL_ZOUT_H				0x3F
#define	ICM20_ACCEL_ZOUT_L				0x40
//温度输出
#define	ICM20_TEMP_OUT_H				0x41
#define	ICM20_TEMP_OUT_L				0x42
//角速度输出
#define	ICM20_GYRO_XOUT_H				0x43
#define	ICM20_GYRO_XOUT_L				0x44
#define	ICM20_GYRO_YOUT_H				0x45
#define	ICM20_GYRO_YOUT_L				0x46
#define	ICM20_GYRO_ZOUT_H				0x47
#define	ICM20_GYRO_ZOUT_L				0x48
//陀螺仪自检输出
#define	ICM20_SELF_TEST_X_GYRO			0x50
#define	ICM20_SELF_TEST_Y_GYRO			0x51
#define	ICM20_SELF_TEST_Z_GYRO			0x52

#define	ICM20_FIFO_WM_TH1				0x60
#define	ICM20_FIFO_WM_TH2				0x61
#define	ICM20_SIGNAL_PATH_RESET			0x68
#define	ICM20_ACCEL_INTEL_CTRL 			0x69
#define	ICM20_USER_CTRL					0x6A
//电源控制
#define	ICM20_PWR_MGMT_1				0x6B //电源管理，典型值：0x00(正常启用)
#define	ICM20_PWR_MGMT_2				0x6C

#define	ICM20_I2C_IF					0x70
#define	ICM20_FIFO_COUNTH				0x72
#define	ICM20_FIFO_COUNTL				0x73
#define	ICM20_FIFO_R_W					0x74

#define	ICM20_WHO_AM_I 					0x75
//加速度静态偏移
#define	ICM20_XA_OFFSET_H				0x77
#define	ICM20_XA_OFFSET_L				0x78
#define	ICM20_YA_OFFSET_H				0x7A
#define	ICM20_YA_OFFSET_L				0x7B
#define	ICM20_ZA_OFFSET_H				0x7D
#define	ICM20_ZA_OFFSET_L 				0x7E
//===========================================================

#define ICM20602_ADDRESS	0xD2

#define CS_ICM  PBout(12)

inline void ICM20602::DelayMs(uint16_t time)
{
	HAL_Delay(time);
}

ICM20602::ICM20602(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *GPIO,
	uint16_t GPIOPin) : _hspi(hspi) , _GPIO(GPIO), _GPIOPin(GPIOPin)
{
	if(GPIO == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(GPIO == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(GPIO == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(GPIO == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if(GPIO == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	state = Init();
}

inline uint8_t ICM20602::SPI_RW(uint8_t data)
{
	uint8_t re_data;
	HAL_SPI_TransmitReceive(_hspi,&data,&re_data,1,0xffff);
	return re_data;
}

//读写配置
void ICM20602::Write(uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);//CS拉低则为SPI模式
	SPI_RW(reg&0x7f);//发送寄存器地址+写命令
	SPI_RW(data);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
}

uint8_t ICM20602::Read(uint8_t reg)
{
	uint8_t data;
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);
	SPI_RW(reg|0x80);//发送寄存器地址+读命令
	data = SPI_RW(0xff);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
	return data;
}

void ICM20602::ReadNbyte(uint8_t reg,uint8_t len,uint8_t *buf)
{
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);
	SPI_RW(reg|0x80);//发送寄存器地址+读命令
	while(len--)
		*buf++ = SPI_RW(0xff);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
}

void ICM20602::Rest(void)
{
	Write(ICM20_PWR_MGMT_1, 0x80);
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
	
	Write(ICM20_PWR_MGMT_1, 0x80);	//复位传感器
	DelayMs(100);//等待复位完成
	Write(  ICM20_SMPLRT_DIV, 0x00);	//陀螺仪采样率，最快1kHZ
	DelayMs(10);
	Write(  ICM20_PWR_MGMT_1, 0x00);	//设置设备时钟源，陀螺仪Z轴  0x01
	//新加入的
	Write(  ICM20_PWR_MGMT_2, 0x00);//输出3轴数据//默认也输出
	DelayMs(10);
	Write(  ICM20_CONFIG, 0x04);   //低通滤波频率，0x03(42Hz) 0x04(20HZ)
	DelayMs(10);//
	//设置量程
	Write(  ICM20_GYRO_CONFIG, (3 << 3));//+-2000deg/s 0x1B
	DelayMs(10);
	Write(  ICM20_ACCEL_CONFIG, (2 << 3));//+-8G  0x1C
	//加速度计滤波配置 20hz
	//新增
	Write(  ICM20_ACCEL_CONFIG2,0x04);
	//因该深入研究具体多多少频率合适
	DelayMs(10);
    //FIFO 电源
	Write(  0x1E, 0x00);//关闭低功耗
	DelayMs(10);
	//新增
	Write(  0x23, 0x00);//关闭FIFO DMP解算才打开 
    DelayMs(10);
	if(Read(  ICM20_WHO_AM_I)!= 0x12) //如果地址 不正确
		return 0;
	return 1;
}
//下面是单独获取
#define  Acc_Read() ReadNbyte(0X3B,6,buffer) //读取加速度
#define  Gyro_Read() ReadNbyte(0x43,6,&buffer[6])  //读取角速度
void ICM20602::Updata(void) //读取数据
{
    uint8_t buffer[14];
    ReadNbyte(ICM20_ACCEL_XOUT_H,14,buffer);
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
	ReadNbyte(ICM20_TEMP_OUT_H,2,buf);
	temp_adc = (buf[0]<<8)+buf[1];
	return (25.0f + (float)temp_adc/326.8f);
}