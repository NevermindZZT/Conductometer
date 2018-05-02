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

/*---------------------ʵ�鲽��궨��-------------------*/
//#define		INPUTSTUDENTNUMBER		1
//#define		TEMPERATURESETTING		2
//#define		BUILDBALANCE			3
//#define		HEATTING				4
//#define		RECORDING				5
//#define		SHOWDATA				6
//#define		COMPLETE				7

enum EXPERIMENTAL_PROGRESS
{
	INPUTSTUDENTNUMBER = 1,													//����ѧ��
	TEMPERATURESETTING,														//�����¶�
	BUILDBALANCE,															//�����Ⱥ�̬
	HEATTING,																//����
	RECORDING,																//��¼����
	SHOWDATA,																//��ʾʵ������
	COMPLETE																//ʵ�����
};



/*---------------------��������----------------------*/
extern uint8_t screenBrightness;

extern uint16_t readFlash[2];

#endif
