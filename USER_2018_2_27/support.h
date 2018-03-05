/*********************************************
*             导热仪支持函数                 *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/2                      *
*********************************************/

#ifndef		__SUPPORT_H__
#define		__SUPPORT_H__

#include	"config.h"

#define		ALLOWBACK																//允许长按左键返回上一步骤
//#define		DEBUG																	//DEBUG模式

#define		SOFTWAREVERSION		"0.2Beta"											//软件版本

#define		DEVICE_CONNECT		0
#define		DEVICE_REGISTER		1
#define		DEVICE_DATA			2

void DYY_SystemSettingScreen(void);

void DYY_SystemSetting(void);

void DYY_WelcomeScreen(void);

void DYY_InputStudentNumberScreen(void);

void DYY_InputStudentNumber(void);

void DYY_TemperatureSettingScreen(void);

void DYY_TemperatureSetting(void);

void DYY_BuildBalanceScreen(void);

void DYY_BuildBalance(void);

void DYY_HeatingScreen(void);

void DYY_Heating(void);

void DYY_RecordingScreen(void);

void DYY_Recording(void);

void DYY_ShowDataScreen(void);

void DYY_ShowData(void);

void DYY_EnterDialogScreen(void);

uint8_t DYY_EnterDialog(void);

void DYY_CompleteScreen(void);

void DYY_Complete(void);

void DYY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color);

void DYY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color);

void DYY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color);

void DYY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color);

void DYY_TemperatureControl(float temperature);

void DYY_UplaodData(uint8_t command);

#endif
