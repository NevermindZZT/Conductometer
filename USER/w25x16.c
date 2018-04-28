/*******************************************************************************
*                            W25X16 SPI Falsh����                              *
*                           ���ϴ�ѧ���������ѧԺ                             *
*                                  �ſ�ǿ                                      *
*                                  2018/4                                      *
*******************************************************************************/

#include	"w25x16.h"


/*******************************************************************************
*�������ƣ�	W25X16_Init
*���ܣ�		W25X16��ʼ��
*������		��
*����ֵ��	��
*******************************************************************************/
void W25X16_Init(void)
{
	SPI2_Config();
	W25X16_Disable();
}


/*******************************************************************************
*�������ƣ�	W25X16_WriteByte
*���ܣ�		дһ���ֽ�
*������		data			��д�������
*����ֵ��	��
*******************************************************************************/
void W25X16_WriteByte(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, data);
}


/*******************************************************************************
*�������ƣ�	W25X16_ReadByte
*���ܣ�		��ȡһ���ֽ�
*������		��
*����ֵ��	��ȡ��������
*******************************************************************************/
uint8_t W25X16_ReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}


/*******************************************************************************
*�������ƣ�	W25X16_EraseSector
*���ܣ�		����������
*������		sectorAddress		��ַ
*����ֵ��	��
*******************************************************************************/
void W25X16_EraseSector(uint32_t sectorAddress)
{
	sectorAddress = (sectorAddress >> 12) << 12;
	
	W25X16_Enable();
	W25X16_WriteByte(W25X16_WRITE_ENABLE);
	W25X16_Disable();
	__nop();
	
	W25X16_Enable();
	
	W25X16_WriteByte(W25X16_SECTOR_ERASE);
	
	W25X16_WriteByte((sectorAddress & 0xFF0000) >> 16);
	W25X16_WriteByte((sectorAddress & 0x00FF00) >> 8);
	W25X16_WriteByte(sectorAddress & 0x0000FF);
	
	W25X16_Disable();
	
	__nop();
	W25X16_Enable();
	W25X16_WriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
}



/*******************************************************************************
*�������ƣ�	W25X16_Read
*���ܣ�		��ȡ����
*������		*data				��ȡ��������
*			readAddress			�����ݵ�ַ
*			dataLength			���ݳ���
*����ֵ��	��
*******************************************************************************/
void W25X16_Read(uint8_t *data, uint32_t readAddress, uint8_t dataLength)
{
	W25X16_Enable();
	
	W25X16_WriteByte(W25X16_READ_DATA);
	
	W25X16_WriteByte((readAddress & 0xFF0000) >> 16);
	W25X16_WriteByte((readAddress & 0x00FF00) >> 8);
	W25X16_WriteByte(readAddress & 0x0000FF);
	
	while (dataLength --)
	{
		*data++ = W25X16_ReadByte();
	}
	
	W25X16_Disable();
}



/*******************************************************************************
*�������ƣ�	W25X16_PageWrite
*���ܣ�		��ҳд����
*������		*data				��д�������
*			readAddress			д���ݵ�ַ
*			dataLength			���ݳ���
*����ֵ��	��
*******************************************************************************/
void W25X16_PageWrite(uint8_t *data, uint32_t writeAddress, uint8_t dataLength)
{
	W25X16_Enable();
	W25X16_WriteByte(W25X16_WRITE_ENABLE);
	W25X16_Disable();
	__nop();
	
	W25X16_Enable();
	
	W25X16_WriteByte(W25X16_WRITE_ENABLE);
	
	W25X16_WriteByte(W25X16_PAGE_PROGRAM);
	W25X16_WriteByte((writeAddress & 0xFF0000) >> 16);
	W25X16_WriteByte((writeAddress & 0x00FF00) >> 8);
	W25X16_WriteByte(writeAddress & 0x0000FF);
	
	while (dataLength --)
	{
		W25X16_WriteByte(*data++);
	}
	
	W25X16_WriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
	
	__nop();
	W25X16_Enable();
	W25X16_WriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
}

