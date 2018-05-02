#ifndef		__CONFIG_H__
#define		__CONFIG_H__

#include 	"stm32f10x.h"

#include	"stdio.h"

#include	"delay.h"
#include	"usart.h"
#include	"tim.h"
#include	"iic.h"
#include	"spi.h"

#include	"asciifont.h"
#include	"font.h"

#include	"qpylcd.h"
#include	"ds18b20.h"
#include	"esp8266.h"
#include	"support.h"
#include	"key.h"
#include	"ec11.h"
#include	"pwm.h"
#include	"eeprom.h"
#include	"beep.h"
#include	"at24cxx.h"
#include	"w25x16.h"

/*---------------------实验步骤宏定义-------------------*/
//#define		INPUTSTUDENTNUMBER		1
//#define		TEMPERATURESETTING		2
//#define		BUILDBALANCE			3
//#define		HEATTING				4
//#define		RECORDING				5
//#define		SHOWDATA				6
//#define		COMPLETE				7

enum EXPERIMENTAL_PROGRESS
{
	INPUTSTUDENTNUMBER = 1,													//输入学号
	TEMPERATURESETTING,														//设置温度
	BUILDBALANCE,															//建立稳恒态
	HEATTING,																//升温
	RECORDING,																//记录数据
	SHOWDATA,																//显示实验数据
	COMPLETE																//实验完成
};



/*---------------------变量声明----------------------*/
extern uint8_t screenBrightness;

extern uint16_t readFlash[2];

#endif
