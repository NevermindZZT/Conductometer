/*********************************************
*      QPY4827-0808液晶显示屏驱动            *
*                8位彩屏                     *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/1                      *
*********************************************/

/*
*说明：		1.此文件为武汉中显科技8位液晶彩屏驱动，分辨率为480*272
*			2.此液晶驱动已经实现了画点，线，矩形，依赖字库可显示ASCII字符和汉字
*			3.默认提供asciifont.c文件，其中包括8*16，16*24，16*32三种ASCII字库，可直接显示数字，字母和英文符号
*			4.由于汉字字库过大，本驱动尚未实现通过字库的方式显示汉字，而是提供函数，通过提供指定汉字的字模显示汉字
*			5.驱动简单地实现了使用printf函数打印字符串到液晶屏的功能，若需要使用，在qpylcd.h文件中定义PRINTTOLCD宏即可
*			6.若修改液晶屏对应的IO口，需修改宏定义，并注意QPYLCD_Init函数与QPYLCD_ProcessData需要改动
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
*函数名称：	QPYLCD_Init
*功能：		初始化端口和液晶屏
*参数：		无
*返回值：	无
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
*函数名称：	QPYLCD_On
*功能：		软件控制液晶屏显示
*参数：		on：		TRUE		屏幕开
*						FALSE		屏幕关
*返回值：	无
*********************************************/
void QPYLCD_On(bool on)
{
	if (on)
		QPYLCDDISP_HIGH;
	else
		QPYLCDDISP_LOW;
}
/**********************************************
*函数名称：	QPYLCD_WriteCmd
*功能：		写命令到液晶屏
*参数：		cmd：		命令
*			data		命令参数，无参数为NULL
*返回值：	无
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
*函数名称：	QPYLCD_ProcessData
*功能：		将数据输出至液晶数据端口
*参数：		data：		数据
*返回值：	无
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
*函数名称：	QPYLCD_WriteData
*功能：		写数据至液晶屏
*参数：		data：		数据
*返回值：	无
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
*函数名称：	QPYLCD_Clear
*功能：		清屏
*参数：		无
*返回值：	无
**********************************************/
void QPYLCD_Clear(void)
{
	QPYLCD_WriteCmd(CLR, 0x55);
	delay_ms(25);
	QPYLCD_WriteCmd(CLR, 0xAA);
}


/*********************************************
*函数名称：	QPYLCD_Control
*功能：		写控制寄存器配置液晶屏
*参数：		backDisable：	背景色禁止位(1位)
*			dir:			单点读写显存方向(1位)
*			led：			背光开关(1位)
*			luminance:		背光亮度(3位)
*返回值：	无
*********************************************/
void QPYLCD_Control(uint8_t backDisable, uint8_t dir, uint8_t led, uint8_t luminance)
{
	uint8_t data;
	
	data = (backDisable << 5) | (dir << 4) | (led << 3) | luminance;
	
	QPYLCD_WriteCmd(CONTROL, data);
}


/********************************************
*函数名称：	QPYLCD_SetFrontColor
*功能：		设置前景颜色
*参数：		color:			颜色(8位)
*返回值：	无
********************************************/
void QPYLCD_SetFrontColor(uint8_t color)
{
	QPYLCD_WriteCmd(FRONTGROUND, (uint16_t)color);
}


/*******************************************
*函数名称：	QPYLCD_SetBackColor
*功能：		设置背景颜色
*参数：		color:			颜色(8位)
*返回值：	无
*******************************************/
void QPYLCD_SetBackColor(uint8_t color)
{
	QPYLCD_WriteCmd(BACKGROUND, (uint16_t)color);
}


/*******************************************
*函数名称：	QPYLCD_SetXYLabel
*功能：		设置坐标
*参数：		xLabel：		x轴坐标
*			yLabel:			y轴坐标
*返回值：	无
*******************************************/
void QPYLCD_SetXYLabel(uint16_t xLabel, uint16_t yLabel)
{
	QPYLCD_WriteCmd(XLABEL, xLabel);
	QPYLCD_WriteCmd(YLABEL, yLabel);
}


