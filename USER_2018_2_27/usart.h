/*********************************************
*          STM32USART��������                *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/1                      *
*********************************************/


#ifndef		__USART_H__
#define		__USART_H__

#include	"stm32f10x.h"
#include	"stm32f10x_usart.h"

//#define		PRINTTOUSART												//printf������ӡ������,��ʹ�û�ʹ��printf������������Ҫע�ʹ���

#define		USARTRECLENGTH		200

extern uint8_t usart1Buff[USARTRECLENGTH];
extern uint16_t usart1RecFlag;

//extern uint8_t usart2Buff[USARTRECLENGTH];
//extern uint16_t usart2RecFlag = 0;

//extern uint8_t usart3Buff[USARTRECLENGTH];
//extern uint16_t usart3RecFlag = 0;

void USART_Config(USART_TypeDef *USARTx, uint32_t baudRate, uint16_t preemptionPriority, uint16_t subPriority);

void USART_Send(USART_TypeDef *USARTx, uint8_t ch);

#endif