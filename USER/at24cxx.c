/*******************************************************************************
*                           AT24C02XX系列EEPROM驱动                            *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/4                                      *
*******************************************************************************/

#include	"at24cxx.h"

/*******************************************************************************
*函数名称： AT24CXX_Init
*功能：     AT24CXX芯片初始化
*参数：     无
*返回值：   无
*******************************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();                                                 //初始化I2C
}


/*******************************************************************************
*函数名称： AT24CXX_Check
*功能：     AT24CXX检查
*参数：     无
*返回值：   TRUE        连接成功
*           FALSE       连接失败
*说明：     通过向AT24C02最后一字节写数据并读数据，判断芯片是否正常工作
*******************************************************************************/
bool AT24CXX_Check(void)
{
	if (AT24CXX_ReadOneByte(255) == 0x55)                       //读最后一字节数据并判断
	{
		return TRUE;                                            //若数据为0x55,连接成功
	}
	else
	{
		AT24CXX_WriteOneByte(255, 0x55);                        //不为0x55,重写数据并读取判断
		if (AT24CXX_ReadOneByte(255) == 0x55)
		{
			return TRUE;
		}
		return FALSE;
	}
}


/*******************************************************************************
*函数名称： AT24CXX_ReadOneByte
*功能：     AT24CXX读一字节数据
*参数：     readAddress 要读取的地址
*返回值：   指定地址的数据
*******************************************************************************/
uint8_t AT24CXX_ReadOneByte(uint16_t readAddress)
{
	uint8_t readData;
	
	IIC_Start();                                                //I2C开始信号
	
	if (AT24CXX_TYPE > AT24C16)                                 //写地址，大容量芯片需写两次
	{
		IIC_SendByte(0xA0);
		IIC_WaitAck();
		IIC_SendByte(readAddress >> 8);
	}
	else
	{
		IIC_SendByte(0xA0 + ((readAddress / 256) << 1));
	}
	IIC_WaitAck();                                              //等待应答
	IIC_SendByte(readAddress % 256);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xA1);                                         //开始读数据
	IIC_WaitAck();
	readData = IIC_ReadByte(0);                                 //读取数据
	IIC_Stop();                                                 //I2C停止信号
	
	return readData;
}


/*******************************************************************************
*函数名称： AT24CXX_WriteOneByte
*功能：     AT24CXX写一字节数据
*参数：     writeAddress    要写数据的地址
*           writeData       需要写入的数据
*返回值：   无
*******************************************************************************/
void AT24CXX_WriteOneByte(uint16_t writeAddress, uint8_t writeData)
{
	IIC_Start();
	if (AT24CXX_TYPE > AT24C16)                                 //写地址
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
	IIC_SendByte(writeData);                                    //写数据
	IIC_WaitAck();
	IIC_Stop();
	delay_ms(10);                                               //每次写数据需等待
}


/*******************************************************************************
*函数名称： AT24CXX_Write
*功能：     AT24CXX写连续数据
*参数：     writeAddress    要写数据的首地址
*           *data           需要写入的数据
*           dataLength      写入数据的长度
*返回值：   无
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
*函数名称： AT24CXX_Read
*功能：     AT24CXX写连续数据
*参数：     writeAddress    要读数据的首地址
*           *data           读取到的数据
*           dataLength      读取数据的长度
*返回值：   无
*******************************************************************************/
void AT24CXX_Read(uint16_t writeAddress, uint8_t *data, uint16_t dataLength)
{
	while (dataLength --)
	{
		*data = AT24CXX_ReadOneByte(writeAddress++);
		data ++;
	}
}
