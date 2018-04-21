/*******************************************************************************
*                                  ģ��I2C                                     *
*                           ���ϴ�ѧ���������ѧԺ                             *
*                                  �ſ�ǿ                                      *
*                                  2018/4                                      *
*******************************************************************************/

#ifndef        __IIC_H__
#define        __IIC_H__

#include    "config.h"

/*---------------------------ģ��I2C���Ŷ���----------------------------------*/
#define        IICSCL_GPIO_CLK         RCC_APB2Periph_GPIOB
#define        IICSCL_GPIO_PORT        GPIOB
#define        IICSCL_GPIO_PIN         GPIO_Pin_6

#define        IICSDA_GPIO_CLK         RCC_APB2Periph_GPIOB
#define        IICSDA_GPIO_PORT        GPIOB
#define        IICSDA_GPIO_PIN         GPIO_Pin_7

#define        IIC_GPIO_CLK            RCC_APB2Periph_GPIOB
#define        IIC_GPIO_PORT           GPIOB
#define        IIC_GPIO_PIN            (GPIO_Pin_6 | GPIO_Pin_7)


/*--------------------------------�궨��--------------------------------------*/
#define        IIC_SclSet()            GPIO_SetBits(IICSCL_GPIO_PORT, IICSCL_GPIO_PIN)                 //ʱ���߸ߵ�ƽ
#define        IIC_SclReset()          GPIO_ResetBits(IICSCL_GPIO_PORT, IICSCL_GPIO_PIN)               //ʱ���ߵ͵�ƽ

#define        IIC_SdaSet()            GPIO_SetBits(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)                 //�����߸ߵ�ƽ
#define        IIC_SdaReset()          GPIO_ResetBits(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)               //ʱ���ߵ͵�ƽ

#define        IIC_SdaRead()           GPIO_ReadInputDataBit(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)        //��ȡ�����ߵ�ƽ

#define        IIC_SdaIn()             IICSDA_GPIO_PORT->CRL&=0X0FFFFFFF;IICSDA_GPIO_PORT->CRL|=8u<<28 //����������ģʽ
#define        IIC_SdaOut()            IICSDA_GPIO_PORT->CRL&=0X0FFFFFFF;IICSDA_GPIO_PORT->CRL|=3<<28  //���������ģʽ


/*----------------------------------��������----------------------------------*/
void IIC_Init(void);                                                            //ģ��I2C��ʼ��

void IIC_Start(void);                                                           //ģ��I2C��ʼ�ź�

void IIC_Stop(void);                                                            //ģ��I2C�����ź�

bool IIC_WaitAck(void);                                                         //�ȴ�Ӧ��

void IIC_Ack(void);                                                             //����Ӧ��

void IIC_Nack(void);                                                            //��Ӧ��

void IIC_SendByte(uint8_t data);                                                //����һ���ֽ�����

uint8_t IIC_ReadByte(uint8_t ack);                                              //��ȡһ���ֽ�����


#endif
