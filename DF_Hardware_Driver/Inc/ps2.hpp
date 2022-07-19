#ifndef __PS2_H__
#define __PS2_H__

#include "main.h"

extern float PS2_LX,PS2_LY,PS2_RX,PS2_RY;  
extern int PS2_KEY;
#define DI   PDin(9)           //PA0  ����
#define DO_H PDout(10)=1        //����λ��
#define DO_L PDout(10)=0        //����λ��

#define CS_H PDout(11)=1       //CS����
#define CS_L PDout(11)=0       //CS����

#define CLK_H PDout(12)=1      //ʱ������
#define CLK_L PDout(12)=0      //ʱ������

//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16

#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16

//#define WHAMMY_BAR		8
//These are stick values
#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6 
#define PSS_LX 7
#define PSS_LY 8

typedef struct
{
	float RX_Val;
	float RY_Val;
	
	float LX_Val;
	float LY_Val;
	
	int Key_Val;
}PS2;

extern PS2 PS2_Data;

extern u8 Data[9];  
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);   //�ж��Ƿ�Ϊ���ģʽ
void PS2_Receive (void); //���ֱ�����
void PS2_Cmd(u8 CMD);		  //���ֱ���������
u8 PS2_DataKey(void);		  //����ֵ��ȡ
u8 PS2_AnologData(u8 button); //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����
void PS2_Vibration(u8 motor1, u8 motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF

void PS2_EnterConfing(void);	 //��������
void PS2_TurnOnAnalogMode(void); //����ģ����
void PS2_VibrationMode(void);    //������
void PS2_ExitConfing(void);	     //�������
void PS2_SetInit(void);		     //���ó�ʼ��

#endif
