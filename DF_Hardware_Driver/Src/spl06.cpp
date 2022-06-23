#include "spl06.hpp"
#include "spi.h"

/* 
 * SPL06写指定寄存器数据
 */
void SPL06::Write(uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_RESET);// 拉低，选中芯片
	SPI_Write_Read_Byte(_hspi,reg & 0x7f);//发送寄存器地址+写命令
	SPI_Write_Read_Byte(_hspi,data);
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_SET);
}

/* 
 * SPL06读指定寄存器数据
 */
uint8_t SPL06::Read(uint8_t reg)
{
	uint8_t data;
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_RESET);		// 拉低，选中芯片
	SPI_Write_Read_Byte(_hspi,reg | 0x80);				//发送寄存器地址+读命令
	data = SPI_Write_Read_Byte(_hspi,0xff);
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_SET);
	return data;
}



void SPL06::PressureRateConfig(uint8_t background_rate,uint8_t oversamply)
{
		uint8_t data;
	
		data = (background_rate<<4)|oversamply;
		if(oversamply > PRESSURE_RATE_8_TIMES)//过采样次数大于EMPERATURE_RATE_8_TIMES，应当允许数据被新的数据覆盖，内部拥有气压和温度共32级的FIFO，在大于8次（也就是大于或等于16次过采样）的时候需要被新的数据覆盖，否则数据就会丢失
		{
				uint8_t data;
				data = Read(CFG_REG);//读取原寄存器值
				data |= 0X04;//P-SHIFT位置1
				Write(CFG_REG, data);//重新写回寄存器					
		}		
		if(oversamply == PRESSURE_RATE_2_TIMES)
			spl06.i32kP = 1572864;
        else if(oversamply == PRESSURE_RATE_4_TIMES)
			spl06.i32kP  = 3670016;
        else if(oversamply == PRESSURE_RATE_8_TIMES)
			spl06.i32kP  = 7864320;
        else if(oversamply == PRESSURE_RATE_16_TIMES)
			spl06.i32kP = 253952;
        else if(oversamply == PRESSURE_RATE_32_TIMES)
			spl06.i32kP = 516096;       
        else if(oversamply == PRESSURE_RATE_64_TIMES)
			spl06.i32kP = 1040384;           
        else if(oversamply == PRESSURE_RATE_128_TIMES)
			spl06.i32kP = 2088960;
        else
            spl06.i32kP = 524288;
		Write( PRS_CFG_REG, data);//写入配置
}


void SPL06::TemperatureRateConfig(uint8_t background_rate,uint8_t oversamply,uint8_t ext)
{
	uint8_t data;

	data = (ext<<7)|(background_rate<<4)|oversamply;
	if(oversamply > TEMPERATURE_RATE_8_TIMES)//过采样次数大于EMPERATURE_RATE_8_TIMES，应当允许数据被新的数据覆盖
	{
			uint8_t data;
			data = Read( CFG_REG);//读取原寄存器值
			data |= 0X08;//T-SHIFT位置1
			Write( CFG_REG, data);	//重新写回寄存器					
	}			
		if(oversamply == TEMPERATURE_RATE_2_TIMES)
				spl06.i32kT = 1572864;
		else if(oversamply == TEMPERATURE_RATE_4_TIMES) 
				spl06.i32kT  = 3670016;
		else if(oversamply == TEMPERATURE_RATE_8_TIMES)
				spl06.i32kT  = 7864320;
		else if(oversamply == TEMPERATURE_RATE_16_TIMES)
				spl06.i32kT = 253952;
		else if(oversamply == TEMPERATURE_RATE_32_TIMES)
				spl06.i32kT = 516096;       
		else if(oversamply == TEMPERATURE_RATE_64_TIMES)
				spl06.i32kT = 1040384;           
		else if(oversamply == TEMPERATURE_RATE_128_TIMES)
				spl06.i32kT = 2088960;
		else
				spl06.i32kT = 524288;
	Write( TMP_CFG_REG, data);//写入配置
}

//获取传感器数据就位状态//传感器就绪状态
inline uint8_t SPL06::GetMeasureStatus(void)
{
	return Read( MEAS_CFG_REG);
}
//设置读取模式+读取方式
inline void SPL06::SetMeasureMode(uint8_t mode)  //参数为模式值
{
	 Write( MEAS_CFG_REG,mode);
}
//启动命令模式读取温度值
inline void SPL06::StartTemperature(void)
{
    Write( MEAS_CFG_REG, MEAS_CFG_MEAS_CTR_COMMAND_TMP);
}
//启动命令模式读取气压值
inline void SPL06::StartPressure(void)
{
    Write(MEAS_CFG_REG, MEAS_CFG_MEAS_CTR_COMMAND_PRS);
}
//进入待机模式，进入后停止采集数据直到再次切换模式
inline void SPL06::EnterStandby(void)
{
    Write( MEAS_CFG_REG, MEAS_CFG_MEAS_CTR_STANDBY);
}

