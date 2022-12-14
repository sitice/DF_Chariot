#include "sd.hpp"
#include "spi.h"

//取消选择,释放SPI总线
void SD::DisSelect(void)
{
    HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_SET);
    SPI_Write_Read_Byte(_hspi,0xff);//提供额外的8个时钟
}

/**
 * 选中SD卡并等待卡准备好
 * @return  0：成功  1：失败
 */
uint8_t SD::Select(void)
{
    HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_RESET);
    if (WaitReady() == 0)return 0; //等待成功
    DisSelect();
    return 1;//等待失败
}

/**
 * 等待SD卡准备好
 * @return  0：成功  other：失败
 */
uint8_t SD::WaitReady(void)
{
    uint32_t t = 0;
    do
    {
        if (SPI_Write_Read_Byte(_hspi,0XFF) == 0XFF)return 0; //OK
        t++;
    }
    while (t < 0XFFFFFF); //等待
    return 1;
}

/**
 * 等待SD卡回应
 * @param  Response 要得到的回应值
 * @return          0：成功  other：失败
 */
uint8_t SD::GetResponse(uint8_t Response)
{
    uint16_t Count = 0xFFF; //等待次数
    while ((SPI_Write_Read_Byte(_hspi,0XFF) != Response) && Count)Count--; //等待得到准确的回应
    if (Count == 0)return RESPONSE_FAILURE; //得到回应失败
    else return RESPONSE_NO_ERROR;//正确回应
}

/**
 * 接收来自SD卡的一包数据
 * @param  buf 存放接收的数据
 * @param  len 接收的数据长度
 * @return     0：成功  other：失败
 */
uint8_t SD::RecvData(uint8_t*buf, uint16_t len)
{
    if (GetResponse(0xFE))return 1; //等待SD卡发回数据起始令牌0xFE
    while (len--) //开始接收数据
    {
        *buf = SPI_Write_Read_Byte(_hspi,0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SPI_Write_Read_Byte(_hspi,0xFF);
    SPI_Write_Read_Byte(_hspi,0xFF);
    return 0;//读取成功
}
//向sd卡写入一个数据包的内容 512字节
//buf:数据缓存区
//cmd:指令
//返回值:0,成功;其他,失败;

/**
 * 向SD卡写入一扇区数据
 * @param  buf 待写入的数据，size=512
 * @param  cmd 指令
 * @return     0：成功  other：失败
 */
uint8_t SD::SendBlock(uint8_t*buf, uint8_t cmd)
{
    uint16_t t;
    if (WaitReady())return 1; //等待准备失效
    SPI_Write_Read_Byte(_hspi,cmd);
    if (cmd != 0XFD) //不是结束指令
    {
        for (t = 0; t < 512; t++)SPI_Write_Read_Byte(_hspi,buf[t]); //提高速度,减少函数传参时间
        SPI_Write_Read_Byte(_hspi,0xFF);//忽略crc
        SPI_Write_Read_Byte(_hspi,0xFF);
        t = SPI_Write_Read_Byte(_hspi,0xFF); //接收响应
        if ((t & 0x1F) != 0x05)return 2; //响应错误
    }
    return 0;//写入成功
}

/**
 * 向SD卡发送命令
 * @param  cmd 待发送的命令
 * @param  arg 参数
 * @param  crc crc校验值
 * @return     SD卡返回的响应值
 */
uint8_t SD::SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t r1;
    uint8_t Retry = 0;
    DisSelect();//取消上次片选
    if (Select())return 0XFF; //片选失效
    //发送
    SPI_Write_Read_Byte(_hspi,cmd | 0x40);//分别写入命令
    SPI_Write_Read_Byte(_hspi,arg >> 24);
    SPI_Write_Read_Byte(_hspi,arg >> 16);
    SPI_Write_Read_Byte(_hspi,arg >> 8);
    SPI_Write_Read_Byte(_hspi,arg);
    SPI_Write_Read_Byte(_hspi,crc);
    if (cmd == CMD12)SPI_Write_Read_Byte(_hspi,0xff); //Skip a stuff byte when stop reading
    //等待响应，或超时退出
    Retry = 0X1F;
    do
    {
        r1 = SPI_Write_Read_Byte(_hspi,0xFF);
    }
    while ((r1 & 0X80) && Retry--);
    //返回状态值
    return r1;
}

/**
 * 查询SD卡的CID信息，包括制造商信息
 * @param  cid_data 存放CID数据，至少16字节
 * @return          0：成功  1：失败
 */
uint8_t SD::GetCID(uint8_t *cid_data)
{
    uint8_t r1;
    //发CMD10命令，读CID
    r1 = SendCmd(CMD10, 0, 0x01);
    if (r1 == 0x00)
    {
        r1 = RecvData(cid_data, 16); //接收16个字节的数据
    }
    DisSelect();//取消片选
    if (r1)return 1;
    else return 0;
}

/**
 * 查询SD卡CID信息，包括制造商信息
 * @param  csd_data 存放CID信息，至少16字节
 * @return          0：成功  1：失败
 */
uint8_t SD::GetCSD(uint8_t *csd_data)
{
    uint8_t r1;
    r1 = SendCmd(CMD9, 0, 0x01); //发CMD9命令，读CSD
    if (r1 == 0)
    {
        r1 = RecvData(csd_data, 16); //接收16个字节的数据
    }
    DisSelect();//取消片选
    if (r1)return 1;
    else return 0;
}
/**
 * 获取SD卡扇区数
 * @return  0：获取出错  other：SD卡扇区数
 */
uint32_t SD::GetSectorCount(void)
{
    uint8_t csd[16];
    uint32_t Capacity;
    uint8_t n;
    uint16_t csize;
    //取CSD信息，如果期间出错，返回0
    if (GetCSD(csd) != 0) return 0;
    //如果为SDHC卡，按照下面方式计算
    if ((csd[0] & 0xC0) == 0x40)	 //V2.00的卡
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
        Capacity = (uint32_t)csize << 10;//得到扇区数
    }
    else //V1.XX的卡
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        Capacity = (uint32_t)csize << (n - 9); //得到扇区数
    }
    return Capacity;
}

