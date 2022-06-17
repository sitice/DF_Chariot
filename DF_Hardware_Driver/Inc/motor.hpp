#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

#define MOTOR1_A_GPIO GPIOC
#define MOTOR1_B_GPIO GPIOC

#define MOTOR2_A_GPIO GPIOE
#define MOTOR2_B_GPIO GPIOE

#define MOTOR3_A_GPIO GPIOE
#define MOTOR3_B_GPIO GPIOB

#define MOTOR4_A_GPIO GPIOB
#define MOTOR4_B_GPIO GPIOB

#define MOTOR1_A GPIO_PIN_5
#define MOTOR1_B GPIO_PIN_4

#define MOTOR2_A GPIO_PIN_10
#define MOTOR2_B GPIO_PIN_8

#define MOTOR3_A GPIO_PIN_7
#define MOTOR3_B GPIO_PIN_2

#define MOTOR4_A GPIO_PIN_1
#define MOTOR4_B GPIO_PIN_0

#define ABS(x) (x)>0?(x):-(x)



#endif