inline void SPL06::SetInterrupt(uint8_t interrupt,uint8_t type)//设置中断使能
{
	uint8_t data;
	data = Read(CFG_REG);
	if(type!=ENABLE )
		data &= ~interrupt;
	else
		data |= interrupt;	
	Write( CFG_REG,data);
}

inline void SPL06::SetSpiWire(uint8_t wire)						//设置SPI线数 //3线/4线SPI读取数据
{
	uint8_t data;
	data = Read(CFG_REG);
	data &= 0xf7;															//SPI线配置所在位清0
	data |= wire;
	Write( CFG_REG,data);
}

inline void SPL06::SetIntruptLevel(uint8_t level)			//设置中断有效电平//INT高电平有效或者低电平有效//level为0则低电平有效,为1则高电平有效
{
	uint8_t data;
	data = Read( CFG_REG);
	data &= 0x7f;//中断电平有效位清0
	data |= level<<7;
	Write( CFG_REG,data);
}


inline uint8_t SPL06::GetIntStatus(void)//读取传感器状态
{
	return Read( INT_STS_REG);
}


inline uint8_t SPL06::GetFifoStatus(void)
{
	return Read( FIFO_STS_REG);
}


inline void SPL06::SoftReset(void)//软件复位
{
	 Write(RESET_REG,RESET_SOFT);
}
inline void SPL06::ResetFifo(void)//软件清FIFO
{
	Write(RESET_REG,RESET_FIFO_FLUSH);
}

inline uint8_t SPL06::GetChipID(void)						//获取产品ID//获取产品版本//由于版本在不同的传感器有不同，本历程只判断ID来识别SPL06
{
	return Read( ID_REG);
}