/*******************************************
*函数名称：	QPYLCD_NewColor
*功能：		设置坐标
*参数：		red：			红色(3位)
*			green:			绿色(3位)
*			blue:			蓝色(2位)
*返回值：	颜色
*******************************************/
uint8_t QPYLCD_NewColor(uint8_t red, uint8_t green, uint8_t blue)
{
	return (red << 5) | (green << 2) | blue;
}


/************************************************
*函数名称：	QPYLCD_DrawPoint
*功能：		在指定坐标画点
*参数：		xLabel：		x轴坐标
*			yLabel:			y轴坐标
*			color：			颜色
*返回值：	无
************************************************/
void QPYLCD_DrawPoint(uint16_t xLabel, uint16_t yLabel, uint8_t color)
{
	QPYLCD_SetXYLabel(xLabel, yLabel);
	QPYLCD_WriteCmd(SINGELPIXEL, NULL);
	QPYLCD_WriteData(color);
}


/**************************************************
*函数名称：	QPYLCD_DrawLine
*功能：		绘制直线
*参数：		startXLabel:	起始点x坐标
*			startYLabel:	起始点y坐标
*			endXLabel：		结束点x坐标
*			endYLabel:		结束点y坐标
*			color：			颜色
*返回值：	无
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
*函数名称：	QPYLCD_DrawRectangle
*功能：		绘制实心矩形
*参数：		xLabel:			x轴坐标
*			yLabel：		y轴坐标
*			width：			宽度
*			height：		高度
*			color：			颜色
*返回值：	无
******************************************/
void QPYLCD_DrawRectangle(uint16_t xLabel, uint16_t yLabel, uint16_t width, uint16_t height, uint8_t color)
{
	uint16_t i, j;
	
	QPYLCD_SetFrontColor(color);
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);
		
		for (j = 0; j < (width / 8); j++)													//8点写显存绘制8的整数列
		{
			QPYLCD_WriteCmd(EIGHTPIXEL, NULL);
			QPYLCD_WriteData(0xFF);
		}
		for (j = 0; j < (width % 8) ; j++)													//单点写显存绘制剩余部分
		{
			QPYLCD_WriteCmd(SINGELPIXEL, NULL);
			QPYLCD_WriteData(color);
		}
	}
		
}


/****************************************************
*函数名称：	QPYLCD_DrawMonochromeImage
*功能：		显示单色图片
*参数：		xLabel：		x轴坐标
*			yLabel：		y轴坐标
*			width：			宽度
*			height：		高度
*			color：			颜色
*			*image：		图片字模
*返回值：	无
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
*函数名称：	QPYLCD_DrawColorImage
*功能：		显示彩色图片
*参数：		xLabel：		x轴坐标
*			yLabel：		y轴坐标
*			width：			宽度
*			height：		高度
*			*image：		图片字模
*返回值：	无
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
*函数名称：	QPYLCD_DisplayString
*功能：		显示字符串(标准ASCII码)
*参数：		xLabel：		x轴坐标
*			yLabel：		y轴坐标
*			color：			颜色
*			font：			字体
*			*string：		字符串
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
*函数名称：	QPYLCD_DisplayCharacter
*功能：		显示汉字
*参数：		xLabel：		x轴坐标
*			yLabel：		y轴坐标
*			color：			颜色
*			font：			字体
*			*character：	汉字字模
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
*函数名称：	QPYLCD_DisplayCharacters
*功能：		显示汉字
*参数：		xLabel：			x轴坐标
*			yLabel：			y轴坐标
*			color：				颜色
*			font：				字体
*			characterNumber：	汉字数量
*			*characters：		汉字字模
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
*函数名称：	QPYLCD_DisplayInt
*功能：		显示整形数据
*参数：		number：			数据
*			xLabel：			x轴坐标
*			yLabel：			y轴坐标
*			color：				颜色
*返回值：	无
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
*函数名称：	QPYLCD_DisplayFloat
*功能：		显示整形数据
*参数：		number：			数据
*			xLabel：			x轴坐标
*			yLabel：			y轴坐标
*			color：				颜色
*返回值：	无
**********************************************/
void QPYLCD_DisplayFloat(uint16_t xLabel, uint16_t yLabel, uint8_t color, FONT font, float number)
{
	uint8_t tmp[20];
	
	sprintf((char *)tmp, "%f", number);
	
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);
}


/***********************************************
*函数名称：	fputc
*功能：		重写fputc函数，实现printf函数
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
