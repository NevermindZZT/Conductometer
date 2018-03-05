/*********************************************
*          STM32USART��������                *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/1                      *
*********************************************/

//#include	"usart.h"
//#include	"stdio.h"
#include	"config.h"

uint8_t usart1Buff[USARTRECLENGTH];
uint16_t usart1RecFlag = 0;

//uint8_t usart2Buff[USARTRECLENGTH];
//uint16_t usart2RecFlag = 0;

//uint8_t usart3Buff[USARTRECLENGTH];
//uint16_t usart3RecFlag = 0;



/***********************************************
*�������ƣ�	USART_Config
*���ܣ�		��ʼ������
*������		*USARTx				����
*			baudRate			������
*			preemptionPriority	��ռ���ȼ�
*			subPriority			�����ȼ�
*����ֵ��	��
***********************************************/
void USART_Config(USART_TypeDef *USARTx, uint32_t baudRate, uint16_t preemptionPriority, uint16_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if (USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;									//PA9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA.9
	
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;									//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA.10
		
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
	else if (USARTx == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//ʹ��USART2��GPIOAʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									//PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA.2
	
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;									//PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA.3
		
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	else if (USARTx == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//ʹ��USART3��GPIOBʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;									//PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);										//��ʼ��GPIOB.10
	
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;									//PB11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOB.11
		
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	}
  

	//Usart1 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;		//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;					//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = baudRate;									//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	
	USART_Init(USARTx, &USART_InitStructure);										//��ʼ������
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);									//�������ڽ����ж�
	USART_Cmd(USARTx, ENABLE);														//ʹ�ܴ���
}


/***********************************************
*�������ƣ�	USART_Send
*���ܣ�		ʹ�ô��ڷ����ַ�
*������		*USARTx				����
*			ch					�ַ�
*����ֵ��	��
***********************************************/
void USART_Send(USART_TypeDef *USARTx, uint8_t ch)
{
	while ((USARTx -> SR & 0x40) == 0);
	USARTx -> DR = ch;
}


/***********************************************
*�������ƣ�	fputc
*���ܣ�		��дfputc������ʵ��printf����
***********************************************/
#ifdef		PRINTTOUSART
int fputc(int ch, FILE *f)
{
	while ((USART1 -> SR & 0x40) == 0);
	USART1 -> DR = (uint8_t)ch;
	return ch;
}
#endif


/***********************************************
*�������ƣ�	USART1_IRQHandler
*���ܣ�		����1�жϷ�����
***********************************************/
void USART1_IRQHandler(void)
{
	uint8_t receiveData;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		receiveData = USART_ReceiveData(USART1);
		usart1Buff[usart1RecFlag++] = receiveData;
		
//		printf("rec:%c  ", receiveData);
		ESP8266_Handler(receiveData);
		
		if (receiveData == '\n')
		{
			usart1Buff[usart1RecFlag++] = NULL;
			//��Ӵ������
			//printf("%s", usart1Buff);
			
			usart1RecFlag = 0;
		}
		if (usart1RecFlag > USARTRECLENGTH - 1)
		{
			usart1RecFlag = 0;
		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);										//����жϱ�־
//		while (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) ;							//�ȴ��������
	}
}


/***********************************************
*�������ƣ�	USART2_IRQHandler
*���ܣ�		����2�жϷ�����
***********************************************/
void USART2_IRQHandler(void)
{
//	uint8_t receiveData;
//	
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		receiveData = USART_ReceiveData(USART1);
//		usart2Buff[usart2RecFlag++] = receiveData;
//		
//		if (receiveData == 0x0D)
//		{
//			//��Ӵ������
//			
//			
//			usart2RecFlag = 0;
//		}
//		if (usart2RecFlag > USARTRECLENGTH)
//		{
//			usart2RecFlag = 0;
//		}
//	}
}


/***********************************************
*�������ƣ�	USART3_IRQHandler
*���ܣ�		����3�жϷ�����
***********************************************/
void USART3_IRQHandler(void)
{
//	uint8_t receiveData;
//	
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{
//		receiveData = USART_ReceiveData(USART1);
//		usart3Buff[usart3RecFlag++] = receiveData;
//		
//		if (receiveData == 0x0D)
//		{
//			//��Ӵ������
//			
//			
//			usart3RecFlag = 0;
//		}
//		if (usart3RecFlag > USARTRECLENGTH)
//		{
//			usart3RecFlag = 0;
//		}
//	}
}


