#ifndef		__BEEP_H__
#define		__BEEP_H__

#include	"config.h"

#define		BEEP_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		BEEP_GPIO_PORT		GPIOB
#define		BEEP_GPIO_PIN		GPIO_Pin_11

#define		BEEP_HIGH			GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define		BEEP_LOW			GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)

#define		BEEP_ON				beepOn = 1
#define		BEEP_OFF			beepOn = 0;BEEP_LOW

void BEEP_Init(void);

extern uint8_t beepOn;

#endif
