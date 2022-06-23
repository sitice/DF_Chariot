#include "oled.hpp"
#include <cmath>
#include "FreeRTOS.h"
#include "task.h"

uint8_t zifu[]={
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//! 1
  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" 2
  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//# 3
  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$ 4
  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//% 5
  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//& 6
  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' 7
  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 8
  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 9
  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 10
  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+ 11
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//, 12
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//- 13
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//. 14
  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,/// 15
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 16
  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//1 17
  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 18
  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//3 19
  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//4 20
  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//5 21
  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//6 22
  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//7 23
  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 24
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//9 25
  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//: 26
  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//; 27
  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//< 28
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//= 29
  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//> 30
  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//? 31
  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@ 32
  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 33
  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 34
  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 35
  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 36
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 37
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 38
  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 39
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 40
  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 41
  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 42
  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 43
  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 44
  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M 45
  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 46
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 47
  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 48
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q 49
  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 50
  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 51
  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 52
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 53
  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 54
  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W 55
  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 56
  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 57
  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 58
  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[ 59
  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\ 60
  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//] 61
  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 62
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 63
  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//` 64
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a 65
  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 66
  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 67
  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d 68
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e 69
  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 70
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 71
  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 72
  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 73
  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 74
  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k 75
  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 76
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 77
  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 78
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 79
  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p 80
  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q 81
  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 82
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 83
  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t 84
  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 85
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v 86
  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w 87
  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x 88
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y 89
  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 90
  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{ 91
  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//| 92
  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//} 93
  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~ 94
};


#define delay_ms(x)  HAL_Delay(x)

uint8_t u8x8_gpio_and_delay_template(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
      break;							// can be used to setup pins
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
		
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
		__NOP();
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
		for (size_t  n = 0; n < 640; n++)
		{
		   __NOP();
		}
      break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
		vTaskDelay(arg_int);
      break;
    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
		arg_int?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
    //case U8X8_MSG_GPIO_SPI_CLOCK:
      break;
    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
		arg_int?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
    //case U8X8_MSG_GPIO_SPI_DATA:
      break;
    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
		arg_int?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);
      break;
    case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
		arg_int?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
      break;
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
		arg_int?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
      break;
  }
  return 1;
}


OLED::OLED():MAX_X(128),MAX_Y(64),SIZE(16){}

OLED::OLED(uint8_t maxX,uint8_t maxY,uint8_t size):MAX_X(maxX),MAX_Y(maxY),SIZE(size){}

