#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwm.h"
#include "ICM20602.hpp"
#include "u8g2.h"
#include "oled.hpp"


ICM20602 *icm20602 = nullptr;

OLED oled;

static void GetDataTask(void *pram);
static void OLEDTask(void *pram);

void defaultTask(void *param)
{
	for(;;)
	{
		/* Initialize all configured peripherals */
		DMAInit();
		UART4Init();
		UART5Init();
		USART1Init();
		USART2Init();
		USART3Init();
		TIM6Init();
		HAL_TIM_Base_Start_IT(&htim6);
		TIM1PWMInit();
		TIM8PWMInit();
		TIM9PWMInit();
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
		
		SetPWM(&htim1,TIM_CHANNEL_1,1000);
		SetPWM(&htim1,TIM_CHANNEL_2,1000);
		SetPWM(&htim1,TIM_CHANNEL_3,1000);
		SetPWM(&htim1,TIM_CHANNEL_4,1000);
		SetPWM(&htim8,TIM_CHANNEL_1,1000);
		SetPWM(&htim8,TIM_CHANNEL_2,1000);
		SetPWM(&htim8,TIM_CHANNEL_3,1000);
		SetPWM(&htim8,TIM_CHANNEL_4,1000);
		SetPWM(&htim9,TIM_CHANNEL_1,1000);
		SetPWM(&htim9,TIM_CHANNEL_2,1000);
		
		MX_ADC1_Init();
		//HAL_ADC_Start_DMA(&hadc1,adc1_val,4);
		
		SPI1Init();
		SPI2Init();
		xTaskCreate( OLEDTask ,"1",128,NULL,4,NULL);
		vTaskDelete(NULL);
	}
}


static void GetDataTask(void *param)
{
	icm20602 = new ICM20602(&hspi1,GPIOD,GPIO_PIN_0);
	for(;;)
	{
		vTaskDelay(100);
		icm20602->Updata();
		printf("%d\n",icm20602->acc.x);
	}
}

void draw(u8g2_t *u8g2)
{
//    u8g2_SetFontMode(u8g2, 1);
//    u8g2_SetFontDirection(u8g2, 0); 
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); 
//    u8g2_DrawStr(u8g2, 0, 20, "U");
//    
//    u8g2_SetFontDirection(u8g2, 1);
//    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
//    u8g2_DrawStr(u8g2, 21,8,"8");
//        
//    u8g2_SetFontDirection(u8g2, 0);
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(u8g2, 51,30,"g");
//    u8g2_DrawStr(u8g2, 67,30,"\xb2");
//    
//    u8g2_DrawHLine(u8g2, 2, 35, 47);
//    u8g2_DrawHLine(u8g2, 3, 36, 47);
//    u8g2_DrawVLine(u8g2, 45, 32, 12);
//    u8g2_DrawVLine(u8g2, 46, 33, 12);
//  
//    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
//    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");


	u8g2_SetFont(u8g2,u8g2_font_unifont_t_symbols);
	u8g2_DrawStr(u8g2,5, 20, "1");

	
}
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
		HAL_Delay(arg_int);
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

static void OLEDTask(void *param)
{
	oled.Init();
	HAL_Delay(1);
	u8g2_t u8g2;
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_template);                                                                       
	u8g2_SetPowerSave(&u8g2, 0);  
	for(;;)
	{
		vTaskDelay(12);
		draw(&u8g2);
		u8g2_SendBuffer(&u8g2);
	}
}
