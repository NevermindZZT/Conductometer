/*********************************************
*      QPY4827-0808Һ����ʾ������            *
*                8λ����                     *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/1                      *
*********************************************/


#ifndef		__QPYLCD_H__
#define		__QPYLCD_H__

#include	"stm32f10x.h"

//#define		PRINTTOLCD																//printf������ӡ��LCD,��ʹ�û�ʹ��printf������������Ҫע�ʹ���	

/*--------------------------IO�ں궨��---------------------*/
#define		LCDDATA0_GPIO_CLK		RCC_APB2Periph_GPIOB								//LCD���ݶ˿ں궨��
#define		LCDDATA0_GPIO_PORT		GPIOB
#define		LCDDATA0_GPIO_PIN		GPIO_Pin_0

#define		LCDDATA1_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		LCDDATA1_GPIO_PORT		GPIOB
#define		LCDDATA1_GPIO_PIN		GPIO_Pin_1

#define		LCDDATA2_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		LCDDATA2_GPIO_PORT		GPIOB
#define		LCDDATA2_GPIO_PIN		GPIO_Pin_2

#define		LCDDATAL_GPIO_CLK		RCC_APB2Periph_GPIOB								//����λ����(����Ӳ���ӷ�����)
#define		LCDDATAL_GPIO_PORT		GPIOB
#define		LCDDATAL_GPIO_PIN		(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2)

#define		LCDDATA3_GPIO_CLK		RCC_APB2Periph_GPIOE								//LCD���ݶ˿ں궨��
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

#define		LCDDATAM_GPIO_CLK		RCC_APB2Periph_GPIOE								//����λ����(����Ӳ���ӷ�����)
#define		LCDDATAM_GPIO_PORT		GPIOE
#define		LCDDATAM_GPIO_PIN		(GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11)

#define		LCDWR_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD WR�˿ں궨��
#define		LCDWR_GPIO_PORT			GPIOD
#define		LCDWR_GPIO_PIN			GPIO_Pin_5

#define		LCDDISP_GPIO_CLK		RCC_APB2Periph_GPIOD								//LCD DISP�˿ں궨��
#define		LCDDISP_GPIO_PORT		GPIOD
#define		LCDDISP_GPIO_PIN		GPIO_Pin_6

#define		LCDRD_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD RD�˿ں궨��
#define		LCDRD_GPIO_PORT			GPIOD
#define		LCDRD_GPIO_PIN			GPIO_Pin_7

#define		LCDCS_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD CS�˿ں궨��
#define		LCDCS_GPIO_PORT			GPIOD
#define		LCDCS_GPIO_PIN			GPIO_Pin_8

#define		LCDRS_GPIO_CLK			RCC_APB2Periph_GPIOD								//LCD RS�˿ں궨��
#define		LCDRS_GPIO_PORT			GPIOD
#define		LCDRS_GPIO_PIN			GPIO_Pin_9

/*---------------------LCD����궨��------------------------*/
#define		XLABEL					0													//X����
#define		YLABEL					1													//Y����
#define		SINGELPIXEL				2													//����д�Դ�
#define		EIGHTPIXEL				3													//�˵�д�Դ�
#define		FRONTGROUND				4													//ǰ����ɫ
#define		BACKGROUND				5													//������ɫ
#define		CONTROL					6													//���ƼĴ���
#define		CLR						7													//����

/*--------------------������ɫ�궨��-----------------------*/
#define		BLACK					0x00												//��ɫ
#define		BLUE					0x03												//��ɫ
#define		GREEN					0x16												//��ɫ
#define		CYAN					0x1F												//��ɫ
#define		RED						0xE0												//��ɫ
#define		PURPLE					0xE3												//��ɫ
#define		YELLOW					0xFC												//��ɫ
#define		WHITE					0xFF												//��ɫ

#define		NULL					0

