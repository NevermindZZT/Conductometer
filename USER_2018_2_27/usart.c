/*********************************************
*          STM32USART串口驱动                *
*         中南大学物理与电子学院             *
*                张克强                      *
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
*函数名称：	USART_Config
*功能：		初始化串口
*参数：		*USARTx				串口
*			baudRate			波特率
*			preemptionPriority	抢占优先级
*			subPriority			子优先级
*返回值：	无
***********************************************/
void USART_Config(USART_TypeDef *USARTx, uint32_t baudRate, uint16_t preemptionPriority, uint16_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if (USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;									//PA9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.9
	
		//USART1_RX	  GPIOA.10初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;									//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.10
		
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
	else if (USARTx == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//使能USART2，GPIOA时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;									//PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.2
	
		//USART1_RX	  GPIOA.10初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;									//PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.3
		
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	else if (USARTx == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//使能USART3，GPIOB时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;									//PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);										//初始化GPIOB.10
	
		//USART1_RX	  GPIOA.10初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;									//PB11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;						//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOB.11
		
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	}
  

	//Usart1 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;		//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;					//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudRate;									//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	
	USART_Init(USARTx, &USART_InitStructure);										//初始化串口
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);									//开启串口接受中断
	USART_Cmd(USARTx, ENABLE);														//使能串口
}


/***********************************************
*函数名称：	USART_Send
*功能：		使用串口发送字符
*参数：		*USARTx				串口
*			ch					字符
*返回值：	无
***********************************************/
void USART_Send(USART_TypeDef *USARTx, uint8_t ch)
{
	while ((USARTx -> SR & 0x40) == 0);
	USARTx -> DR = ch;
}


/***********************************************
*函数名称：	fputc
*功能：		重写fputc函数，实现printf函数
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
*函数名称：	USART1_IRQHandler
*功能：		串口1中断服务函数
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
			//添加处理语句
			//printf("%s", usart1Buff);
			
			usart1RecFlag = 0;
		}
		if (usart1RecFlag > USARTRECLENGTH - 1)
		{
			usart1RecFlag = 0;
		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);										//清除中断标志
//		while (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) ;							//等待接收完成
	}
}


/***********************************************
*函数名称：	USART2_IRQHandler
*功能：		串口2中断服务函数
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
//			//添加处理语句
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
*函数名称：	USART3_IRQHandler
*功能：		串口3中断服务函数
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
//			//添加处理语句
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


