/*********************************************
*               ESP8266����                  *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/2                      *
*********************************************/

#ifndef		__ESP8266_H__
#define		__ESP8266_H__

#include	"config.h"

#define		TCP_PORT				8266
#define		WIFI_SSID				"CSUDRY"
#define		WIDI_PASSWORD			"csuwlsyzxdry"

#define		ESPDATALENGTH			200							//����ESP8266���ͽ������ݵ���󳤶�

extern uint8_t espFlag;

void ESP8266_Init(void);

void ESP8266_Cmd(uint8_t espFlag);

void ESP8266_SendByte(uint8_t ch);

void ESP8266_SendString(char *string);

void ESP8266_Handler(uint8_t usartData);

#endif
