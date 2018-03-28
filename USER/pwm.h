#ifndef		__PWM_H__
#define		__PWM_H__

#include	"config.h"

#define		PWM_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		PWM_GPIO_PORT		GPIOA
#define		PWM_GPIO_PIN		GPIO_Pin_12

#define		PWMEnable()			TIM_Cmd(TIM4, ENABLE)
#define		PWMDisable()		TIM_SetCompare4(TIM4, 0)//;TIM_Cmd(TIM4, DISABLE);GPIO_SetBits(GPIOD, GPIO_Pin_15)


void PWM_Init(void);

void PWM_SetDutyCycle(uint8_t dutyCycle);

#endif
