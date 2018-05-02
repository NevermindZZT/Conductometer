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
*函数名称：	W25X16_ReadWriteByte
*功能：		读一个字节，同时写一个字节
*参数：		data			待写入的数据
*返回值：	读到的数据
*******************************************************************************/
uint8_t W25X16_ReadWriteByte(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}



/*******************************************************************************
*函数名称：	W25X16_EraseSector
*功能：		按扇区擦除
*参数：		sectorAddress		地址
*返回值：	无
*******************************************************************************/
W25X16_Status W25X16_EraseSector(uint32_t sectorAddress)
{
	uint32_t timeout = 0;
	
	while (W25X16_IsBusy() == TRUE)
	{
		if (timeout++ > W25X16_MAX_DELAY)
		{
			return W25X16_TIMEOUT;
		}
	}
	
	sectorAddress = (sectorAddress >> 12) << 12;
	
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_ENABLE);
	W25X16_Disable();
	__nop();
	
	W25X16_Enable();
	
	W25X16_ReadWriteByte(W25X16_SECTOR_ERASE);
	
	W25X16_ReadWriteByte((sectorAddress & 0x00FF0000) >> 16);
	W25X16_ReadWriteByte((sectorAddress & 0x0000FF00) >> 8);
	W25X16_ReadWriteByte(sectorAddress & 0x000000FF);
	
	W25X16_Disable();
	
	__nop();
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
	
	return W25X16_OK;
}


/*******************************************************************************
*函数名称：	W25X16_EraseBlock
*功能：		按快擦除
*参数：		blockAddress		地址
*返回值：	无
*******************************************************************************/
W25X16_Status W25X16_EraseBlock(uint32_t blockAddress)
{
	uint32_t timeout = 0;
	
	while (W25X16_IsBusy() == TRUE)
	{
		if (timeout++ > W25X16_MAX_DELAY)
		{
			return W25X16_TIMEOUT;
		}
	}
	
	blockAddress = (blockAddress >> 16) << 16;
	
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_ENABLE);
	W25X16_Disable();
	__nop();
	
	W25X16_Enable();
	
	W25X16_ReadWriteByte(W25X16_BLOCK_ERASE);
	
	W25X16_ReadWriteByte((blockAddress & 0x00FF0000) >> 16);
	W25X16_ReadWriteByte((blockAddress & 0x0000FF00) >> 8);
	W25X16_ReadWriteByte(blockAddress & 0x000000FF);
	
	W25X16_Disable();
	
	__nop();
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
	
	return W25X16_OK;
}



/*******************************************************************************
*函数名称：	W25X16_EraseChip
*功能：		整片擦除
*参数：		无
*返回值：	无
*******************************************************************************/
W25X16_Status W25X16_EraseChip(void)
{
	uint32_t timeout = 0;
	
	while (W25X16_IsBusy() == TRUE)
	{
		if (timeout++ > W25X16_MAX_DELAY)
		{
			return W25X16_TIMEOUT;
		}
	}
	
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_ENABLE);
	W25X16_Disable();
	__nop();
	
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_CHIP_ERASE);
	W25X16_Disable();
	
	__nop();
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
	
	return W25X16_OK;
	
}



/*******************************************************************************
*函数名称：	W25X16_Read
*功能：		读取数据
*参数：		*data				读取到的数据
*			readAddress			读数据地址
*			dataLength			数据长度
*返回值：	无
*******************************************************************************/
W25X16_Status W25X16_Read(uint8_t *data, uint32_t readAddress, uint8_t dataLength)
{
	uint32_t timeout = 0;
	
	while (W25X16_IsBusy() == TRUE)
	{
		if (timeout++ > W25X16_MAX_DELAY)
		{
			return W25X16_TIMEOUT;
		}
	}
	
	W25X16_Enable();
	
	W25X16_ReadWriteByte(W25X16_FAST_READ);
	
	W25X16_ReadWriteByte((readAddress & 0x00FF0000) >> 16);
	W25X16_ReadWriteByte((readAddress & 0x0000FF00) >> 8);
	W25X16_ReadWriteByte(readAddress & 0x000000FF);
	W25X16_ReadWriteByte(0x00);
	
	while (dataLength --)
	{
		*data++ = W25X16_ReadWriteByte(0x00);
	}
	
	W25X16_Disable();
	
	return W25X16_OK;
}



/*******************************************************************************
*函数名称：	W25X16_PageWrite
*功能：		按页写数据
*参数：		*data				需写入的数据
*			readAddress			写数据地址
*			dataLength			数据长度
*返回值：	无
*******************************************************************************/
W25X16_Status W25X16_PageWrite(uint8_t *data, uint32_t writeAddress, uint8_t dataLength)
{
	uint32_t timeout = 0;

	while (W25X16_IsBusy() == TRUE)
	{
		if (timeout++ > W25X16_MAX_DELAY)
		{
			return W25X16_TIMEOUT;
		}
	}
	
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_ENABLE);
	W25X16_Disable();
	__nop();
	
	W25X16_Enable();
	
	W25X16_ReadWriteByte(W25X16_PAGE_PROGRAM);
	W25X16_ReadWriteByte((writeAddress & 0x00FF0000) >> 16);
	W25X16_ReadWriteByte((writeAddress & 0x0000FF00) >> 8);
	W25X16_ReadWriteByte(writeAddress & 0x000000FF);
	
	while (dataLength --)
	{
		W25X16_ReadWriteByte(*data++);
	}
	
	W25X16_Disable();
	
	__nop();
	W25X16_Enable();
	W25X16_ReadWriteByte(W25X16_WRITE_DISABLE);
	W25X16_Disable();
	
	return W25X16_OK;
}



/*******************************************************************************
*函数名称：	W25X16_ReadJedecID
*功能：		读JEDEC ID
*参数：		无
*返回值：	ID(对于W25X16 返回值应为0x00EF4015)
*******************************************************************************/
uint32_t W25X16_ReadJedecID(void)
{
	uint32_t id = 0x00000000;
	uint32_t timeout = 0;

	while (W25X16_IsBusy() == TRUE)
	{
		if (timeout++ > W25X16_TIMEOUT)
		{
			return 0x00000000;
		}
	}
	
	W25X16_Enable();
	
	W25X16_ReadWriteByte(W25X16_JEDEC_ID);
	
	for (uint8_t i = 0; i < 3; i++)
	{
		id |= (uint8_t)W25X16_ReadWriteByte(0x00) << ((2 - i) * 8);
	}
	
	W25X16_Disable();
	
	return id;
}



/*******************************************************************************
*函数名称：	W25X16_IsBusy
*功能：		读忙
*参数：		无
*返回值：	FALSE		空闲
*			TRUE		忙
*******************************************************************************/
bool W25X16_IsBusy(void)
{
	uint8_t status;
	
	W25X16_Enable();
	
	W25X16_ReadWriteByte(W25X16_READ_STATUS_REG);
	
	status = W25X16_ReadWriteByte(0x00);
	
	W25X16_Disable();
	
	if ((status & 0x01) == 0x00)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}



/*******************************************************************************
*函数名称：	W25X16_Check
*功能：		查询W25X16是否正常驱动
*参数：		无
*返回值：	FALSE		驱动失败
*			TRUE		驱动成功
*******************************************************************************/
bool W25X16_Check(void)
{
	if ((W25X16_ReadJedecID() & 0x00FF0000) == 0x00EF0000)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

