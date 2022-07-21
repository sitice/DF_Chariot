#ifndef __DF_FILTER_H
#define __DF_FILTER_H

#include "main.h"
#include "ICM20602.hpp"

#define Pi	3.1415927f                         
#define Radian_to_Angle	   57.2957795f  //弧转角
#define RawData_to_Angle	0.0610351f	//原转角
#define RawData_to_Radian	0.0010653f	//原转弧

struct _filter{
	double inBuf[7];
	double outBuf[7];
};

extern ICM20602::Acc_f  filter_acc;//滤波后加速度
extern ICM20602::Gyro_f filter_gyro;//滤波后角速度
extern ICM20602::Gyro_f SI_gyro;//角速度数据（rad）
extern ICM20602::Acc_f  nacc;//惯导加速度

float IIRLowPass(int16_t x, struct _filter *Acc);
void GROY_IIR_Filter( ICM20602::Gyro_t *Acc_in,ICM20602::Gyro_f *Acc_out);
void ACC_IIR_Filter( ICM20602::Acc_t *Acc_in, ICM20602::Acc_f *Acc_out);
void Get_Radian(ICM20602::Gyro_t *Gyro_in, ICM20602::Gyro_f *Gyro_out);

#endif
