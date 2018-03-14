/*********************************************
*         DS18B20温度传感器驱动              *
*               适用导热仪                   *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/3                      *
*********************************************/

/*
*说明：		1.此文件为DS18B20驱动
*			2.驱动传感器需先定义传感器，使用DS18B20_TypeDef定义
*			3.驱动内定义了三个传感器，如使用此驱动至其他工程，可删除这三个传感器并根据需求自定义
*/

//#include	"ds18b20.h"
//#include	"delay.h"
#include	"config.h"


DS18B20_TypeDef DS18B20A = {DS18B20A_GPIO_PORT, DS18B20A_GPIO_PIN, DS18B20A_GPIO_CLK};			//定义三个传感器
DS18B20_TypeDef DS18B20B = {DS18B20B_GPIO_PORT, DS18B20B_GPIO_PIN, DS18B20B_GPIO_CLK};
DS18B20_TypeDef DS18B20C = {DS18B20C_GPIO_PORT, DS18B20C_GPIO_PIN, DS18B20C_GPIO_CLK};


/*********************************************
*函数名称：	DS18B20_SetIOMode
*功能：		设置IO口模式
*参数：		DS18B20x	传感器
*			IOMode		IO口模式(IN/OUT)
*返回值：	无
*********************************************/
void DS18B20_SetIOMode(DS18B20_TypeDef DS18B20x, uint8_t IOMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (IOMode == IN)																		//判断IO口模式
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;										//上拉输入
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									//推挽输出
	}
	GPIO_InitStructure.GPIO_Pin = DS18B20x.pin;												//指定引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//IO口速度
	
	RCC_APB2PeriphClockCmd(DS18B20x.clk, ENABLE);											//开启时钟
	
	GPIO_Init(DS18B20x.port, &GPIO_InitStructure);											//初始化IO口
}


/*********************************************
*函数名称：	DS18B20_Init
*功能：		初始化DS18B20
*参数：		DS18B20x		传感器
*返回值：	TRUE/FALSE
*********************************************/
bool DS18B20_Init(DS18B20_TypeDef DS18B20x)
{
	uint8_t i = 0;
	
	DS18B20_SetIOMode(DS18B20x, OUT);														//设置IO口模式为输出
	
	DS18B20DQ_HIGH;																			//初始化时序
	delay_us(2);
	DS18B20DQ_LOW;
	delay_us(750);
	DS18B20DQ_HIGH;
	
	delay_us(15);
	
	DS18B20_SetIOMode(DS18B20x, IN);														//设置IO口模式为输入
	while (DS18B20DQ_READ && i < 200)														//等待传感器响应
	{
		delay_us(1);
		i++;
	}
	if (i >= 200)																			//响应超时，返回FALSE
		return FALSE;
	
	i = 0;
	while (!DS18B20DQ_READ && i < 240)														//等待传感器响应
	{
		delay_us(1);
		i++;
	}
	if (i >= 240)																			//响应超时，返回FALSE
		return FALSE;
	
	return TRUE;																			//初始化成功，返回TRUE
}


/*********************************************
*函数名称：	DS18B20_ReadByte
*功能：		从DS18B20读取一字节数据
*参数：		DS18B20x		传感器
*返回值：	读取到的数据
*********************************************/
uint8_t DS18B20_ReadByte(DS18B20_TypeDef DS18B20x)
{
	uint8_t i, readData;
	readData = 0;
	
	for (i = 0; i < 8; i++)																	//一位一位读
	{
		readData >>= 1;
		DS18B20_SetIOMode(DS18B20x, OUT);													//设置IO口模式为输出
		DS18B20DQ_LOW;																		//读数据时序
		delay_us(2);
		DS18B20DQ_HIGH;
		
		DS18B20_SetIOMode(DS18B20x, IN);													//设置IO口模式为输入
		delay_us(10);
		
		if (DS18B20DQ_READ != 0)															//读取数据
		{
			readData |= 0x80;
		}
		delay_us(60);
	}
//	DS18B20_SetIOMode(OUT);
//	DS18B20DQ_HIGH;
	
	return readData;																		//返回读取到的数据
}


/*********************************************
*函数名称：	DS18B20_WriteByte
*功能：		写一字节数据到DS18B20
*参数：		DS18B20x		传感器
*			writeData		要写入的数据
*返回值：	无
*********************************************/
void DS18B20_WriteByte(DS18B20_TypeDef DS18B20x, uint8_t writeData)
{
	uint8_t i;
	
	DS18B20_SetIOMode(DS18B20x, OUT);														//设置IO口模式为输出
	
	for (i = 0; i < 8; i++)
	{
		DS18B20DQ_LOW;																		//写数据时序
		delay_us(2);
		if (writeData & 0x01)																//写数据
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
*参数：		DS18B20x		传感器
*返回值：	无
*********************************************/
void DS18B20_ConvertTemp(DS18B20_TypeDef DS18B20x)
{
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	
	DS18B20_WriteByte(DS18B20x, SKIPROM);													//写命令
	DS18B20_WriteByte(DS18B20x, CONVERTTEMP);												//开始转换温度
}


/*********************************************
*函数名称：	DS18B20_ReadTemp
*功能：		读取温度
*参数：		DS18B20x		传感器
*返回值：	读取到的温度
*********************************************/
float DS18B20_ReadTemp(DS18B20_TypeDef DS18B20x)
{
	uint16_t temp;
	uint8_t tempL, tempH;
	float realTemp;
	
	DS18B20_Init(DS18B20x);																	//初始化一次传感器
	delay_ms(1);
	
	DS18B20_ConvertTemp(DS18B20x);															//转换温度
//	delay_ms(100);
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	DS18B20_WriteByte(DS18B20x, SKIPROM);													//读数据
	DS18B20_WriteByte(DS18B20x, READSCRTCHPAD);
	tempL = DS18B20_ReadByte(DS18B20x);														//读取两字节数据
	tempH = DS18B20_ReadByte(DS18B20x);
	temp = (uint16_t)tempH;
	temp = (temp << 8) | tempL;
	if (tempH > 7)																			//数据处理
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

