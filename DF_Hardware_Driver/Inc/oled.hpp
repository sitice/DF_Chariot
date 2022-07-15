#ifndef __OLED_H
#define __OLED_H

#include "u8g2.h"
#include "main.h"

uint8_t u8x8_gpio_and_delay_template(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

class OLED
{
public:
	OLED(uint8_t maxX,uint8_t maxY,uint8_t size);
	OLED();
	const OLED & Set_CS_Pin(GPIO_TypeDef *CS_GPIO,uint16_t CS_Pin);
	const OLED & Set_DS_Pin(GPIO_TypeDef *DC_GPIO,uint16_t DC_Pin);
	const OLED & Set_RES_Pin(GPIO_TypeDef *RES_GPIO,uint16_t RES_Pin);
	const OLED & Set_D1_Pin(GPIO_TypeDef *D1_GPIO,uint16_t D1_Pin);
	const OLED & Set_SCK_Pin(GPIO_TypeDef *SCK_GPIO,uint16_t SCK_Pin);
	void Init(void);
	void OFF(void);
	void ON(void);
	void SetPosition(uint8_t x, uint8_t y);
	void Clear(void);
	void ClearBlock(uint8_t x, uint8_t y);
	void ClearLine(uint8_t y);
	void ClearArea(u8 x1, u8 x2, u8 y1,u8 y2);
	void Show(uint8_t x,uint8_t y,uint8_t no,uint8_t **data);
	void ShowChar(uint8_t x,uint8_t y,uint8_t no);
	void ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
	void ShowStrings(u8 x, u8 y, u8 *p);
	void ShowJPG(uint8_t num,uint8_t **jpg);
	void ShowNums(u8 x,u8 y,float num);
private:
	void WriteByte(uint8_t dat, bool cmd);
	const bool CMD = 0; 
	const bool DATA = 1;
	const uint8_t MAX_X = 128;
	const uint8_t MAX_Y = 8;
	const uint8_t SIZE = 16;
	GPIO_TypeDef *CS_GPIO = GPIOE;
	uint16_t CS_Pin = GPIO_PIN_0;
	GPIO_TypeDef *DC_GPIO = GPIOE;
	uint16_t DC_Pin = GPIO_PIN_1;
	GPIO_TypeDef *RES_GPIO = GPIOE;
	uint16_t RES_Pin = GPIO_PIN_2;
	GPIO_TypeDef *D1_GPIO = GPIOE;
	uint16_t D1_Pin = GPIO_PIN_3;
	GPIO_TypeDef *SCK_GPIO = GPIOE;
	uint16_t SCK_Pin = GPIO_PIN_4;
};



#endif