/*--------------------LCD�����˿ں궨��--------------------*/
#define		QPYLCDRS_LOW			GPIO_ResetBits(LCDRS_GPIO_PORT, LCDRS_GPIO_PIN)		//RS�͵�ƽ
#define		QPYLCDRS_HIGH			GPIO_SetBits(LCDRS_GPIO_PORT, LCDRS_GPIO_PIN)		//RS�ߵ�ƽ
#define		QPYLCDRD_LOW			GPIO_ResetBits(LCDRD_GPIO_PORT, LCDRD_GPIO_PIN)		//RD�͵�ƽ
#define		QPYLCDRD_HIGH			GPIO_SetBits(LCDRD_GPIO_PORT, LCDRD_GPIO_PIN)		//RD�ߵ�ƽ
#define		QPYLCDWR_LOW			GPIO_ResetBits(LCDWR_GPIO_PORT, LCDWR_GPIO_PIN)		//WR�͵�ƽ
#define		QPYLCDWR_HIGH			GPIO_SetBits(LCDWR_GPIO_PORT, LCDWR_GPIO_PIN)		//WR�ߵ�ƽ
#define		QPYLCDCS_LOW			GPIO_ResetBits(LCDCS_GPIO_PORT, LCDCS_GPIO_PIN)		//CS�͵�ƽ
#define		QPYLCDCS_HIGH			GPIO_SetBits(LCDCS_GPIO_PORT, LCDCS_GPIO_PIN)		//CS�ߵ�ƽ
#define		QPYLCDDISP_LOW			GPIO_ResetBits(LCDDISP_GPIO_PORT, LCDDISP_GPIO_PIN)	//DISP�͵�ƽ
#define		QPYLCDDISP_HIGH			GPIO_SetBits(LCDDISP_GPIO_PORT, LCDDISP_GPIO_PIN)	//DISP�ߵ�ƽ

/*----------------------�ṹ�嶨��-------------------------*/
typedef struct																			//����ṹ�嶨��
{
	uint16_t width;																		//������
	uint16_t height;																	//����߶�
}FONT;

/*------------------------��������--------------------------*/
#ifdef		PRINTTOLCD
extern uint16_t xLabelForPrint;
extern uint16_t yLabelForPrint;
#endif

extern FONT FONT8X16;																	//����
extern FONT FONT16X16;
extern FONT FONT16X24;
extern FONT FONT24X24;
extern FONT FONT16X32;
extern FONT FONT32X32;

/*-------------------------��������------------------------*/
void QPYLCD_Init(void);																	//LCD��ʼ��

void QPYLCD_On(bool on);																//����ʾ

void QPYLCD_WriteCmd(uint16_t cmd, uint16_t data);										//д����

void QPYLCD_ProcessData(uint16_t data);													//����дIO��

void QPYLCD_WriteData(uint16_t data);													//д����

void QPYLCD_Clear(void);																//����

void QPYLCD_Control(uint8_t backDisable, uint8_t dir, uint8_t led, uint8_t luminance);	//д���ƼĴ���

void QPYLCD_SetFrontColor(uint8_t color);												//����ǰ��ɫ

void QPYLCD_SetBackColor(uint8_t color);												//���ñ���ɫ

void QPYLCD_SetXYLabel(uint16_t xLabel, uint16_t yLabel);								//��������

uint8_t QPYLCD_NewColor(uint8_t red, uint8_t green, uint8_t blue);						//������ɫ

void QPYLCD_DrawPoint(uint16_t xLabel, uint16_t yLabel, uint8_t color);					//����

void QPYLCD_DrawLine(uint16_t startXLabel, uint16_t startYLabel, uint16_t endXLabel, uint16_t endYLabel, uint8_t color);	//����

void QPYLCD_DrawRectangle(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color);				//����ʵ�ľ���

void QPYLCD_DrawMonochromeImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color, const uint8_t *image);	//���Ƶ�ɫͼƬ

void QPYLCD_DrawColorImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, const uint8_t *image);		//��ʾ��ɫͼƬ

void QPYLCD_DisplayString(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *string);				//��ʾ�ַ���(ASCII)

void QPYLCD_DisplayCharacter(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *character);			//��ʾ����

void QPYLCD_DisplayCharacters(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, uint8_t characterNumber, const uint8_t *characters);	//��ʾ�������

void QPYLCD_DisplayInt(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, int32_t number);							//��ʾ��������

void QPYLCD_DisplayFloat(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, float number);							//��ʾ����������

#endif
