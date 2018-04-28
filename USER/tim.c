/*********************************************
*           STM32������ʱ������              *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*********************************************/

//#include	"tim.h"
#include	"config.h"

uint32_t tim2Count = 0;
uint32_t tim3Count = 0;

/********************************************
*�������ƣ�	BASICTIM2_Config
*���ܣ�		������ʱ��2��ʼ��
*������		period				����ֵ
*			precaler			Ԥ��Ƶֵ
*			preemptionPriority	��ռ���ȼ�
*			subPriority			�����ȼ�
*����ֵ��	��
********************************************/
void BASICTIM2_Config(uint16_t period, uint16_t prescaler, uint16_t preemptionPriority, uint16_t subPriority)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStricture;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);						//������ʱ��ʱ��
	
	TIM_TimeBaseInitStricture.TIM_Period = period;								//������ֵ
	TIM_TimeBaseInitStricture.TIM_Prescaler = prescaler;						//Ԥ��Ƶ
	TIM_TimeBaseInitStricture.TIM_CounterMode = TIM_CounterMode_Up;				//������ģʽ
	TIM_TimeBaseInitStricture.TIM_ClockDivision = TIM_CKD_DIV1;					//ʱ�ӷ�Ƶ����
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStricture);							//��ʼ����ʱ��
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);									//����������ж�λ��־
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);									//�����������ж�
	TIM_Cmd(TIM2, ENABLE);														//ʹ�ܼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;								//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


/********************************************
*�������ƣ�	BASICTIM3_Config
*���ܣ�		������ʱ��3��ʼ��
*������		period				����ֵ
*			precaler			Ԥ��Ƶֵ
*			preemptionPriority	��ռ���ȼ�
*			subPriority			�����ȼ�
*����ֵ��	��
********************************************/
void BASICTIM3_Config(uint16_t period, uint16_t prescaler, uint16_t preemptionPriority, uint16_t subPriority)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStricture;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);						//������ʱ��ʱ��
	
	TIM_TimeBaseInitStricture.TIM_Period = period;								//������ֵ
	TIM_TimeBaseInitStricture.TIM_Prescaler = prescaler;						//Ԥ��Ƶ
	TIM_TimeBaseInitStricture.TIM_CounterMode = TIM_CounterMode_Up;				//������ģʽ
	TIM_TimeBaseInitStricture.TIM_ClockDivision = TIM_CKD_DIV1;					//ʱ�ӷ�Ƶ����
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStricture);							//��ʼ����ʱ��
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);									//����������ж�λ��־
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);									//�����������ж�
	TIM_Cmd(TIM3, ENABLE);														//ʹ�ܼ�����
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;								//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;				//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


/********************************************
*�������ƣ�	BASICTIM4_Config
*���ܣ�		������ʱ��4��ʼ��
*������		period				����ֵ
*			precaler			Ԥ��Ƶֵ
*����ֵ��	��
********************************************/
void BASICTIM4_Config(uint16_t period, uint16_t prescaler)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStricture;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	//����PD15����
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						//������ʱ��ʱ��
	
	TIM_TimeBaseInitStricture.TIM_Period = period;								//������ֵ
	TIM_TimeBaseInitStricture.TIM_Prescaler = prescaler;						//Ԥ��Ƶ
	TIM_TimeBaseInitStricture.TIM_CounterMode = TIM_CounterMode_Up;				//������ģʽ
	TIM_TimeBaseInitStricture.TIM_ClockDivision = TIM_CKD_DIV1;					//ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStricture);							//��ʼ����ʱ��
	
//	TIM_Cmd(TIM4, ENABLE);														//ʹ�ܼ�����
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}


/***********************************************
*�������ƣ�	TIM2_IRQHandler
*���ܣ�		������ʱ��2�жϷ�����
***********************************************/
void TIM2_IRQHandler(void)
{
	static uint8_t registerFlag = 0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)								//����ж�
	{
		tim2Count++;
		
		if (tim2Count % 10 == 0)												//5��ͬ�����ݣ���ʱ��500ms
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
		
		if (beepOn == 1)														//������
		{
			if ((tim2Count % 2) == 0)											//һ��Ϊ����
			{
				BEEP_HIGH;
			}
			else
			{
				BEEP_LOW;
			}
		}
		
		if ((temperatureControl.isHeating ==  TRUE) && ((tim2Count % 20) == 0))						//һ���Ӵ���һ�μ���
		{
#ifdef		PID_CONTROL																				//PID�㷨
			temperatureControl.pidTemperature[0] = temperatureControl.pidTemperature[1];
			temperatureControl.pidTemperature[1] = temperatureControl.pidTemperature[2];
			temperatureControl.pidTemperature[2] = DS18B20_ReadTemp(DS18B20A);						//��ȡ�������¶�
#endif
			DRY_TemperatureControl();																
		}

		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);									//����жϱ�־λ
}


/***********************************************
*�������ƣ�	TIM3_IRQHandler
*���ܣ�		������ʱ��3�жϷ�����
***********************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)								//����ж�
	{
		//�������
		//QPYLCD_DisplayInt(0, 50, RED, FONT8X16, tim3Count);
		tim3Count++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);									//����жϱ�־λ
}
