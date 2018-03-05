#include		"delay.h"


void delay_us(uint32_t i)
{
    uint32_t temp;
    SysTick->LOAD=9*i;      
    SysTick->CTRL=0X01;
    SysTick->VAL=0;
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&(!(temp&(1<<16))));
    SysTick->CTRL=0;
    SysTick->VAL=0;
}



void delay_ms(uint32_t i)
{
    uint32_t temp;
    SysTick->LOAD=9000*i;
    SysTick->CTRL=0X01;
    SysTick->VAL=0;
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&(!(temp&(1<<16))));
    SysTick->CTRL=0;
    SysTick->VAL=0;
}



void delay_s(uint32_t i)
{
	while (i--)
	{
		delay_ms(1000);
	}
}


