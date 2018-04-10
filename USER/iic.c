/*******************************************************************************
*                                  ģ��I2C                                     *
*                           ���ϴ�ѧ���������ѧԺ                             *
*                                  �ſ�ǿ                                      *
*                                  2018/4                                      *
*******************************************************************************/

#include	"config.h"


/*******************************************************************************
*�������ƣ� IIC_Init
*���ܣ�     ģ��I2C��ʼ��
*������     ��
*����ֵ��   ��
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
*�������ƣ� IIC_Start
*���ܣ�     ģ��I2C��ʼ�ź�
*������     ��
*����ֵ��   ��
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
*�������ƣ� IIC_Stop
*���ܣ�     ģ��I2Cֹͣ�ź�
*������     ��
*����ֵ��   ��
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
*�������ƣ� IIC_WaitAck
*���ܣ�     ģ��I2C�ȴ�Ӧ��
*������     ��
*����ֵ��   TRUE		�õ�Ӧ��
*			FALSE		Ӧ��ʧ��
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
*�������ƣ� IIC_Ack
*���ܣ�     ģ��I2CӦ���ź�
*������     ��
*����ֵ��   ��
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
*�������ƣ� IIC_Ack
*���ܣ�     ģ��I2C��Ӧ��
*������     ��
*����ֵ��   ��
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
*�������ƣ� IIC_SendByte
*���ܣ�     ģ��I2C����һ���ֽ�
*������     data		����
*����ֵ��   ��
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
*�������ƣ� IIC_ReadByte
*���ܣ�     ģ��I2C��ȡһ���ֽ�
*������     ack			1			��Ӧ��
*						0			��Ӧ��
*����ֵ��   ��
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
