#include "DF_IMU.hpp"
#include "DF_Filter.hpp"
#include "math.h"

#define Kp 	0.7f    // ��������
#define Ki 	0.001f  // ���ֳ���
#define halfT 0.0005f//������
#define T	0.001f  // ����Ϊ1ms

static float q0 = 1.0f,  q1 = 0.0f,  q2 = 0.0f,q3 = 0.0f;     	// ��Ԫ��
static float exInt = 0, eyInt = 0, ezInt = 0;    	// �������ۼ�ֵ 
static float rMat[3][3];//��Ԫ������ת����

float aex, aey, aez;
struct _out_angle out_angle;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) 
{ 
 	uint8_t  cnnt=0;
	float norm;
	float ex, ey, ez;

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
	rMat[0][0]=1.0f-2.0f*q2q2-2.0f*q3q3;rMat[0][1] = 2.0f * (q1q2 + -q0q3)  ;rMat[0][2] = 2.0f * (q1q3 - -q0q2);
    rMat[1][0] = 2.0f * (q1q2 - -q0q3) ;rMat[1][1] =1.0f-2.0f*q1q1-2.0f*q3q3;rMat[1][2] = 2.0f * (q2q3 + -q0q1);
    rMat[2][0] = 2.0f * (q1q3 + -q0q2) ;rMat[2][1] = 2.0f * (q2q3 - -q0q1)   ;rMat[2][2]=1.0f-2.0f*q1q1-2.0f*q2q2;
	
	norm = invSqrt(ax*ax + ay*ay + az*az);
	
	
	aex=ax*0.00244140625f;
	aey=ay*0.00244140625f;
	aez=az*0.00244140625f;//1.0f/4096.0f;
	
	ax = ax * norm;//ʵ�ʼ��ٶ�
	ay = ay * norm;
	az = az * norm;
    //N--->B////������
	ex = (ay * rMat[2][2] - az * rMat[2][1]);
	ey = (az * rMat[2][0] - ax * rMat[2][2]);
	ez = (ax * rMat[2][1] - ay * rMat[2][0]);

	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;
	//PI
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;
	//һ���������
	float q0_last = q0;	
	float q1_last = q1;	
	float q2_last = q2;	
	float q3_last = q3;	
	q0 = q0_last + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
	q1 = q1_last + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
	q2 = q2_last + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
	q3 = q3_last + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT; 
	//��λ��
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
    if(!cnnt)
	{nacc.x=rMat[0][0]*aex+rMat[0][1]*aey+rMat[0][2]*aez;
	nacc.y=rMat[1][0]*aex+rMat[1][1]*aey+rMat[1][2]*aez;
	nacc.z=rMat[2][0]*aex+rMat[2][1]*aey+rMat[2][2]*aez;}
	
	//ģ��̵����������ԣ������ͻ����ԣ�
	if(filter_gyro.z>3||filter_gyro.z<-3)
	out_angle.yaw-=(filter_gyro.z * RawData_to_Angle * 0.001f);
}

// ���ټ��㿪���ŵĵ���
float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void Get_Eulerian_Angle( struct _out_angle *angle)//���õ�ֻ�� pitch roll
{
	angle->roll = -atan((2.0f*(q0*q1 + q2*q3)/(q0*q0 - q1*q1 - q2*q2 + q3*q3)))*Radian_to_Angle;
	angle->pitch  =asin (2.0f*(q0*q2 - q1*q3))*Radian_to_Angle;
}
