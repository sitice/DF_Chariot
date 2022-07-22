#ifndef __DF_IMU_H
#define __DF_IMU_H

#include "main.h"
#include "icm20602.hpp"


struct EulerAngle_t
{
	float yaw;
	float roll;
	float pitch;
};


EulerAngle_t IMUupdate(ICM20602::Acc_t &acc,ICM20602::Gyro_t &gyro); 
float invSqrt(float x);// 快速计算开根号的倒数
#endif



