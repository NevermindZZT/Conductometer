/*********************************************
*      QPY4827-0808液晶显示屏驱动            *
*                8位彩屏                     *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/4                      *
*********************************************/

/**
*说明：		1.此文件为武汉中显科技8位液晶彩屏驱动，分辨率为480*272
*			2.此液晶驱动已经实现了画点，线，矩形，依赖字库可显示ASCII字符和汉字
*			3.默认提供asciifont.c文件，其中包括8*16，16*24，16*32三种ASCII字库，可直接显示数字，字母和英文符号
*			4.由于汉字字库过大，本驱动尚未实现通过字库的方式显示汉字，而是提供函数，通过提供指定汉字的字模显示汉字
*			5.驱动简单地实现了使用printf函数打印字符串到液晶屏的功能，若需要使用，在qpylcd.h文件中定义PRINTTOLCD宏即可
*			6.若修改液晶屏对应的IO口，需修改宏定义，并注意QPYLCD_Init函数与QPYLCD_ProcessData需要改动
*/

/**
*更改：     2018/4/11：修改时序以及IO口操作为寄存器方式，提高屏幕的显示速度
*/

//#include		"qpylcd.h"
//#include		"delay.h"
//#include		"asciifont.h"
//#include		"stdio.h"
#include	"config.h"

#ifdef		PRINTTOLCD
uint16_t xLabelForPrint = 0;																//用作fputc函数重写
uint16_t yLabelForPrint = 0;
#endif

FONT FONT8X16 = {8, 16};																	//定义字体
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
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;										//初始化LCD所有端口
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
	
	QPYLCDCS_HIGH;																			//拉高CS引脚，默认保持
	QPYLCD_On(TRUE);																		//开显示
	QPYLCD_Clear();																			//清屏
	QPYLCD_Control(0, 0, 0, 2);																//设置默认亮度级别2
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
		QPYLCDDISP_HIGH;																	//DISP引脚高电平，开显示
	else
		QPYLCDDISP_LOW;																		//DISP引脚地点品，关显示
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
	uint8_t i, dataWidth;																	//数据宽度
	if (cmd == SINGELPIXEL ||cmd == EIGHTPIXEL)												//根据命令判断数据宽度
		dataWidth = 0;
	else if (cmd == CONTROL || cmd == CLR || cmd == FRONTGROUND || cmd == BACKGROUND)
		dataWidth = 1;
	else
		dataWidth = 2;
	
	QPYLCD_ProcessData(cmd);																//命令写入IO口

	QPYLCDRS_LOW;																			//写命令时序
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
	
	for (i = 0; i < dataWidth; i++)															//8位一次写数据
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
//	uint16_t dataL;
//	uint16_t dataM;
//	
//	dataL = data & 0x0007;																	//根据LCD数据位与MCU的连接方式，确定数据处理方式
//	dataM = data & 0x00F8;
	
    LCDDATAL_GPIO_PORT -> ODR = (LCDDATAL_GPIO_PORT -> ODR & (~LCDDATAL_GPIO_PIN)) | (data & 0x0007);
    LCDDATAM_GPIO_PORT -> ODR = (LCDDATAM_GPIO_PORT -> ODR & (~LCDDATAM_GPIO_PIN)) | ((data & 0x00F8) << 4);
    
//	GPIO_Write(LCDDATAL_GPIO_PORT, ((GPIO_ReadOutputData(LCDDATAL_GPIO_PORT) & (~LCDDATAL_GPIO_PIN)) | dataL));
//	GPIO_Write(LCDDATAM_GPIO_PORT, ((GPIO_ReadOutputData(LCDDATAM_GPIO_PORT) & (~LCDDATAM_GPIO_PIN)) | (dataM << 4)));
}


/************************************************
*函数名称：	QPYLCD_WriteData
*功能：		写数据至液晶屏
*参数：		data：		数据
*返回值：	无
*************************************************/
void QPYLCD_WriteData(uint16_t data)
{
	QPYLCD_ProcessData(data);																//数据写入IO口
	
	QPYLCDRS_HIGH;																			//写数据时序
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
	QPYLCD_WriteCmd(CLR, 0x55);																//开始清屏
	delay_ms(25);																			//等待清屏完成
	QPYLCD_WriteCmd(CLR, 0xAA);																//关闭清屏
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
	
	QPYLCD_WriteCmd(CONTROL, data);															//CONTOL命令写控制寄存器
}


