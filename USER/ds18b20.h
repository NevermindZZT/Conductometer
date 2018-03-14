/*********************************************
*         DS18B20温度传感器驱动              *
*               适用导热仪                   *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/3                      *
*********************************************/

#ifndef		__DS18B20_H__
#define		__DS18B20_H__

#include	"stm32f10x.h"


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
#define		DS18B20DQ_LOW			GPIO_ResetBits(DS18B20x.port, DS18B20x.pin)					//数据位低电平
#define		DS18B20DQ_HIGH			GPIO_SetBits(DS18B20x.port, DS18B20x.pin)					//数据位高电平

#define		DS18B20DQ_READ			GPIO_ReadInputDataBit(DS18B20x.port, DS18B20x.pin)			//读数据位电平

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

/*----------------------IO模式枚举------------------------*/
enum DS18B20_IOMode
{
	IN,																								//IO输入
	OUT																								//IO输出
};

/*--------------------传感器结构体声明-----------------------*/
typedef struct
{
	GPIO_TypeDef* port;																				//传感器数据位端口
	uint16_t pin;																					//传感器数据位引脚
	uint32_t clk;																					//传感器数据位时钟
}DS18B20_TypeDef;


/*----------------------全局变量声明-------------------------*/
extern DS18B20_TypeDef DS18B20A;
extern DS18B20_TypeDef DS18B20B;
extern DS18B20_TypeDef DS18B20C;


/*------------------------函数声明--------------------------*/

void DS18B20_SetIOMode(DS18B20_TypeDef DS18B20x, uint8_t IOMode);									//设置数据位模式

bool DS18B20_Init(DS18B20_TypeDef DS18B20x);														//初始化传感器

uint8_t DS18B20_ReadByte(DS18B20_TypeDef DS18B20x);													//读一字节数据

void DS18B20_WriteByte(DS18B20_TypeDef DS18B20x, uint8_t writeData);								//写一字节数据

void DS18B20_ConvertTemp(DS18B20_TypeDef DS18B20x);													//转换温度

float DS18B20_ReadTemp(DS18B20_TypeDef DS18B20x);													//读取温度

#endif