/// SD卡进入闲置状态
uint8_t SD::Idle_Sta(void)
{
    uint16_t i;
    uint8_t retry;
    for (i = 0; i < 0xf00; i++); //纯延时，等待SD卡上电完成
    //先产生>74个脉冲，让SD卡自己初始化完成
    for (i = 0; i < 10; i++)SPI_Write_Read_Byte(_hspi,0xFF);
    //-----------------SD卡复位到idle开始-----------------
    //循环连续发送CMD0，直到SD卡返回0x01,进入IDLE状态
    //超时则直接退出
    retry = 0;
    do
    {
        //发送CMD0，让SD卡进入IDLE状态
        i = SendCmd(CMD0, 0, 0x95);
        retry++;
    }
    while ((i != 0x01) && (retry < 200));
    //跳出循环后，检查原因：初始化成功？or 重试超时？
    if (retry == 200)return 1; //失败
    return 0;//成功
}
/// 初始化SD卡
uint8_t SD::Init(void)
{
    uint8_t r1;      // 存放SD卡的返回值
    uint16_t retry;  // 用来进行超时计数
    uint8_t buf[4];
    uint16_t i;

	SPI_Write_Read_Byte(_hspi,0xFF);
    HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_SET);
	
    for (i = 0; i < 10; i++)SPI_Write_Read_Byte(_hspi,0XFF); //发送最少74个脉冲
    retry = 20;
    do
    {
        r1 = SendCmd(CMD0, 0, 0x95); //进入IDLE状态
    }
    while ((r1 != 0X01) && retry--);
    Type = 0; //默认无卡
    if (r1 == 0X01)
    {
        if (SendCmd(CMD8, 0x1AA, 0x87) == 1) //SD V2.0
        {
            for (i = 0; i < 4; i++)buf[i] = SPI_Write_Read_Byte(_hspi,0XFF);	//Get trailing return value of R7 resp
            if (buf[2] == 0X01 && buf[3] == 0XAA) //卡是否支持2.7~3.6V
            {
                retry = 0XFFFE;
                do
                {
                    SendCmd(CMD55, 0, 0X01);	//发送CMD55
                    r1 = SendCmd(CMD41, 0x40000000, 0X01); //发送CMD41
                }
                while (r1 && retry--);
                if (retry && SendCmd(CMD58, 0, 0X01) == 0) //鉴别SD2.0卡版本开始
                {
                    for (i = 0; i < 4; i++)buf[i] = SPI_Write_Read_Byte(_hspi,0XFF); //得到OCR值
                    if (buf[0] & 0x40)Type = TYPE_V2HC; //检查CCS
                    else Type = TYPE_V2;
                }
            }
        }
        else //SD V1.x/ MMC	V3
        {
            SendCmd(CMD55, 0, 0X01);		//发送CMD55
            r1 = SendCmd(CMD41, 0, 0X01);	//发送CMD41
            if (r1 <= 1)
            {
                Type = TYPE_V1;
                retry = 0XFFFE;
                do //等待退出IDLE模式
                {
                    SendCmd(CMD55, 0, 0X01);	//发送CMD55
                    r1 = SendCmd(CMD41, 0, 0X01); //发送CMD41
                }
                while (r1 && retry--);
            }
            else
            {
                Type = TYPE_MMC; //MMC V3
                retry = 0XFFFE;
                do //等待退出IDLE模式
                {
                    r1 = SendCmd(CMD1, 0, 0X01); //发送CMD1
                }
                while (r1 && retry--);
            }
            if (retry == 0 || SendCmd(CMD16, 512, 0X01) != 0)Type = TYPE_ERR; //错误的卡
        }
    }
    DisSelect();//取消片选
    if (Type)return 0;
    else if (r1)return r1;
    return 0xaa;//其他错误
}

