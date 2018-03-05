/*********************************************
*         DS18B20�¶ȴ���������              *
*               ���õ�����                   *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/2                      *
*********************************************/

/*
*˵����		1.���ļ�ΪDS18B20����
*			2.������ʵ���˶�ȡ���������ϵ�DS18B20���ݣ�����ds18b20.h�ļ��ж���꣬���Ӵ�����������ͬʱ��Ҫ�޸�DS18B20_Select����
*/

//#include	"ds18b20.h"
//#include	"delay.h"
#include	"config.h"

uint16_t DS18B20x_GPIO_CLK, DS18B20x_GPIO_PIN;
GPIO_TypeDef *DS18B20x_GPIO_PORT;



/*********************************************
*�������ƣ�	DS18B20_Select
*���ܣ�		ѡ��DS18B20�˿�
*������		DS18B20x		DS18B20���
*����ֵ��	��
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
*�������ƣ�	DS18B20_SetIOMode
*���ܣ�		����IO��ģʽ
*������		IOMode		IO��ģʽ(IN/OUT)
*����ֵ��	��
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
*�������ƣ�	DS18B20_Init
*���ܣ�		��ʼ��DS18B20
*������		DS18B20x		DS18B20���
*����ֵ��	TRUE/FALSE
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
*�������ƣ�	DS18B20_ReadByte
*���ܣ�		��DS18B20��ȡһ�ֽ�����
*������		DS18B20x		DS18B20���
*����ֵ��	��ȡ��������
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
*�������ƣ�	DS18B20_WriteByte
*���ܣ�		дһ�ֽ����ݵ�DS18B20
*������		DS18B20x		DS18B20���
*			writeData		Ҫд�������
*����ֵ��	��
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
*�������ƣ�	DS18B20_ConvertTemp
*���ܣ�		д���ʼת���¶�
*������		DS18B20x		DS18B20���
*����ֵ��	��
*********************************************/
void DS18B20_ConvertTemp(uint16_t DS18B20x)
{
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	
	DS18B20_WriteByte(DS18B20x, SKIPROM);
	DS18B20_WriteByte(DS18B20x, CONVERTTEMP);
}


/*********************************************
*�������ƣ�	DS18B20_ReadTemp
*���ܣ�		��ȡ�¶�
*������		DS18B20x		DS18B20���
*����ֵ��	��ȡ�����¶�
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

