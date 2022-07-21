#ifndef __DF_COMMUNICATE_H
#define __DF_COMMUNICATE_H

#include "main.h"

//数据拆分宏定义
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

void sendSenser(s16 a_x,s16 a_y,s16 a_z,u8 _fu);
void send_ac_gy_map(s16 a_x,  s16 a_y,  s16 a_z, s16 g_x, s16 g_y, s16 g_z,u8 _fu);
void DF_send_control_motor(s16 a_x,s16 a_y,s16 a_z,s16 a_m);
void DF_send_targrt_attitude (s16 a_x,s16 a_y,s16 a_z);
void DF_send_X_Y_Z_Flight_speed (s16 a_x,s16 a_y,s16 a_z);
void send_yaw_data_to(s16 yaw);

#endif
