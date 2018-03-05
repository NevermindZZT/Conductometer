/*********************************************
*              ���󰴼�����                  *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/2                      *
*********************************************/

#include	"key.h"


/***********************************************
*�������ƣ�	KEY_Init
*���ܣ�		��ʼ������
*������		��
*����ֵ��	��
***********************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = KEYC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(KEYC_GPIO_CLK, ENABLE);
	GPIO_Init(KEYC_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEYR_GPIO_PIN;
	
	RCC_APB2PeriphClockCmd(KEYR_GPIO_CLK, ENABLE);
	GPIO_Init(KEYR_GPIO_PORT, &GPIO_InitStructure);
}


/***********************************************
*�������ƣ�	KEY_Write
*���ܣ�		д���ݵ�����
*������		data			��д�������
*����ֵ��	��
***********************************************/
void KEY_Write(uint8_t data)
{
	GPIO_WriteBit(KEYC0_GPIO_PORT, KEYC0_GPIO_PIN, (BitAction)(data & 0x01));
	GPIO_WriteBit(KEYC1_GPIO_PORT, KEYC1_GPIO_PIN, (BitAction)(data & 0x02));
	GPIO_WriteBit(KEYC2_GPIO_PORT, KEYC2_GPIO_PIN, (BitAction)(data & 0x04));
	GPIO_WriteBit(KEYC3_GPIO_PORT, KEYC3_GPIO_PIN, (BitAction)(data & 0x08));
}


/***********************************************
*�������ƣ�	KEY_Read
*���ܣ�		�����ݵ�����
*������		��
*����ֵ��	����������
***********************************************/
uint8_t KEY_Read(void)
{
	uint8_t data = 0;
	data |= GPIO_ReadInputDataBit(KEYR0_GPIO_PORT, KEYR0_GPIO_PIN);
	data |= GPIO_ReadInputDataBit(KEYR1_GPIO_PORT, KEYR1_GPIO_PIN) << 1;
	data |= GPIO_ReadInputDataBit(KEYR2_GPIO_PORT, KEYR2_GPIO_PIN) << 2;
	data |= GPIO_ReadInputDataBit(KEYR3_GPIO_PORT, KEYR3_GPIO_PIN) << 3;
	return data;
}


/***********************************************
*�������ƣ�	KEY_Scan
*���ܣ�		ɨ�谴��
*������		��
*����ֵ��	������ֵ(1~16,0Ϊ�ް�������)
***********************************************/
uint8_t KEY_Scan(void)	
{
	uint8_t i;
	uint8_t scanData = 0;
	const uint8_t writeData[] = {0x0E, 0x0D, 0x0B, 0x07};
	
	for (i = 0; i < 4; i++)
	{
		KEY_Write(writeData[i]);
		delay_ms(10);

		if (KEY_Read() != 0x0F)
			delay_ms(10);
		
		switch (KEY_Read())
		{
			case 0x0E:
				scanData += 1;
				return scanData;
//				break;
			
			case 0x0D:
				scanData += 2;
				return scanData;
//				break;
			
			case 0x0B:
				scanData += 3;
				return scanData;
//				break;
			
			case 0x07:
				scanData += 4;
				return scanData;
//				break;
			
			default:
				break;
		}
		scanData += 4;
	}
	return 0;
}


/***********************************************
*�������ƣ�	KEYANDEC11_Scan
*���ܣ�		ɨ�谴����EC11������
*������		��
*����ֵ��	��ֵ(1~23,0Ϊ�ް������£�17~23ΪEC11״̬)
***********************************************/
uint8_t KEYANDEC11_Scan(void)	
{
	static uint8_t lastKeyValue;
	uint8_t i;
	uint8_t scanData = 0;
	uint8_t keyValue = 0;
	int8_t ec11Data;
	const uint8_t writeData[] = {0x0E, 0x0D, 0x0B, 0x07};
	uint16_t count;
	
	for (i = 0; i < 4; i++)
	{
		KEY_Write(writeData[i]);
		
		//delay_ms(10);
				
		count = 25000;
		while (count--)
		{
			ec11Data = EC11_Scan();
			if (ec11Data != 0)
			{
				return 20 + ec11Data;
			}
			delay_us(1);
		}
//		if (KEY_Read() != 0x0F)
//			delay_ms(10);
		
		switch (KEY_Read())
		{
			case 0x0E:
				scanData += 1;
//				return scanData;
				keyValue = scanData;
				break;
			
			case 0x0D:
				scanData += 2;
//				return scanData;
				keyValue = scanData;
				break;
			
			case 0x0B:
				scanData += 3;
//				return scanData;
				keyValue = scanData;
				break;
			
			case 0x07:
				scanData += 4;
//				return scanData;
				keyValue = scanData;
				break;
			
			default:
				break;
		}
		scanData += 4;
	}
	
	if (keyValue == lastKeyValue)													//��ֹ������������
	{
		keyValue  = 0;
	}
	lastKeyValue = keyValue;
	
	return keyValue;
}