/**
 * 读取SD卡一个扇区
 * @param  buf    存放读取的数据
 * @param  sector 扇区编号
 * @param  cnt    要读取的扇区个数
 * @return        0：成功  other：失败
 */
uint8_t SD::ReadDisk(uint8_t*buf, uint32_t sector, uint8_t cnt)
{
    uint8_t r1;
    if (Type != TYPE_V2HC)sector <<= 9; //转换为字节地址
    if (cnt == 1)
    {
        r1 = SendCmd(CMD17, sector, 0X01); //读命令
        if (r1 == 0) //指令发送成功
        {
            r1 = RecvData(buf, 512); //接收512个字节
        }
    }
    else
    {
        r1 = SendCmd(CMD18, sector, 0X01); //连续读命令
        do
        {
            r1 = RecvData(buf, 512); //接收512个字节
            buf += 512;
        }
        while (--cnt && r1 == 0);
        SendCmd(CMD12, 0, 0X01);	//发送停止命令
    }
    DisSelect();//取消片选
    return r1;//
}

/**
 * SD卡写一个扇区的数据
 * @param  buf    存放待写入的数据
 * @param  sector 扇区编号
 * @param  cnt    要写入的扇区个数
 * @return        0：成功  other：失败
 */
uint8_t SD::WriteDisk(uint8_t*buf, uint32_t sector, uint8_t cnt)
{
    uint8_t r1;
    if (Type != TYPE_V2HC)sector *= 512; //转换为字节地址
    if (cnt == 1)
    {
        r1 = SendCmd(CMD24, sector, 0X01); //读命令
        if (r1 == 0) //指令发送成功
        {
            r1 = SendBlock(buf, 0xFE); //写512个字节
        }
    }
    else
    {
        if (Type != TYPE_MMC)
        {
            SendCmd(CMD55, 0, 0X01);
            SendCmd(CMD23, cnt, 0X01); //发送指令
        }
        r1 = SendCmd(CMD25, sector, 0X01); //连续读命令
        if (r1 == 0)
        {
            do
            {
                r1 = SendBlock(buf, 0xFC); //接收512个字节
                buf += 512;
            }
            while (--cnt && r1 == 0);
            r1 = SendBlock(0, 0xFD); //接收512个字节
        }
    }
    DisSelect();//取消片选
    return r1;//
}