/********************************************
*函数名称：	QPYLCD_SetFrontColor
*功能：		设置前景颜色
*参数：		color:			颜色(8位)
*返回值：	无
********************************************/
void QPYLCD_SetFrontColor(uint8_t color)
{
	QPYLCD_WriteCmd(FRONTGROUND, (uint16_t)color);											//FONTGROUND命令设置前景色
}


/*******************************************
*函数名称：	QPYLCD_SetBackColor
*功能：		设置背景颜色
*参数：		color:			颜色(8位)
*返回值：	无
*******************************************/
void QPYLCD_SetBackColor(uint8_t color)
{
	QPYLCD_WriteCmd(BACKGROUND, (uint16_t)color);											//BACKGROUND命令设置背景色
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
	QPYLCD_WriteCmd(XLABEL, xLabel);														//分别设置x轴坐标和y轴坐标
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
	return (red << 5) | (green << 2) | blue;												//返回颜色
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
	QPYLCD_SetXYLabel(xLabel, yLabel);														//设置坐标
	QPYLCD_WriteCmd(SINGELPIXEL, NULL);														//单点写显存命令
	QPYLCD_WriteData(color);																//写显存
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
	uint16_t xLengthAbs, yLengthAbs, tmpLabel;												//x方向长度，y方向长度，坐标中间变量
	int16_t xLength, yLength, i;															//x，y方向坐标差值
	
	xLengthAbs = xLength = (int16_t)endXLabel - (int16_t)startXLabel;						//计算x，y方向坐标差值
	yLengthAbs = yLength = (int16_t)endYLabel - (int16_t)startYLabel;
	
	if (xLength < 0)																		//长度取正
		xLengthAbs = -xLength;
	if (yLength < 0)
		yLengthAbs = -yLength;
	
	if ((xLengthAbs == yLengthAbs) && (xLengthAbs == 0))									//若起止点坐标一致，画单点
	{
		QPYLCD_DrawPoint(startXLabel, startYLabel, color);
	}
	else if (xLengthAbs >= yLengthAbs)														//若x方向长度大于y方向，以x方向作为基准划线
	{
		tmpLabel = startYLabel;																//记录起始y坐标
		for (i = 0; i <= xLengthAbs; i++)
		{
			QPYLCD_DrawPoint(startXLabel, startYLabel, color);								//在当前位置绘制一个点
			startXLabel += xLength / xLengthAbs;											//x方向坐标加(减)1(由起止点坐标差决定画线方向)
			startYLabel = tmpLabel + (int16_t)((i+1) * ((float)yLength / (float)xLengthAbs));	//根据起止点坐标计算斜率，算出下一点y坐标
		}
	}
	else																					//若y方向长度大于x方向，以y方向作为基准划线
	{                                                                                       
		tmpLabel = startXLabel;                                                             //记录起始x坐标
		for (i = 0; i <= yLengthAbs; i++)                                                   
		{                                                                                   
			QPYLCD_DrawPoint(startXLabel, startYLabel, color);                              //在当前位置绘制一个点
			startYLabel += yLength / yLengthAbs;                                            //y方向坐标加(减)1(由起止点坐标差决定画线方向)
			startXLabel = tmpLabel + (int16_t)((i+1) * ((float)xLength / (float)yLengthAbs));	//根据起止点坐标计算斜率，算出下一点x坐标;
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
	
	QPYLCD_SetFrontColor(color);															//设置前景色
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);												//设置每一行起点坐标，y方向坐标加1
		
		for (j = 0; j < (width / 8); j++)													//8点写显存绘制8的整数列
		{
			QPYLCD_WriteCmd(EIGHTPIXEL, NULL);												//8点写显存命令
			QPYLCD_WriteData(0xFF);															//8点全部填充为前景色
		}
		for (j = 0; j < (width % 8) ; j++)													//单点写显存绘制剩余部分
		{
			QPYLCD_WriteCmd(SINGELPIXEL, NULL);												//单点写显存命令
			QPYLCD_WriteData(color);														//填充单点颜色
		}
	}
		
}


