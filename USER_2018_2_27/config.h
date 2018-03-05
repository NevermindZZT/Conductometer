#ifndef		__CONFIG_H__
#define		__CONFIG_H__

#include 	"stm32f10x.h"

#include	"stdio.h"

#include	"delay.h"
#include	"usart.h"
#include	"tim.h"

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

#define		INPUTSTUDENTNUMBER		1
#define		TEMPERATURESETTING		2
#define		BUILDBALANCE			3
#define		HEATTING				4
#define		RECORDING				5
#define		SHOWDATA				6
#define		COMPLETE				7

typedef struct
{
	uint32_t time;
	float temperature;
}DYY_MeasuredData;

typedef struct
{
	uint8_t machineNumber;						//机器号5
	uint8_t studentNumber[12];					//学号
	uint8_t progress;							//实验进度
	float settedTemprature;						//设置加热盘温度
	float balanceTempeatrue;					//稳恒态散热盘温度
	float heatingTempeatrue;					//升温后散热盘温度
	DYY_MeasuredData measuredData[20];			//实验数据
}DYY_ExperimentalData;

extern DYY_ExperimentalData experimentalData;

extern uint8_t screenBrightness;

extern uint16_t readFlash[2];

#endif
