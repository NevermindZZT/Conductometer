/*********************************************
*          ESP8266TCP客户端驱动              *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/3                      *
*********************************************/

/*
*说明：		此驱动用作ESP8266作为客户端，使用TCP/IP协议与上位机进行数据传输的驱动
*
*使用方法：	1. 将ESP8266_Init函数放入用户程序，进行ESP8266的初始化
*			2. 实现MCU的串口，在ESP8266_SendByte函数中添加串口发送一个字符的代码块
*			3. 将ESP8266_Handler函数添加到串口中断服务函数中，此函数的参数为串口接收到的字符
*			4. 在ESP8266_Handler函数标记的位置添加数据处理的代码块
*
*注意：		1. 通信采取字符串的形式接收发送，每条数据都必须以"\r\n"结尾
*			2. 使用ESP8266_SendString将字符串发送到服务器，若实现了printf函数，则可以直接使用printf函数发送数据
*			3. 使用宏WIFI_SSID与WIFI_PASSWORD定义Wifi ssid与password
*			4. 使用宏TCP_PORT定义TCP服务器监听端口
*			5. 驱动默认服务器即为开启WiFi热点的PC，故服务器ip默认连接至局域网下编号1的设备，若服务器为其他设备，
*				需自行处理服务器ip，修改处位于ESP8266_Cmd函数中
*/

//#include	"stdio.h"
#include	"config.h"
#include	"stdarg.h"
#include	"string.h"

ESP8266_State espState;
uint8_t espBuff[ESPDATALENGTH];
uint16_t espBuffFlag = 0;


/*******************************************
*函数名称：	ESP8266_Init
*功能：		初始化ESP8266
*参数：		无
*返回值：	无
*说明：		添加到用户代码中实现ESP8266初始化
*******************************************/
void ESP8266_Init(void)
{
	espState.isConnect = FALSE;
	espState.currentCommand = ESP8266_NULL;
}


/******************************************
*函数名称：	ESP8266_Cmd
*功能：		使用指令配置ESP8266
*参数：		ESPFlag			指令编号
*返回值：	无
******************************************/
//void ESP8266_Cmd(uint8_t espFlag)
//{
//	char string[ESPDATALENGTH];
//	delay_ms(10);
//		switch (espFlag)
//	{
//		case 0:
//			ESP8266_SendString("\r\n");
//			delay_ms(30);
////		break;
//		
//		case 1:
//			ESP8266_SendString("AT+CWMODE=1\r\n");								//STA模式
//		break;
//		
//		case 2:
//		{
//			sprintf(string, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
//			ESP8266_SendString(string);
//		}
//		break;
//		
//		case 3:
//			ESP8266_SendString("AT+CIPMUX=0\r\n");								//多连接
//		break;
//		
//		case 4:
//			ESP8266_SendString("AT+CIFSR\r\n");									//获取ip
//		break;
//		
//		case 5:
//		{
//			sprintf(string, "AT+CIPSTART=\"TCP\",\"%d.%d.%d.1\",%d\r\n", ip[0], ip[1], ip[2], TCP_PORT);
//			ESP8266_SendString(string);											//TCP
//		}
//		break;
//		
//		case 6:
//			ESP8266_SendString("AT+CIPMODE=1\r\n");								//开启数据透传
//		break;
//		
//		case 7:
//			ESP8266_SendString("AT+CIPSEND\r\n");								//进入透传模式
//		break;
//		
//		default:
//			
//		break;
//	}
//}


