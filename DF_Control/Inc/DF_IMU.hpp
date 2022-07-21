#ifndef __DF_IMU_H
#define __DF_IMU_H

#include "main.h"

#define MIN(a, b) 	(((a) < (b)) ? (a) : (b))
#define MAX(a, b) 	(((a) > (b)) ? (a) : (b))
#define M_PIf       3.14159265358979323846f

struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};

extern struct _out_angle out_angle;

void Get_Eulerian_Angle( struct _out_angle *angle);//所得的只是 pitch roll
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) ;
float invSqrt(float x);// 快速计算开根号的倒数
#endif



