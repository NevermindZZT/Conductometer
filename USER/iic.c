#include	"config.h"

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


void IIC_SetSdaIOMode(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if (mode == 1)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else 
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	}
	
	GPIO_InitStructure.GPIO_Pin = IICSDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(IICSDA_GPIO_PORT, &GPIO_InitStructure);	
}


void IIC_Start(void)
{
	IIC_SdaOut();
	
	IIC_SdaSet();
	IIC_SclSet();
	
	delay_us(4);
	
	IIC_SdaReset();
	
	delay_us(4);
	
	IIC_SclReset();
}


void IIC_Stop(void)
{
	IIC_SdaOut();
	
	IIC_SclReset();
	IIC_SdaReset();
	
	delay_us(4);
	
	IIC_SclSet();
	
	IIC_SdaSet();
	
	delay_us(4);
}


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
			printf("ack failed\r\n");
			IIC_Stop();
			return FALSE;
		}
	}
	
	IIC_SclReset();
	
	printf("ack sucess\r\n");
	return TRUE;
}



void IIC_Ack(void)
{
	IIC_SclReset();
	
	IIC_SdaOut();
	
	IIC_SdaReset();
	
	delay_us(2);
	IIC_SclSet();
	delay_us(2);
	IIC_SclReset();
}



void IIC_Nack(void)
{
	IIC_SclReset();
	
	IIC_SdaOut();
	
	IIC_SdaSet();
	
	delay_us(2);
	IIC_SclSet();
	delay_us(2);
	IIC_SclReset();
}


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
