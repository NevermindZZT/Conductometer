/*********************************************
*           STM32基础定时器驱动              *
*         中南大学物理与电子学院             *
*                张克强                      *
*********************************************/

//#include	"tim.h"
#include	"config.h"

uint32_t tim2Count = 0;
uint32_t tim3Count = 0;

/********************************************
*函数名称：	BASICTIM2_Config
*功能：		基础定时器2初始化
*参数：		period				计数值
*			precaler			预分频值
*			preemptionPriority	抢占优先级
*			subPriority			子优先级
*返回值：	无
********************************************/
void BASICTIM2_Config(uint16_t period, uint16_t prescaler, uint16_t preemptionPriority, uint16_t subPriority)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStricture;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);						//开启定时器时钟
	
	TIM_TimeBaseInitStricture.TIM_Period = period;								//计数器值
	TIM_TimeBaseInitStricture.TIM_Prescaler = prescaler;						//预分频
	TIM_TimeBaseInitStricture.TIM_CounterMode = TIM_CounterMode_Up;				//计数器模式
	TIM_TimeBaseInitStricture.TIM_ClockDivision = TIM_CKD_DIV1;					//时钟分频因子
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStricture);							//初始化定时器
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);									//清除计数器中断位标志
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);									//开启计数器中断
	TIM_Cmd(TIM2, ENABLE);														//使能计数器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;								//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;				//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


/********************************************
*函数名称：	BASICTIM3_Config
*功能：		基础定时器3初始化
*参数：		period				计数值
*			precaler			预分频值
*			preemptionPriority	抢占优先级
*			subPriority			子优先级
*返回值：	无
********************************************/
void BASICTIM3_Config(uint16_t period, uint16_t prescaler, uint16_t preemptionPriority, uint16_t subPriority)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStricture;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);						//开启定时器时钟
	
	TIM_TimeBaseInitStricture.TIM_Period = period;								//计数器值
	TIM_TimeBaseInitStricture.TIM_Prescaler = prescaler;						//预分频
	TIM_TimeBaseInitStricture.TIM_CounterMode = TIM_CounterMode_Up;				//计数器模式
	TIM_TimeBaseInitStricture.TIM_ClockDivision = TIM_CKD_DIV1;					//时钟分频因子
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStricture);							//初始化定时器
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);									//清除计数器中断位标志
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);									//开启计数器中断
	TIM_Cmd(TIM3, ENABLE);														//使能计数器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;								//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;				//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


/********************************************
*函数名称：	BASICTIM4_Config
*功能：		基础定时器4初始化
*参数：		period				计数值
*			precaler			预分频值
*返回值：	无
********************************************/
void BASICTIM4_Config(uint16_t period, uint16_t prescaler)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStricture;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	//复用PD15引脚
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						//开启定时器时钟
	
	TIM_TimeBaseInitStricture.TIM_Period = period;								//计数器值
	TIM_TimeBaseInitStricture.TIM_Prescaler = prescaler;						//预分频
	TIM_TimeBaseInitStricture.TIM_CounterMode = TIM_CounterMode_Up;				//计数器模式
	TIM_TimeBaseInitStricture.TIM_ClockDivision = TIM_CKD_DIV1;					//时钟分频因子
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStricture);							//初始化定时器
	
//	TIM_Cmd(TIM4, ENABLE);														//使能计数器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}


/***********************************************
*函数名称：	TIM2_IRQHandler
*功能：		基础定时器2中断服务函数
***********************************************/
void TIM2_IRQHandler(void)
{
	static uint8_t registerFlag = 0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)								//溢出中断
	{
		tim2Count++;
		
		if (tim2Count % 10 == 0)												//5秒同步数据，定时器500ms
		{
			if (registerFlag == 0 && espState.currentCommand == ESP8266_CIPSEND
				&& (experimentalData.progress == TEMPERATURESETTING || experimentalData.progress == HEATTING
				|| experimentalData.progress == BUILDBALANCE || experimentalData.progress == RECORDING))
			{
				DRY_UplaodData(DEVICE_REGISTER);
				registerFlag = 1;
			}
			else if (registerFlag == 1)
			{
				DRY_UplaodData(DEVICE_DATA);
			}
		}
		
		if (beepOn == 1)														//蜂鸣器
		{
			if ((tim2Count % 2) == 0)											//一秒为周期
			{
				BEEP_HIGH;
			}
			else
			{
				BEEP_LOW;
			}
		}
		
		if ((temperatureControl.isHeating ==  TRUE) && ((tim2Count % 20) == 0))						//一秒钟处理一次加热
		{
#ifdef		PID_CONTROL																				//PID算法
			temperatureControl.pidTemperature[0] = temperatureControl.pidTemperature[1];
			temperatureControl.pidTemperature[1] = temperatureControl.pidTemperature[2];
			temperatureControl.pidTemperature[2] = DS18B20_ReadTemp(DS18B20A);						//读取加热盘温度
#endif
			DRY_TemperatureControl();																
		}

		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);									//清除中断标志位
}


/***********************************************
*函数名称：	TIM3_IRQHandler
*功能：		基础定时器3中断服务函数
***********************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)								//溢出中断
	{
		//处理语句
		//QPYLCD_DisplayInt(0, 50, RED, FONT8X16, tim3Count);
		tim3Count++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);									//清除中断标志位
}
