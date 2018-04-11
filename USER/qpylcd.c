/*********************************************
*      QPY4827-0808Һ����ʾ������            *
*                8λ����                     *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/4                      *
*********************************************/

/**
*˵����		1.���ļ�Ϊ�人���ԿƼ�8λҺ�������������ֱ���Ϊ480*272
*			2.��Һ�������Ѿ�ʵ���˻��㣬�ߣ����Σ������ֿ����ʾASCII�ַ��ͺ���
*			3.Ĭ���ṩasciifont.c�ļ������а���8*16��16*24��16*32����ASCII�ֿ⣬��ֱ����ʾ���֣���ĸ��Ӣ�ķ���
*			4.���ں����ֿ���󣬱�������δʵ��ͨ���ֿ�ķ�ʽ��ʾ���֣������ṩ������ͨ���ṩָ�����ֵ���ģ��ʾ����
*			5.�����򵥵�ʵ����ʹ��printf������ӡ�ַ�����Һ�����Ĺ��ܣ�����Ҫʹ�ã���qpylcd.h�ļ��ж���PRINTTOLCD�꼴��
*			6.���޸�Һ������Ӧ��IO�ڣ����޸ĺ궨�壬��ע��QPYLCD_Init������QPYLCD_ProcessData��Ҫ�Ķ�
*/

/**
*���ģ�     2018/4/11���޸�ʱ���Լ�IO�ڲ���Ϊ�Ĵ�����ʽ�������Ļ����ʾ�ٶ�
*/

//#include		"qpylcd.h"
//#include		"delay.h"
//#include		"asciifont.h"
//#include		"stdio.h"
#include	"config.h"

#ifdef		PRINTTOLCD
uint16_t xLabelForPrint = 0;																//����fputc������д
uint16_t yLabelForPrint = 0;
#endif

FONT FONT8X16 = {8, 16};																	//��������
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
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;										//��ʼ��LCD���ж˿�
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
	
	QPYLCDCS_HIGH;																			//����CS���ţ�Ĭ�ϱ���
	QPYLCD_On(TRUE);																		//����ʾ
	QPYLCD_Clear();																			//����
	QPYLCD_Control(0, 0, 0, 2);																//����Ĭ�����ȼ���2
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
		QPYLCDDISP_HIGH;																	//DISP���Ÿߵ�ƽ������ʾ
	else
		QPYLCDDISP_LOW;																		//DISP���ŵص�Ʒ������ʾ
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
	uint8_t i, dataWidth;																	//���ݿ��
	if (cmd == SINGELPIXEL ||cmd == EIGHTPIXEL)												//���������ж����ݿ��
		dataWidth = 0;
	else if (cmd == CONTROL || cmd == CLR || cmd == FRONTGROUND || cmd == BACKGROUND)
		dataWidth = 1;
	else
		dataWidth = 2;
	
	QPYLCD_ProcessData(cmd);																//����д��IO��

	QPYLCDRS_LOW;																			//д����ʱ��
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
	
	for (i = 0; i < dataWidth; i++)															//8λһ��д����
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
//	uint16_t dataL;
//	uint16_t dataM;
//	
//	dataL = data & 0x0007;																	//����LCD����λ��MCU�����ӷ�ʽ��ȷ�����ݴ���ʽ
//	dataM = data & 0x00F8;
	
    LCDDATAL_GPIO_PORT -> ODR = (LCDDATAL_GPIO_PORT -> ODR & (~LCDDATAL_GPIO_PIN)) | (data & 0x0007);
    LCDDATAM_GPIO_PORT -> ODR = (LCDDATAM_GPIO_PORT -> ODR & (~LCDDATAM_GPIO_PIN)) | ((data & 0x00F8) << 4);
    
//	GPIO_Write(LCDDATAL_GPIO_PORT, ((GPIO_ReadOutputData(LCDDATAL_GPIO_PORT) & (~LCDDATAL_GPIO_PIN)) | dataL));
//	GPIO_Write(LCDDATAM_GPIO_PORT, ((GPIO_ReadOutputData(LCDDATAM_GPIO_PORT) & (~LCDDATAM_GPIO_PIN)) | (dataM << 4)));
}


/************************************************
*�������ƣ�	QPYLCD_WriteData
*���ܣ�		д������Һ����
*������		data��		����
*����ֵ��	��
*************************************************/
void QPYLCD_WriteData(uint16_t data)
{
	QPYLCD_ProcessData(data);																//����д��IO��
	
	QPYLCDRS_HIGH;																			//д����ʱ��
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
	QPYLCD_WriteCmd(CLR, 0x55);																//��ʼ����
	delay_ms(25);																			//�ȴ��������
	QPYLCD_WriteCmd(CLR, 0xAA);																//�ر�����
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
	
	QPYLCD_WriteCmd(CONTROL, data);															//CONTOL����д���ƼĴ���
}


