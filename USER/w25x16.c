/*******************************************************************************
*                            W25X16 SPI Falsh驱动                              *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/4                                      *
*******************************************************************************/

#include	"w25x16.h"


/*******************************************************************************
*函数名称：	W25X16_Init
*功能：		W25X16初始化
*参数：		无
*返回值：	无
*******************************************************************************/
void W25X16_Init(void)
{
	SPI2_Config();
	W25X16_Disable();
}


/*******************************************************************************
*函数名称：	W25X16_WriteByte
*功能：		写一个字节
*参数：		data			待写入的数据
*返回值：	无
*******************************************************************************/
void W25X16_WriteByte(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, data);
}


/*******************************************************************************
*函数名称：	W25X16_ReadByte
*功能：		读取一个字节
*参数：		无
*返回值：	读取到的数据
*******************************************************************************/
uint8_t W25X16_ReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}


/*******************************************************************************
*函数名称：	W25X16_EraseSector
*功能：		按扇区擦除
*参数：		sectorAddress		地址
*返回值：	无
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
*函数名称：	W25X16_Read
*功能：		读取数据
*参数：		*data				读取到的数据
*			readAddress			读数据地址
*			dataLength			数据长度
*返回值：	无
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
*函数名称：	W25X16_PageWrite
*功能：		按页写数据
*参数：		*data				需写入的数据
*			readAddress			写数据地址
*			dataLength			数据长度
*返回值：	无
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

