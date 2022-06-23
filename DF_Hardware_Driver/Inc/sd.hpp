#ifndef __SD_HPP__
#define __SD_HPP__

#include "main.h"

class SD
{
public:
	
	uint8_t  Type = 0; //SD��������
/**
  * @brief ���캯��
  * @param hspi spi���
  * @param CS_GPIO ƬѡGPIO
  * @param CS_Pin Ƭѡ����
  * @retval none
  */
	SD(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *CS_GPIO,
	uint16_t CS_Pin) : _hspi(hspi),_CS_GPIO(CS_GPIO),_CS_Pin(CS_Pin){};
	
	uint8_t Init(void);							//��ʼ��
	uint8_t ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//����
	uint8_t WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//д��

private:
 /* register address -----------------------------------------------------------*/
	// SD��������С
	const uint16_t SECTOR_SIZE=512U;

	// SD�����Ͷ���
	const uint8_t TYPE_ERR=0X00;
	const uint8_t TYPE_MMC=0X01;
	const uint8_t TYPE_V1=0X02;
	const uint8_t TYPE_V2=0X04;
	const uint8_t TYPE_V2HC=0X06;

	// SD��ָ���
	const uint8_t CMD0=0;       //����λ
	const uint8_t CMD1=1;
	const uint8_t CMD8=8;       //����8 ��SEND_IF_COND
	const uint8_t CMD9=9;       //����9 ����CSD����
	const uint8_t CMD10=10;      //����10����CID����
	const uint8_t CMD12=12;      //����12��ֹͣ���ݴ���
	const uint8_t CMD16=16;      //����16������SectorSize Ӧ����0x00
	const uint8_t CMD17=17;      //����17����sector
	const uint8_t CMD18=18;      //����18����Multi sector
	const uint8_t CMD23=23;      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
	const uint8_t CMD24=24;      //����24��дsector
	const uint8_t CMD25=25;      //����25��дMulti sector
	const uint8_t CMD41=41;      //����41��Ӧ����0x00
	const uint8_t CMD55=55;      //����55��Ӧ����0x01
	const uint8_t CMD58=58;      //����58����OCR��Ϣ
	const uint8_t CMD59=59;      //����59��ʹ��/��ֹCRC��Ӧ����0x00

	//����д���Ӧ������
	const uint8_t DATA_OK=0x05;
	const uint8_t DATA_CRC_ERROR=0x0B;
	const uint8_t DATA_WRITE_ERROR=0x0D;
	const uint8_t DATA_OTHER_ERROR=0xFF;

	//SD����Ӧ�����
	const uint8_t RESPONSE_NO_ERROR=0x00;
	const uint8_t IN_IDLE_STATE=0x01;
	const uint8_t ERASE_RESET=0x02;
	const uint8_t ILLEGAL_COMMAND=0x04;
	const uint8_t COM_CRC_ERROR=0x08;
	const uint8_t ERASE_SEQUENCE_ERROR=0x10;
	const uint8_t ADDRESS_ERROR=0x20;
	const uint8_t PARAMETER_ERROR=0x40;
	const uint8_t RESPONSE_FAILURE=0xFF;

/* Internal variables -----------------------------------------------------------*/
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_CS_GPIO;
	uint16_t _CS_Pin;

/* Internal function -----------------------------------------------------------*/
	uint8_t SendBlock(uint8_t*buf, uint8_t cmd);
	uint8_t SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc);
	uint8_t Idle_Sta(void);
	uint8_t Select(void);
	void DisSelect(void);
	uint8_t RecvData(uint8_t*buf, uint16_t len);
	uint8_t WaitReady(void);							//�ȴ�SD��׼��
	uint8_t GetResponse(uint8_t Response);					//�����Ӧ
	uint32_t GetSectorCount(void);   					//��������
	uint8_t GetCID(uint8_t *cid_data);                     //��SD��CID
	uint8_t GetCSD(uint8_t *csd_data);                     //��SD��CSD
};


#endif