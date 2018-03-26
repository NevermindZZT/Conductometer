#include	"config.h"

DRY_ExperimentalData experimentalData;									//实验数据
uint8_t screenBrightness;												//亮度
uint16_t readFlash[2];													//读Flash数据

int main()
{
	uint8_t a[20];
	
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
	
	printf("AT24C02 Init\r\n");
	AT24CXX_WriteOneByte(1, 'A');
	printf("read a byte:%c\r\n", AT24CXX_ReadOneByte(1));
	AT24CXX_Write(5, "data from at24c02", 17);
	AT24CXX_Read(5, a, 17);
	printf("read : %s\r\n", a);
	
	MemReadByte(readFlash, 2);											//读取flash中保存的机器号和亮度
	experimentalData.machineNumber = (uint8_t)readFlash[0];
	screenBrightness = (uint8_t)readFlash[1];
	
	if ((KEYANDEC11_Scan()) == KEY_ENTER || (KEYANDEC11_Scan()) == KEY_ENTER_LONG
		|| experimentalData.machineNumber > 30 || experimentalData.machineNumber < 1
		|| screenBrightness > 7 || screenBrightness < 1)				//进入系统设置
	{
		DRY_SystemSettingScreen();
		DRY_SystemSetting();
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
