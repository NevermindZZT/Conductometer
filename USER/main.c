#include	"config.h"

DRY_ExperimentalData experimentalData;									//ʵ������
uint8_t screenBrightness;												//����
uint16_t readFlash[2];													//��Flash����

int main()
{
	uint8_t a[20];
	
	QPYLCD_Init();
	QPYLCD_SetBackColor(WHITE);
	
	USART_Config(USART1, 115200, 0, 3);									//����
	BASICTIM3_Config(10000, 7199, 0, 1);								//��ʱ��3���붨ʱ
	BASICTIM2_Config(5000, 7199, 0, 2);									//��ʱ��2��500ms��ʱ
	
	KEY_Init();															//������ʼ��
	EC11_Init();														//��������ʼ��
	PWM_Init();															//PWM��ʼ��
	BEEP_Init();														//��������ʼ��
	ESP8266_Init();														//ESP8266��ʼ��
	AT24CXX_Init();
	
	printf("AT24C02 Init\r\n");
	AT24CXX_WriteOneByte(1, 'A');
	printf("read a byte:%c\r\n", AT24CXX_ReadOneByte(1));
	AT24CXX_Write(5, "data from at24c02", 17);
	AT24CXX_Read(5, a, 17);
	printf("read : %s\r\n", a);
	
	MemReadByte(readFlash, 2);											//��ȡflash�б���Ļ����ź�����
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	if ((KEYANDEC11_Scan()) == KEY_ENTER || (KEYANDEC11_Scan()) == KEY_ENTER_LONG
		|| experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//����ϵͳ����
	{
		DRY_SystemSettingScreen();
		DRY_SystemSetting();
	}
	
	if (screenBrightness < 1 || screenBrightness > 7)					//��������
	{
		screenBrightness = 1;
	}
	QPYLCD_Control(0, 0, 0, screenBrightness);
	
	DRY_WelcomeScreen();												//��ʾ��ʼ������(�¶ȴ�������ʼ��)

	experimentalData.progress = INPUTSTUDENTNUMBER;						//ʵ�鲽�����
	
	while(1)
	{
		switch (experimentalData.progress)
		{
			case INPUTSTUDENTNUMBER:
				DRY_InputStudentNumberScreen();
				DRY_InputStudentNumber();
				break;
			
			case TEMPERATURESETTING:
				DRY_TemperatureSettingScreen();
				DRY_TemperatureSetting();
				break;
			
			case BUILDBALANCE:
				DRY_BuildBalanceScreen();
				DRY_BuildBalance();
				break;
			
			case HEATTING:
				DRY_HeatingScreen();
				DRY_Heating();
				break;
			
			case RECORDING:
				DRY_RecordingScreen();
				DRY_Recording();
				break;
			
			case SHOWDATA:
				DRY_ShowDataScreen();
				DRY_ShowData();
				break;
			
			case COMPLETE:
				DRY_CompleteScreen();
				DRY_Complete();
				break;
			
			default:
				break;
			
		}
	}
}
