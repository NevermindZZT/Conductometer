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
#define		DEBUG																	//DEBUG模式

#define		SOFTWAREVERSION		"0.2Beta"											//软件版本

/*----------------------仪器命令宏定义---------------------*/
#define		DEVICE_CONNECT		0
#define		DEVICE_REGISTER		1
#define		DEVICE_DATA			2

/*--------------------------函数声明-----------------------*/
void DYY_SystemSettingScreen(void);													//设置界面

void DYY_SystemSetting(void);														//系统设置

void DYY_WelcomeScreen(void);														//初始化界面

void DYY_InputStudentNumberScreen(void);											//学号输入界面

void DYY_InputStudentNumber(void);													//输入学号

void DYY_TemperatureSettingScreen(void);											//温度设置界面

void DYY_TemperatureSetting(void);													//温度设置

void DYY_BuildBalanceScreen(void);													//建立稳恒态界面

void DYY_BuildBalance(void);														//建立稳恒态

void DYY_HeatingScreen(void);														//升温界面

void DYY_Heating(void);																//升温

void DYY_RecordingScreen(void);														//数据记录界面

void DYY_Recording(void);															//记录数据

void DYY_ShowDataScreen(void);														//显示数据界面

void DYY_ShowData(void);															//显示数据

void DYY_EnterDialogScreen(void);													//确认对话框界面

uint8_t DYY_EnterDialog(void);														//确认对话框

void DYY_CompleteScreen(void);														//实验完成对话框界面

void DYY_Complete(void);															//实验完成对话框

void DYY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//绘制加热盘

void DYY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//绘制不良热导体

void DYY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//绘制散热盘

void DYY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color);		//显示数据

void DYY_TemperatureControl(float temperature);										//温度控制

void DYY_UplaodData(uint8_t command);												//上传数据

#endif
