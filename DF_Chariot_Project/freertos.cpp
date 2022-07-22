#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "queue.h"
#include "semphr.h"
#include "pwm.h"
#include "ICM20602.hpp"
#include "u8g2.h"
#include "oled.hpp"
#include "ak8975.hpp"
#include "spl06.hpp"
#include "Motor.hpp"
#include "sd.hpp"
#include "DF_Move.hpp"
#include "ps2.hpp"
#include "DF_Filter.hpp"
#include "DF_IMU.hpp"
#include "DF_Communicate.hpp"
#include <cstring>

ICM20602 icm20602(&hspi1,GPIOD,GPIO_PIN_0);
OLED oled;
AK8975 ak8975(&hspi1,GPIOA,GPIO_PIN_4);
SPL06 spl06(&hspi2,GPIOD,GPIO_PIN_3);//这个用的是spi1吧
SD sd(&hspi2,GPIOB,GPIO_PIN_12);

EulerAngle_t angle;

Motor motor1(GPIOC,GPIO_PIN_4,GPIOC,GPIO_PIN_5,&htim1,TIM_CHANNEL_4,&htim2);
Motor motor2(GPIOE,GPIO_PIN_8,GPIOE,GPIO_PIN_10,&htim1,TIM_CHANNEL_1,&htim5);
Motor motor3(GPIOB,GPIO_PIN_2,GPIOE,GPIO_PIN_7,&htim1,TIM_CHANNEL_2,&htim4);
Motor motor4(GPIOB,GPIO_PIN_0,GPIOB,GPIO_PIN_1,&htim1,TIM_CHANNEL_3,&htim3);

static void GetDataTask(void *pram);
static void OLEDTask(void *pram);
static void MotorTask(void *param);
static void DebugTask(void *param);
static void UARTDataHandelTask(void *param);
	
extern UARTInfo_t uart1Info;
extern UARTInfo_t uart2Info;
extern UARTInfo_t uart3Info;
extern UARTInfo_t uart4Info;
extern UARTInfo_t uart5Info;

extern SemaphoreHandle_t uart1Semaphore;
extern SemaphoreHandle_t uart2Semaphore;
extern SemaphoreHandle_t uart3Semaphore;
extern SemaphoreHandle_t uart4Semaphore;
extern SemaphoreHandle_t uart5Semaphore;

extern QueueHandle_t uartQueueHandle;

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
		HAL_UART_Receive_DMA(uart1Info.uart, uart1Info.dataAddr, uart1Info.dataLength);  // 启动DMA接收
		__HAL_UART_ENABLE_IT(uart1Info.uart, UART_IT_IDLE);              // 使能空闲中断
		HAL_UART_Receive_DMA(uart2Info.uart, uart2Info.dataAddr, uart2Info.dataLength);  // 启动DMA接收
		__HAL_UART_ENABLE_IT(uart2Info.uart, UART_IT_IDLE);              // 使能空闲中断
		HAL_UART_Receive_DMA(uart3Info.uart, uart3Info.dataAddr, uart3Info.dataLength);  // 启动DMA接收
		__HAL_UART_ENABLE_IT(uart3Info.uart, UART_IT_IDLE);              // 使能空闲中断
		HAL_UART_Receive_DMA(uart4Info.uart, uart4Info.dataAddr, uart4Info.dataLength);  // 启动DMA接收
		__HAL_UART_ENABLE_IT(uart4Info.uart, UART_IT_IDLE);              // 使能空闲中断
		HAL_UART_Receive_DMA(uart5Info.uart, uart5Info.dataAddr, uart5Info.dataLength);  // 启动DMA接收
		__HAL_UART_ENABLE_IT(uart5Info.uart, UART_IT_IDLE);              // 使能空闲中断
		TIM6Init();
		HAL_TIM_Base_Start_IT(&htim6);
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		TIM2EncoderInit();
		TIM3EncoderInit();
		TIM4EncoderInit();
		TIM5EncoderInit();
		TIM8PWMInit();
		TIM9PWMInit();
		
		MX_ADC1_Init();
		//HAL_ADC_Start_DMA(&hadc1,adc1_val,4);
		vSemaphoreCreateBinary(uart1Semaphore);
		vSemaphoreCreateBinary(uart2Semaphore);
		vSemaphoreCreateBinary(uart3Semaphore);
		vSemaphoreCreateBinary(uart4Semaphore);
		vSemaphoreCreateBinary(uart5Semaphore);
		SPI1Init();
		SPI2Init();
		xTaskCreate( GetDataTask ,"GetDataTask",128,NULL,4,NULL);
        xTaskCreate( DebugTask ,"DebugTask",128,NULL,3,NULL);
		xTaskCreate( OLEDTask ,"OLEDTask",128,NULL,4,NULL);
		xTaskCreate( UARTDataHandelTask ,"UARTDataHandelTask",128,NULL,4,NULL);
//		xTaskCreate( MotorTask ,"MotorTask",128,NULL,4,NULL);
		vTaskDelete(NULL);
	}
}


