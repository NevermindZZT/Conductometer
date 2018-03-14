/*********************************************
*         DS18B20�¶ȴ���������              *
*               ���õ�����                   *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/3                      *
*********************************************/

/*
*˵����		1.���ļ�ΪDS18B20����
*			2.�������������ȶ��崫������ʹ��DS18B20_TypeDef����
*			3.�����ڶ�������������������ʹ�ô��������������̣���ɾ�������������������������Զ���
*/

//#include	"ds18b20.h"
//#include	"delay.h"
#include	"config.h"


DS18B20_TypeDef DS18B20A = {DS18B20A_GPIO_PORT, DS18B20A_GPIO_PIN, DS18B20A_GPIO_CLK};			//��������������
DS18B20_TypeDef DS18B20B = {DS18B20B_GPIO_PORT, DS18B20B_GPIO_PIN, DS18B20B_GPIO_CLK};
DS18B20_TypeDef DS18B20C = {DS18B20C_GPIO_PORT, DS18B20C_GPIO_PIN, DS18B20C_GPIO_CLK};


/*********************************************
*�������ƣ�	DS18B20_SetIOMode
*���ܣ�		����IO��ģʽ
*������		DS18B20x	������
*			IOMode		IO��ģʽ(IN/OUT)
*����ֵ��	��
*********************************************/
void DS18B20_SetIOMode(DS18B20_TypeDef DS18B20x, uint8_t IOMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (IOMode == IN)																		//�ж�IO��ģʽ
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;										//��������
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									//�������
	}
	GPIO_InitStructure.GPIO_Pin = DS18B20x.pin;												//ָ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//IO���ٶ�
	
	RCC_APB2PeriphClockCmd(DS18B20x.clk, ENABLE);											//����ʱ��
	
	GPIO_Init(DS18B20x.port, &GPIO_InitStructure);											//��ʼ��IO��
}


/*********************************************
*�������ƣ�	DS18B20_Init
*���ܣ�		��ʼ��DS18B20
*������		DS18B20x		������
*����ֵ��	TRUE/FALSE
*********************************************/
bool DS18B20_Init(DS18B20_TypeDef DS18B20x)
{
	uint8_t i = 0;
	
	DS18B20_SetIOMode(DS18B20x, OUT);														//����IO��ģʽΪ���
	
	DS18B20DQ_HIGH;																			//��ʼ��ʱ��
	delay_us(2);
	DS18B20DQ_LOW;
	delay_us(750);
	DS18B20DQ_HIGH;
	
	delay_us(15);
	
	DS18B20_SetIOMode(DS18B20x, IN);														//����IO��ģʽΪ����
	while (DS18B20DQ_READ && i < 200)														//�ȴ���������Ӧ
	{
		delay_us(1);
		i++;
	}
	if (i >= 200)																			//��Ӧ��ʱ������FALSE
		return FALSE;
	
	i = 0;
	while (!DS18B20DQ_READ && i < 240)														//�ȴ���������Ӧ
	{
		delay_us(1);
		i++;
	}
	if (i >= 240)																			//��Ӧ��ʱ������FALSE
		return FALSE;
	
	return TRUE;																			//��ʼ���ɹ�������TRUE
}


/*********************************************
*�������ƣ�	DS18B20_ReadByte
*���ܣ�		��DS18B20��ȡһ�ֽ�����
*������		DS18B20x		������
*����ֵ��	��ȡ��������
*********************************************/
uint8_t DS18B20_ReadByte(DS18B20_TypeDef DS18B20x)
{
	uint8_t i, readData;
	readData = 0;
	
	for (i = 0; i < 8; i++)																	//һλһλ��
	{
		readData >>= 1;
		DS18B20_SetIOMode(DS18B20x, OUT);													//����IO��ģʽΪ���
		DS18B20DQ_LOW;																		//������ʱ��
		delay_us(2);
		DS18B20DQ_HIGH;
		
		DS18B20_SetIOMode(DS18B20x, IN);													//����IO��ģʽΪ����
		delay_us(10);
		
		if (DS18B20DQ_READ != 0)															//��ȡ����
		{
			readData |= 0x80;
		}
		delay_us(60);
	}
//	DS18B20_SetIOMode(OUT);
//	DS18B20DQ_HIGH;
	
	return readData;																		//���ض�ȡ��������
}


/*********************************************
*�������ƣ�	DS18B20_WriteByte
*���ܣ�		дһ�ֽ����ݵ�DS18B20
*������		DS18B20x		������
*			writeData		Ҫд�������
*����ֵ��	��
*********************************************/
void DS18B20_WriteByte(DS18B20_TypeDef DS18B20x, uint8_t writeData)
{
	uint8_t i;
	
	DS18B20_SetIOMode(DS18B20x, OUT);														//����IO��ģʽΪ���
	
	for (i = 0; i < 8; i++)
	{
		DS18B20DQ_LOW;																		//д����ʱ��
		delay_us(2);
		if (writeData & 0x01)																//д����
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
*������		DS18B20x		������
*����ֵ��	��
*********************************************/
void DS18B20_ConvertTemp(DS18B20_TypeDef DS18B20x)
{
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	
	DS18B20_WriteByte(DS18B20x, SKIPROM);													//д����
	DS18B20_WriteByte(DS18B20x, CONVERTTEMP);												//��ʼת���¶�
}


/*********************************************
*�������ƣ�	DS18B20_ReadTemp
*���ܣ�		��ȡ�¶�
*������		DS18B20x		������
*����ֵ��	��ȡ�����¶�
*********************************************/
float DS18B20_ReadTemp(DS18B20_TypeDef DS18B20x)
{
	uint16_t temp;
	uint8_t tempL, tempH;
	float realTemp;
	
	DS18B20_Init(DS18B20x);																	//��ʼ��һ�δ�����
	delay_ms(1);
	
	DS18B20_ConvertTemp(DS18B20x);															//ת���¶�
//	delay_ms(100);
	DS18B20_Init(DS18B20x);
//	delay_ms(1);
	DS18B20_WriteByte(DS18B20x, SKIPROM);													//������
	DS18B20_WriteByte(DS18B20x, READSCRTCHPAD);
	tempL = DS18B20_ReadByte(DS18B20x);														//��ȡ���ֽ�����
	tempH = DS18B20_ReadByte(DS18B20x);
	temp = (uint16_t)tempH;
	temp = (temp << 8) | tempL;
	if (tempH > 7)																			//���ݴ���
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

