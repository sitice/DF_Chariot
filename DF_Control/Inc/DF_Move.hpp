#ifndef __DF_MOVE_H
#define __DF_MOVE_H

#include "main.h"
#include "motor.hpp"

//结构体命名……
struct Move_t
{
	Motor* motor1;
	Motor* motor2;
	Motor* motor3;
	Motor* motor4;
	
	struct 
	{
		float x;
		float y;
	}Distance_t;
	
};

extern MOVE Move;

void OmnibearingMove(uint16_t x,uint16_t y,uint16_t z);
#endif

