#include	"config.h"

DYY_ExperimentalData experimentalData;									//实验数据
uint8_t screenBrightness;												//亮度
uint16_t readFlash[2];													//读Flash数据

int main()
{
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
	
	MemReadByte(readFlash, 2);											//读取flash中保存的机器号和亮度
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	
	if ((KEYANDEC11_Scan()) == KEY_ENTER || experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//进入系统设置
	{
		DYY_SystemSettingScreen();
		DYY_SystemSetting();
	}
	
	if (screenBrightness < 1 || screenBrightness > 7)					//调节亮度
	{
		screenBrightness = 1;
	}
	QPYLCD_Control(0, 0, 0, screenBrightness);
	
	DYY_WelcomeScreen();												//显示初始化界面(温度传感器初始化)

	experimentalData.progress = INPUTSTUDENTNUMBER;						//实验步骤起点
	
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
