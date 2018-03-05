/*********************************************
*      QPY4827-0808Һ����ʾ������            *
*                8λ����                     *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/1                      *
*********************************************/

/*
*˵����		1.���ļ�Ϊ�人���ԿƼ�8λҺ�������������ֱ���Ϊ480*272
*			2.��Һ�������Ѿ�ʵ���˻��㣬�ߣ����Σ������ֿ����ʾASCII�ַ��ͺ���
*			3.Ĭ���ṩasciifont.c�ļ������а���8*16��16*24��16*32����ASCII�ֿ⣬��ֱ����ʾ���֣���ĸ��Ӣ�ķ���
*			4.���ں����ֿ���󣬱�������δʵ��ͨ���ֿ�ķ�ʽ��ʾ���֣������ṩ������ͨ���ṩָ�����ֵ���ģ��ʾ����
*			5.�����򵥵�ʵ����ʹ��printf������ӡ�ַ�����Һ�����Ĺ��ܣ�����Ҫʹ�ã���qpylcd.h�ļ��ж���PRINTTOLCD�꼴��
*			6.���޸�Һ������Ӧ��IO�ڣ����޸ĺ궨�壬��ע��QPYLCD_Init������QPYLCD_ProcessData��Ҫ�Ķ�
*/

//#include		"qpylcd.h"
//#include		"delay.h"
//#include		"asciifont.h"
//#include		"stdio.h"
#include	"config.h"


uint16_t xLabelForPrint = 0;
uint16_t yLabelForPrint = 0;

FONT FONT8X16 = {8, 16};
FONT FONT16X16 = {16, 16};
FONT FONT16X24 = {16, 24};
FONT FONT24X24 = {24, 24};
FONT FONT16X32 = {16, 32};
FONT FONT32X32 = {32, 32};



