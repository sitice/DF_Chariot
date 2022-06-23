#include "ak8975.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "spi.h"

void AK8975::Write(uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(_GPIO, _GPIOPin, GPIO_PIN_RESET);		// 拉低，选中芯片

	SPI_Write_Read_Byte(_hspi,reg & 0x7f);//发送寄存器地址+写命令
	SPI_Write_Read_Byte(_hspi,data);
	
	HAL_GPIO_WritePin(_GPIO, _GPIOPin, GPIO_PIN_SET);	
}

uint8_t AK8975::Read(uint8_t reg)
{
	uint8_t data;
	HAL_GPIO_WritePin(_GPIO, _GPIOPin, GPIO_PIN_RESET);		// 拉低，选中芯片
	
	SPI_Write_Read_Byte(_hspi,reg | 0x80);				//发送寄存器地址+读命令
	data = SPI_Write_Read_Byte(_hspi,0xff);
	
	HAL_GPIO_WritePin(_GPIO, _GPIOPin, GPIO_PIN_SET);
	return data;
}

void AK8975::Restart(void)
{ 
	uint8_t mode = 0x11;
	Write(CONTROL, 0x11);//0x10 16位模式 | 0x01 单次测量模式  //开始转换，和读取数据之间必须间隔7ms以上的时间
} 


uint8_t AK8975::Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _GPIOPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_GPIO, &GPIO_InitStruct);
	
	uint8_t count = 5;
	
	uint8_t id;

	while (count--)
	{
		id = Read(ID_REG);

		if (id == ID)
		{
			Restart(); 			//    0x10 16位模式|0x01 单次测量模式		//14位 - 0.6uT/LSB      16位 - 0.15uT/LSB
			vTaskDelay(10); 		//延时等待磁力计可用
			
			return 1;
		}
		vTaskDelay(1);
	}
	return 0;
}	


AK8975::Mag_t AK8975::GetMagVal(void)
{	
	Mag_t mag;
	uint8_t BUF[2];

	BUF[0] = Read (XOUT_L);
	BUF[1] = Read (XOUT_H);
	mag.x = (int16_t)((BUF[1]<<8)|BUF[0]);

	BUF[0] = Read (YOUT_L);
	BUF[1] = Read (YOUT_H);
	mag.y = (int16_t)((BUF[1]<<8)|BUF[0]);

	BUF[0] = Read (ZOUT_L);
	BUF[1] = Read (ZOUT_H);
	mag.z = (int16_t)((BUF[1]<<8)|BUF[0]);

	Restart(); 
	return mag;
}