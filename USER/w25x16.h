/*******************************************************************************
*                            W25X16 SPI Falsh����                              *
*                           ���ϴ�ѧ���������ѧԺ                             *
*                                  �ſ�ǿ                                      *
*                                  2018/4                                      *
*******************************************************************************/

#ifndef		__W25X16_H__
#define		__W25X16_H__

#include	"config.h"

#define		W25X16_WRITE_ENABLE			0x06
#define		W25X16_WRITE_DISABLE		0x04
#define		W25X16_READ_STATUS_REG		0x05
#define		W25X16_WRITE_STATUS_REG		0x01
#define		W25X16_READ_DATA			0x03
#define		W25X16_FAST_READ			0x0B
#define		W25X16_FAST_READ_DUAL		0x3B
#define		W25X16_PAGE_PROGRAM			0x02
#define		W25X16_SECTOR_ERASE			0x20
#define		W25X16_BLOCK_ERASE			0xD8
#define		W25X16_CHIP_ERASE			0xC7
#define		W25X16_POWER_DOWN			0xB9
#define		W25X16_RELEASE_POWER_DOWN	0xAB
#define		W25X16_MANUFACT_ID			0x90
#define		W25X16_JEDEC_ID				0x9F


#define		W25X16_CS_PIN				GPIO_Pin_12
#define		W25X16_CS_PORT				GPIOB


#define		W25X16_Enable()				GPIO_ResetBits(W25X16_CS_PORT, W25X16_CS_PIN)
#define		W25X16_Disable()			GPIO_SetBits(W25X16_CS_PORT, W25X16_CS_PIN);



void W25X16_Init(void);

void W25X16_WriteByte(uint8_t data);

uint8_t W25X16_ReadByte(void);

void W25X16_EraseSector(uint32_t sectorAddress);

void W25X16_Read(uint8_t *data, uint32_t readAddress, uint8_t dataLength);

void W25X16_PageWrite(uint8_t *data, uint32_t writeAddress, uint8_t dataLength);

#endif