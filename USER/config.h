#ifndef		__CONFIG_H__
#define		__CONFIG_H__

#include 	"stm32f10x.h"

#include	"stdio.h"

#include	"delay.h"
#include	"usart.h"
#include	"tim.h"
#include	"iic.h"

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

/*------------------数据结构体定义---------------------*/
typedef struct																//单次记录数据
{
	uint32_t time;															//记录数据的时间
	float temperature;														//记录数据的温度
}DRY_MeasuredData;

typedef struct																//实验数据
{
	uint8_t machineNumber;													//机器号5
	uint8_t studentNumber[12];												//学号
	uint8_t progress;														//实验进度
	float settedTemperature;												//设置加热盘温度
	float balanceTempeatrue;												//稳恒态散热盘温度
	float heatingTempeatrue;												//升温后散热盘温度
	DRY_MeasuredData measuredData[20];										//实验数据
}DRY_ExperimentalData;

/*---------------------变量声明----------------------*/
extern DRY_ExperimentalData experimentalData;

extern uint8_t screenBrightness;

extern uint16_t readFlash[2];

#endif
