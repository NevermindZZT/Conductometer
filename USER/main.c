#include	"config.h"

DRY_ExperimentalData experimentalData =
{
	.studentNumber = "0",
};									//ʵ������
uint8_t screenBrightness;												//����
uint16_t readFlash[2];													//��Flash����

int main(void)
{
	uint8_t keyScanData;
	
	QPYLCD_Init();
	
	USART_Config(USART1, 115200, 0, 3);									//����
	BASICTIM3_Config(10000, 7199, 0, 1);								//��ʱ��3���붨ʱ
	BASICTIM2_Config(5000, 7199, 0, 2);									//��ʱ��2��500ms��ʱ
	
	KEY_Init();															//������ʼ��
	EC11_Init();														//��������ʼ��
	PWM_Init();															//PWM��ʼ��
	BEEP_Init();														//��������ʼ��
	ESP8266_Init();														//ESP8266��ʼ��
	AT24CXX_Init();
	W25X16_Init();
	SPI2_Config();
	
	
	MemReadByte(readFlash, 2);											//��ȡflash�б���Ļ����ź�����
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	keyScanData = KEYANDEC11_Scan();
	if (keyScanData == KEY_ENTER_LONG
		|| experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//����ϵͳ����
	{
		DRY_SystemSetting();
	}
	else if ((keyScanData == KEY_COUNT_LONG) && (W25X16_Check() == TRUE))	//�鿴��һ��ʵ������
	{
		DRY_DataSearch();												//���ݲ�ѯ
		__set_FAULTMASK(1);												//�ر������ж�
		NVIC_SystemReset();												//ϵͳ��λ
	}
	
	if (screenBrightness < 1 || screenBrightness > 7)					//��������
	{
		screenBrightness = 1;
	}
	QPYLCD_Control(0, 0, 0, screenBrightness);
	
	DRY_Booting();														//��ʾ��ʼ������(�¶ȴ�������ʼ��)
	
	experimentalData.progress = INPUTSTUDENTNUMBER;						//ʵ�鲽�����
	
	while(1)
	{
		switch (experimentalData.progress)
		{
			case INPUTSTUDENTNUMBER:
				DRY_InputStudentNumber();
				break;
			
			case TEMPERATURESETTING:
				DRY_TemperatureSetting();
				break;
			
			case BUILDBALANCE:
				DRY_BuildBalance();
				break;
			
			case HEATTING:
				DRY_Heating();
				break;
			
			case RECORDING:
				DRY_Recording();
				break;
			
			case SHOWDATA:
				DRY_ShowData();
				break;
			
			case COMPLETE:
				DRY_Complete();
				break;
			
			default:
				break;
			
		}
	}
}
