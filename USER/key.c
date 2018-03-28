/*********************************************
*              矩阵按键驱动                  *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/3                      *
*********************************************/

#include	"key.h"


/***********************************************
*函数名称：	KEY_Init
*功能：		初始化按键
*参数：		无
*返回值：	无
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
*函数名称：	KEY_Write
*功能：		写数据到按键
*参数：		data			需写入的数据
*返回值：	无
***********************************************/
void KEY_Write(uint8_t data)
{
	GPIO_WriteBit(KEYC0_GPIO_PORT, KEYC0_GPIO_PIN, (BitAction)(data & 0x01));
	GPIO_WriteBit(KEYC1_GPIO_PORT, KEYC1_GPIO_PIN, (BitAction)(data & 0x02));
	GPIO_WriteBit(KEYC2_GPIO_PORT, KEYC2_GPIO_PIN, (BitAction)(data & 0x04));
	GPIO_WriteBit(KEYC3_GPIO_PORT, KEYC3_GPIO_PIN, (BitAction)(data & 0x08));
}


/***********************************************
*函数名称：	KEY_Read
*功能：		读数据到按键
*参数：		无
*返回值：	读出的数据
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
*函数名称：	KEY_Scan
*功能：		扫描按键
*参数：		无
*返回值：	按键键值(1~56,0为无按键按下)
***********************************************/
uint8_t KEY_Scan(void)	
{
	static uint8_t lastKeyValue;
	uint16_t count;
	uint8_t i;
	uint8_t scanData = 0;
	uint8_t keyValue = 0;
	const uint8_t writeData[] = {0x0E, 0x0D, 0x0B, 0x07};
	
	KEY_Write(0x00);
	count = 10000;
	while((KEY_Read() == 0x0F) && (count != 0))
	{
		delay_us(1);
		count--;
	}
	if (count == 0)
	{
		lastKeyValue = 0;
		return 0;
	}
	
	delay_ms(5);
	
	for (i = 0; i < 4; i++)
	{
		KEY_Write(writeData[i]);
		delay_us(1);
		switch (KEY_Read())
		{
			case 0x0E:
				scanData += 1;
				keyValue = scanData;
				break;
			
			case 0x0D:
				scanData += 2;
				keyValue = scanData;
				break;
			
			case 0x0B:
				scanData += 3;
				keyValue = scanData;
				break;
			
			case 0x07:
				scanData += 4;
				keyValue = scanData;
				break;
			
			default:
				break;
		}
		scanData += 4;
	}
	if (keyValue == 0)
	{
		lastKeyValue = 0;
		return keyValue;
	}
	
	KEY_Write(0x00);
	count = 0;
	while ((KEY_Read() != 0x0F) && count < KEY_LONG_PRESS_MAX_TIME)
	{
		count++;
		delay_ms(1);
	}
	
	if (count > KEY_SHORT_PRESS_MAX_TIME)
	{
		keyValue += 20;
	}
#ifdef	KEY_DOUBLE_CLICK
	else
	{
		delay_ms(KEY_DOUBLE_CLICK_DELAY);
		KEY_Write(writeData[(keyValue - 1) / 4]);
		delay_us(1);
		if (KEY_Read() == writeData[(keyValue - 1) % 4])
		{
			keyValue += 40;
			delay_ms(10);
			KEY_Write(0x00);
			count = 0;
			while ((KEY_Read() != 0x0F) && count < 500)
			{
				count++;
				delay_ms(1);
			}
		}
	}
#endif
	
	if ((lastKeyValue == keyValue) || (lastKeyValue == keyValue + 20) || (lastKeyValue == keyValue -20))
	{
		keyValue = 0;
		return 0;
	}
	lastKeyValue = keyValue;
	
	return keyValue;
}


/***********************************************
*函数名称：	KEYANDEC11_Scan
*功能：		扫描按键和EC11编码器
*参数：		无
*返回值：	键值(1~63,0为无按键按下，1~16按键单击，
*			21~36按键长按，41~56按键双击，57~63为EC11状态)
***********************************************/
uint8_t KEYANDEC11_Scan(void)
{
	static uint8_t lastKeyValue;
	uint16_t count;
	uint8_t i;
	int8_t ec11Data;
	uint8_t scanData = 0;
	uint8_t keyValue = 0;
	const uint8_t writeData[] = {0x0E, 0x0D, 0x0B, 0x07};
	
	KEY_Write(0x00);
	count = 10000;
	while((KEY_Read() == 0x0F) && (count != 0))
	{
		ec11Data = EC11_Scan();
		if (ec11Data != 0)
		{
			return 60 + ec11Data;
		}
		delay_us(1);
		count--;
	}
	if (count == 0)
	{
		lastKeyValue = 0;
		return 0;
	}
	
	delay_ms(5);
	
	for (i = 0; i < 4; i++)
	{
		KEY_Write(writeData[i]);
		delay_us(1);
		switch (KEY_Read())
		{
			case 0x0E:
				scanData += 1;
				keyValue = scanData;
				break;
			
			case 0x0D:
				scanData += 2;
				keyValue = scanData;
				break;
			
			case 0x0B:
				scanData += 3;
				keyValue = scanData;
				break;
			
			case 0x07:
				scanData += 4;
				keyValue = scanData;
				break;
			
			default:
				break;
		}
		scanData += 4;
	}
	if (keyValue == 0)
	{
		lastKeyValue = 0;
		return keyValue;
	}
	
	KEY_Write(0x00);
	count = 0;
	while ((KEY_Read() != 0x0F) && count < KEY_LONG_PRESS_MAX_TIME)
	{
		count++;
		delay_ms(1);
	}
	
	if (count > KEY_SHORT_PRESS_MAX_TIME)
	{
		keyValue += 20;
	}
#ifdef	KEY_DOUBLE_CLICK
	else
	{
		delay_ms(KEY_DOUBLE_CLICK_DELAY);
		KEY_Write(writeData[(keyValue - 1) / 4]);
		delay_us(1);
		if (KEY_Read() == writeData[(keyValue - 1) % 4])
		{
			keyValue += 40;
			delay_ms(10);
			KEY_Write(0x00);
			count = 0;
			while ((KEY_Read() != 0x0F) && count < 500)
			{
				count++;
				delay_ms(1);
			}
		}
	}
#endif
	
	if ((lastKeyValue == keyValue) || (lastKeyValue == keyValue + 20) || (lastKeyValue == keyValue -20))
	{
		keyValue = 0;
		return 0;
	}
	lastKeyValue = keyValue;
	
	return keyValue;
}

