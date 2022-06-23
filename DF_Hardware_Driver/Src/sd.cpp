#include "sd.hpp"
#include "spi.h"


void Read_Sectorx(uint32_t sec);
uint8_t Init(void);
uint8_t WaitReady(void);
uint8_t GetResponse(uint8_t Response);
uint32_t GetSectorCount(void);
uint8_t GetCID(uint8_t *cid_data);
uint8_t GetCSD(uint8_t *csd_data);
uint8_t ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);
uint8_t WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);

uint8_t WriteRead(uint8_t data)
{
	uint8_t recData;
	HAL_SPI_TransmitReceive(&hspi2,&data,&recData,1,0xffff);
	return recData;
}

uint8_t  Type=0;

uint8_t WaitReady(void)
{
	uint32_t t=0;
	uint8_t reg;
	for(t=0;t<0xffff;t++)
	{
		reg = WriteRead(0XFF);
		if(reg==0XFF)
			break;
	}
	if(t<0xffffff)
		return 0;
	else
		return 1;
}

void DisSelect(void)
{
	CS=1;
 	WriteRead(0xff);
}

uint8_t Select(void)
{
	CS=0;
	if(WaitReady()==0)
		return 0;
	DisSelect();
	return 1;
}

uint8_t SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	uint8_t r1=0;	
	uint8_t Retry=0; 
	DisSelect();

	if(Select())
	{
		return 0XFF;
	}

	WriteRead(cmd | 0x40);
	WriteRead(arg >> 24);
	WriteRead(arg >> 16);
	WriteRead(arg >> 8);
	WriteRead(arg); 
	WriteRead(crc); 
	
	if(cmd==CMD12)WriteRead(0xff);
	
	Retry=0X1F;
	do
	{
		r1=WriteRead(0xFF);
	}
	while((r1&0X80) && Retry--);

  return r1;
}	

uint8_t Init(void)
{
	uint8_t r1=0;
	u16 retry;
	uint8_t buf[4];
	u16 i;
	SPI_SetSpeed(&hspi2,SPI_BAUDRATEPRESCALER_256);
	for(i=0;i<10;i++)
	{
		WriteRead(0xff);
	}
	retry=20;
	do
	{

		r1=SendCmd(CMD0,0,0x95);
	}	
	while((r1!=0X01) && (retry--));

	Type=0;

	if(r1==0X01)
	{
		if(SendCmd(CMD8,0x1AA,0x87)==1)
		{
			for(i=0;i<4;i++)buf[i]=WriteRead(0XFF);	
			if(buf[2]==0X01&&buf[3]==0XAA)
			{
				retry=0XFFFE;
				do
				{
					SendCmd(CMD55,0,0X01);	
					r1=SendCmd(CMD41,0x40000000,0X01);
				}
				while(r1&&retry--);
				if(retry&&SendCmd(CMD58,0,0X01)==0)
				{
					for(i=0;i<4;i++)buf[i]=WriteRead(0XFF);
					if(buf[0]&0x40)
					{
						Type=TYPE_V2HC;   
					}
					else
					{
						Type=TYPE_V2; 
					}
				}
			}
		}
	}
	else
	{
		SendCmd(CMD55,0,0X01);		
		r1=SendCmd(CMD41,0,0X01);	
		if(r1<=1)
		{		
			Type=TYPE_V1;
			retry=0XFFFE;
			do 
			{
				SendCmd(CMD55,0,0X01);	
				r1=SendCmd(CMD41,0,0X01);
			}while(r1&&retry--);
		}
		else
		{
			Type=TYPE_MMC;
			retry=0XFFFE;
			do 
			{   
				r1=SendCmd(CMD1,0,0X01);
			}while(r1&&retry--);  
		}
		if(retry==0||SendCmd(CMD16,512,0X01)!=0)
		{
			Type=TYPE_ERR;
		}
	}
	DisSelect();
	SPI_SetSpeed(&hspi2,SPI_BAUDRATEPRESCALER_4);
	if(Type)
	{
		return 0;
	}
	else if(r1)
	{
		return r1; 
	}
	return 0xaa;
}