static void GetDataTask(void *param)
{
	
	icm20602.Init();
	ak8975.Init();
	spl06.Init();
    TickType_t lastTick = xTaskGetTickCount();
    TickType_t period = 1 / portTICK_RATE_MS; //1ms 
	icm20602.CalibrationAll();//校准打开
	for(;;)
	{
		AK8975::Mag_t mag = ak8975.GetMagVal();
		SPL06::Baro_t baro = spl06.Updata();
		icm20602.Updata();
		ICM20602::Acc_t acc = icm20602.GetAccVal();
		ICM20602::Gyro_t gyro = icm20602.GetRadian();
		EulerAngle_t angle = IMUupdate(acc,gyro);
		vTaskDelayUntil(&lastTick,period); //绝对延时，执行固定频率的任务
	}
}
//调试数据在这个函数内发送，在其他固定频率任务内影响其执行周期，其任务优先级低
static void DebugTask(void *param)
{
    for(;;)
    {
         sendSenser(angle.roll*100, angle.pitch*100, angle.yaw*100 , 1); 
		//函数内部改一下，不要一个一个数据发送。用“UART1_SendData(data,len)”函数发送整体数据
                
                
        //		printf("%f  ",(float)(gyro.x*RawData_to_Radian));
        //		printf("%f  ",filter_gyro.x);
        //		printf("%f  ",filter_gyro.y);
        //		printf("%f  ",filter_gyro.z);
        //		
        //		printf("        %f  ",SI_gyro.x);
        //		printf("%f  ",SI_gyro.y);
        //		printf("%f \n ",SI_gyro.z);
                
        //		printf("%f  ",out_angle.roll);
        //		printf("%f  ",out_angle.pitch);
        //		printf("%f \n ",out_angle.yaw);
                
        //		printf("%f  ",PS2_Data.RX_Val);
        //		printf("%f  ",PS2_Data.RY_Val);
        //		printf("%f  ",PS2_Data.LX_Val);
        //		printf("%f  ",PS2_Data.LY_Val);
        //		printf("%d \n ",PS2_Data.Key_Val);

        //		printf("%f  ",acc.x);
        //		printf("%f  ",acc.y);
        //		printf("%f  ",acc.z);
        //		printf("%f  ",gyro.x);
        //		printf("%f  ",gyro.y);
        //		printf("%f \n ",gyro.z);

        //		printf("%d  ",mag.x);
        //		printf("%d  ",mag.y);
        //		printf("%d  \n",mag.z);
        //		printf("%f  \n",baro.alti);
    }

}

void draw(u8g2_t *u8g2)
{
	u8g2_SetFont(u8g2,u8g2_font_unifont_t_symbols);
	u8g2_DrawStr(u8g2,5, 20, "Success");  
}

static void OLEDTask(void *param)
{
	oled.Init();
	vTaskDelay(1);
	u8g2_t u8g2;
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_template);                                                                       
	u8g2_SetPowerSave(&u8g2, 0); 

	PS2_Init();
	PS2_Receive();
	for(;;)
	{
		vTaskDelay(300);
		draw(&u8g2);
		u8g2_SendBuffer(&u8g2);
//		oled.ShowChar(0,0,'A');
	}  
}


static void MotorTask(void *param)
{
	TIM1PWMInit();

	/*这两句应该可以删掉*/
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);

	motor1.Init();
	motor2.Init();
	motor3.Init();
	motor4.Init();
	
	Move.motor1 = &motor1;
	Move.motor2 = &motor2;
	Move.motor3 = &motor3;
	Move.motor4 = &motor4;
	
	for(;;)
	{
		vTaskDelay(100);
		OmnibearingMove(0,0,500);
//		motor1.SetRpm(500);
//		motor2.SetRpm(500);
//		motor3.SetRpm(0);
//		motor4.SetRpm(0);
//		printf("1 = %d\n",motor1.GetRpm());
//		printf("2 = %d\n",motor2.GetRpm());
//		printf("3 = %d\n",motor3.GetRpm());
//		printf("4 = %d\n",motor4.GetRpm());
	}
}


//采用串口空闲中断+循环dma+环形数组+队列
//此任务不要使用任何阻塞程序的代码，否则影响接收效率。如果发现数据丢失现象，加大对应串口数据缓冲区的大小
static void UARTDataHandelTask(void *param)
{
	uartQueueHandle = xQueueCreate( 10 , sizeof(UARTInfo_t));
	UARTInfo_t uartInfo;
	for(;;)
	{
		xQueueReceive(uartQueueHandle,&uartInfo,portMAX_DELAY);
		bool isOverFlow = uartInfo.lastIndex > uartInfo.index;
		uint16_t len;
		if(isOverFlow)
		{
			len = uartInfo.dataLength - uartInfo.lastIndex + uartInfo.index ;
		}
		else
		{
			len = uartInfo.index - uartInfo.lastIndex;
		}
		u8 data[len];//获取的数据
		if(isOverFlow)
		{
			memcpy(data,uartInfo.dataAddr + uartInfo.lastIndex,uartInfo.dataLength - uartInfo.lastIndex);
			memcpy(data + uartInfo.dataLength - uartInfo.lastIndex,uartInfo.dataAddr,uartInfo.index);
		}
		else
		{
			memcpy(data,uartInfo.dataAddr + uartInfo.lastIndex,len);
		}
		if(uartInfo.uart == &huart1)
		{
			//所以串口发送都在任务内进行，此函数禁止在中断中使用
			UART1_SendData(data,len);
		}
		else if(uartInfo.uart == &huart2)
		{
			UART2_SendData(data,len);
		}
		else if(uartInfo.uart == &huart3)
		{
			UART3_SendData(data,len);
		}
		else if(uartInfo.uart == &huart4)
		{
			UART4_SendData(data,len);
		}
		else if(uartInfo.uart == &huart5)
		{
			UART5_SendData(data,len);
		}
	}
}
