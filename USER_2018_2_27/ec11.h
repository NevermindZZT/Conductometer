/*********************************************
*             EC11����������                 *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/1                      *
*********************************************/

/***************************************
*-------------EC11������---------------*
*����˵��		EC11A		EC11����A
*				EC11B		EC11����B
*				EC11KEY		EC11����
*				EC11TYPE	EC11����(һ��λһ����/����λһ����)
***************************************/

#ifndef		__EC11_H__
#define		__EC11_H__

#include "stm32f10x.h"

#define		EC11TYPE			1									//EC11����

#define		EC11SENSI			1									//EC11������

#define		EC11A_GPIO_CLK		RCC_APB2Periph_GPIOD				//EC11A�˿�ʱ��
#define		EC11A_GPIO_PORT		GPIOD								//EC11A�˿�
#define		EC11A_GPIO_PIN		GPIO_Pin_0							//EC11A�˿�����

#define		EC11B_GPIO_CLK		RCC_APB2Periph_GPIOD				//EC11B�˿�ʱ��
#define		EC11B_GPIO_PORT		GPIOD								//EC11B�˿�
#define		EC11B_GPIO_PIN		GPIO_Pin_1							//EC11B�˿�����

#define		EC11KEY_GPIO_CLK	RCC_APB2Periph_GPIOD				//EC11KEY�˿�ʱ��
#define		EC11KEY_GPIO_PORT	GPIOD								//EC11KEY�˿�
#define		EC11KEY_GPIO_PIN	GPIO_Pin_2							//EC11KEY�˿�����

#define		EC11_GPIO_CLK		RCC_APB2Periph_GPIOD				//EC11�˿�ʱ��
#define		EC11_GPIO_PORT		GPIOD								//EC11�˿�
#define		EC11_GOIO_PIN		(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2)	//EC11����

#define		EC11A_NOW			GPIO_ReadInputDataBit(EC11A_GPIO_PORT, EC11A_GPIO_PIN)		//��ȡEC11A��ƽ
#define		EC11B_NOW			GPIO_ReadInputDataBit(EC11B_GPIO_PORT, EC11B_GPIO_PIN)		//��ȡEC11B��ƽ
#define		EC11KEY_NOW			GPIO_ReadInputDataBit(EC11KEY_GPIO_PORT, EC11KEY_GPIO_PIN)	//��ȡEC11KEY��ƽ

void EC11_Init(void);												//EC11��ʼ��

int8_t EC11_Scan(void);												//EC11ɨ��

#endif
