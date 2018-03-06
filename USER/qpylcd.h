/*********************************************
*      QPY4827-0808液晶显示屏驱动            *
*                8位彩屏                     *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/1                      *
*********************************************/


#ifndef		__QPYLCD_H__
#define		__QPYLCD_H__

#include	"stm32f10x.h"

//#define		PRINTTOLCD																//printf函数打印到LCD,不使用或使用printf函数做他用需要注释此行	

/*--------------------------IO口宏定义---------------------*/
#define		LCDDATA0_GPIO_CLK		RCC_APB2Periph_GPIOB								//LCD数据端口宏定义
#define		LCDDATA0_GPIO_PORT		GPIOB
#define		LCDDATA0_GPIO_PIN		GPIO_Pin_0

#define		LCDDATA1_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		LCDDATA1_GPIO_PORT		GPIOB
#define		LCDDATA1_GPIO_PIN		GPIO_Pin_1

#define		LCDDATA2_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		LCDDATA2_GPIO_PORT		GPIOB
#define		LCDDATA2_GPIO_PIN		GPIO_Pin_2

#define		LCDDATAL_GPIO_CLK		RCC_APB2Periph_GPIOB								//低三位数据(根据硬件接法定义)
#define		LCDDATAL_GPIO_PORT		GPIOB
#define		LCDDATAL_GPIO_PIN		(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2)

#define		LCDDATA3_GPIO_CLK		RCC_APB2Periph_GPIOE								//LCD数据端口宏定义
#define		LCDDATA3_GPIO_PORT		GPIOE
#define		LCDDATA3_GPIO_PIN		GPIO_Pin_7

#define		LCDDATA4_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		LCDDATA4_GPIO_PORT		GPIOE
#define		LCDDATA4_GPIO_PIN		GPIO_Pin_8

#define		LCDDATA5_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		LCDDATA5_GPIO_PORT		GPIOE
#define		LCDDATA5_GPIO_PIN		GPIO_Pin_9

#define		LCDDATA6_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		LCDDATA6_GPIO_PORT		GPIOE
#define		LCDDATA6_GPIO_PIN		GPIO_Pin_10

#define		LCDDATA7_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		LCDDATA7_GPIO_PORT		GPIOE
#define		LCDDATA7_GPIO_PIN		GPIO_Pin_11

#define		LCDDATAM_GPIO_CLK		RCC_APB2Periph_GPIOE								//高五位数据(根据硬件接法定义)
#define		LCDDATAM_GPIO_PORT		GPIOE
#define		LCDDATAM_GPIO_PIN		(GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11)

#define		LCDWR_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD WR端口宏定义
#define		LCDWR_GPIO_PORT			GPIOD
#define		LCDWR_GPIO_PIN			GPIO_Pin_5

#define		LCDDISP_GPIO_CLK		RCC_APB2Periph_GPIOD								//LCD DISP端口宏定义
#define		LCDDISP_GPIO_PORT		GPIOD
#define		LCDDISP_GPIO_PIN		GPIO_Pin_6

#define		LCDRD_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD RD端口宏定义
#define		LCDRD_GPIO_PORT			GPIOD
#define		LCDRD_GPIO_PIN			GPIO_Pin_7

#define		LCDCS_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD CS端口宏定义
#define		LCDCS_GPIO_PORT			GPIOD
#define		LCDCS_GPIO_PIN			GPIO_Pin_8

#define		LCDRS_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD RS端口宏定义
#define		LCDRS_GPIO_PORT			GPIOD
#define		LCDRS_GPIO_PIN			GPIO_Pin_9

/*---------------------LCD命令宏定义------------------------*/
#define		XLABEL					0													//X坐标
#define		YLABEL					1													//Y坐标
#define		SINGELPIXEL				2													//单点写显存
#define		EIGHTPIXEL				3													//八点写显存
#define		FRONTGROUND				4													//前景颜色
#define		BACKGROUND				5													//背景颜色
#define		CONTROL					6													//控制寄存器
#define		CLR						7													//清屏

/*--------------------常用颜色宏定义-----------------------*/
#define		BLACK					0x00												//黑色
#define		BLUE					0x03												//蓝色
#define		GREEN					0x16												//绿色
#define		CYAN					0x1F												//青色
#define		RED						0xE0												//红色
#define		PURPLE					0xE3												//紫色
#define		YELLOW					0xFC												//黄色
#define		WHITE					0xFF												//白色

