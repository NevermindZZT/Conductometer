/*********************************************
*             ������֧�ֺ���                 *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/4                      *
*********************************************/

#ifndef		__SUPPORT_H__
#define		__SUPPORT_H__

#include	"config.h"

#define		ALLOWBACK																//���������������һ����
//#define		DEBUG																	//DEBUGģʽ
//#define		DEBUG_TEST
//#define		PID_CONTROL																//ʹ��PID�㷨�����¶�

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
#define		PID_KP				4													//PID�㷨����ϵ��
#define		PID_KI				2													//PID�㷨����ϵ��
#define		PID_KD				8													//PID�㷨΢��ϵ��
#endif

#define		SOFTWAREVERSION		"0.6.2-Beta"											//����汾
#define     BUILDDATE           __DATE__"  \n"__TIME__                              //����ʱ��

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

typedef enum                                                                        //��������������
{
    ITEM_COUNT,                                                                     //����������
    ITEM_BOOL,                                                                      //����������
    ITEM_STRING                                                                     //�ַ���ʾ��
}DRY_SettingType;


/*-----------------------�ṹ������---------------------*/
typedef struct                                                                      //�¶ȿ��ƽṹ��
{
	float heatingAimTemperature;                                                    //����Ŀ���¶�
	bool isHeating;                                                                 //����״̬
#ifdef		PID_CONTROL
	float pidTemperature[3];                                                        //PID
#endif  
}TEMP_Control;


typedef union                                                                       //����������
{
    int8_t countData;                                                               //����ֵ
    bool isTrue;                                                                    //�������ֵ
    uint8_t *stringData;                                                            //�ַ���
}DRY_SettingItemData;


typedef struct                                                                      //��������Ŀ�ṹ��
{
    uint8_t *id;                                                                    //����������
    uint8_t minCount;                                                               //����������Сֵ(������Ϊ�������������)
    uint8_t maxCount;                                                               //�����������ֵ(������Ϊ�������������)
    DRY_SettingType type;                                                           //����������
    DRY_SettingItemData itemData;                                                   //����������
}DRY_SettingItem;


/*------------------------ȫ�ֱ�������----------------------*/
extern TEMP_Control temperatureControl;


/*--------------------------��������-----------------------*/
void DRY_SystemSettingScreen(void);													//���ý���

void DRY_SystemSetting(void);														//ϵͳ����

void DRY_DisplaySettingItem(uint16_t location, uint8_t color, DRY_SettingItem settingItem);

void DRY_WelcomeScreen(void);														//��ʼ������

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

void DRY_DataSaveDialog(uint8_t mode);												//����/��ȡ���ݶԻ���

void DRY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//���Ƽ�����

void DRY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//���Ʋ����ȵ���

void DRY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//����ɢ����

void DRY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color);		//��ʾ����

void DRY_TemperatureControl(void);													//�¶ȿ���

void DRY_UplaodData(uint8_t command);												//�ϴ�����

#endif
