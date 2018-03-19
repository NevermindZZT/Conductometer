#ifndef		__IIC_H__
#define		__IIC_H__

#include	"config.h"

#define		IICSCL_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		IICSCL_GPIO_PORT		GPIOB
#define		IICSCL_GPIO_PIN			GPIO_Pin_6

#define		IICSDA_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		IICSDA_GPIO_PORT		GPIOB
#define		IICSDA_GPIO_PIN			GPIO_Pin_7

#define		IIC_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		IIC_GPIO_PORT			GPIOB
#define		IIC_GPIO_PIN			(GPIO_Pin_6 | GPIO_Pin_7)

#define		IIC_SclSet()			GPIO_SetBits(IICSCL_GPIO_PORT, IICSCL_GPIO_PIN)
#define		IIC_SclReset()			GPIO_ResetBits(IICSCL_GPIO_PORT, IICSCL_GPIO_PIN)

#define		IIC_SdaSet()			GPIO_SetBits(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)
#define		IIC_SdaReset()			GPIO_ResetBits(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)

#define		IIC_SdaRead()			GPIO_ReadInputDataBit(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)

#define		IIC_SdaIn()				IIC_SetSdaIOMode(0)//IICSDA_GPIO_PORT->CRL&=0X0FFFFFFF;IICSDA_GPIO_PORT->CRL|=8u<<28
#define		IIC_SdaOut()			IIC_SetSdaIOMode(1)//IICSDA_GPIO_PORT->CRL&=0X0FFFFFFF;IICSDA_GPIO_PORT->CRL|=3<<28

void IIC_Init(void);

void IIC_SetSdaIOMode(uint8_t mode);

void IIC_Start(void);

void IIC_Stop(void);

bool IIC_WaitAck(void);

void IIC_Ack(void);

void IIC_Nack(void);

void IIC_SendByte(uint8_t data);

uint8_t IIC_ReadByte(uint8_t ack);


#endif
