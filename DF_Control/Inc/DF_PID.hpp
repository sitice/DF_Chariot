#ifndef __DF_PID_H
#define __DF_PID_H

#include "main.h"

typedef struct 
{
	float KP;
	float KI;
	float KD;
	float Taget;
	int16_t INT_UP;
	int16_t INT_DOWN;
	float Error;
	float Last_Error;
	float Pre_Error;
	float Integral;
	float PID_Out;
}_PID_Position;

extern _PID_Position PID_Pos_1;
extern _PID_Position PID_Pos_2;
extern _PID_Position PID_Pos_3;
extern _PID_Position PID_Pos_4;
extern _PID_Position PID_Pos_5;
extern _PID_Position PID_Pos_6;
extern _PID_Position PID_Pos_7;
extern _PID_Position PID_Pos_8;
extern _PID_Position PID_Pos_9;

#endif