/********************************************
*�������ƣ�	QPYLCD_SetFrontColor
*���ܣ�		����ǰ����ɫ
*������		color:			��ɫ(8λ)
*����ֵ��	��
********************************************/
void QPYLCD_SetFrontColor(uint8_t color)
{
	QPYLCD_WriteCmd(FRONTGROUND, (uint16_t)color);											//FONTGROUND��������ǰ��ɫ
}


/*******************************************
*�������ƣ�	QPYLCD_SetBackColor
*���ܣ�		���ñ�����ɫ
*������		color:			��ɫ(8λ)
*����ֵ��	��
*******************************************/
void QPYLCD_SetBackColor(uint8_t color)
{
	QPYLCD_WriteCmd(BACKGROUND, (uint16_t)color);											//BACKGROUND�������ñ���ɫ
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
	QPYLCD_WriteCmd(XLABEL, xLabel);														//�ֱ�����x�������y������
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
	return (red << 5) | (green << 2) | blue;												//������ɫ
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
	QPYLCD_SetXYLabel(xLabel, yLabel);														//��������
	QPYLCD_WriteCmd(SINGELPIXEL, NULL);														//����д�Դ�����
	QPYLCD_WriteData(color);																//д�Դ�
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
	uint16_t xLengthAbs, yLengthAbs, tmpLabel;												//x���򳤶ȣ�y���򳤶ȣ������м����
	int16_t xLength, yLength, i;															//x��y���������ֵ
	
	xLengthAbs = xLength = (int16_t)endXLabel - (int16_t)startXLabel;						//����x��y���������ֵ
	yLengthAbs = yLength = (int16_t)endYLabel - (int16_t)startYLabel;
	
	if (xLength < 0)																		//����ȡ��
		xLengthAbs = -xLength;
	if (yLength < 0)
		yLengthAbs = -yLength;
	
	if ((xLengthAbs == yLengthAbs) && (xLengthAbs == 0))									//����ֹ������һ�£�������
	{
		QPYLCD_DrawPoint(startXLabel, startYLabel, color);
	}
	else if (xLengthAbs >= yLengthAbs)														//��x���򳤶ȴ���y������x������Ϊ��׼����
	{
		tmpLabel = startYLabel;																//��¼��ʼy����
		for (i = 0; i <= xLengthAbs; i++)
		{
			QPYLCD_DrawPoint(startXLabel, startYLabel, color);								//�ڵ�ǰλ�û���һ����
			startXLabel += xLength / xLengthAbs;											//x���������(��)1(����ֹ�������������߷���)
			startYLabel = tmpLabel + (int16_t)((i+1) * ((float)yLength / (float)xLengthAbs));	//������ֹ���������б�ʣ������һ��y����
		}
	}
	else																					//��y���򳤶ȴ���x������y������Ϊ��׼����
	{                                                                                       
		tmpLabel = startXLabel;                                                             //��¼��ʼx����
		for (i = 0; i <= yLengthAbs; i++)                                                   
		{                                                                                   
			QPYLCD_DrawPoint(startXLabel, startYLabel, color);                              //�ڵ�ǰλ�û���һ����
			startYLabel += yLength / yLengthAbs;                                            //y���������(��)1(����ֹ�������������߷���)
			startXLabel = tmpLabel + (int16_t)((i+1) * ((float)xLength / (float)yLengthAbs));	//������ֹ���������б�ʣ������һ��x����;
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
	
	QPYLCD_SetFrontColor(color);															//����ǰ��ɫ
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);												//����ÿһ��������꣬y���������1
		
		for (j = 0; j < (width / 8); j++)													//8��д�Դ����8��������
		{
			QPYLCD_WriteCmd(EIGHTPIXEL, NULL);												//8��д�Դ�����
			QPYLCD_WriteData(0xFF);															//8��ȫ�����Ϊǰ��ɫ
		}
		for (j = 0; j < (width % 8) ; j++)													//����д�Դ����ʣ�ಿ��
		{
			QPYLCD_WriteCmd(SINGELPIXEL, NULL);												//����д�Դ�����
			QPYLCD_WriteData(color);														//��䵥����ɫ
		}
	}
		
}


