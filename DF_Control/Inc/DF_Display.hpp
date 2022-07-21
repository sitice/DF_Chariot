#ifndef __DF_DISPLAY_H
#define __DF_DISPLAY_H

#include "oled.hpp"

#define MaxPageNum  10//页面最大值

typedef struct 
{
	struct 
	{
		uint8_t PageNum;//当前显示的页面号
		uint8_t LinePoint[MaxPageNum];//行指针
		uint8_t TrainPoint[MaxPageNum];//列指针
	}Page;  
	
	
	
}DISPLAY;

extern DISPLAY Display;

#endif