/***********************************************
*�������ƣ�	QPYLCD_Init
*���ܣ�		��ʼ���˿ں�Һ����
*������		��
*����ֵ��	��
***********************************************/
void QPYLCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LCDDATAL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(LCDDATAL_GPIO_CLK, ENABLE);
	
	GPIO_Init(LCDDATAL_GPIO_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = LCDDATAM_GPIO_PIN;
	
	RCC_APB2PeriphClockCmd(LCDDATAM_GPIO_CLK, ENABLE);
	
	GPIO_Init(LCDDATAM_GPIO_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LCDWR_GPIO_PIN | LCDDISP_GPIO_PIN | LCDRD_GPIO_PIN;
	
	RCC_APB2PeriphClockCmd(LCDWR_GPIO_CLK, ENABLE);
	
	GPIO_Init(LCDWR_GPIO_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = LCDCS_GPIO_PIN | LCDRS_GPIO_PIN;
	
	RCC_APB2PeriphClockCmd(LCDCS_GPIO_CLK, ENABLE);
	
	GPIO_Init(LCDCS_GPIO_PORT, &GPIO_InitStructure);
	
	QPYLCDCS_HIGH;
	QPYLCD_On(TRUE);
	QPYLCD_Clear();
	QPYLCD_Control(0, 0, 0, 2);
}


/*********************************************
*�������ƣ�	QPYLCD_On
*���ܣ�		�������Һ������ʾ
*������		on��		TRUE		��Ļ��
*						FALSE		��Ļ��
*����ֵ��	��
*********************************************/
void QPYLCD_On(bool on)
{
	if (on)
		QPYLCDDISP_HIGH;
	else
		QPYLCDDISP_LOW;
}
/**********************************************
*�������ƣ�	QPYLCD_WriteCmd
*���ܣ�		д���Һ����
*������		cmd��		����
*			data		����������޲���ΪNULL
*����ֵ��	��
**********************************************/
void QPYLCD_WriteCmd(uint16_t cmd, uint16_t data)
{
	uint8_t i, dataWidth;
	if (cmd == SINGELPIXEL ||cmd == EIGHTPIXEL)
		dataWidth = 0;
	else if (cmd == CONTROL || cmd == CLR || cmd == FRONTGROUND || cmd == BACKGROUND)
		dataWidth = 1;
	else
		dataWidth = 2;
	
	QPYLCD_ProcessData(cmd);

	QPYLCDRS_LOW;
	QPYLCDRD_HIGH;
	QPYLCDCS_LOW;
	QPYLCDWR_LOW;
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	QPYLCDWR_HIGH;
	QPYLCDCS_HIGH;
	
//	delay_us(1);						//ADD
	
	for (i = 0; i < dataWidth; i++)
	{
		QPYLCD_WriteData(data);
		data = data >> 8;
	}
	
}


/*************************************************
*�������ƣ�	QPYLCD_ProcessData
*���ܣ�		�����������Һ�����ݶ˿�
*������		data��		����
*����ֵ��	��
*************************************************/
void QPYLCD_ProcessData(uint16_t data)
{
	uint16_t dataL;
	uint16_t dataM;
	
	dataL = data & 0x0007;
	dataM = data & 0x00F8;
	
	GPIO_Write(LCDDATAL_GPIO_PORT, ((GPIO_ReadOutputData(LCDDATAL_GPIO_PORT) & (~LCDDATAL_GPIO_PIN)) | dataL));
	GPIO_Write(LCDDATAM_GPIO_PORT, ((GPIO_ReadOutputData(LCDDATAM_GPIO_PORT) & (~LCDDATAM_GPIO_PIN)) | (dataM << 4)));
}


/************************************************
*�������ƣ�	QPYLCD_WriteData
*���ܣ�		д������Һ����
*������		data��		����
*����ֵ��	��
*************************************************/
void QPYLCD_WriteData(uint16_t data)
{
	QPYLCD_ProcessData(data);
	
	QPYLCDRS_HIGH;
	QPYLCDRD_HIGH;
	QPYLCDCS_LOW;
	QPYLCDWR_LOW;
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	QPYLCDWR_HIGH;
	QPYLCDCS_HIGH;
	
//	delay_us(1);						//ADD
}


/**********************************************
*�������ƣ�	QPYLCD_Clear
*���ܣ�		����
*������		��
*����ֵ��	��
**********************************************/
void QPYLCD_Clear(void)
{
	QPYLCD_WriteCmd(CLR, 0x55);
	delay_ms(25);
	QPYLCD_WriteCmd(CLR, 0xAA);
}


/*********************************************
*�������ƣ�	QPYLCD_Control
*���ܣ�		д���ƼĴ�������Һ����
*������		backDisable��	����ɫ��ֹλ(1λ)
*			dir:			�����д�Դ淽��(1λ)
*			led��			���⿪��(1λ)
*			luminance:		��������(3λ)
*����ֵ��	��
*********************************************/
void QPYLCD_Control(uint8_t backDisable, uint8_t dir, uint8_t led, uint8_t luminance)
{
	uint8_t data;
	
	data = (backDisable << 5) | (dir << 4) | (led << 3) | luminance;
	
	QPYLCD_WriteCmd(CONTROL, data);
}


/********************************************
*�������ƣ�	QPYLCD_SetFrontColor
*���ܣ�		����ǰ����ɫ
*������		color:			��ɫ(8λ)
*����ֵ��	��
********************************************/
void QPYLCD_SetFrontColor(uint8_t color)
{
	QPYLCD_WriteCmd(FRONTGROUND, (uint16_t)color);
}


/*******************************************
*�������ƣ�	QPYLCD_SetBackColor
*���ܣ�		���ñ�����ɫ
*������		color:			��ɫ(8λ)
*����ֵ��	��
*******************************************/
void QPYLCD_SetBackColor(uint8_t color)
{
	QPYLCD_WriteCmd(BACKGROUND, (uint16_t)color);
}


/*******************************************
*�������ƣ�	QPYLCD_SetXYLabel
*���ܣ�		��������
*������		xLabel��		x������
*			yLabel:			y������
*����ֵ��	��
*******************************************/
void QPYLCD_SetXYLabel(uint16_t xLabel, uint16_t yLabel)
{
	QPYLCD_WriteCmd(XLABEL, xLabel);
	QPYLCD_WriteCmd(YLABEL, yLabel);
}


/*******************************************
*�������ƣ�	QPYLCD_NewColor
*���ܣ�		��������
*������		red��			��ɫ(3λ)
*			green:			��ɫ(3λ)
*			blue:			��ɫ(2λ)
*����ֵ��	��ɫ
*******************************************/
uint8_t QPYLCD_NewColor(uint8_t red, uint8_t green, uint8_t blue)
{
	return (red << 5) | (green << 2) | blue;
}


/************************************************
*�������ƣ�	QPYLCD_DrawPoint
*���ܣ�		��ָ�����껭��
*������		xLabel��		x������
*			yLabel:			y������
*			color��			��ɫ
*����ֵ��	��
************************************************/
void QPYLCD_DrawPoint(uint16_t xLabel, uint16_t yLabel, uint8_t color)
{
	QPYLCD_SetXYLabel(xLabel, yLabel);
	QPYLCD_WriteCmd(SINGELPIXEL, NULL);
	QPYLCD_WriteData(color);
}


/**************************************************
*�������ƣ�	QPYLCD_DrawLine
*���ܣ�		����ֱ��
*������		startXLabel:	��ʼ��x����
*			startYLabel:	��ʼ��y����
*			endXLabel��		������x����
*			endYLabel:		������y����
*			color��			��ɫ
*����ֵ��	��
**************************************************/
void QPYLCD_DrawLine(uint16_t startXLabel, uint16_t startYLabel, uint16_t endXLabel, uint16_t endYLabel, uint8_t color)
{
	uint16_t xLengthAbs, yLengthAbs, tmpLabel;
	int16_t xLength, yLength, i;
	
	xLengthAbs = xLength = (int16_t)endXLabel - (int16_t)startXLabel;
	yLengthAbs = yLength = (int16_t)endYLabel - (int16_t)startYLabel;
	
	if (xLength < 0)
		xLengthAbs = -xLength;
	if (yLength < 0)
		yLengthAbs = -yLength;
	
	if ((xLengthAbs == yLengthAbs) && (xLengthAbs == 0))
	{
		QPYLCD_DrawPoint(startXLabel, startYLabel, color);
	}
	else if (xLengthAbs >= yLengthAbs)
	{
		tmpLabel = startYLabel;
		for (i = 0; i <= xLengthAbs; i++)
		{
			QPYLCD_DrawPoint(startXLabel, startYLabel, color);
			startXLabel += xLength / xLengthAbs;
			startYLabel = tmpLabel + (int16_t)((i+1) * ((float)yLength / (float)xLengthAbs));
		}
	}
	else
	{
		
		tmpLabel = startXLabel;
		for (i = 0; i <= yLengthAbs; i++)
		{
			QPYLCD_DrawPoint(startXLabel, startYLabel, color);
			startYLabel += yLength / yLengthAbs;
			startXLabel = tmpLabel + (int16_t)((i+1) * ((float)xLength / (float)yLengthAbs));
		}
	}
}


/******************************************
*�������ƣ�	QPYLCD_DrawRectangle
*���ܣ�		����ʵ�ľ���
*������		xLabel:			x������
*			yLabel��		y������
*			width��			���
*			height��		�߶�
*			color��			��ɫ
*����ֵ��	��
******************************************/
void QPYLCD_DrawRectangle(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color)
{
	uint16_t i, j;
	
	QPYLCD_SetFrontColor(color);
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);
		
		for (j = 0; j < (width / 8); j++)													//8��д�Դ����8��������
		{
			QPYLCD_WriteCmd(EIGHTPIXEL, NULL);
			QPYLCD_WriteData(0xFF);
		}
		for (j = 0; j < (width % 8) ; j++)													//����д�Դ����ʣ�ಿ��
		{
			QPYLCD_WriteCmd(SINGELPIXEL, NULL);
			QPYLCD_WriteData(color);
		}
	}
		
}