/****************************************************
*�������ƣ�	QPYLCD_DrawMonochromeImage
*���ܣ�		��ʾ��ɫͼƬ(8��д�Դ淽ʽ)
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
	
	QPYLCD_SetFrontColor(color);															//����ǰ��ɫ
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);												//����������꣬y���������1
		
		QPYLCD_WriteCmd(EIGHTPIXEL, NULL);													//�˵�д�Դ�����
		
		for (j = 0; j < (width / 8); j++)													//���/8���˺���Ҫ������Ҫ��8�ı���
		{
			QPYLCD_WriteData(*image++);														//д�Դ�
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
	
	for (i = 0; i < height; i++)															//ͼƬ�߶�
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);												//����������꣬y���������1
		
		QPYLCD_WriteCmd(SINGELPIXEL, NULL);													//����д�Դ�����
		
		for (j = 0; j < width; j++)															//ͼƬ���
		{
			QPYLCD_WriteData(*image++);														//д�Դ�
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
	const uint8_t *p;																		//�ֿ�ָ��
	uint8_t i;
    uint16_t xLabelCopy;
    
	xLabelCopy = xLabel;
    
	if (font.width == 8 && font.height == 16)												//����font������ֵ���ж����壬�������ֿ�ָ��ָ���Ӧ�ֿ�
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
	
	while (*string)																			//�ж��ַ����Ƿ����
	{
        if (*string == '\n')                                                                //���з��ж�
        {
            xLabel = xLabelCopy;
            yLabel += font.height;
        }
        else
        {
		for (i = 0; i < (font.width / 8); i++)												//���ֿ�ȡģ��ʽȷ��
            {
                QPYLCD_DrawMonochromeImage(xLabel, yLabel, 8, font.height, color,
                    p + (*string - 32) * font.width * font.height / 8 + font.height * i);		//��������ȷ��ÿ�л��ƶ����У����Ƶ�ɫͼ��
                xLabel += 8;																	//x���������8
            }
        }
		string++;																			//�ַ�ָ���1
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
	for (i = 0; i < (font.width / 8); i++)													//��ȡģ��ʽȷ����ʾ˳��
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
	for (i = 0; i < characterNumber; i++)													//��ʾָ����������
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
	int8_t	i, j, m, flag = 0;
	i = 0;
	
	if (number == 0)																		//������Ϊ0��ֱ�������ַ�
	{
		tmp[i++] = 48;
	}
	else if (number < 0)																	//������С��0���ַ�����λ����Ϊ'-'
	{
		flag = 1;																			//��¼ֵС��0
//		tmp[i++] = '-';																		
		number = -number;																	//��¼���ź�ȡ����ֵ
	}
	
	while (number > 0)																		//���������ݴ�����ַ���
	{
		tmp[i++] = number % 10 + 48;
		number /= 10;
	}
	
	for (j = 0; j < (i / 2); j++)															//�����ַ���˳��
	{
		m = tmp[j];
		tmp[j] = tmp[i - j - 1];
		tmp[i - j - 1] = m;
	}
	tmp[i] = NULL;																			//�ַ�������
	
	if (flag == 1)																			//����С��0����ʾ����
	{
		QPYLCD_DisplayString(xLabel, yLabel, color, font, "-");
		xLabel += font.width;
	}
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);									//��ʾ
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
	
	sprintf((char *)tmp, "%f", number);														//ʹ��sprintf����ת��С��Ϊ�ַ���
	
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);									//��ʾ
}


/***********************************************
*�������ƣ�	fputc
*���ܣ�		��дfputc������ʵ��printf����
***********************************************/
#ifdef		PRINTTOLCD
int fputc(int ch, FILE *f)
{
	if ((uint8_t)ch == '\n')																//�������з�������
	{
		yLabelForPrint += 16;																//y���������16
		xLabelForPrint = 0;																	//x������������
	}
	else
	{
		QPYLCD_SetBackColor(BLACK);															//���ñ���ɫ
		QPYLCD_DrawMonochromeImage(xLabelForPrint, yLabelForPrint, 8, 16, WHITE, asciiFont8x16 + (ch - 32) * 16);	//��ʾһ���ַ�
		xLabelForPrint += 8;																//x���������8
		if (xLabelForPrint >= 480)															//����ʾ��ȳ�����Ļ���򣬻���
		{
			yLabelForPrint += 16;
			xLabelForPrint = 0;
		}
	}
	if (yLabelForPrint >= 272)																//��ȫ������������ʾ��ˢ��������Ļ
	{
		yLabelForPrint = 0;
		QPYLCD_Clear();
	}
	return ch;
}
#endif
