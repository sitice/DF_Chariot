#ifndef __SD_HPP__
#define __SD_HPP__

#include "main.h"

class SD
{
public:
	
	uint8_t  Type = 0; //SD卡的类型
/**
  * @brief 构造函数
  * @param hspi spi句柄
  * @param CS_GPIO 片选GPIO
  * @param CS_Pin 片选引脚
  * @retval none
  */
	SD(SPI_HandleTypeDef *hspi,
	GPIO_TypeDef *CS_GPIO,
	uint16_t CS_Pin) : _hspi(hspi),_CS_GPIO(CS_GPIO),_CS_Pin(CS_Pin){};
	
	uint8_t Init(void);							//初始化
	uint8_t ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//读块
	uint8_t WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//写块

private:
 /* register address -----------------------------------------------------------*/
	// SD卡扇区大小
	const uint16_t SECTOR_SIZE=512U;

	// SD卡类型定义
	const uint8_t TYPE_ERR=0X00;
	const uint8_t TYPE_MMC=0X01;
	const uint8_t TYPE_V1=0X02;
	const uint8_t TYPE_V2=0X04;
	const uint8_t TYPE_V2HC=0X06;

	// SD卡指令表
	const uint8_t CMD0=0;       //卡复位
	const uint8_t CMD1=1;
	const uint8_t CMD8=8;       //命令8 ，SEND_IF_COND
	const uint8_t CMD9=9;       //命令9 ，读CSD数据
	const uint8_t CMD10=10;      //命令10，读CID数据
	const uint8_t CMD12=12;      //命令12，停止数据传输
	const uint8_t CMD16=16;      //命令16，设置SectorSize 应返回0x00
	const uint8_t CMD17=17;      //命令17，读sector
	const uint8_t CMD18=18;      //命令18，读Multi sector
	const uint8_t CMD23=23;      //命令23，设置多sector写入前预先擦除N个block
	const uint8_t CMD24=24;      //命令24，写sector
	const uint8_t CMD25=25;      //命令25，写Multi sector
	const uint8_t CMD41=41;      //命令41，应返回0x00
	const uint8_t CMD55=55;      //命令55，应返回0x01
	const uint8_t CMD58=58;      //命令58，读OCR信息
	const uint8_t CMD59=59;      //命令59，使能/禁止CRC，应返回0x00

	//数据写入回应字意义
	const uint8_t DATA_OK=0x05;
	const uint8_t DATA_CRC_ERROR=0x0B;
	const uint8_t DATA_WRITE_ERROR=0x0D;
	const uint8_t DATA_OTHER_ERROR=0xFF;

	//SD卡回应标记字
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
	uint8_t WaitReady(void);							//等待SD卡准备
	uint8_t GetResponse(uint8_t Response);					//获得相应
	uint32_t GetSectorCount(void);   					//读扇区数
	uint8_t GetCID(uint8_t *cid_data);                     //读SD卡CID
	uint8_t GetCSD(uint8_t *csd_data);                     //读SD卡CSD
};


#endif