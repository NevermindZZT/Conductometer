#ifndef		__AT24CXX_H__
#define		__AT24CXX_H__

#include	"config.h"

#define		AT24CXX_TYPE	AT24C02

enum
{
	AT24C02,
	AT24C04,
	AT24C08,
	AT24C16,
	AT24C32
};


void AT24CXX_Init(void);

bool AT24CXX_Check(void);

uint8_t AT24CXX_ReadOneByte(uint16_t readAddress);

void AT24CXX_WriteOneByte(uint16_t writeAddress, uint8_t writeData);

void AT24CXX_Write(uint16_t writeAddress, uint8_t *data, uint16_t dataLength);

void AT24CXX_Read(uint16_t writeAddress, uint8_t *data, uint16_t dataLength);


#endif
