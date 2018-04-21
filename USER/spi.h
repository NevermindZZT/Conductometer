/*******************************************************************************
*                                  SPI 配置                                    *
*                           中南大学物理与电子学院                             *
*                                  张克强                                      *
*                                  2018/4                                      *
*******************************************************************************/


#ifndef		__SPI_H__
#define		__SPI_H__

#include	"config.h"
#include	"stm32f10x_spi.h"

#define		SPI2_NSS_PIN		GPIO_Pin_12
#define		SPI2_NSS_PORT		GPIOB

#define		SPI2_CLK_PIN		GPIO_Pin_13
#define		SPI2_CLK_PORT		GPIOB

#define		SPI2_MISO_PIN		GPIO_Pin_14
#define		SPI2_MISO_PORT		GPIOB

#define		SPI2_MOSI_PIN		GPIO_Pin_15
#define		SPI2_MOSI_PORT		GPIOB


void SPI2_Config(void);

#endif

