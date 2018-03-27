/*********************************************
*              矩阵按键驱动                  *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/2                      *
*********************************************/

#ifndef		__KEY_H__
#define		__KEY_H__

#include	"config.h"

//#define		KEY_DOUBLE_CLICK														//允许双击

#define		KEYC_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYC_GPIO_PORT		GPIOA
#define		KEYC_GPIO_PIN		GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3

#define		KEYR_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYR_GPIO_PORT		GPIOA
#define		KEYR_GPIO_PIN		GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7

#define		KEYC0_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYC0_GPIO_PORT		GPIOA
#define		KEYC0_GPIO_PIN		GPIO_Pin_0

#define		KEYC1_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYC1_GPIO_PORT		GPIOA
#define		KEYC1_GPIO_PIN		GPIO_Pin_1

#define		KEYC2_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYC2_GPIO_PORT		GPIOA
#define		KEYC2_GPIO_PIN		GPIO_Pin_2

#define		KEYC3_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYC3_GPIO_PORT		GPIOA
#define		KEYC3_GPIO_PIN		GPIO_Pin_3

#define		KEYR0_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYR0_GPIO_PORT		GPIOA
#define		KEYR0_GPIO_PIN		GPIO_Pin_4

#define		KEYR1_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYR1_GPIO_PORT		GPIOA
#define		KEYR1_GPIO_PIN		GPIO_Pin_5

#define		KEYR2_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYR2_GPIO_PORT		GPIOA
#define		KEYR2_GPIO_PIN		GPIO_Pin_6

#define		KEYR3_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		KEYR3_GPIO_PORT		GPIOA
#define		KEYR3_GPIO_PIN		GPIO_Pin_7

#define		KEY_DEFALUT			0
#define		KEY_ENTER			16
#define		KEY_RIGHT			3
#define		KEY_LEFT			7
#define		KEY_COUNT			12
#define		KEY_UP				13

#define		KEY_ENTER_LONG		36
#define		KEY_RIGHT_LONG		23
#define		KEY_LEFT_LONG		27
#define		KEY_COUNT_LONG		32
#define		KEY_UP_LONG			33

#define		EC11_UP				61
#define		EC11_DOWN			59

void KEY_Init(void);

void KEY_Write(uint8_t data);

uint8_t KEY_Read(void);

uint8_t KEY_Scan(void);

uint8_t KEYANDEC11_Scan(void);
	
#endif

