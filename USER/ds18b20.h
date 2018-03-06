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

/*--------------------传感器编号宏定义----------------------*/
#define		DS18B20A				0
#define		DS18B20B				1
#define		DS18B20C				2

/*--------------------数据位模式宏定义----------------------*/
#define		IN						0
#define		OUT						1

/*--------------------传感器端口宏定义----------------------*/
#define		DS18B20A_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20A_GPIO_PORT		GPIOE
#define		DS18B20A_GPIO_PIN		GPIO_Pin_0

#define		DS18B20B_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20B_GPIO_PORT		GPIOE
#define		DS18B20B_GPIO_PIN		GPIO_Pin_1

#define		DS18B20C_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20C_GPIO_PORT		GPIOE
#define		DS18B20C_GPIO_PIN		GPIO_Pin_2

/*---------------------端口操作宏定义------------------------*/
#define		DS18B20DQ_LOW			GPIO_ResetBits(DS18B20x_GPIO_PORT, DS18B20x_GPIO_PIN)			//数据位低电平
#define		DS18B20DQ_HIGH			GPIO_SetBits(DS18B20x_GPIO_PORT, DS18B20x_GPIO_PIN)				//数据位高电平

#define		DS18B20DQ_READ			GPIO_ReadInputDataBit(DS18B20x_GPIO_PORT, DS18B20x_GPIO_PIN)	//读数据位电平

/*----------------------传感器命令宏定义-------------------*/
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

/*------------------------函数声明--------------------------*/
void DS18B20_Select(uint16_t DS18B20x);																//选择传感器

void DS18B20_SetIOMode(uint8_t IOMode);																//设置数据位模式

bool DS18B20_Init(uint16_t DS18B20x);																//初始化传感器

uint8_t DS18B20_ReadByte(uint16_t DS18B20x);														//读一字节数据

void DS18B20_WriteByte(uint16_t DS18B20x, uint8_t writeData);										//写一字节数据

void DS18B20_ConvertTemp(uint16_t DS18B20x);														//转换温度

float DS18B20_ReadTemp(uint16_t DS18B20x);															//读取温度

#endif