/****************************************************
*�������ƣ�	QPYLCD_DrawMonochromeImage
*���ܣ�		��ʾ��ɫͼƬ
*������		xLabel��		x������
*			yLabel��		y������
*			width��			���
*			height��		�߶�
*			color��			��ɫ
*			*image��		ͼƬ��ģ
*����ֵ��	��
****************************************************/
void QPYLCD_DrawMonochromeImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color, const uint8_t *image)
{
	uint16_t i, j;
	
	QPYLCD_SetFrontColor(color);
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);
		
		QPYLCD_WriteCmd(EIGHTPIXEL, NULL);
		
		for (j = 0; j < (width / 8); j++)	
		{
			QPYLCD_WriteData(*image++);
		}
	}
			
}


/*****************************************
*�������ƣ�	QPYLCD_DrawColorImage
*���ܣ�		��ʾ��ɫͼƬ
*������		xLabel��		x������
*			yLabel��		y������
*			width��			���
*			height��		�߶�
*			*image��		ͼƬ��ģ
*����ֵ��	��
*****************************************/
void QPYLCD_DrawColorImage(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, const uint8_t *image)
{
	uint16_t i, j;
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);
		
		QPYLCD_WriteCmd(SINGELPIXEL, NULL);
		
		for (j = 0; j < width; j++)	
		{
			QPYLCD_WriteData(*image++);
		}
	}
	
}


