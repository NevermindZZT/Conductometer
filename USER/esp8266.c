/*********************************************
*          ESP8266TCP�ͻ�������              *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/2                      *
*********************************************/

/*
*˵����		����������ESP8266��Ϊ�ͻ��ˣ�ʹ��TCP/IPЭ������λ���������ݴ��������
*
*ʹ�÷�����	1. ��ESP8266_Init���������û����򣬽���ESP8266�ĳ�ʼ��
*			2. ʵ��MCU�Ĵ��ڣ���ESP8266_SendByte��������Ӵ��ڷ���һ���ַ��Ĵ����
*			3. ��ESP8266_Handler������ӵ������жϷ������У��˺����Ĳ���Ϊ���ڽ��յ����ַ�
*			4. ��ESP8266_Handler������ǵ�λ��������ݴ���Ĵ����
*
*ע�⣺		1. ͨ�Ų�ȡ�ַ�������ʽ���շ��ͣ�ÿ�����ݶ�������"\r\n"��β
*			2. ʹ��ESP8266_SendString���ַ������͵�����������ʵ����printf�����������ֱ��ʹ��printf������������
*			3. ʹ�ú�WIFI_SSID��WIFI_PASSWORD����Wifi ssid��password
*			4. ʹ�ú�TCP_PORT����TCP�����������˿�
*			5. ����Ĭ�Ϸ�������Ϊ����WiFi�ȵ��PC���ʷ�����ipĬ���������������±��1���豸����������Ϊ�����豸��
*				�����д��������ip���޸Ĵ�λ��ESP8266_Cmd������
*/

//#include	"stdio.h"
#include	"config.h"
#include	"stdarg.h"
#include	"string.h"

ESP8266_State espState;
uint8_t espFlag = 0;
uint32_t ip[4];
uint8_t espBuff[ESPDATALENGTH];
uint16_t espBuffFlag = 0;


/*******************************************
*�������ƣ�	ESP8266_Init
*���ܣ�		��ʼ��ESP8266
*������		��
*����ֵ��	��
*˵����		��ӵ��û�������ʵ��ESP8266��ʼ��
*******************************************/
void ESP8266_Init(void)
{
	espState.isConnect = FALSE;
	espState.currentCommand = ESP8266_NULL;
}


/******************************************
*�������ƣ�	ESP8266_Cmd
*���ܣ�		ʹ��ָ������ESP8266
*������		ESPFlag			ָ����
*����ֵ��	��
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
//			ESP8266_SendString("AT+CWMODE=1\r\n");								//STAģʽ
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
//			ESP8266_SendString("AT+CIPMUX=0\r\n");								//������
//		break;
//		
//		case 4:
//			ESP8266_SendString("AT+CIFSR\r\n");									//��ȡip
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
//			ESP8266_SendString("AT+CIPMODE=1\r\n");								//��������͸��
//		break;
//		
//		case 7:
//			ESP8266_SendString("AT+CIPSEND\r\n");								//����͸��ģʽ
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
*�������ƣ�	ESP8266_SendByte
*���ܣ�		ͨ��ESP8266����һ���ֽ�����
*������		ch			�ַ�
*����ֵ��	��
*˵����		��Ҫʵ���ַ�����
*******************************************/
void ESP8266_SendByte(uint8_t ch)
{
	USART_Send(USART1, ch);
}


/*******************************************
*�������ƣ�	ESP8266_SendString
*���ܣ�		ͨ��ESP8266�����ַ���
*������		*string		�ַ���
*����ֵ��	��
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
*�������ƣ�	ESP8266_Handler
*���ܣ�		������յ�������
*������		usartData	ͨ�����ڽ��յ����ַ�
*����ֵ��	��
*˵����		��Ҫ�ڱ��λ��������ݴ�������
*******************************************/
void ESP8266_Handler(uint8_t usartData)
{
	uint32_t command = 0, machineNumber = 0;
	
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
					espState.currentCommand = ESP8266_CIFSR;
					break;
				
				case ESP8266_CIFSR:
					ESP8266_Cmd(ESP8266_CIPSTART, "TCP", "192.168.137.1", 8266);
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
			
			if (espState.currentCommand == ESP8266_CIPSEND)											//ESP8266��ʼ����ɣ�������������
			{
				DYY_UplaodData(DEVICE_CONNECT);
			}
		}
		if (espState.currentCommand == ESP8266_CIFSR)
		{
			sscanf((const char *)espBuff, "+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", &ip[0], &ip[1], &ip[2], &ip[3]);
		}
		if (espState.currentCommand == ESP8266_CIPSEND)
		{
			//�������ݽ��գ��ڴ˴����н������ݴ���
			
			sscanf((const char *)espBuff, "rcv-%d-%d\r\n", &command, &machineNumber);
			if (machineNumber == experimentalData.machineNumber)
			{
				DYY_UplaodData(command);
			}
			else if(machineNumber == 0)
			{
				DYY_UplaodData(DEVICE_CONNECT);
			}
			
			//���ݴ������
		}
		espBuffFlag = 0;
	}
}

