/*********************************************
*             ������֧�ֺ���                 *
*         ���ϴ�ѧ���������ѧԺ             *
*                 �ſ�ǿ                     *
*                 2018/4                     *
*********************************************/

#ifndef		__SUPPORT_H__
#define		__SUPPORT_H__

#include	"config.h"


#define		SOFTWAREVERSION								"0.9-Beta"					//����汾
#define		BUILDDATE									__DATE__"  \n"__TIME__		//����ʱ��

#define		ALLOWBACK																//���������������һ����
//#define		DEBUG																	//DEBUGģʽ
//#define		DEBUG_TEST

#define		PID_CONTROL									1							//ʹ��PID�㷨�����¶�(0:��ʹ��  1:λ��ʽ  2:����ʽ)


#define		DATA_CONTROLLER_ADDRESS						0x00000000
#define		DATA_START_ADDRESS							0x00001000
#define		DATA_END_ADDRESS							0x001FFFFF


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

	
#define		HeatingEnable()		PWMEnable();temperatureControl.isHeating = TRUE		//��ʼ����
#define		HeatingDisable()	temperatureControl.isHeating = FALSE;PWMDisable()	//��������

/*----------------------��������궨��---------------------*/
//#define		DEVICE_CONNECT		0
//#define		DEVICE_REGISTER		1
//#define		DEVICE_DATA			2

enum DEVICE_COMMAND
{
	DEVICE_CONNECT,																	//�豸����
	DEVICE_REGISTER,																//�豸ע��
	DEVICE_DATA																		//ͬ������
};

typedef enum																		//��������������
{
	ITEM_COUNT,																		//����������
	ITEM_BOOL,																		//����������
	ITEM_STRING,																	//�ַ���ʾ��
	ITEM_FUNCTION																	//����ִ����
}DRY_SettingType;

typedef enum
{
	DATA_READ,
	DATA_SAVE,
	DATA_LOOKUP,
	DATA_READ_FAILED,
	DATA_LOOKUP_FAILED,
	DATA_LOOKUP_END,
	DATA_ERASEING,
	DATA_ERASE_FAILED,
}DRY_DataHandlerType;

typedef enum
{
	DRY_OK,
	DRY_ERROR,
}DRY_Status;



typedef void (*settingFunction)();											//�������ִ�к�������

/*-----------------------�ṹ������---------------------*/
typedef struct																//���μ�¼����
{
	uint32_t time;															//��¼���ݵ�ʱ��
	float temperature;														//��¼���ݵ��¶�
}DRY_MeasuredData;

typedef struct																//ʵ������
{
	uint8_t machineNumber;													//������5
	uint8_t studentNumber[12];												//ѧ��
	uint8_t progress;														//ʵ�����
	float settedTemperature;												//���ü������¶�
	float balanceHeaterTempeature;											//�Ⱥ�̬�������¶�
	float balanceCoolerTempeature;											//�Ⱥ�̬ɢ�����¶�
	float heatingTempeature;												//���º�ɢ�����¶�
	DRY_MeasuredData measuredData[20];										//ʵ������
}DRY_ExperimentalData;



typedef struct																		//�¶ȿ��ƽṹ��
{
	float heatingAimTemperature;													//����Ŀ���¶�
	bool isHeating;																	//����״̬
#if		PID_CONTROL != 0
	float pidKP;
	float pidKI;
	float pidKD;
	
	#if PID_CONTROL == 1
	long double pidErr;
	#endif
	
	float pidTemperature[3];														//PID
#endif  
}TEMP_Control;


typedef union																		//����������
{
	int8_t countData;																//����ֵ
	bool isTrue;																	//�������ֵ
	uint8_t *stringData;															//�ַ���
	settingFunction function;														//��ִ�к���
}DRY_SettingItemData;


typedef struct																		//��������Ŀ�ṹ��
{
	uint8_t *id;																	//����������
	uint8_t minCount;																//����������Сֵ(������Ϊ�������������)
	uint8_t maxCount;																//�����������ֵ(������Ϊ�������������)
	DRY_SettingType type;															//����������
	DRY_SettingItemData itemData;													//����������
}DRY_SettingItem;


typedef struct
{
	uint16_t dataCount;
	uint32_t startAddress;
	uint32_t endAddress;
}DRY_DataSaveController;

/*------------------------ȫ�ֱ�������----------------------*/
extern TEMP_Control temperatureControl;

extern DRY_ExperimentalData experimentalData;


/*--------------------------��������-----------------------*/
void DRY_SystemSettingScreen(void);													//���ý���

void DRY_SystemSetting(void);														//ϵͳ����

void DRY_DisplaySettingItem(uint16_t location, uint8_t color, DRY_SettingItem settingItem);	//��ʾ������Ŀ

void DRY_DataSearchScreen(void);													//��ʾ���ҽ���

void DRY_DataSearch(void);															//���ݲ���

uint8_t DRY_ShowSearchResult(void);													//��ʾ���ҽ��

void DRY_Booting(void);																//��ʼ������

void DRY_InputStudentNumberScreen(void);											//ѧ���������

void DRY_InputStudentNumber(void);													//����ѧ��

void DRY_TemperatureSettingScreen(void);											//�¶����ý���

void DRY_TemperatureSetting(void);													//�¶�����

void DRY_BuildBalanceScreen(void);													//�����Ⱥ�̬����

void DRY_BuildBalance(void);														//�����Ⱥ�̬

void DRY_HeatingScreen(void);														//���½���

void DRY_Heating(void);																//����

void DRY_RecordingScreen(void);														//���ݼ�¼����

void DRY_Recording(void);															//��¼����

void DRY_ShowDataScreen(void);														//��ʾ���ݽ���

void DRY_ShowData(void);															//��ʾ����

void DRY_EnterDialogScreen(void);													//ȷ�϶Ի������

uint8_t DRY_EnterDialog(void);														//ȷ�϶Ի���

void DRY_CompleteScreen(void);														//ʵ����ɶԻ������

void DRY_Complete(void);															//ʵ����ɶԻ���

void DRY_DataHandlerDialog(DRY_DataHandlerType mode);												//����/��ȡ���ݶԻ���

void DRY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//���Ƽ�����

void DRY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//���Ʋ����ȵ���

void DRY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//����ɢ����

void DRY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color);		//��ʾ����

void DRY_TemperatureControl(void);													//�¶ȿ���

void DRY_UplaodData(uint8_t command);												//�ϴ�����

DRY_Status DRY_SaveExperimentalData(DRY_ExperimentalData experimentalData);

DRY_Status DRY_ReadExperimentalData(DRY_ExperimentalData *experimentalDataPointer, uint32_t dataAddress);

void DRY_EraseFlash(void);

#endif
