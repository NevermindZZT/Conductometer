/*********************************************
*               ESP8266����                  *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/3                      *
*********************************************/

#ifndef		__ESP8266_H__
#define		__ESP8266_H__

#include	"config.h"

#define		GET_HOSTIP_BY_SLAVEIP

#define		TCP_PORT				8266
#define		WIFI_SSID				"CSUDRY"
#define		WIFI_PASSWORD			"csuwlsyzxdry"

#define		ESPDATALENGTH			200							//����ESP8266���ͽ������ݵ���󳤶�

typedef enum 
{
	ESP8266_NULL,
	ESP8266_AT,
	ESP8266_CWMODE,
	ESP8266_CWJAP,
	ESP8266_CIPMUX,
	ESP8266_CIFSR,
	ESP8266_CIPSTART,
	ESP8266_CIPMODE,
	ESP8266_CIPSEND,
	ESP8266_CONNECT,
}ESP8266_Command;

typedef struct
{
	bool isConnect;
	ESP8266_Command currentCommand;
	uint8_t currentIp[16];
	uint8_t hostIp[16];
}ESP8266_State;

extern ESP8266_State espState;

void ESP8266_Init(void);

void ESP8266_Cmd(uint8_t command, ...);
//void ESP8266_Cmd(uint8_t espFlag);

void ESP8266_SendByte(uint8_t ch);

void ESP8266_SendString(char *string);

void ESP8266_Handler(uint8_t usartData);

#endif
