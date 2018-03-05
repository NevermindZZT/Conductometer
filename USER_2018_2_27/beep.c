#include	"beep.h"

uint8_t beepOn = 0;

void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);
	
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
	
}


