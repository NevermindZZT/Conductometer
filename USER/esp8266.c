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
	espFlag = 0;
	ESP8266_Cmd(espFlag);
	espFlag += 2;
}


/******************************************
*�������ƣ�	ESP8266_Cmd
*���ܣ�		ʹ��ָ������ESP8266
*������		ESPFlag			ָ����
*����ֵ��	��
******************************************/
void ESP8266_Cmd(uint8_t espFlag)
{
	char string[ESPDATALENGTH];
	delay_ms(10);
		switch (espFlag)
	{
		case 0:
			ESP8266_SendString("\r\n");
			delay_ms(30);
//		break;
		
		case 1:
			ESP8266_SendString("AT+CWMODE=1\r\n");								//STAģʽ
		break;
		
		case 2:
		{
			sprintf(string, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);
			ESP8266_SendString(string);
		}
		break;
		
		case 3:
			ESP8266_SendString("AT+CIPMUX=0\r\n");								//������
		break;
		
		case 4:
			ESP8266_SendString("AT+CIFSR\r\n");									//��ȡip
		break;
		
		case 5:
		{
			sprintf(string, "AT+CIPSTART=\"TCP\",\"%d.%d.%d.1\",%d\r\n", ip[0], ip[1], ip[2], TCP_PORT);
			ESP8266_SendString(string);											//TCP
		}
		break;
		
		case 6:
			ESP8266_SendString("AT+CIPMODE=1\r\n");								//��������͸��
		break;
		
		case 7:
			ESP8266_SendString("AT+CIPSEND\r\n");								//����͸��ģʽ
		break;
		
		default:
			
		break;
	}
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
		if ((espFlag < 8) && (espBuff[espBuffFlag - 3] == 'K'))
		{
			delay_ms(10);
			ESP8266_Cmd(espFlag);
			espFlag++;
			
			if (espFlag == 8)											//ESP8266��ʼ����ɣ�������������
			{
				DYY_UplaodData(DEVICE_CONNECT);
			}
		}
		if (espFlag == 5)
		{
			sscanf((const char *)espBuff, "+CIFSR:STAIP,\"%d.%d.%d.%d\"\r\n", &ip[0], &ip[1], &ip[2], &ip[3]);
		}
		if (espFlag == 8)
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

