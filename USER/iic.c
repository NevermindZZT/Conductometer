/*******************************************************************************
*                                  模拟I2C                                     *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/4                                      *
*******************************************************************************/

#include	"config.h"


/*******************************************************************************
*函数名称： IIC_Init
*功能：     模拟I2C初始化
*参数：     无
*返回值：   无
*******************************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = IIC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(IIC_GPIO_CLK, ENABLE);
	GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
	
	IIC_SclSet();
	IIC_SdaSet();
}


/*******************************************************************************
*函数名称： IIC_Start
*功能：     模拟I2C开始信号
*参数：     无
*返回值：   无
*******************************************************************************/
void IIC_Start(void)
{
	IIC_SdaOut();
	
	IIC_SdaSet();
	IIC_SclSet();
	
	delay_us(6);
	
	IIC_SdaReset();
	
	delay_us(6);
	
	IIC_SclReset();
}


/*******************************************************************************
*函数名称： IIC_Stop
*功能：     模拟I2C停止信号
*参数：     无
*返回值：   无
*******************************************************************************/
void IIC_Stop(void)
{
	IIC_SdaOut();
	
	IIC_SclReset();
	IIC_SdaReset();
	
	delay_us(6);
	
	IIC_SclSet();
	
	delay_us(6);
	
	IIC_SdaSet();
}


/*******************************************************************************
*函数名称： IIC_WaitAck
*功能：     模拟I2C等待应答
*参数：     无
*返回值：   TRUE		得到应答
*			FALSE		应答失败
*******************************************************************************/
bool IIC_WaitAck(void)
{
	uint8_t waitTime = 0;
	
	IIC_SdaIn();
	
	IIC_SdaSet();
	delay_us(1);
	IIC_SclSet();
	delay_us(1);
	
	while (IIC_SdaRead())
	{
		waitTime ++;
		if (waitTime > 250)
		{
			IIC_Stop();
			return FALSE;
		}
	}
	
	IIC_SclReset();
	
	return TRUE;
}



/*******************************************************************************
*函数名称： IIC_Ack
*功能：     模拟I2C应答信号
*参数：     无
*返回值：   无
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SclReset();
	
	IIC_SdaOut();
	
	IIC_SdaReset();
	
	delay_us(2);
	IIC_SclSet();
	delay_us(5);
	IIC_SclReset();
}



/*******************************************************************************
*函数名称： IIC_Ack
*功能：     模拟I2C不应答
*参数：     无
*返回值：   无
*******************************************************************************/
void IIC_Nack(void)
{
	IIC_SclReset();
	
	IIC_SdaOut();
	
	IIC_SdaSet();
	
	delay_us(2);
	IIC_SclSet();
	delay_us(5);
	IIC_SclReset();
}


/*******************************************************************************
*函数名称： IIC_SendByte
*功能：     模拟I2C发送一个字节
*参数：     data		数据
*返回值：   无
*******************************************************************************/
void IIC_SendByte(uint8_t data)
{
	uint8_t i;
	
	IIC_SdaOut();
	
	IIC_SclReset();
	
	for (i = 0; i < 8; i++)
	{
		if ((data & 0x80) != 0)
		{
			IIC_SdaSet();
		}
		else
		{
			IIC_SdaReset();
		}
		data <<= 1;
		
		delay_us(2);
		IIC_SclSet();
		delay_us(2);
		
		IIC_SclReset();
		delay_us(2);
	}
}


/*******************************************************************************
*函数名称： IIC_ReadByte
*功能：     模拟I2C读取一个字节
*参数：     ack			1			有应答
*						0			无应答
*返回值：   无
*******************************************************************************/
uint8_t IIC_ReadByte(uint8_t ack)
{
	uint8_t i, readData = 0;
	
	IIC_SdaIn();
	
	for (i = 0; i < 8; i++)
	{
		IIC_SclReset();
		delay_us(2);
		IIC_SclSet();
		
		readData <<= 1;
		
		if (IIC_SdaRead())
		{
			readData ++;
		}
		
		delay_us(1);
	}
	
	if (ack)
	{
		IIC_Ack();
	}
	else
	{
		IIC_Nack();
	}
	
	return readData;
}
