/*******************************************************************************
*                           AT24C02XXϵ��EEPROM����                            *
*                           ���ϴ�ѧ���������ѧԺ                             *
*                                  �ſ�ǿ                                      *
*                                  2018/4                                      *
*******************************************************************************/

#ifndef        __AT24CXX_H__
#define        __AT24CXX_H__

#include    "config.h"

/*----------------------------ATоƬ�ͺŶ���----------------------------------*/
#define        AT24CXX_TYPE    AT24C02

/*----------------------------ATоƬ�ͺ�ö��----------------------------------*/
enum
{
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32
};

/*-------------------------------��������-------------------------------------*/
void AT24CXX_Init(void);                                                        //AT24CXX��ʼ��

bool AT24CXX_Check(void);                                                       //AT24CXXУ��

uint8_t AT24CXX_ReadOneByte(uint16_t readAddress);                              //��һ���ֽ�����

void AT24CXX_WriteOneByte(uint16_t writeAddress, uint8_t writeData);            //дһ���ֽ�����

void AT24CXX_Write(uint16_t writeAddress, uint8_t *data, uint16_t dataLength);  //д��������

void AT24CXX_Read(uint16_t writeAddress, uint8_t *data, uint16_t dataLength);   //����������


#endif
