#include	"pwm.h"

uint8_t PWM_dutyCycle;
void PWM_Init(void)
{
	BASICTIM4_Config(10000, 719);				//10Hz
}

void PWM_SetDutyCycle(uint8_t dutyCycle)
{
	TIM_SetCompare4(TIM4, (uint16_t)dutyCycle * 100);
}
