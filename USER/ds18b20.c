/*********************************************
*         DS18B20温度传感器驱动              *
*               适用导热仪                   *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/2                      *
*********************************************/

/*
*说明：		1.此文件为DS18B20驱动
*			2.此驱动实现了读取多条总线上的DS18B20数据，可在ds18b20.h文件中定义宏，增加传感器数量，同时需要修改DS18B20_Select函数
*/

//#include	"ds18b20.h"
//#include	"delay.h"
#include	"config.h"

uint16_t DS18B20x_GPIO_CLK, DS18B20x_GPIO_PIN;
GPIO_TypeDef *DS18B20x_GPIO_PORT;



/*********************************************
*函数名称：	DS18B20_Select
*功能：		选择DS18B20端口
*参数：		DS18B20x		DS18B20编号
*返回值：	无
*********************************************/
void DS18B20_Select(uint16_t DS18B20x)
{
	switch (DS18B20x)
	{
		case DS18B20A:
		{
			DS18B20x_GPIO_CLK = DS18B20A_GPIO_CLK;
			DS18B20x_GPIO_PORT = DS18B20A_GPIO_PORT;
			DS18B20x_GPIO_PIN = DS18B20A_GPIO_PIN;
			break;
		}
		case DS18B20B:
		{
			DS18B20x_GPIO_CLK = DS18B20B_GPIO_CLK;
			DS18B20x_GPIO_PORT = DS18B20B_GPIO_PORT;
			DS18B20x_GPIO_PIN = DS18B20B_GPIO_PIN;
			break;
		}
		case DS18B20C:
		{
			DS18B20x_GPIO_CLK = DS18B20C_GPIO_CLK;
			DS18B20x_GPIO_PORT = DS18B20C_GPIO_PORT;
			DS18B20x_GPIO_PIN = DS18B20C_GPIO_PIN;
			break;
		}
		default :
		{
			break;
		}
			
	}
}


/*********************************************
*函数名称：	DS18B20_SetIOMode
*功能：		设置IO口模式
*参数：		IOMode		IO口模式(IN/OUT)
*返回值：	无
*********************************************/
void DS18B20_SetIOMode(uint8_t IOMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	DS18B20_Select(DS18B20x);
	
	if (IOMode == IN)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	GPIO_InitStructure.GPIO_Pin = DS18B20x_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(DS18B20x_GPIO_CLK, ENABLE);
	
	GPIO_Init(DS18B20x_GPIO_PORT, &GPIO_InitStructure);
}


/*********************************************
*函数名称：	DS18B20_Init
*功能：		初始化DS18B20
*参数：		DS18B20x		DS18B20编号
*返回值：	TRUE/FALSE
*********************************************/
bool DS18B20_Init(uint16_t DS18B20x)
{
	uint8_t i = 0;
	DS18B20_Select(DS18B20x);
	
	DS18B20_SetIOMode(OUT);
	
	DS18B20DQ_HIGH;
	delay_us(2);
	DS18B20DQ_LOW;
	delay_us(750);
	DS18B20DQ_HIGH;
	
	delay_us(15);
	
	DS18B20_SetIOMode(IN);
	while (DS18B20DQ_READ && i < 200)
	{
		delay_us(1);
		i++;
	}
	if (i >= 200)
		return FALSE;
	
	i = 0;
	while (!DS18B20DQ_READ && i < 240)
	{
		delay_us(1);
		i++;
	}
	if (i >= 240)
		return FALSE;
	
	return TRUE;
}


/*********************************************
*函数名称：	DS18B20_ReadByte
*功能：		从DS18B20读取一字节数据
*参数：		DS18B20x		DS18B20编号
*返回值：	读取到的数据
*********************************************/
uint8_t DS18B20_ReadByte(uint16_t DS18B20x)
{
	uint8_t i, readData;
	readData = 0;
	DS18B20_Select(DS18B20x);
	
	for (i = 0; i < 8; i++)
	{
		readData >>= 1;
		DS18B20_SetIOMode(OUT);
		DS18B20DQ_LOW;
		delay_us(2);
		DS18B20DQ_HIGH;
		
		DS18B20_SetIOMode(IN);
		delay_us(10);
		
		if (DS18B20DQ_READ != 0)
		{
			readData |= 0x80;
		}
		delay_us(60);
	}
//	DS18B20_SetIOMode(OUT);
//	DS18B20DQ_HIGH;
	
	return readData;
}


/*********************************************
*函数名称：	DS18B20_WriteByte
*功能：		写一字节数据到DS18B20
*参数：		DS18B20x		DS18B20编号
*			writeData		要写入的数据
*返回值：	无
*********************************************/
void DS18B20_WriteByte(uint16_t DS18B20x, uint8_t writeData)
{
	uint8_t i;
	
	DS18B20_Select(DS18B20x);
	
	
	DS18B20_SetIOMode(OUT);
	
	for (i = 0; i < 8; i++)
	{
		DS18B20DQ_LOW;
		delay_us(2);
		if (writeData & 0x01)
		{
			DS18B20DQ_HIGH;
		}
		else
		{
			DS18B20DQ_LOW;
		}
		delay_us(60);
		DS18B20DQ_HIGH;
		writeData >>= 1;
		delay_us(2);
	}
}


/*********************************************
*函数名称：	DS18B20_ConvertTemp
*功能：		写命令开始转换温度
*参数：		DS18B20x		DS18B20编号
*返回值：	无
*********************************************/
void DS18B20_ConvertTemp(uint16_t DS18B20x)
{
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	
	DS18B20_WriteByte(DS18B20x, SKIPROM);
	DS18B20_WriteByte(DS18B20x, CONVERTTEMP);
}


/*********************************************
*函数名称：	DS18B20_ReadTemp
*功能：		读取温度
*参数：		DS18B20x		DS18B20编号
*返回值：	读取到的温度
*********************************************/
float DS18B20_ReadTemp(uint16_t DS18B20x)
{
	uint16_t temp;
	uint8_t tempL, tempH;
	float realTemp;
	
	DS18B20_Init(DS18B20x);
	delay_ms(1);
	
	DS18B20_ConvertTemp(DS18B20x);
//	delay_ms(100);
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	DS18B20_WriteByte(DS18B20x, SKIPROM);
	DS18B20_WriteByte(DS18B20x, READSCRTCHPAD);
	tempL = DS18B20_ReadByte(DS18B20x);
	tempH = DS18B20_ReadByte(DS18B20x);
	temp = (uint16_t)tempH;
	temp = (temp << 8) | tempL;
	if (tempH > 7)
	{
		temp = ~temp;
		realTemp = (float)temp * 0.0625;
		return -realTemp;
	}
	else
	{
		realTemp = (float)temp * 0.0625;
		return realTemp;
	}
}

