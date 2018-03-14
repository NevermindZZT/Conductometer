/*********************************************
*         DS18B20�¶ȴ���������              *
*               ���õ�����                   *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/3                      *
*********************************************/

#ifndef		__DS18B20_H__
#define		__DS18B20_H__

#include	"stm32f10x.h"


/*--------------------�������˿ں궨��----------------------*/
#define		DS18B20A_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20A_GPIO_PORT		GPIOE
#define		DS18B20A_GPIO_PIN		GPIO_Pin_0

#define		DS18B20B_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20B_GPIO_PORT		GPIOE
#define		DS18B20B_GPIO_PIN		GPIO_Pin_1

#define		DS18B20C_GPIO_CLK		RCC_APB2Periph_GPIOE
#define		DS18B20C_GPIO_PORT		GPIOE
#define		DS18B20C_GPIO_PIN		GPIO_Pin_2

/*---------------------�˿ڲ����궨��------------------------*/
#define		DS18B20DQ_LOW			GPIO_ResetBits(DS18B20x.port, DS18B20x.pin)					//����λ�͵�ƽ
#define		DS18B20DQ_HIGH			GPIO_SetBits(DS18B20x.port, DS18B20x.pin)					//����λ�ߵ�ƽ

#define		DS18B20DQ_READ			GPIO_ReadInputDataBit(DS18B20x.port, DS18B20x.pin)			//������λ��ƽ

/*----------------------����������궨��-------------------*/
#define		READROM					0x33
#define		MATCHROM				0x55
#define		SKIPROM					0xCC
#define		SEARCHROM				0xF0
#define		ALARMSERACH				0xEC
#define		CONVERTTEMP				0x44
#define		READSCRTCHPAD			0xBE
#define		WRITESCRTCHPAD			0x4E
#define		COPYSCRATCHPAD			0x48
#define		RECALLEE				0xB8
#define		READPOWERSUPPLY			0xB4

/*----------------------IOģʽö��------------------------*/
enum DS18B20_IOMode
{
	IN,																								//IO����
	OUT																								//IO���
};

/*--------------------�������ṹ������-----------------------*/
typedef struct
{
	GPIO_TypeDef* port;																				//����������λ�˿�
	uint16_t pin;																					//����������λ����
	uint32_t clk;																					//����������λʱ��
}DS18B20_TypeDef;


/*----------------------ȫ�ֱ�������-------------------------*/
extern DS18B20_TypeDef DS18B20A;
extern DS18B20_TypeDef DS18B20B;
extern DS18B20_TypeDef DS18B20C;


/*------------------------��������--------------------------*/

void DS18B20_SetIOMode(DS18B20_TypeDef DS18B20x, uint8_t IOMode);									//��������λģʽ

bool DS18B20_Init(DS18B20_TypeDef DS18B20x);														//��ʼ��������

uint8_t DS18B20_ReadByte(DS18B20_TypeDef DS18B20x);													//��һ�ֽ�����

void DS18B20_WriteByte(DS18B20_TypeDef DS18B20x, uint8_t writeData);								//дһ�ֽ�����

void DS18B20_ConvertTemp(DS18B20_TypeDef DS18B20x);													//ת���¶�

float DS18B20_ReadTemp(DS18B20_TypeDef DS18B20x);													//��ȡ�¶�

#endif
