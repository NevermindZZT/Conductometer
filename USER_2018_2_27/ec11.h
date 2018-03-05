/*********************************************
*             EC11编码器驱动                 *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/1                      *
*********************************************/

/***************************************
*-------------EC11编码器---------------*
*参数说明		EC11A		EC11引脚A
*				EC11B		EC11引脚B
*				EC11KEY		EC11按键
*				EC11TYPE	EC11类型(一定位一脉冲/两定位一脉冲)
***************************************/

#ifndef		__EC11_H__
#define		__EC11_H__

#include "stm32f10x.h"

#define		EC11TYPE			1									//EC11类型

#define		EC11SENSI			1									//EC11灵敏度

#define		EC11A_GPIO_CLK		RCC_APB2Periph_GPIOD				//EC11A端口时钟
#define		EC11A_GPIO_PORT		GPIOD								//EC11A端口
#define		EC11A_GPIO_PIN		GPIO_Pin_0							//EC11A端口引脚

#define		EC11B_GPIO_CLK		RCC_APB2Periph_GPIOD				//EC11B端口时钟
#define		EC11B_GPIO_PORT		GPIOD								//EC11B端口
#define		EC11B_GPIO_PIN		GPIO_Pin_1							//EC11B端口引脚

#define		EC11KEY_GPIO_CLK	RCC_APB2Periph_GPIOD				//EC11KEY端口时钟
#define		EC11KEY_GPIO_PORT	GPIOD								//EC11KEY端口
#define		EC11KEY_GPIO_PIN	GPIO_Pin_2							//EC11KEY端口引脚

#define		EC11_GPIO_CLK		RCC_APB2Periph_GPIOD				//EC11端口时钟
#define		EC11_GPIO_PORT		GPIOD								//EC11端口
#define		EC11_GOIO_PIN		(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2)	//EC11引脚

#define		EC11A_NOW			GPIO_ReadInputDataBit(EC11A_GPIO_PORT, EC11A_GPIO_PIN)		//读取EC11A电平
#define		EC11B_NOW			GPIO_ReadInputDataBit(EC11B_GPIO_PORT, EC11B_GPIO_PIN)		//读取EC11B电平
#define		EC11KEY_NOW			GPIO_ReadInputDataBit(EC11KEY_GPIO_PORT, EC11KEY_GPIO_PIN)	//读取EC11KEY电平

void EC11_Init(void);												//EC11初始化

int8_t EC11_Scan(void);												//EC11扫描

#endif
