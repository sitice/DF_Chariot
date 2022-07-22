#ifndef __DF_FILTER_H
#define __DF_FILTER_H

#include "main.h"
#include "ICM20602.hpp"

#define PI	3.1415927f                         
#define RADIAN_TO_ANGLE	   57.2957795f  

struct Filter_t{
	double inBuf[7];
	double outBuf[7];
};


float IIRLowPass(int16_t x, Filter_t &data);
void IIRFilter( const ICM20602::Gyro_t &Gyro_in, ICM20602::Gyro_t &Gyro_out);
void IIRFilter( const ICM20602::Acc_t &Acc_in, ICM20602::Acc_t &Acc_out);
void Get_Radian(ICM20602::Gyro_t &Gyro_in, ICM20602::Gyro_t &Gyro_out);

#endif
