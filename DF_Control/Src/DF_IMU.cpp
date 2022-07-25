#include "DF_IMU.hpp"
#include "DF_Filter.hpp"
#include "math.h"

#define KP 	0.7f    // 比例常数
#define KI 	0.001f  // 积分常数
#define T	0.001f  // 周期为1ms
#define HALFT (T * 0.5f)//半周期

#define RAWDATA_TO_ANGLE	0.0610351f
#define RAWDATA_TO_RADIAN	0.0010653f

EulerAngle_t IMUupdate(ICM20602::Acc_t &acc,ICM20602::Gyro_t &gyro,ICM20602::Gyro_t &filter_gyro) 
{ 
	float norm = 0;
	float ex = 0, ey = 0, ez = 0;
	
	static float q0 = 1.0f,  q1 = 0.0f,  q2 = 0.0f,q3 = 0.0f;     	// 四元数
	static float exInt = 0, eyInt = 0, ezInt = 0;    	// 误差积分累计值 
	static float rMat[3][3] = {0};//四元数的旋转矩阵

	static float aex = 0, aey = 0, aez = 0;

	float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q3 = q2 * q3;
	//B--->N
	rMat[0][0]=1.0f-2.0f * q2q2-2.0f * q3q3;rMat[0][1] = 2.0f  *  (q1q2 + -q0q3)  ;rMat[0][2] = 2.0f  *  (q1q3 - -q0q2);
    rMat[1][0] = 2.0f  *  (q1q2 - -q0q3) ;rMat[1][1] =1.0f-2.0f * q1q1-2.0f * q3q3;rMat[1][2] = 2.0f  *  (q2q3 + -q0q1);
    rMat[2][0] = 2.0f  *  (q1q3 + -q0q2) ;rMat[2][1] = 2.0f  *  (q2q3 - -q0q1)   ;rMat[2][2]=1.0f-2.0f * q1q1-2.0f * q2q2;
	
	norm = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
	
	aex=acc.x * 0.00244140625f;
	aey=acc.y * 0.00244140625f;
	aez=acc.z * 0.00244140625f;//1.0f/4096.0f;
	
	acc.x = acc.x * norm;//实际加速度
	acc.y = acc.y * norm;
	acc.z = acc.z * norm;
    //N--->B////叉积误差
	ex = (acc.y * rMat[2][2] - acc.z * rMat[2][1]);
	ey = (acc.z * rMat[2][0] - acc.x * rMat[2][2]);
	ez = (acc.x * rMat[2][1] - acc.y * rMat[2][0]);

	exInt = exInt + ex * KI;
	eyInt = eyInt + ey * KI;
	ezInt = ezInt + ez * KI;
	//PI
	gyro.x = gyro.x + KP * ex + exInt;
	gyro.y = gyro.y + KP * ey + eyInt;
	gyro.z = gyro.z + KP * ez + ezInt;
	//一阶龙格库塔
	float q0_last = q0;	
	float q1_last = q1;	
	float q2_last = q2;	
	float q3_last = q3;	
	
	q0 = q0_last + (-q1_last * gyro.x - q2_last * gyro.y - q3_last * gyro.z) * HALFT;
	q1 = q1_last + ( q0_last * gyro.x + q2_last * gyro.z - q3_last * gyro.y) * HALFT;
	q2 = q2_last + ( q0_last * gyro.y - q1_last * gyro.z + q3_last * gyro.x) * HALFT;
	q3 = q3_last + ( q0_last * gyro.z + q1_last * gyro.y - q2_last * gyro.x)  *  HALFT; 
	//单位化
	norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0  *  norm;
	q1 = q1  *  norm;
	q2 = q2  *  norm;
	q3 = q3  *  norm;
	
//	nacc.x= rMat[0][0] * aex+rMat[0][1] * aey+rMat[0][2] * aez;
//	nacc.y=rMat[1][0] * aex+rMat[1][1] * aey+rMat[1][2] * aez;
//	nacc.z=rMat[2][0] * aex+rMat[2][1] * aey+rMat[2][2] * aez;
	
	static EulerAngle_t angle;
	//模拟继电器死区特性（不含滞回特性）
	if(filter_gyro.z > 3||filter_gyro.z < -3 )
		angle.yaw -= (filter_gyro.z  *  RAWDATA_TO_ANGLE  *  0.001f);
	
	angle.roll = -atan((2.0f * (q0 * q1 + q2 * q3)/(q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3))) * RAWDATA_TO_ANGLE;
	angle.pitch = asin (2.0f * (q0 * q2 - q1 * q3)) * RAWDATA_TO_ANGLE;
	return angle;
}

// 快速计算开根号的倒数
float invSqrt(float x)
{
	float halfx = 0.5f  *  x;
	float y = x;
	long i =  * (long * )&y;
	i = 0x5f3759df - (i>>1);
	y =  * (float * )&i;
	y = y  *  (1.5f - (halfx  *  y  *  y));
	return y;
}