/****************************************
*�������ƣ�	QPYLCD_DisplayString
*���ܣ�		��ʾ�ַ���(��׼ASCII��)
*������		xLabel��		x������
*			yLabel��		y������
*			color��			��ɫ
*			font��			����
*			*string��		�ַ���
****************************************/
void QPYLCD_DisplayString(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *string)
{
	const uint8_t *p;
	uint8_t i;
	if (font.width == 8 && font.height == 16)
	{
		p = asciiFont8x16;
	}
	else if (font.width == 16 && font.height == 24)
	{
		p = asciiFont16x24;
	}
	else if (font.width == 16 && font.height == 32)
	{
		p = asciiFont16x32;
	}
	
	while (*string)
	{
		for (i = 0; i < (font.width / 8); i++)
		{
			QPYLCD_DrawMonochromeImage(xLabel, yLabel, 8, font.height, color,
				p + (*string - 32) * font.width * font.height / 8 + font.height * i);
			xLabel += 8;
		}
		string++;
	}
}


/*******************************************
*�������ƣ�	QPYLCD_DisplayCharacter
*���ܣ�		��ʾ����
*������		xLabel��		x������
*			yLabel��		y������
*			color��			��ɫ
*			font��			����
*			*character��	������ģ
*******************************************/
void QPYLCD_DisplayCharacter(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, const uint8_t *character)
{
	uint8_t i;
	for (i = 0; i < (font.width / 8); i++)
	{
		QPYLCD_DrawMonochromeImage((xLabel + i * 8), yLabel, 8, font.height, color, (character + i * font.height));
	}
}


/*******************************************
*�������ƣ�	QPYLCD_DisplayCharacters
*���ܣ�		��ʾ����
*������		xLabel��			x������
*			yLabel��			y������
*			color��				��ɫ
*			font��				����
*			characterNumber��	��������
*			*characters��		������ģ
*******************************************/
void QPYLCD_DisplayCharacters(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, uint8_t characterNumber, const uint8_t *characters)
{
	uint8_t i;
	for (i = 0; i < characterNumber; i++)
	{
		QPYLCD_DisplayCharacter(xLabel + font.width * i, yLabel, color, font, characters + i * font.height * font.width / 8);
	}
}


/**********************************************
*�������ƣ�	QPYLCD_DisplayInt
*���ܣ�		��ʾ��������
*������		number��			����
*			xLabel��			x������
*			yLabel��			y������
*			color��				��ɫ
*����ֵ��	��
**********************************************/
void QPYLCD_DisplayInt(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, int32_t number)
{
	uint8_t tmp[10];
	int8_t	i, j, m;
	i = 0;
	
	if (number == 0)
	{
		tmp[i++] = 48;
	}
	else if (number < 0)
	{
		tmp[i++] = '-';
	}
	
	while (number > 0)
	{
		tmp[i++] = number % 10 + 48;
		number /= 10;
	}
	
	for (j = 0; j < (i / 2); j++)
	{
		m = tmp[j];
		tmp[j] = tmp[i - j - 1];
		tmp[i - j - 1] = m;
	}
	tmp[i] = NULL;
	
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);
}


/**********************************************
*�������ƣ�	QPYLCD_DisplayFloat
*���ܣ�		��ʾ��������
*������		number��			����
*			xLabel��			x������
*			yLabel��			y������
*			color��				��ɫ
*����ֵ��	��
**********************************************/
void QPYLCD_DisplayFloat(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, float number)
{
	uint8_t tmp[20];
	
	sprintf((char *)tmp, "%f", number);
	
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);
}


/***********************************************
*�������ƣ�	fputc
*���ܣ�		��дfputc������ʵ��printf����
***********************************************/
#ifdef		PRINTTOLCD
int fputc(int ch, FILE *f)
{
	if ((uint8_t)ch == '\n')
	{
		yLabelForPrint += 16;
		xLabelForPrint = 0;
	}
	else
	{
		QPYLCD_SetBackColor(BLACK);
		QPYLCD_DrawMonochromeImage(xLabelForPrint, yLabelForPrint, 8, 16, WHITE, asciiFont8x16 + (ch - 32) * 16);
		xLabelForPrint += 8;
		if (xLabelForPrint >= 480)
		{
			yLabelForPrint += 16;
			xLabelForPrint = 0;
		}
	}
	if (yLabelForPrint >= 272)
	{
		yLabelForPrint = 0;
		QPYLCD_Clear();
	}
	return ch;
}
#endif
