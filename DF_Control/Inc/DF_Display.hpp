#ifndef __DF_DISPLAY_H
#define __DF_DISPLAY_H

#include "oled.hpp"

#define MaxPageNum  10//ҳ�����ֵ

typedef struct 
{
	struct 
	{
		uint8_t PageNum;//��ǰ��ʾ��ҳ���
		uint8_t LinePoint[MaxPageNum];//��ָ��
		uint8_t TrainPoint[MaxPageNum];//��ָ��
	}Page;  
	
	
	
}DISPLAY;

extern DISPLAY Display;

#endif

