/*******************************************************************************
*                                  模拟I2C                                     *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/4                                      *
*******************************************************************************/

#ifndef        __IIC_H__
#define        __IIC_H__

#include    "config.h"

/*---------------------------模拟I2C引脚定义----------------------------------*/
#define        IICSCL_GPIO_CLK         RCC_APB2Periph_GPIOB
#define        IICSCL_GPIO_PORT        GPIOB
#define        IICSCL_GPIO_PIN         GPIO_Pin_6

#define        IICSDA_GPIO_CLK         RCC_APB2Periph_GPIOB
#define        IICSDA_GPIO_PORT        GPIOB
#define        IICSDA_GPIO_PIN         GPIO_Pin_7

#define        IIC_GPIO_CLK            RCC_APB2Periph_GPIOB
#define        IIC_GPIO_PORT           GPIOB
#define        IIC_GPIO_PIN            (GPIO_Pin_6 | GPIO_Pin_7)


/*--------------------------------宏定义--------------------------------------*/
#define        IIC_SclSet()            GPIO_SetBits(IICSCL_GPIO_PORT, IICSCL_GPIO_PIN)                 //时钟线高电平
#define        IIC_SclReset()          GPIO_ResetBits(IICSCL_GPIO_PORT, IICSCL_GPIO_PIN)               //时钟线低电平

#define        IIC_SdaSet()            GPIO_SetBits(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)                 //数据线高电平
#define        IIC_SdaReset()          GPIO_ResetBits(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)               //时钟线低电平

#define        IIC_SdaRead()           GPIO_ReadInputDataBit(IICSDA_GPIO_PORT, IICSDA_GPIO_PIN)        //读取数据线电平

#define        IIC_SdaIn()             IICSDA_GPIO_PORT->CRL&=0X0FFFFFFF;IICSDA_GPIO_PORT->CRL|=8u<<28 //数据线输入模式
#define        IIC_SdaOut()            IICSDA_GPIO_PORT->CRL&=0X0FFFFFFF;IICSDA_GPIO_PORT->CRL|=3<<28  //数据线输出模式


/*----------------------------------函数声明----------------------------------*/
void IIC_Init(void);                                                            //模拟I2C初始化

void IIC_Start(void);                                                           //模拟I2C开始信号

void IIC_Stop(void);                                                            //模拟I2C结束信号

bool IIC_WaitAck(void);                                                         //等待应答

void IIC_Ack(void);                                                             //发送应答

void IIC_Nack(void);                                                            //无应答

void IIC_SendByte(uint8_t data);                                                //发送一个字节数据

uint8_t IIC_ReadByte(uint8_t ack);                                              //读取一个字节数据


#endif
