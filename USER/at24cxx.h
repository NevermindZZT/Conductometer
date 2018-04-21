/*******************************************************************************
*                           AT24C02XX系列EEPROM驱动                            *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/4                                      *
*******************************************************************************/

#ifndef        __AT24CXX_H__
#define        __AT24CXX_H__

#include    "config.h"

/*----------------------------AT芯片型号定义----------------------------------*/
#define        AT24CXX_TYPE    AT24C02

/*----------------------------AT芯片型号枚举----------------------------------*/
enum
{
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32
};

/*-------------------------------函数声明-------------------------------------*/
void AT24CXX_Init(void);                                                        //AT24CXX初始化

bool AT24CXX_Check(void);                                                       //AT24CXX校验

uint8_t AT24CXX_ReadOneByte(uint16_t readAddress);                              //读一个字节数据

void AT24CXX_WriteOneByte(uint16_t writeAddress, uint8_t writeData);            //写一个字节数据

void AT24CXX_Write(uint16_t writeAddress, uint8_t *data, uint16_t dataLength);  //写连续数据

void AT24CXX_Read(uint16_t writeAddress, uint8_t *data, uint16_t dataLength);   //读连续数据


#endif
