/*********************************************
*             导热仪支持函数                 *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/4                      *
*********************************************/

#ifndef		__SUPPORT_H__
#define		__SUPPORT_H__

#include	"config.h"

#define		ALLOWBACK																//允许长按左键返回上一步骤
//#define		DEBUG																	//DEBUG模式
//#define		DEBUG_TEST
//#define		PID_CONTROL																//使用PID算法控制温度

#ifdef		DEBUG
	#ifndef		PRINTTOUSART
	#define		PRINTTOUSART
	#endif
	
	#define		DEBUG_PrintInfo(info)					printf(info)
	#define		DEBUG_PrintInfoWithLocation(info)		printf("File:"__FILE__", Line:%d, %s\r\n", __LINE__, info)
	#define		DEBUG_LOG(...)							printf(__VA_ARGS__)
#else
	#define		DEBUG_PrintInfo(info)
	#define		DEBUG_PrintInfoWithLocation(info)
	#define		DEBUG_LOG(...)
#endif

#ifdef		PID_CONTROL
#define		PID_KP				4													//PID算法比例系数
#define		PID_KI				2													//PID算法积分系数
#define		PID_KD				8													//PID算法微分系数
#endif

#define		SOFTWAREVERSION		"0.6.2-Beta"											//软件版本
#define     BUILDDATE           __DATE__"  \n"__TIME__                              //编译时间

#define		HeatingEnable()		PWMEnable();temperatureControl.isHeating = TRUE		//开始加热
#define		HeatingDisable()	temperatureControl.isHeating = FALSE;PWMDisable()	//结束加热

/*----------------------仪器命令宏定义---------------------*/
//#define		DEVICE_CONNECT		0
//#define		DEVICE_REGISTER		1
//#define		DEVICE_DATA			2

enum DEVICE_COMMAND
{
	DEVICE_CONNECT,																	//设备连接
	DEVICE_REGISTER,																//设备注册
	DEVICE_DATA																		//同步数据
};

typedef enum                                                                        //仪器设置项属性
{
    ITEM_COUNT,                                                                     //数字设置项
    ITEM_BOOL,                                                                      //开关设置项
    ITEM_STRING                                                                     //字符显示项
}DRY_SettingType;


/*-----------------------结构体声明---------------------*/
typedef struct                                                                      //温度控制结构体
{
	float heatingAimTemperature;                                                    //加热目标温度
	bool isHeating;                                                                 //加热状态
#ifdef		PID_CONTROL
	float pidTemperature[3];                                                        //PID
#endif  
}TEMP_Control;


typedef union                                                                       //设置项数据
{
    int8_t countData;                                                               //数字值
    bool isTrue;                                                                    //开关真假值
    uint8_t *stringData;                                                            //字符串
}DRY_SettingItemData;


typedef struct                                                                      //设置项条目结构体
{
    uint8_t *id;                                                                    //设置项名称
    uint8_t minCount;                                                               //设置数据最小值(仅类型为数字设置项可用)
    uint8_t maxCount;                                                               //设置数据最大值(仅类型为数字设置项可用)
    DRY_SettingType type;                                                           //设置项类型
    DRY_SettingItemData itemData;                                                   //设置项数据
}DRY_SettingItem;


/*------------------------全局变量声明----------------------*/
extern TEMP_Control temperatureControl;


/*--------------------------函数声明-----------------------*/
void DRY_SystemSettingScreen(void);													//设置界面

void DRY_SystemSetting(void);														//系统设置

void DRY_DisplaySettingItem(uint16_t location, uint8_t color, DRY_SettingItem settingItem);

void DRY_WelcomeScreen(void);														//初始化界面

void DRY_InputStudentNumberScreen(void);											//学号输入界面

void DRY_InputStudentNumber(void);													//输入学号

void DRY_TemperatureSettingScreen(void);											//温度设置界面

void DRY_TemperatureSetting(void);													//温度设置

void DRY_BuildBalanceScreen(void);													//建立稳恒态界面

void DRY_BuildBalance(void);														//建立稳恒态

void DRY_HeatingScreen(void);														//升温界面

void DRY_Heating(void);																//升温

void DRY_RecordingScreen(void);														//数据记录界面

void DRY_Recording(void);															//记录数据

void DRY_ShowDataScreen(void);														//显示数据界面

void DRY_ShowData(void);															//显示数据

void DRY_EnterDialogScreen(void);													//确认对话框界面

uint8_t DRY_EnterDialog(void);														//确认对话框

void DRY_CompleteScreen(void);														//实验完成对话框界面

void DRY_Complete(void);															//实验完成对话框

void DRY_DataSaveDialog(uint8_t mode);												//保存/读取数据对话框

void DRY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//绘制加热盘

void DRY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//绘制不良热导体

void DRY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//绘制散热盘

void DRY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color);		//显示数据

void DRY_TemperatureControl(void);													//温度控制

void DRY_UplaodData(uint8_t command);												//上传数据

#endif
