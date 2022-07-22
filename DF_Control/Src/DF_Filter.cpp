#include "DF_Filter.hpp"

//截止频率20hz
double b[7]={4.863987500781e-08,2.918392500469e-07,7.295981251171e-07,9.727975001562e-07,
  7.295981251171e-07,2.918392500469e-07,4.863987500781e-08};
double a[7]={1,   -5.514535121166,    12.68911305652,    -15.5936352107,
      10.79329667049,   -3.989359404231,   0.6151231220526};

Filter_t acc_filter[3];
Filter_t gyro_filter[3];

float IIRLowPass(int16_t x, Filter_t &data)
{
	int i;
	//运算之前Buf向前移动一个位置，以保存之前Buf的数据；
	for(i=6; i>0; i--){
		data.outBuf[i] = data.outBuf[i-1];
		data.inBuf[i] = data.inBuf[i-1];
	}
	data.inBuf[0] = x;
	data.outBuf[0] = 0;
	for(i=1;i<7;i++)
	{
		data.outBuf[0] = data.outBuf[0] + b[i] * data.inBuf[i];
		data.outBuf[0] = data.outBuf[0] - a[i] * data.outBuf[i];
	}
	data.outBuf[0] = data.outBuf[0] + b[0] * data.inBuf[0];
	return data.outBuf[0];
}

void IIRFilter( const ICM20602::Gyro_t &Gyro_in, ICM20602::Gyro_t &Gyro_out)
{ //将新得的数据和上次输出的数据进行处理
	Gyro_out.x = IIRLowPass(Gyro_in.x, gyro_filter[0]); 
	Gyro_out.y = IIRLowPass(Gyro_in.y, gyro_filter[1]);
	Gyro_out.z = IIRLowPass(Gyro_in.z, gyro_filter[2]); 
}

void IIRFilter( const ICM20602::Acc_t &Acc_in, ICM20602::Acc_t &Acc_out)
{ //将新得的数据和上次输出的数据进行处理
	Acc_out.x = IIRLowPass(Acc_in.x, acc_filter[0]); 
	Acc_out.y = IIRLowPass(Acc_in.y, acc_filter[1]);  
	Acc_out.z = IIRLowPass(Acc_in.z, acc_filter[2]); 
}