#define		NULL					0

/*--------------------LCD操作端口宏定义--------------------*/
#define		QPYLCDRS_LOW			GPIO_ResetBits(LCDRS_GPIO_PORT, LCDRS_GPIO_PIN)		//RS低电平
#define		QPYLCDRS_HIGH			GPIO_SetBits(LCDRS_GPIO_PORT, LCDRS_GPIO_PIN)		//RS高电平
#define		QPYLCDRD_LOW			GPIO_ResetBits(LCDRD_GPIO_PORT, LCDRD_GPIO_PIN)		//RD低电平
#define		QPYLCDRD_HIGH			GPIO_SetBits(LCDRD_GPIO_PORT, LCDRD_GPIO_PIN)		//RD高电平
#define		QPYLCDWR_LOW			GPIO_ResetBits(LCDWR_GPIO_PORT, LCDWR_GPIO_PIN)		//WR低电平
#define		QPYLCDWR_HIGH			GPIO_SetBits(LCDWR_GPIO_PORT, LCDWR_GPIO_PIN)		//WR高电平
#define		QPYLCDCS_LOW			GPIO_ResetBits(LCDCS_GPIO_PORT, LCDCS_GPIO_PIN)		//CS低电平
#define		QPYLCDCS_HIGH			GPIO_SetBits(LCDCS_GPIO_PORT, LCDCS_GPIO_PIN)		//CS高电平
#define		QPYLCDDISP_LOW			GPIO_ResetBits(LCDDISP_GPIO_PORT, LCDDISP_GPIO_PIN)	//DISP低电平
#define		QPYLCDDISP_HIGH			GPIO_SetBits(LCDDISP_GPIO_PORT, LCDDISP_GPIO_PIN)	//DISP高电平

/*----------------------结构体定义-------------------------*/
typedef struct																			//字体结构体定义
{
	uint16_t width;																		//字体宽度
	uint16_t height;																	//字体高度
}FONT;

/*------------------------变量声明--------------------------*/
#ifdef		PRINTTOLCD
extern uint16_t xLabelForPrint;
extern uint16_t yLabelForPrint;
#endif

extern FONT FONT8X16;																	//字体
extern FONT FONT16X16;
extern FONT FONT16X24;
extern FONT FONT24X24;
extern FONT FONT16X32;
extern FONT FONT32X32;

/*-------------------------函数声明------------------------*/
void QPYLCD_Init(void);																	//LCD初始化

void QPYLCD_On(bool on);																//开显示

void QPYLCD_WriteCmd(uint16_t cmd, uint16_t data);										//写命令

void QPYLCD_ProcessData(uint16_t data);													//数据写IO口

void QPYLCD_WriteData(uint16_t data);													//写数据

void QPYLCD_Clear(void);																//清屏

void QPYLCD_Control(uint8_t backDisable, uint8_t dir, uint8_t led, uint8_t luminance);	//写控制寄存器

void QPYLCD_SetFrontColor(uint8_t color);												//设置前景色

void QPYLCD_SetBackColor(uint8_t color);												//设置背景色

void QPYLCD_SetXYLabel(uint16_t xLabel, uint16_t yLabel);								//设置坐标

uint8_t QPYLCD_NewColor(uint8_t red, uint8_t green, uint8_t blue);						//生成颜色

void QPYLCD_DrawPoint(uint16_t xLabel, uint16_t yLabel, uint8_t color);					//画点

void QPYLCD_DrawLine(uint16_t startXLabel, uint16_t startYLabel, uint16_t endXLabel, uint16_t endYLabel, uint8_t color);	//画线

void QPYLCD_DrawRectangle(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color);				//绘制实心矩形

void QPYLCD_DrawMonochromeImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color, const uint8_t *image);	//绘制单色图片

void QPYLCD_DrawColorImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, const uint8_t *image);		//显示彩色图片

void QPYLCD_DisplayString(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *string);				//显示字符串(ASCII)

void QPYLCD_DisplayCharacter(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *character);			//显示汉字

void QPYLCD_DisplayCharacters(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, uint8_t characterNumber, const uint8_t *characters);	//显示多个汉字

void QPYLCD_DisplayInt(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, int32_t number);							//显示整形数据

void QPYLCD_DisplayFloat(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, float number);							//显示浮点型数据

#endif