void ESP8266_Cmd(uint8_t command, ...)
{
	char str[ESPDATALENGTH];
	char *p[2];
	int temp;
	va_list arg;
	
	va_start(arg, command);																	//
	
	switch (command)
	{
		case ESP8266_AT:
			ESP8266_SendString("AT\r\n");
			break;
		
		case ESP8266_CWMODE:
			sprintf(str, "AT+CWMODE=%d\r\n", (char)va_arg(arg, int));
			ESP8266_SendString(str);
			break;
		
		case ESP8266_CWJAP:
			p[0] = (char*)va_arg(arg, char*);
			p[1] = (char*)va_arg(arg, char*);
			sprintf(str, "AT+CWJAP=\"%s\",\"%s\"\r\n", p[0], p[1]);
			ESP8266_SendString(str);
			break;
		
		case ESP8266_CIPMUX:
			sprintf(str, "AT+CIPMUX=%d\r\n", (char)va_arg(arg, int));
			ESP8266_SendString(str);
			break;
		
		case ESP8266_CIFSR:
			ESP8266_SendString("AT+CIFSR\r\n");
			break;
		
		case ESP8266_CIPSTART:
			p[0] = (char*)va_arg(arg, char*);
			p[1] = (char*)va_arg(arg, char*);
			temp = (short)va_arg(arg, int);
			sprintf(str, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", p[0], p[1], temp);
			ESP8266_SendString(str);
			break;
		
		case ESP8266_CIPMODE:
			sprintf(str, "AT+CIPMODE=%d\r\n", (char)va_arg(arg, int));
			ESP8266_SendString(str);
			break;
		
		case ESP8266_CIPSEND:
			ESP8266_SendString("AT+CIPSEND\r\n");
			break;
		
		default:
		
			break;
	}
	
	va_end(arg);
}

/*******************************************
*函数名称：	ESP8266_SendByte
*功能：		通过ESP8266发送一个字节数据
*参数：		ch			字符
*返回值：	无
*说明：		需要实现字符发送
*******************************************/
void ESP8266_SendByte(uint8_t ch)
{
	USART_Send(USART1, ch);
}


/*******************************************
*函数名称：	ESP8266_SendString
*功能：		通过ESP8266发送字符串
*参数：		*string		字符串
*返回值：	无
*******************************************/
void ESP8266_SendString(char *string)
{
	while (*string)
	{
		ESP8266_SendByte(*string);
		string++;
	}
}


/*******************************************
*函数名称：	ESP8266_Handler
*功能：		处理接收到的数据
*参数：		usartData	通过串口接收到的字符
*返回值：	无
*说明：		需要在标记位置添加数据处理代码块
*******************************************/
void ESP8266_Handler(uint8_t usartData)
{
	uint32_t command = 0, machineNumber = 0;
	uint16_t tempIp[4];
	
	espBuff[espBuffFlag++] = usartData;
	
	if (espBuffFlag > ESPDATALENGTH - 1)
	{
		espBuffFlag = 0;
	}
	
	if (usartData == '\n')
	{
		espBuff[espBuffFlag] = NULL;
		
		if ((espState.isConnect == FALSE) && ((strcmp((char*)espBuff, "ready\r\n") == 0)
			|| (espState.currentCommand == ESP8266_AT && (strcmp((char*)espBuff, "OK\r\n") == 0))))
		{
			espState.isConnect = TRUE;
			if (strcmp((char*)espBuff, "ready\r\n") == 0)
			{
				ESP8266_Cmd(ESP8266_AT);
			}
		}
		
		if ((espState.currentCommand != ESP8266_CIPSEND) && (strcmp((char*)espBuff, "OK\r\n") == 0))
		{
			delay_ms(10);
			
			switch ((uint8_t)espState.currentCommand)
			{
				case ESP8266_NULL:
					ESP8266_Cmd(ESP8266_AT);
					espState.currentCommand = ESP8266_AT;
					break;
				
				case ESP8266_AT:
					ESP8266_Cmd(ESP8266_CWMODE, 1);
					espState.currentCommand = ESP8266_CWMODE;
					break;
				
				case ESP8266_CWMODE:
					ESP8266_Cmd(ESP8266_CWJAP, WIFI_SSID, WIFI_PASSWORD);
					espState.currentCommand = ESP8266_CWJAP;
					break;
				
				case ESP8266_CWJAP:
					ESP8266_Cmd(ESP8266_CIPMUX, 0);
					espState.currentCommand = ESP8266_CIPMUX;
					break;
				
				case ESP8266_CIPMUX:
					ESP8266_Cmd(ESP8266_CIFSR);
					espState.currentCommand = ESP8266_CIFSR;
					break;
				
				case ESP8266_CIFSR:
					ESP8266_Cmd(ESP8266_CIPSTART, "TCP", espState.hostIp, TCP_PORT);
					espState.currentCommand = ESP8266_CIPSTART;
					break;
				
				case ESP8266_CIPSTART:
					ESP8266_Cmd(ESP8266_CIPMODE, 1);
					espState.currentCommand = ESP8266_CIPMODE;
					break;
				
				case ESP8266_CIPMODE:
					ESP8266_Cmd(ESP8266_CIPSEND);
					espState.currentCommand = ESP8266_CIPSEND;
					break;
				
				default :
					break;
			}
			
			if (espState.currentCommand == ESP8266_CIPSEND)											//ESP8266初始化完成，主动建立连接
			{
				DRY_UplaodData(DEVICE_CONNECT);
			}
		}
		if ((espState.currentCommand == ESP8266_CIFSR) && (espBuff[0] == '+') && (espBuff[10] == 'I'))
		{
			//sscanf((const char *)espBuff, "+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", &tempIp[0], &tempIp[1], &tempIp[2], &tempIp[3]);
			sscanf((const char *)espBuff, "+CIFSR:STAIP,\"%s\"\r\n", espState.currentIp);
#ifdef		GET_HOSTIP_BY_SLAVEIP
			sscanf((char*)espState.currentIp, "%hd.%hd.%hd.%hd", &tempIp[0], &tempIp[1], &tempIp[2], &tempIp[3]);
			sprintf((char*)espState.hostIp, "%d.%d.%d.1", tempIp[0], tempIp[1], tempIp[2]);
#endif
		}
		if (espState.currentCommand == ESP8266_CIPSEND)
		{
			//正常数据接收，在此处进行接收数据处理
			
			sscanf((const char *)espBuff, "rcv-%d-%d\r\n", &command, &machineNumber);
			if (machineNumber == experimentalData.machineNumber)
			{
				DRY_UplaodData(command);
			}
			else if(machineNumber == 0)
			{
				DRY_UplaodData(DEVICE_CONNECT);
			}
			
			//数据处理结束
		}
		espBuffFlag = 0;
	}
}

