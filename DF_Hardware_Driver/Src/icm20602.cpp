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


//��д����
void ICM20602::Write(uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);//CS������ΪSPIģʽ
	SPI_Write_Read_Byte(_hspi,reg&0x7f);//���ͼĴ�����ַ+д����
	SPI_Write_Read_Byte(_hspi,data);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
}

uint8_t ICM20602::Read(uint8_t reg)
{
	uint8_t data;
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);
	SPI_Write_Read_Byte(_hspi,reg|0x80);//���ͼĴ�����ַ+������
	data = SPI_Write_Read_Byte(_hspi,0xff);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
	return data;
}

void ICM20602::ReadNbyte(uint8_t reg,uint8_t len,uint8_t *buf)
{
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_RESET);
	SPI_Write_Read_Byte(_hspi,reg|0x80);//���ͼĴ�����ַ+������
	while(len--)
		*buf++ = SPI_Write_Read_Byte(_hspi,0xff);
	HAL_GPIO_WritePin(_GPIO,_GPIOPin,GPIO_PIN_SET);
}

void ICM20602::Rest(void)
{
	Write(PWR_MGMT_1, 0x80);
	DelayMs(20);
}

uint8_t ICM20602::Init(void) //��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _GPIOPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_GPIO, &GPIO_InitStruct);
	
	Write(PWR_MGMT_1, 0x80);	//��λ������
	DelayMs(100);//�ȴ���λ���
	Write(SMPLRT_DIV, 0x00);	//�����ǲ����ʣ����1kHZ
	DelayMs(10);
	Write(PWR_MGMT_1, 0x00);	//�����豸ʱ��Դ��������Z��  0x01
	//�¼����
	Write(PWR_MGMT_2, 0x00);//���3������//Ĭ��Ҳ���
	DelayMs(10);
	Write(CONFIG, 0x04);   //��ͨ�˲�Ƶ�ʣ�0x03(42Hz) 0x04(20HZ)
	DelayMs(10);//
	//��������
	Write(GYRO_CONFIG, (3 << 3));//+-2000deg/s 0x1B
	DelayMs(10);
	Write(ACCEL_CONFIG, (2 << 3));//+-8G  0x1C
	//���ٶȼ��˲����� 20hz
	//����
	Write(ACCEL_CONFIG2,0x04);
	//��������о���������Ƶ�ʺ���
	DelayMs(10);
    //FIFO ��Դ
	Write(0x1E, 0x00);//�رյ͹���
	DelayMs(10);
	//����
	Write(0x23, 0x00);//�ر�FIFO DMP����Ŵ� 
    DelayMs(10);
	if(Read(WHO_AM_I)!= 0x12) //�����ַ ����ȷ
		return 0;
	return 1;
}
//�����ǵ�����ȡ
#define  Acc_Read() ReadNbyte(0X3B,6,buffer) //��ȡ���ٶ�
#define  Gyro_Read() ReadNbyte(0x43,6,&buffer[6])  //��ȡ���ٶ�
void ICM20602::Updata(void) //��ȡ����
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