/****************************************************
*函数名称：	QPYLCD_DrawMonochromeImage
*功能：		显示单色图片(8点写显存方式)
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
	
	QPYLCD_SetFrontColor(color);															//设置前景色
	
	for (i = 0; i < height; i++)
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);												//设置起点坐标，y方向坐标加1
		
		QPYLCD_WriteCmd(EIGHTPIXEL, NULL);													//八点写显存命令
		
		for (j = 0; j < (width / 8); j++)													//宽度/8，此函数要求宽度需要是8的倍数
		{
			QPYLCD_WriteData(*image++);														//写显存
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
	
	for (i = 0; i < height; i++)															//图片高度
	{
		QPYLCD_SetXYLabel(xLabel, yLabel++);												//设置起点坐标，y方向坐标加1
		
		QPYLCD_WriteCmd(SINGELPIXEL, NULL);													//单点写显存命令
		
		for (j = 0; j < width; j++)															//图片宽度
		{
			QPYLCD_WriteData(*image++);														//写显存
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
	const uint8_t *p;																		//字库指针
	uint8_t i;
    uint16_t xLabelCopy;
    
	xLabelCopy = xLabel;
    
	if (font.width == 8 && font.height == 16)												//根据font参数的值，判断字体，并设置字库指针指向对应字库
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
	
	while (*string)																			//判断字符串是否结束
	{
        if (*string == '\n')                                                                //换行符判定
        {
            xLabel = xLabelCopy;
            yLabel += font.height;
        }
        else
        {
		for (i = 0; i < (font.width / 8); i++)												//由字库取模方式确定
            {
                QPYLCD_DrawMonochromeImage(xLabel, yLabel, 8, font.height, color,
                    p + (*string - 32) * font.width * font.height / 8 + font.height * i);		//根据字体确定每列绘制多少行，绘制单色图形
                xLabel += 8;																	//x方向坐标加8
            }
        }
		string++;																			//字符指针加1
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
	for (i = 0; i < (font.width / 8); i++)													//由取模方式确定显示顺序
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
	for (i = 0; i < characterNumber; i++)													//显示指定数量汉字
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
	int8_t	i, j, m, flag = 0;
	i = 0;
	
	if (number == 0)																		//若数据为0，直接设置字符
	{
		tmp[i++] = 48;
	}
	else if (number < 0)																	//若数据小于0，字符串首位设置为'-'
	{
		flag = 1;																			//记录值小于0
//		tmp[i++] = '-';																		
		number = -number;																	//记录负号后取绝对值
	}
	
	while (number > 0)																		//将整形数据处理成字符串
	{
		tmp[i++] = number % 10 + 48;
		number /= 10;
	}
	
	for (j = 0; j < (i / 2); j++)															//整理字符串顺序
	{
		m = tmp[j];
		tmp[j] = tmp[i - j - 1];
		tmp[i - j - 1] = m;
	}
	tmp[i] = NULL;																			//字符串结束
	
	if (flag == 1)																			//数据小于0，显示负号
	{
		QPYLCD_DisplayString(xLabel, yLabel, color, font, "-");
		xLabel += font.width;
	}
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);									//显示
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
	
	sprintf((char *)tmp, "%f", number);														//使用sprintf函数转换小数为字符串
	
	QPYLCD_DisplayString(xLabel, yLabel, color, font, tmp);									//显示
}


/***********************************************
*函数名称：	fputc
*功能：		重写fputc函数，实现printf函数
***********************************************/
#ifdef		PRINTTOLCD
int fputc(int ch, FILE *f)
{
	if ((uint8_t)ch == '\n')																//遇到换行符，换行
	{
		yLabelForPrint += 16;																//y方向坐标加16
		xLabelForPrint = 0;																	//x方向坐标重置
	}
	else
	{
		QPYLCD_SetBackColor(BLACK);															//设置背景色
		QPYLCD_DrawMonochromeImage(xLabelForPrint, yLabelForPrint, 8, 16, WHITE, asciiFont8x16 + (ch - 32) * 16);	//显示一个字符
		xLabelForPrint += 8;																//x方向坐标加8
		if (xLabelForPrint >= 480)															//若显示宽度超出屏幕区域，换行
		{
			yLabelForPrint += 16;
			xLabelForPrint = 0;
		}
	}
	if (yLabelForPrint >= 272)																//若全屏都已用作显示，刷新整个屏幕
	{
		yLabelForPrint = 0;
		QPYLCD_Clear();
	}
	return ch;
}
#endif
