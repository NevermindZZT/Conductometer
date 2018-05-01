/*******************************************************************************
*                                   PWM                                        *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/2                                      *
*******************************************************************************/

#ifndef		__PWM_H__
#define		__PWM_H__

#include	"config.h"

#define		PWM_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		PWM_GPIO_PORT		GPIOA
#define		PWM_GPIO_PIN		GPIO_Pin_12

#define		PWMEnable()			TIM_Cmd(TIM4, ENABLE)
#define		PWMDisable()		TIM_SetCompare4(TIM4, 0)//;TIM_Cmd(TIM4, DISABLE);GPIO_SetBits(GPIOD, GPIO_Pin_15)


void PWM_Init(void);                                                    //初始化

void PWM_SetDutyCycle(float dutyCycle);                               //设置占空比

#endif