uint8_t GetResponse(uint8_t Response)
{
	u16 Count=0xFFFF; 
	while ((WriteRead(0XFF)!=Response)&&Count)
	{
		Count--;  
	}
	if (Count==0)
	{
		return MSD_RESPONSE_FAILURE;  
	}
	else
	{
		return MSD_RESPONSE_NO_ERROR;
	}
}


uint8_t RecvData(uint8_t*buf,u16 len)
{
	if(GetResponse(0xFE))
	{
		return 1;
	}
	while(len--)
	{
		*buf=WriteRead(0xFF);
		buf++;
	}
  WriteRead(0xFF);
  WriteRead(0xFF);  
  return 0;
}

uint8_t GetCSD(uint8_t *csd_data)
{
	uint8_t r1;	 
	r1=SendCmd(CMD9,0,0x01);
	if(r1==0)
	{
		r1=RecvData(csd_data, 16);
	}
	DisSelect();
	if(r1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}  


uint32_t GetSectorCount(void)
{
    uint8_t csd[16];
    uint32_t Capacity;  
    uint8_t n;
		u16 csize;
    if(GetCSD(csd)!=0)
		{
			return 0;	    
		}
    if((csd[0]&0xC0)==0x40)	 
    {	
			csize = csd[9] + ((u16)csd[8] << 8) + 1;
			Capacity = (uint32_t)csize << 10;
    }

		else
    {	
			n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
			csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;

			Capacity= (uint32_t)csize << (n - 9);  
    }
    return Capacity;
}





uint8_t ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt)
{
	uint8_t r1;
	if(Type != TYPE_V2HC)
	{
		sector <<= 9;
	}
	if(cnt == 1)
	{
		r1=SendCmd(CMD17,sector,0X01);
		if(r1 == 0)
		{
			r1=RecvData(buf,512);  
		}
	}
	else
	{
		r1 = SendCmd(CMD18,sector,0X01);
		do
		{
			r1=RecvData(buf,512);
			buf += 512;  
		}
		while(--cnt && r1 == 0); 	
		SendCmd(CMD12,0,0X01);	
	}   
	DisSelect();
	return r1;
}



void Read_Sectorx(uint32_t sec)
{
	uint8_t buf[512];
	u16 i;
	
	if(ReadDisk(buf,sec,1)==0)	
	{	
		
	}
}

 
uint8_t GetCID(uint8_t *cid_data)
{
  uint8_t r1;	   
  r1=SendCmd(CMD10,0,0x01);
  if(r1==0x00)
	{
		r1=RecvData(cid_data,16);
  }
	DisSelect();
	if(r1)
		return 1;
	else
		return 0;
}


uint8_t SendBlock(uint8_t*buf,uint8_t cmd)
{	
	u16 t; 
	if(WaitReady())
	{
		return 1;
	}
	WriteRead(cmd);
	if(cmd!=0XFD)
	{
		for(t=0;t<512;t++)
		{
			WriteRead(buf[t]);
		}
	  WriteRead(0xFF);
	  WriteRead(0xFF);
		t=WriteRead(0xFF);
		if((t&0x1F)!=0x05)
		{
			return 2;
		}
	} 
	return 0;
}





uint8_t WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt)
{
	uint8_t r1;
	if(Type!=TYPE_V2HC)
	{
		sector *= 512;
	}
	if(cnt==1)
	{
		r1=SendCmd(CMD24,sector,0X01);

		if(r1==0)
		{ 
			r1=SendBlock(buf,0xFE);
		}
	}
	else
	{
		if(Type!=TYPE_MMC)
		{
			SendCmd(CMD55,0,0X01);
			SendCmd(CMD23,cnt,0X01);
		}
 		r1=SendCmd(CMD25,sector,0X01);
		if(r1==0)
		{
			do
			{
				r1=SendBlock(buf,0xFC);
				buf += 512;  
			}
			while(--cnt && r1==0);
			r1=SendBlock(0,0xFD);
		}
	}   
	DisSelect();
	return r1;
}	





