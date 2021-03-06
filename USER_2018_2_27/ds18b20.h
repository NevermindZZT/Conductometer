/*********************************************
*         DS18B20温度传感器驱动              *
*               适用导热仪                   *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/2                      *
*********************************************/

#ifndef		__DS18B20_H__
#define		__DS18B20_H__

#include	"stm32f10x.h"

#define		DS18B20A				0
#define		DS18B20B				1
#define		DS18B20C				2

#define		IN						0
#define		OUT						1

#define		DS18B20A_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20A_GPIO_PORT		GPIOE
#define		DS18B20A_GPIO_PIN		GPIO_Pin_0

#define		DS18B20B_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20B_GPIO_PORT		GPIOE
#define		DS18B20B_GPIO_PIN		GPIO_Pin_1

#define		DS18B20C_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20C_GPIO_PORT		GPIOE
#define		DS18B20C_GPIO_PIN		GPIO_Pin_2

#define		DS18B20DQ_LOW			GPIO_ResetBits(DS18B20x_GPIO_PORT, DS18B20x_GPIO_PIN)
#define		DS18B20DQ_HIGH			GPIO_SetBits(DS18B20x_GPIO_PORT, DS18B20x_GPIO_PIN)

#define		DS18B20DQ_READ			GPIO_ReadInputDataBit(DS18B20x_GPIO_PORT, DS18B20x_GPIO_PIN)

#define		READROM					0x33
#define		MATCHROM				0x55
#define		SKIPROM					0xCC
#define		SEARCHROM				0xF0
#define		ALARMSERACH				0xEC
#define		CONVERTTEMP				0x44
#define		READSCRTCHPAD			0xBE
#define		WRITESCRTCHPAD			0x4E
#define		COPYSCRATCHPAD			0x48
#define		RECALLEE				0xB8
#define		READPOWERSUPPLY			0xB4


void DS18B20_Select(uint16_t DS18B20x);

void DS18B20_SetIOMode(uint8_t IOMode);

bool DS18B20_Init(uint16_t DS18B20x);

uint8_t DS18B20_ReadByte(uint16_t DS18B20x);

void DS18B20_WriteByte(uint16_t DS18B20x, uint8_t writeData);

void DS18B20_ConvertTemp(uint16_t DS18B20x);

float DS18B20_ReadTemp(uint16_t DS18B20x);

#endif
