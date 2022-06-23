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
#include "ak8975.hpp"
#include "spl06.hpp"

ICM20602 icm20602(&hspi1,GPIOD,GPIO_PIN_0);
OLED oled;
AK8975 ak8975(&hspi2,GPIOD,GPIO_PIN_7);
SPL06 spl06(&hspi2,GPIOD,GPIO_PIN_8);

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
		xTaskCreate( GetDataTask ,"1",128,NULL,4,NULL);
		xTaskCreate( OLEDTask ,"1",128,NULL,4,NULL);
		vTaskDelete(NULL);
	}
}


static void GetDataTask(void *param)
{
	icm20602.Init();
	ak8975.Init();
	spl06.Init();
	for(;;)
	{
		vTaskDelay(100);
		icm20602.Updata();
		ICM20602::Acc_t acc = icm20602.GetAccVal();
		ICM20602::Gyro_t gyro = icm20602.GetGyroVal();
		AK8975::Mag_t mag = ak8975.GetMagVal();
		SPL06::Baro_t baro = spl06.Updata();
		printf("%f\n",baro.alti);
	}
}

void draw(u8g2_t *u8g2)
{
	u8g2_SetFont(u8g2,u8g2_font_unifont_t_symbols);
	u8g2_DrawStr(u8g2,5, 20, "1");
}

static void OLEDTask(void *param)
{
	oled.Init();
	vTaskDelay(1);
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
