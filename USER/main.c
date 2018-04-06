#include	"config.h"

DRY_ExperimentalData experimentalData;									//实验数据
uint8_t screenBrightness;												//亮度
uint16_t readFlash[2];													//读Flash数据

int main(void)
{
	uint8_t keyScanData;
	DRY_ExperimentalData *tmpExperimentalDataPointer;
	
	QPYLCD_Init();
	QPYLCD_SetBackColor(WHITE);
	
	USART_Config(USART1, 115200, 0, 3);									//串口
	BASICTIM3_Config(10000, 7199, 0, 1);								//定时器3，秒定时
	BASICTIM2_Config(5000, 7199, 0, 2);									//定时器2，500ms定时
	
	KEY_Init();															//按键初始化
	EC11_Init();														//编码器初始化
	PWM_Init();															//PWM初始化
	BEEP_Init();														//蜂鸣器初始化
	ESP8266_Init();														//ESP8266初始化
	AT24CXX_Init();
	
	MemReadByte(readFlash, 2);											//读取flash中保存的机器号和亮度
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	keyScanData = KEYANDEC11_Scan();
	if (keyScanData == KEY_ENTER_LONG
		|| experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//进入系统设置
	{
		DRY_SystemSettingScreen();
		DRY_SystemSetting();
	}
	else if ((keyScanData == KEY_COUNT_LONG) && (AT24CXX_Check() == TRUE))	//查看上一次实验数据
	{
		DRY_DataSaveDialog(0);
		tmpExperimentalDataPointer = &experimentalData;
		AT24CXX_Read(0, (uint8_t *)tmpExperimentalDataPointer, sizeof(DRY_ExperimentalData));
		DRY_ShowDataScreen();
		DRY_ShowData();
		__set_FAULTMASK(1);												//关闭所有中断
		NVIC_SystemReset();												//系统复位
	}
	
	if (screenBrightness < 1 || screenBrightness > 7)					//调节亮度
	{
		screenBrightness = 1;
	}
	QPYLCD_Control(0, 0, 0, screenBrightness);
	
	DRY_WelcomeScreen();												//显示初始化界面(温度传感器初始化)

	experimentalData.progress = INPUTSTUDENTNUMBER;						//实验步骤起点
	
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
