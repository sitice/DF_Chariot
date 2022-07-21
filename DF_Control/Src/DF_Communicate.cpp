#include "DF_Communicate.hpp"
#include "usart.h"

uint8_t DataTOSend[40];

void PrintHexU8(uint8_t data)
{
//	USART1->DR = data;
//	while ((USART1->SR & 0X40) == 0);
	//HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
	HAL_UART_Transmit(&huart1,&data,1,0xffff);
}

//欧拉角
void sendSenser(s16 a_x, s16 a_y, s16 a_z, u8 _fu)
{
    u8 _cnt = 0;

    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0x03;
    DataTOSend[_cnt++] = 7;

    DataTOSend[_cnt++] = BYTE0(a_x);
    DataTOSend[_cnt++] = BYTE1(a_x);

    DataTOSend[_cnt++] = BYTE0(a_y);
    DataTOSend[_cnt++] = BYTE1(a_y);

    DataTOSend[_cnt++] = BYTE0(a_z);
    DataTOSend[_cnt++] = BYTE1(a_z);

    DataTOSend[_cnt++] = _fu;
    u8 sc = 0;
    u8 ac = 0;

    for (u8 i = 0; i < (DataTOSend[3] + 4); i++)
    {
        sc += DataTOSend[i];
        ac += sc;
    }
    DataTOSend[_cnt++] = sc;
    DataTOSend[_cnt++] = ac;

    for (u8 temp = 0; temp < _cnt; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}

//加速度角速度计
void send_ac_gy_map(s16 a_x, s16 a_y, s16 a_z, s16 g_x, s16 g_y, s16 g_z,
                    u8 _fu)
{
    u8 _cnt = 0;

    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0x01;
    DataTOSend[_cnt++] = 13;

    DataTOSend[_cnt++] = BYTE0(a_x);
    DataTOSend[_cnt++] = BYTE1(a_x);

    DataTOSend[_cnt++] = BYTE0(a_y);
    DataTOSend[_cnt++] = BYTE1(a_y);

    DataTOSend[_cnt++] = BYTE0(a_z);
    DataTOSend[_cnt++] = BYTE1(a_z);

    DataTOSend[_cnt++] = BYTE0(g_x);
    DataTOSend[_cnt++] = BYTE1(g_x);

    DataTOSend[_cnt++] = BYTE0(g_y);
    DataTOSend[_cnt++] = BYTE1(g_y);

    DataTOSend[_cnt++] = BYTE0(g_z);
    DataTOSend[_cnt++] = BYTE1(g_z);

    DataTOSend[_cnt++] = _fu;
    u8 sc = 0;
    u8 ac = 0;

    for (u8 i = 0; i < (DataTOSend[3] + 4); i++)
    {
        sc += DataTOSend[i];
        ac += sc;
    }
    DataTOSend[_cnt++] = sc;
    DataTOSend[_cnt++] = ac;

    for (u8 temp = 0; temp < _cnt; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}

// out control_motor
void DF_send_control_motor(s16 a_x, s16 a_y, s16 a_z, s16 a_m)
{
    u8 _cnt = 0;
    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0x21;
    DataTOSend[_cnt++] = 8;

    DataTOSend[_cnt++] = BYTE0(a_x);
    DataTOSend[_cnt++] = BYTE1(a_x);

    DataTOSend[_cnt++] = BYTE0(a_y);
    DataTOSend[_cnt++] = BYTE1(a_y);

    DataTOSend[_cnt++] = BYTE0(a_z);
    DataTOSend[_cnt++] = BYTE1(a_z);

    DataTOSend[_cnt++] = BYTE0(a_m);
    DataTOSend[_cnt++] = BYTE1(a_m);

    u8 sc = 0;
    u8 ac = 0;

    for (u8 i = 0; i < (DataTOSend[3] + 4); i++)
    {
        sc += DataTOSend[i];
        ac += sc;
    }
    DataTOSend[_cnt++] = sc;
    DataTOSend[_cnt++] = ac;
    for (u8 temp = 0; temp < _cnt; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}
// targrt attitude 0.01deegra
void DF_send_targrt_attitude(s16 a_x, s16 a_y, s16 a_z)
{
    u8 _cnt = 0;
    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0x0A;
    DataTOSend[_cnt++] = 6;

    DataTOSend[_cnt++] = BYTE0(a_x);
    DataTOSend[_cnt++] = BYTE1(a_x);

    DataTOSend[_cnt++] = BYTE0(a_y);
    DataTOSend[_cnt++] = BYTE1(a_y);

    DataTOSend[_cnt++] = BYTE0(a_z);
    DataTOSend[_cnt++] = BYTE1(a_z);

    u8 sc = 0;
    u8 ac = 0;

    for (u8 i = 0; i < (DataTOSend[3] + 4); i++)
    {
        sc += DataTOSend[i];
        ac += sc;
    }
    DataTOSend[_cnt++] = sc;
    DataTOSend[_cnt++] = ac;

    for (u8 temp = 0; temp < _cnt; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}
// XYZFlight speed  cm/s
void DF_send_X_Y_Z_Flight_speed(s16 a_x, s16 a_y, s16 a_z)
{
    u8 _cnt = 0;
    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0x07;
    DataTOSend[_cnt++] = 6;

    DataTOSend[_cnt++] = BYTE0(a_x);
    DataTOSend[_cnt++] = BYTE1(a_x);

    DataTOSend[_cnt++] = BYTE0(a_y);
    DataTOSend[_cnt++] = BYTE1(a_y);

    DataTOSend[_cnt++] = BYTE0(a_z);
    DataTOSend[_cnt++] = BYTE1(a_z);

    u8 sc = 0;
    u8 ac = 0;

    for (u8 i = 0; i < (DataTOSend[3] + 4); i++)
    {
        sc += DataTOSend[i];
        ac += sc;
    }
    DataTOSend[_cnt++] = sc;
    DataTOSend[_cnt++] = ac;

    for (u8 temp = 0; temp < _cnt; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}

void DF_Sendcheck(u8 _id, u8 _sc, u8 _ac)
{
    u8 _cnt = 0;
    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0x00;
    DataTOSend[_cnt++] = 0;
    DataTOSend[_cnt++] = _id;
    DataTOSend[_cnt++] = _sc;
    DataTOSend[_cnt++] = _ac;

    DataTOSend[3] = _cnt - 4;
    u8 check_sum1 = 0, check_sum2 = 0;
    for (u8 i = 0; i < _cnt; i++)
    {
        check_sum1 += DataTOSend[i];
        check_sum2 += check_sum1;
    }
    DataTOSend[_cnt++] = check_sum1;
    DataTOSend[_cnt++] = check_sum2;
    for (u8 temp = 0; temp < _cnt + 1; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}
void DF_SendPar(u16 _id, s32 _val)
{
    u8 _cnt = 0;
    DataTOSend[_cnt++] = 0xAA;
    DataTOSend[_cnt++] = 0xFF;
    DataTOSend[_cnt++] = 0xE2;
    DataTOSend[_cnt++] = 0;
    DataTOSend[_cnt++] = BYTE0(_id);
    DataTOSend[_cnt++] = BYTE1(_id);
    DataTOSend[_cnt++] = BYTE0(_val);
    DataTOSend[_cnt++] = BYTE1(_val);
    DataTOSend[_cnt++] = BYTE2(_val);
    DataTOSend[_cnt++] = BYTE3(_val);
    DataTOSend[3] = _cnt - 4;
    u8 check_sum1 = 0, check_sum2 = 0;
    for (u8 i = 0; i < _cnt; i++)
    {
        check_sum1 += DataTOSend[i];
        check_sum2 += check_sum1;
    }
    DataTOSend[_cnt++] = check_sum1;
    DataTOSend[_cnt++] = check_sum2;
    for (u8 temp = 0; temp < _cnt + 1; temp++)
    {
        PrintHexU8(DataTOSend[temp]);
    }
}
