#include	"at24cxx.h"

void AT24CXX_Init(void)
{
	IIC_Init();
}



bool AT24CXX_Check(void)
{
	if (AT24CXX_ReadOneByte(255) == 0x55)
	{
		return TRUE;
	}
	else
	{
		AT24CXX_WriteOneByte(255, 0x55);
		if (AT24CXX_ReadOneByte(255) == 0x55)
		{
			return TRUE;
		}
		return FALSE;
	}
}



uint8_t AT24CXX_ReadOneByte(uint16_t readAddress)
{
	uint8_t readData;
	
	IIC_Start();
	
	if (AT24CXX_TYPE > AT24C16)
	{
		IIC_SendByte(0xA0);
		IIC_WaitAck();
		IIC_SendByte(readAddress >> 8);
	}
	else
	{
		IIC_SendByte(0xA0 + ((readAddress / 256) << 1));
	}
	IIC_WaitAck();
	IIC_SendByte(readAddress % 256);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	readData = IIC_ReadByte(0);
	IIC_Stop();
	
	return readData;
}



void AT24CXX_WriteOneByte(uint16_t writeAddress, uint8_t writeData)
{
	IIC_Start();
	if (AT24CXX_TYPE > AT24C16)
	{
		IIC_SendByte(0xA0);
		IIC_WaitAck();
		IIC_SendByte(writeAddress >> 8);
	}
	else
	{
		IIC_SendByte(0xA0 + ((writeAddress / 256) << 1));
	}
	IIC_WaitAck();
	IIC_SendByte(writeAddress % 256);
	IIC_WaitAck();
	IIC_SendByte(writeData);
	IIC_WaitAck();
	IIC_Stop();
	delay_ms(10);
}


void AT24CXX_Write(uint16_t writeAddress, uint8_t *data, uint16_t dataLength)
{
	while (dataLength --)
	{
		AT24CXX_WriteOneByte(writeAddress ++, *data);
		data ++;
	}
}


void AT24CXX_Read(uint16_t writeAddress, uint8_t *data, uint16_t dataLength)
{
	while (dataLength --)
	{
		*data = AT24CXX_ReadOneByte(writeAddress++);
		data ++;
	}
}