/***********************************************************************
 * 获取气压计内部的校准参数
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
void SPL06::GetCalibParam(void)//内部校准值//气压计解算以及温补使用//由内部出厂设定
{
    unsigned long h;
    unsigned long m;
    unsigned long l;
    h =  Read( 0x10);
    l  =  Read(  0x11);
    spl06_calib_param.c0 = (int16_t)h<<4 | l>>4;
    spl06_calib_param.c0 = (spl06_calib_param.c0&0x0800)?(0xF000|spl06_calib_param.c0):spl06_calib_param.c0;
    h =  Read(  0x11);
    l  =  Read(  0x12);
    spl06_calib_param.c1 = (int16_t)(h&0x0F)<<8 | l;
    spl06_calib_param.c1 = (spl06_calib_param.c1&0x0800)?(0xF000|spl06_calib_param.c1):spl06_calib_param.c1;
    h =  Read(  0x13);
    m =  Read(  0x14);
    l =  Read(  0x15);
    spl06_calib_param.c00 = (int32_t)h<<12 | (int32_t)m<<4 | (int32_t)l>>4;
    spl06_calib_param.c00 = (spl06_calib_param.c00&0x080000)?(0xFFF00000|spl06_calib_param.c00):spl06_calib_param.c00;
    h =  Read(  0x15);
    m =  Read(  0x16);
    l =  Read(  0x17);
    spl06_calib_param.c10 = (int32_t)h<<16 | (int32_t)m<<8 | l;
    spl06_calib_param.c10 = (spl06_calib_param.c10&0x080000)?(0xFFF00000|spl06_calib_param.c10):spl06_calib_param.c10;
    h =  Read(  0x18);
    l  =  Read(  0x19);
    spl06_calib_param.c01 = (int16_t)h<<8 | l;
    h =  Read(  0x1A);
    l  =  Read(  0x1B);
    spl06_calib_param.c11 = (int16_t)h<<8 | l;
    h =  Read(  0x1C);
    l  =  Read(  0x1D);
    spl06_calib_param.c20 = (int16_t)h<<8 | l;
    h =  Read(  0x1E);
    l  =  Read(  0x1F);
    spl06_calib_param.c21 = (int16_t)h<<8 | l;
    h =  Read(  0x20);
    l  =  Read(  0x21);
    spl06_calib_param.c30 = (int16_t)h<<8 | l;
}



/***********************************************************************
 * 获取原始温度值
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
void SPL06::GetRawTemp(void)
{
    uint8_t h[3] = {0};
    
		h[0] = Read(  0x03);
		h[1] = Read(  0x04);
		h[2] = Read(  0x05);

    spl06.i32rawTemperature = (int32_t)h[0]<<16 | (int32_t)h[1]<<8 | (int32_t)h[2];
    spl06.i32rawTemperature= (spl06.i32rawTemperature&0x800000) ? (0xFF000000|spl06.i32rawTemperature) : spl06.i32rawTemperature;
}

/***********************************************************************
 * 获取原始气压值
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
void SPL06::GetRawPressure(void)
{
    uint8_t h[3];
	
		h[0] = Read(  0x00);
		h[1] = Read(  0x01);
		h[2] = Read(  0x02);
    
    spl06.i32rawPressure = (int32_t)h[0]<<16 | (int32_t)h[1]<<8 | (int32_t)h[2];
    spl06.i32rawPressure= (spl06.i32rawPressure&0x800000) ? (0xFF000000|spl06.i32rawPressure) : spl06.i32rawPressure;
}


/***********************************************************************
 * 温度解算值
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
float SPL06::GetTemperature(void)
{
    float fTCompensate;
    float fTsc;

    fTsc = spl06.i32rawTemperature / (float)spl06.i32kT;
    fTCompensate =  spl06_calib_param.c0 * 0.5 + spl06_calib_param.c1 * fTsc;
    return fTCompensate;
}

/***********************************************************************
 * 气压解算并进行温度补偿
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
float SPL06::GetPressure(void)
{
    float fTsc, fPsc;
    float qua2, qua3;
    float fPCompensate;

    fTsc = spl06.i32rawTemperature / (float)spl06.i32kT;
    fPsc = spl06.i32rawPressure / (float)spl06.i32kP;
    qua2 = spl06_calib_param.c10 + fPsc * (spl06_calib_param.c20 + fPsc* spl06_calib_param.c30);
    qua3 = fTsc * fPsc * (spl06_calib_param.c11 + fPsc * spl06_calib_param.c21);
		//qua3 = 0.9f *fTsc * fPsc * (spl06_calib_param.c11 + fPsc * spl06_calib_param.c21);
	
    fPCompensate = spl06_calib_param.c00 + fPsc * qua2 + fTsc * spl06_calib_param.c01 + qua3;
		//fPCompensate = spl06_calib_param.c00 + fPsc * qua2 + 0.9f *fTsc  * spl06_calib_param.c01 + qua3;
    return fPCompensate;
}



/***********************************************************************
 * 获取温度值
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
float SPL06::GetRealTemperature()
{
	GetRawTemp();//读取温度原始值
	return GetTemperature();//温度解算后的值
}
/***********************************************************************
 * 获取气压计温度补偿值
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/

float SPL06::GetRealPresure()
{
	GetRawPressure();			//读取气压值原始值
	return GetPressure();	//气压解算并经过温度补偿后的气压值
}

/***********************************************************************
 * 初始化
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
uint8_t SPL06::Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = _CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_CS_GPIO, &GPIO_InitStruct);
	
	uint8_t spl06_start_status;

	//等待内部校准数据可用
	do
	{
		spl06_start_status = GetMeasureStatus();//读取气压计启动状态
		printf("%d",_CS_Pin);
	}
	while((spl06_start_status&MEAS_CFG_COEF_RDY)!=MEAS_CFG_COEF_RDY);
	//读取内部校准值
	GetCalibParam();
	
	//等待传感器内部初始化完成
	do
		spl06_start_status = GetMeasureStatus();//读取气压计启动状态
	while((spl06_start_status&MEAS_CFG_SENSOR_RDY)!=MEAS_CFG_SENSOR_RDY);
	//读取CHIP ID
	spl06.chip_id = GetChipID();                                                                    
	
	//判断读取的ID是否正确，这里只判断高4位的ID，不判断低4位的版本号
	if((spl06.chip_id&0xf0)!=PRODUCT_ID)
		return 0;//如果ID读取失败，则返回失败
	//后台数据采样速率128HZ 过采样率32次
	PressureRateConfig(PRESSURE_RATE_128_TIMES,PRESSURE_RATE_32_TIMES);
	//后台数据采样速率32HZ 过采样率8次//设置传感器上的温度计作为温度采集
	TemperatureRateConfig(TEMPERATURE_RATE_32_TIMES,TEMPERATURE_RATE_8_TIMES,TEMPERATURE_RATE_TMP_EXT_EXTERNAL);
	//启动后台读取数据
	SetMeasureMode(MEAS_CFG_MEAS_CTR_BACKGROUND_PSR_TMP);
	return 1;//初始化成功
}

/*
 * 获取气压计SPL06数据
 * 单位：m
 */
SPL06::Baro_t SPL06::Updata(void)
{
	Baro_t baro;
	baro.temperature = GetRealTemperature();
	baro.pressure = GetRealPresure();
	baro.alti = (102000.0f	- baro.pressure) * 0.078740f;  //每1mpar平均海拔高度为78.740m
	return baro;
}

