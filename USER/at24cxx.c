/*******************************************************************************
*                           AT24C02XXϵ��EEPROM����                            *
*                           ���ϴ�ѧ���������ѧԺ                             *
*                                  �ſ�ǿ                                      *
*                                  2018/4                                      *
*******************************************************************************/

#include	"at24cxx.h"

/*******************************************************************************
*�������ƣ� AT24CXX_Init
*���ܣ�     AT24CXXоƬ��ʼ��
*������     ��
*����ֵ��   ��
*******************************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();                                                 //��ʼ��I2C
}


/*******************************************************************************
*�������ƣ� AT24CXX_Check
*���ܣ�     AT24CXX���
*������     ��
*����ֵ��   TRUE        ���ӳɹ�
*           FALSE       ����ʧ��
*˵����     ͨ����AT24C02���һ�ֽ�д���ݲ������ݣ��ж�оƬ�Ƿ���������
*******************************************************************************/
bool AT24CXX_Check(void)
{
	if (AT24CXX_ReadOneByte(255) == 0x55)                       //�����һ�ֽ����ݲ��ж�
	{
		return TRUE;                                            //������Ϊ0x55,���ӳɹ�
	}
	else
	{
		AT24CXX_WriteOneByte(255, 0x55);                        //��Ϊ0x55,��д���ݲ���ȡ�ж�
		if (AT24CXX_ReadOneByte(255) == 0x55)
		{
			return TRUE;
		}
		return FALSE;
	}
}


/*******************************************************************************
*�������ƣ� AT24CXX_ReadOneByte
*���ܣ�     AT24CXX��һ�ֽ�����
*������     readAddress Ҫ��ȡ�ĵ�ַ
*����ֵ��   ָ����ַ������
*******************************************************************************/
uint8_t AT24CXX_ReadOneByte(uint16_t readAddress)
{
	uint8_t readData;
	
	IIC_Start();                                                //I2C��ʼ�ź�
	
	if (AT24CXX_TYPE > AT24C16)                                 //д��ַ��������оƬ��д����
	{
		IIC_SendByte(0xA0);
		IIC_WaitAck();
		IIC_SendByte(readAddress >> 8);
	}
	else
	{
		IIC_SendByte(0xA0 + ((readAddress / 256) << 1));
	}
	IIC_WaitAck();                                              //�ȴ�Ӧ��
	IIC_SendByte(readAddress % 256);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xA1);                                         //��ʼ������
	IIC_WaitAck();
	readData = IIC_ReadByte(0);                                 //��ȡ����
	IIC_Stop();                                                 //I2Cֹͣ�ź�
	
	return readData;
}


/*******************************************************************************
*�������ƣ� AT24CXX_WriteOneByte
*���ܣ�     AT24CXXдһ�ֽ�����
*������     writeAddress    Ҫд���ݵĵ�ַ
*           writeData       ��Ҫд�������
*����ֵ��   ��
*******************************************************************************/
void AT24CXX_WriteOneByte(uint16_t writeAddress, uint8_t writeData)
{
	IIC_Start();
	if (AT24CXX_TYPE > AT24C16)                                 //д��ַ
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
	IIC_SendByte(writeData);                                    //д����
	IIC_WaitAck();
	IIC_Stop();
	delay_ms(10);                                               //ÿ��д������ȴ�
}


/*******************************************************************************
*�������ƣ� AT24CXX_Write
*���ܣ�     AT24CXXд��������
*������     writeAddress    Ҫд���ݵ��׵�ַ
*           *data           ��Ҫд�������
*           dataLength      д�����ݵĳ���
*����ֵ��   ��
*******************************************************************************/
void AT24CXX_Write(uint16_t writeAddress, uint8_t *data, uint16_t dataLength)
{
	while (dataLength --)
	{
		AT24CXX_WriteOneByte(writeAddress ++, *data);
		data ++;
	}
}


/*******************************************************************************
*�������ƣ� AT24CXX_Read
*���ܣ�     AT24CXXд��������
*������     writeAddress    Ҫ�����ݵ��׵�ַ
*           *data           ��ȡ��������
*           dataLength      ��ȡ���ݵĳ���
*����ֵ��   ��
*******************************************************************************/
void AT24CXX_Read(uint16_t writeAddress, uint8_t *data, uint16_t dataLength)
{
	while (dataLength --)
	{
		*data = AT24CXX_ReadOneByte(writeAddress++);
		data ++;
	}
}
