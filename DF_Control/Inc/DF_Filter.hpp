#ifndef __DF_FILTER_H
#define __DF_FILTER_H

#include "main.h"
#include "ICM20602.hpp"

#define Pi	3.1415927f                         
#define Radian_to_Angle	   57.2957795f  //ת
#define RawData_to_Angle	0.0610351f	//ԭת
#define RawData_to_Radian	0.0010653f	//ԭת

struct _filter{
	double inBuf[7];
	double outBuf[7];
};

extern ICM20602::Acc_f  filter_acc;//˲ٶ
extern ICM20602::Gyro_f filter_gyro;//˲ٶ
extern ICM20602::Gyro_f SI_gyro;//ٶݣrad
extern ICM20602::Acc_f  nacc;//ߵٶ


float IIRLowPass(float x, struct _filter *Acc);
void GROY_IIR_Filter( ICM20602::Gyro_f *Acc_in,ICM20602::Gyro_f *Acc_out);
void ACC_IIR_Filter( ICM20602::Acc_f *Acc_in, ICM20602::Acc_f *Acc_out);
void Get_Radian(ICM20602::Gyro_f *Gyro_in, ICM20602::Gyro_f *Gyro_out);

#endif
