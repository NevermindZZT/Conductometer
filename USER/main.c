#include	"config.h"

DRY_ExperimentalData experimentalData;									//ʵ������
uint8_t screenBrightness;												//����
uint16_t readFlash[2];													//��Flash����

int main(void)
{
	uint8_t keyScanData;
	DRY_ExperimentalData *tmpExperimentalDataPointer;
	
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
	
	MemReadByte(readFlash, 2);											//��ȡflash�б���Ļ����ź�����
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	keyScanData = KEYANDEC11_Scan();
	if (keyScanData == KEY_ENTER_LONG
		|| experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//����ϵͳ����
	{
		DRY_SystemSettingScreen();
		DRY_SystemSetting();
	}
	else if ((keyScanData == KEY_COUNT_LONG) && (AT24CXX_Check() == TRUE))	//�鿴��һ��ʵ������
	{
		DRY_DataSaveDialog(0);
		tmpExperimentalDataPointer = &experimentalData;
		AT24CXX_Read(0, (uint8_t *)tmpExperimentalDataPointer, sizeof(DRY_ExperimentalData));
		DRY_ShowDataScreen();
		DRY_ShowData();
		__set_FAULTMASK(1);												//�ر������ж�
		NVIC_SystemReset();												//ϵͳ��λ
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
