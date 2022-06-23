#include "spl06.hpp"
#include "spi.h"

/* 
 * SPL06дָ���Ĵ�������
 */
void SPL06::Write(uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_RESET);// ���ͣ�ѡ��оƬ
	SPI_Write_Read_Byte(_hspi,reg & 0x7f);//���ͼĴ�����ַ+д����
	SPI_Write_Read_Byte(_hspi,data);
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_SET);
}

/* 
 * SPL06��ָ���Ĵ�������
 */
uint8_t SPL06::Read(uint8_t reg)
{
	uint8_t data;
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_RESET);		// ���ͣ�ѡ��оƬ
	SPI_Write_Read_Byte(_hspi,reg | 0x80);				//���ͼĴ�����ַ+������
	data = SPI_Write_Read_Byte(_hspi,0xff);
	HAL_GPIO_WritePin(_CS_GPIO, _CS_Pin, GPIO_PIN_SET);
	return data;
}



void SPL06::PressureRateConfig(uint8_t background_rate,uint8_t oversamply)
{
		uint8_t data;
	
		data = (background_rate<<4)|oversamply;
		if(oversamply > PRESSURE_RATE_8_TIMES)//��������������EMPERATURE_RATE_8_TIMES��Ӧ���������ݱ��µ����ݸ��ǣ��ڲ�ӵ����ѹ���¶ȹ�32����FIFO���ڴ���8�Σ�Ҳ���Ǵ��ڻ����16�ι���������ʱ����Ҫ���µ����ݸ��ǣ��������ݾͻᶪʧ
		{
				uint8_t data;
				data = Read(CFG_REG);//��ȡԭ�Ĵ���ֵ
				data |= 0X04;//P-SHIFTλ��1
				Write(CFG_REG, data);//����д�ؼĴ���					
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
		Write( PRS_CFG_REG, data);//д������
}


void SPL06::TemperatureRateConfig(uint8_t background_rate,uint8_t oversamply,uint8_t ext)
{
	uint8_t data;

	data = (ext<<7)|(background_rate<<4)|oversamply;
	if(oversamply > TEMPERATURE_RATE_8_TIMES)//��������������EMPERATURE_RATE_8_TIMES��Ӧ���������ݱ��µ����ݸ���
	{
			uint8_t data;
			data = Read( CFG_REG);//��ȡԭ�Ĵ���ֵ
			data |= 0X08;//T-SHIFTλ��1
			Write( CFG_REG, data);	//����д�ؼĴ���					
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
	Write( TMP_CFG_REG, data);//д������
}

//��ȡ���������ݾ�λ״̬//����������״̬
inline uint8_t SPL06::GetMeasureStatus(void)
{
	return Read( MEAS_CFG_REG);
}
//���ö�ȡģʽ+��ȡ��ʽ
inline void SPL06::SetMeasureMode(uint8_t mode)  //����Ϊģʽֵ
{
	 Write( MEAS_CFG_REG,mode);
}
//��������ģʽ��ȡ�¶�ֵ
inline void SPL06::StartTemperature(void)
{
    Write( MEAS_CFG_REG, MEAS_CFG_MEAS_CTR_COMMAND_TMP);
}
//��������ģʽ��ȡ��ѹֵ
inline void SPL06::StartPressure(void)
{
    Write(MEAS_CFG_REG, MEAS_CFG_MEAS_CTR_COMMAND_PRS);
}
//�������ģʽ�������ֹͣ�ɼ�����ֱ���ٴ��л�ģʽ
inline void SPL06::EnterStandby(void)
{
    Write( MEAS_CFG_REG, MEAS_CFG_MEAS_CTR_STANDBY);
}

inline void SPL06::SetInterrupt(uint8_t interrupt,uint8_t type)//�����ж�ʹ��
{
	uint8_t data;
	data = Read(CFG_REG);
	if(type!=ENABLE )
		data &= ~interrupt;
	else
		data |= interrupt;	
	Write( CFG_REG,data);
}

inline void SPL06::SetSpiWire(uint8_t wire)						//����SPI���� //3��/4��SPI��ȡ����
{
	uint8_t data;
	data = Read(CFG_REG);
	data &= 0xf7;															//SPI����������λ��0
	data |= wire;
	Write( CFG_REG,data);
}

inline void SPL06::SetIntruptLevel(uint8_t level)			//�����ж���Ч��ƽ//INT�ߵ�ƽ��Ч���ߵ͵�ƽ��Ч//levelΪ0��͵�ƽ��Ч,Ϊ1��ߵ�ƽ��Ч
{
	uint8_t data;
	data = Read( CFG_REG);
	data &= 0x7f;//�жϵ�ƽ��Чλ��0
	data |= level<<7;
	Write( CFG_REG,data);
}


inline uint8_t SPL06::GetIntStatus(void)//��ȡ������״̬
{
	return Read( INT_STS_REG);
}


inline uint8_t SPL06::GetFifoStatus(void)
{
	return Read( FIFO_STS_REG);
}


inline void SPL06::SoftReset(void)//�����λ
{
	 Write(RESET_REG,RESET_SOFT);
}
inline void SPL06::ResetFifo(void)//�����FIFO
{
	Write(RESET_REG,RESET_FIFO_FLUSH);
}

inline uint8_t SPL06::GetChipID(void)						//��ȡ��ƷID//��ȡ��Ʒ�汾//���ڰ汾�ڲ�ͬ�Ĵ������в�ͬ��������ֻ�ж�ID��ʶ��SPL06
{
	return Read( ID_REG);
}

/***********************************************************************
 * ��ȡ��ѹ���ڲ���У׼����
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
void SPL06::GetCalibParam(void)//�ڲ�У׼ֵ//��ѹ�ƽ����Լ��²�ʹ��//���ڲ������趨
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
 * ��ȡԭʼ�¶�ֵ
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
 * ��ȡԭʼ��ѹֵ
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
 * �¶Ƚ���ֵ
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
 * ��ѹ���㲢�����¶Ȳ���
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
 * ��ȡ�¶�ֵ
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/
float SPL06::GetRealTemperature()
{
	GetRawTemp();//��ȡ�¶�ԭʼֵ
	return GetTemperature();//�¶Ƚ�����ֵ
}
/***********************************************************************
 * ��ȡ��ѹ���¶Ȳ���ֵ
 * @param[in] 
 * @param[out] 
 * @return     
 **********************************************************************/

float SPL06::GetRealPresure()
{
	GetRawPressure();			//��ȡ��ѹֵԭʼֵ
	return GetPressure();	//��ѹ���㲢�����¶Ȳ��������ѹֵ
}

/***********************************************************************
 * ��ʼ��
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

	//�ȴ��ڲ�У׼���ݿ���
	do
	{
		spl06_start_status = GetMeasureStatus();//��ȡ��ѹ������״̬
		printf("%d",_CS_Pin);
	}
	while((spl06_start_status&MEAS_CFG_COEF_RDY)!=MEAS_CFG_COEF_RDY);
	//��ȡ�ڲ�У׼ֵ
	GetCalibParam();
	
	//�ȴ��������ڲ���ʼ�����
	do
		spl06_start_status = GetMeasureStatus();//��ȡ��ѹ������״̬
	while((spl06_start_status&MEAS_CFG_SENSOR_RDY)!=MEAS_CFG_SENSOR_RDY);
	//��ȡCHIP ID
	spl06.chip_id = GetChipID();                                                                    
	
	//�ж϶�ȡ��ID�Ƿ���ȷ������ֻ�жϸ�4λ��ID�����жϵ�4λ�İ汾��
	if((spl06.chip_id&0xf0)!=PRODUCT_ID)
		return 0;//���ID��ȡʧ�ܣ��򷵻�ʧ��
	//��̨���ݲ�������128HZ ��������32��
	PressureRateConfig(PRESSURE_RATE_128_TIMES,PRESSURE_RATE_32_TIMES);
	//��̨���ݲ�������32HZ ��������8��//���ô������ϵ��¶ȼ���Ϊ�¶Ȳɼ�
	TemperatureRateConfig(TEMPERATURE_RATE_32_TIMES,TEMPERATURE_RATE_8_TIMES,TEMPERATURE_RATE_TMP_EXT_EXTERNAL);
	//������̨��ȡ����
	SetMeasureMode(MEAS_CFG_MEAS_CTR_BACKGROUND_PSR_TMP);
	return 1;//��ʼ���ɹ�
}

/*
 * ��ȡ��ѹ��SPL06����
 * ��λ��m
 */
SPL06::Baro_t SPL06::Updata(void)
{
	Baro_t baro;
	baro.temperature = GetRealTemperature();
	baro.pressure = GetRealPresure();
	baro.alti = (102000.0f	- baro.pressure) * 0.078740f;  //ÿ1mparƽ�����θ߶�Ϊ78.740m
	return baro;
}

