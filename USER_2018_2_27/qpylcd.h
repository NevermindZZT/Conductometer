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

//#define		PRINTTOLCD												//printf函数打印到LCD,不使用或使用printf函数做他用需要注释此行	

#define		LCDDATA0_GPIO_CLK		RCC_APB2Periph_GPIOB			//
#define		LCDDATA0_GPIO_PORT		GPIOB
#define		LCDDATA0_GPIO_PIN		GPIO_Pin_0

#define		LCDDATA1_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		LCDDATA1_GPIO_PORT		GPIOB
#define		LCDDATA1_GPIO_PIN		GPIO_Pin_1

#define		LCDDATA2_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		LCDDATA2_GPIO_PORT		GPIOB
#define		LCDDATA2_GPIO_PIN		GPIO_Pin_2

#define		LCDDATAL_GPIO_CLK		RCC_APB2Periph_GPIOB			//低三位数据
#define		LCDDATAL_GPIO_PORT		GPIOB
#define		LCDDATAL_GPIO_PIN		(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2)

#define		LCDDATA3_GPIO_CLK		RCC_APB2Periph_GPIOE			//
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

#define		LCDDATAM_GPIO_CLK		RCC_APB2Periph_GPIOE			//高五位数据
#define		LCDDATAM_GPIO_PORT		GPIOE
#define		LCDDATAM_GPIO_PIN		(GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11)

#define		LCDWR_GPIO_CLK			RCC_APB2Periph_GPIOD			//
#define		LCDWR_GPIO_PORT			GPIOD
#define		LCDWR_GPIO_PIN			GPIO_Pin_5

#define		LCDDISP_GPIO_CLK		RCC_APB2Periph_GPIOD
#define		LCDDISP_GPIO_PORT		GPIOD
#define		LCDDISP_GPIO_PIN		GPIO_Pin_6

#define		LCDRD_GPIO_CLK			RCC_APB2Periph_GPIOD
#define		LCDRD_GPIO_PORT			GPIOD
#define		LCDRD_GPIO_PIN			GPIO_Pin_7

#define		LCDCS_GPIO_CLK			RCC_APB2Periph_GPIOD
#define		LCDCS_GPIO_PORT			GPIOD
#define		LCDCS_GPIO_PIN			GPIO_Pin_8

#define		LCDRS_GPIO_CLK			RCC_APB2Periph_GPIOD
#define		LCDRS_GPIO_PORT			GPIOD
#define		LCDRS_GPIO_PIN			GPIO_Pin_9


#define		XLABEL					0
#define		YLABEL					1
#define		SINGELPIXEL				2
#define		EIGHTPIXEL				3
#define		FRONTGROUND				4
#define		BACKGROUND				5
#define		CONTROL					6
#define		CLR						7

#define		BLACK					0x00
#define		BLUE					0x03
#define		GREEN					0x16
#define		CYAN					0x1F
#define		RED						0xE0
#define		PURPLE					0xE3
#define		YELLOW					0xFC
#define		WHITE					0xFF

#define		NULL					0

#define		QPYLCDRS_LOW			GPIO_ResetBits(LCDRS_GPIO_PORT, LCDRS_GPIO_PIN)
#define		QPYLCDRS_HIGH			GPIO_SetBits(LCDRS_GPIO_PORT, LCDRS_GPIO_PIN)
#define		QPYLCDRD_LOW			GPIO_ResetBits(LCDRD_GPIO_PORT, LCDRD_GPIO_PIN)
#define		QPYLCDRD_HIGH			GPIO_SetBits(LCDRD_GPIO_PORT, LCDRD_GPIO_PIN)
#define		QPYLCDWR_LOW			GPIO_ResetBits(LCDWR_GPIO_PORT, LCDWR_GPIO_PIN)
#define		QPYLCDWR_HIGH			GPIO_SetBits(LCDWR_GPIO_PORT, LCDWR_GPIO_PIN)
#define		QPYLCDCS_LOW			GPIO_ResetBits(LCDCS_GPIO_PORT, LCDCS_GPIO_PIN)
#define		QPYLCDCS_HIGH			GPIO_SetBits(LCDCS_GPIO_PORT, LCDCS_GPIO_PIN)
#define		QPYLCDDISP_LOW			GPIO_ResetBits(LCDDISP_GPIO_PORT, LCDDISP_GPIO_PIN)
#define		QPYLCDDISP_HIGH			GPIO_SetBits(LCDDISP_GPIO_PORT, LCDDISP_GPIO_PIN)

typedef struct
{
	uint16_t width;
	uint16_t height;
}FONT;

extern uint16_t xLabelForPrint;
extern uint16_t yLabelForPrint;
extern FONT FONT8X16;
extern FONT FONT16X16;
extern FONT FONT16X24;
extern FONT FONT24X24;
extern FONT FONT16X32;
extern FONT FONT32X32;


void QPYLCD_Init(void);

void QPYLCD_On(uint8_t on);

void QPYLCD_WriteCmd(uint16_t cmd, uint16_t data);

void QPYLCD_ProcessData(uint16_t data);

void QPYLCD_WriteData(uint16_t data);

void QPYLCD_Clear(void);

void QPYLCD_Control(uint8_t backDisable, uint8_t dir, uint8_t led, uint8_t luminance);

void QPYLCD_SetFrontColor(uint8_t color);

void QPYLCD_SetBackColor(uint8_t color);

void QPYLCD_SetXYLabel(uint16_t xLabel, uint16_t yLabel);

uint8_t QPYLCD_NewColor(uint8_t red, uint8_t green, uint8_t blue);

void QPYLCD_DrawPoint(uint16_t xLabel, uint16_t yLabel, uint8_t color);

void QPYLCD_DrawLine(uint16_t startXLabel, uint16_t startYLabel, uint16_t endXLabel, uint16_t endYLabel, uint8_t color);

void QPYLCD_DrawRectangle(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color);

void QPYLCD_DrawMonochromeImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color, const uint8_t *image);

void QPYLCD_DrawColorImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, const uint8_t *image);

void QPYLCD_DisplayString(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *string);

void QPYLCD_DisplayCharacter(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *character);

void QPYLCD_DisplayCharacters(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, uint8_t characterNumber, const uint8_t *characters);

void QPYLCD_DisplayInt(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, int32_t number);

void QPYLCD_DisplayFloat(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, float number);

#endif
