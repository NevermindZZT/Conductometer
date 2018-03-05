#ifndef		__PWM_H__
#define		__PWM_H__

#include	"config.h"

#define		PWM_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		PWM_GPIO_PORT		GPIOA
#define		PWM_GPIO_PIN		GPIO_Pin_12

#define		PWM_ON				TIM_Cmd(TIM4, ENABLE)
#define		PWM_OFF				TIM_SetCompare4(TIM4, 0)

extern uint8_t PWM_dutyCycle;

void PWM_Init(void);

void PWM_SetDutyCycle(uint8_t dutyCycle);

#endif
