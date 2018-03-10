#include	"config.h"

DYY_ExperimentalData experimentalData;									//ʵ������
uint8_t screenBrightness;												//����
uint16_t readFlash[2];													//��Flash����

int main()
{
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
	
	MemReadByte(readFlash, 2);											//��ȡflash�б���Ļ����ź�����
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	
	if ((KEYANDEC11_Scan()) == KEY_ENTER || experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//����ϵͳ����
	{
		DYY_SystemSettingScreen();
		DYY_SystemSetting();
	}
	
	if (screenBrightness < 1 || screenBrightness > 7)					//��������
	{
		screenBrightness = 1;
	}
	QPYLCD_Control(0, 0, 0, screenBrightness);
	
	DYY_WelcomeScreen();												//��ʾ��ʼ������(�¶ȴ�������ʼ��)

	experimentalData.progress = INPUTSTUDENTNUMBER;						//ʵ�鲽�����
	
	while(1)
	{
		switch (experimentalData.progress)
		{
			case INPUTSTUDENTNUMBER:
				DYY_InputStudentNumberScreen();
				DYY_InputStudentNumber();
				break;
			
			case TEMPERATURESETTING:
				DYY_TemperatureSettingScreen();
				DYY_TemperatureSetting();
				break;
			
			case BUILDBALANCE:
				DYY_BuildBalanceScreen();
				DYY_BuildBalance();
				break;
			
			case HEATTING:
				DYY_HeatingScreen();
				DYY_Heating();
				break;
			
			case RECORDING:
				DYY_RecordingScreen();
				DYY_Recording();
				break;
			
			case SHOWDATA:
				DYY_ShowDataScreen();
				DYY_ShowData();
				break;
			
			case COMPLETE:
				DYY_CompleteScreen();
				DYY_Complete();
				break;
			
			default:
				break;
			
		}
	}
}
