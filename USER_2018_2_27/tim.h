/*********************************************
*           STM32������ʱ������              *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*********************************************/

#ifndef		__TIM_H__
#define		__TIM_H__

#include	"stm32f10x.h"
#include	"stm32f10x_tim.h"

extern uint32_t tim2Count;
extern uint32_t tim3Count;

void BASICTIM2_Config(uint16_t period, uint16_t prescaler, uint16_t preemptionPriority, uint16_t subPriority);

void BASICTIM3_Config(uint16_t period, uint16_t prescaler, uint16_t preemptionPriority, uint16_t subPriority);

void BASICTIM4_Config(uint16_t period, uint16_t prescaler);

#endif