void OLED::WriteByte(uint8_t dat, bool cmd)
{
	if( cmd ) HAL_GPIO_WritePin(DC_GPIO,DC_Pin, GPIO_PIN_SET );
	else HAL_GPIO_WritePin(DC_GPIO,DC_Pin,GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(CS_GPIO,CS_Pin,GPIO_PIN_RESET);
	for(uint8_t i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(SCK_GPIO,SCK_Pin,GPIO_PIN_RESET);
		
		if( dat&0x80 ) HAL_GPIO_WritePin(D1_GPIO,D1_Pin,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(D1_GPIO,D1_Pin,GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(SCK_GPIO,SCK_Pin,GPIO_PIN_SET);
		dat <<= 1;
	}
	HAL_GPIO_WritePin(CS_GPIO,CS_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DC_GPIO,DC_Pin,GPIO_PIN_SET);
}


void OLED::SetPosition(uint8_t x, uint8_t y)
{
	 WriteByte(0xb0+y, CMD);
	 WriteByte( ((x&0xf0)>>4)|0x10, CMD );
	 WriteByte( (x&0x0f)|0x01, CMD );
}

void OLED::ON(void)
{
	 WriteByte(0x8d, CMD);
	 WriteByte(0X14, CMD);  
	 WriteByte(0XAF, CMD);  
}

void OLED::OFF(void)
{
	 WriteByte(0X8D, CMD);  //SET DCDC命令
	 WriteByte(0X10, CMD);  //DCDC OFF
	 WriteByte(0XAE, CMD);  //DISPLAY OFF
}		

void OLED::Clear(void)  
{   
	for(u8 i=0;i<8;i++)  
	{  
		WriteByte (0xb0+i, CMD);    //设置页地址（0~7）
		WriteByte (0x00, CMD);      //设置显示位置—列低地址
		WriteByte (0x10, CMD);      //设置显示位置—列高地址   
		for(u8 n=0;n<128;n++)
			WriteByte(0, DATA); 
	} //更新显示
}

void OLED::ClearBlock(uint8_t x, uint8_t y)  
{  
	for(u8 i=y;i<y+2;i++)  
	{   
		SetPosition(x,i);
		for(u8 n=0;n<18;n++)
		 WriteByte(0, DATA); 
	} //更新显示
}

void OLED::ClearLine(uint8_t y)  
{  
	u8 i,n;  
	for(i=y;i<y+2;i++)  
	{  
		 WriteByte (0xb0+i, CMD);    //设置页地址（0~7）
		 WriteByte (0x00, CMD);      //设置显示位置—列低地址
		 WriteByte (0x10, CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		 WriteByte(0, DATA); 
	} //更新显示
}


void OLED::ShowJPG(uint8_t num,uint8_t **jpg)
{
	uint8_t x,y;
	uint32_t temp;
	x=0;y=0;
	temp = num * 64;
	for( u8 i=0; i<8; i++ )
	{
		 SetPosition(x,y+i);
		for( u8 j=i*8; j<8*(i+1); j++ )
		{
			for( u8 k=0; k<16; k++ )
			 WriteByte(jpg[temp][k], DATA);
			temp++;
		}
	}
}

void OLED::Show(uint8_t x,uint8_t y,uint8_t no,uint8_t **hzk)
{ 
	u8 t,adder=0;
	 SetPosition(x,y);	
    for(t=0;t<16;t++)
	{
		 WriteByte(hzk[2*no][t], DATA);
		adder+=1;
    }	
	 SetPosition(x,y+1);
    for(t=0;t<16;t++)
	{
		 WriteByte(hzk[2*no+1][t], DATA);
		adder+=1;
	}
}

void OLED::ShowChar(uint8_t x, uint8_t y,uint8_t chr)
{
	unsigned char c=0,i=0;
	c=chr-' ';//得到偏移后的值
	if(x>MAX_X-1)
	{
		x = 0;
		y = y + 2;
	}
	if(SIZE ==16)
	{
		SetPosition(x,y);
		for( i = 0;i < 8;i++)
			WriteByte(zifu[c*16+i], DATA);
		SetPosition(x,y+1);
		for(i = 0;i < 8;i++)
			WriteByte(zifu[c*16+i+8], DATA);
	}
}

void OLED::ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(int)(num/ pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		}
	 	ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 


void OLED::ClearArea(u8 x1, u8 x2, u8 y1,u8 y2)
{
	u8 deta_x = x2-x1;
	u8 deta_y = y2-y1;
	for( u8 i=0; i<= deta_y; i++ )
	{
		SetPosition(x1,y1+i);
		for( u8 n=0; n<= deta_x; n++ )
			 WriteByte(0, DATA); 
	}
}


void OLED::ShowStrings(u8 x, u8 y, u8 *p)
{
    u8 i=0;
    while( *p != '\0' )
    {
        ShowChar(x+i,y,*p);
        p++;
        i+=8;
    }
}


void OLED::ShowNums(u8 x,u8 y,float num)
{
	s16 num_int= (s16)num;
	s16 num_fra = (num - (int)num)*100;
	u8 sig =0;
	sig = num<0?1:0;
	if( sig == 1 )
	{
		num_int = - num_int;
		num_fra = - num_fra;
	  ShowChar(x,y,'-'); ShowNum(x+8,y,(u32)num_int,5,4*4); ShowChar(x+8+5*8,y,'.'); ShowNum(x+8+5*8+8,y,(u32)num_fra,2,8*2); }
	if( sig == 0 )
	{  ShowChar(x,y,' '); ShowNum(x+8,y,(u32)num_int,5,4*4); ShowChar(x+8+5*8,y,'.'); ShowNum(x+8+5*8+8,y,(u32)num_fra,2,8*2); }
}


//初始化SSD1306
void OLED::Init(void)
{
	GPIO_TypeDef *GPIOs[]{CS_GPIO,DC_GPIO,RES_GPIO,D1_GPIO,SCK_GPIO};
	uint16_t pins[]{CS_Pin,DC_Pin,RES_Pin,D1_Pin,SCK_Pin};
	for(size_t i = 0 ; i < 5 ; i++)
	{
		if(GPIOs[i] == GPIOA)
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOB)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOC)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOD)
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if(GPIOs[i] == GPIOE)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOs[i], &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOs[i],pins[i], GPIO_PIN_SET);
	}
	
	WriteByte(0xAE, CMD);//--turn off oled panel
	WriteByte(0x00, CMD);//---set low column address
	WriteByte(0x10, CMD);//---set high column address
	WriteByte(0x40, CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	WriteByte(0x81, CMD);//--set contrast control register
	WriteByte(0xCF, CMD); // Set SEG Output Current Brightness
	WriteByte(0xA1, CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	WriteByte(0xC8, CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	WriteByte(0xA6, CMD);//--set normal display
	WriteByte(0xA8, CMD);//--set multiplex ratio(1 to 64)
	WriteByte(0x3f, CMD);//--1/64 duty
	WriteByte(0xD3, CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	WriteByte(0x00, CMD);//-not offset
	WriteByte(0xd5, CMD);//--set display clock divide ratio/oscillator frequency
	WriteByte(0x80, CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	WriteByte(0xD9, CMD);//--set pre-charge period
	WriteByte(0xF1, CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	WriteByte(0xDA, CMD);//--set com pins hardware configuration
	WriteByte(0x12, CMD);
	WriteByte(0xDB, CMD);//--set vcomh
	WriteByte(0x40, CMD);//Set VCOM Deselect Level
	WriteByte(0x20, CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	WriteByte(0x02, CMD);//
	WriteByte(0x8D, CMD);//--set Charge Pump enable/disable
	WriteByte(0x14, CMD);//--set(0x10) disable
	WriteByte(0xA4, CMD);// Disable Entire Display On (0xa4/0xa5)
	WriteByte(0xA6, CMD);// Disable Inverse Display On (0xa6/a7) 
	WriteByte(0xAF, CMD);//--turn on oled panel
	WriteByte(0xAF, CMD); /*display ON*/ 
	Clear();
	SetPosition(0,0);
}  
