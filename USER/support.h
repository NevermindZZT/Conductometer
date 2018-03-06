/*********************************************
*             ������֧�ֺ���                 *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/2                      *
*********************************************/

#ifndef		__SUPPORT_H__
#define		__SUPPORT_H__

#include	"config.h"

#define		ALLOWBACK																//���������������һ����
#define		DEBUG																	//DEBUGģʽ

#define		SOFTWAREVERSION		"0.2Beta"											//����汾

/*----------------------��������궨��---------------------*/
#define		DEVICE_CONNECT		0
#define		DEVICE_REGISTER		1
#define		DEVICE_DATA			2

/*--------------------------��������-----------------------*/
void DYY_SystemSettingScreen(void);													//���ý���

void DYY_SystemSetting(void);														//ϵͳ����

void DYY_WelcomeScreen(void);														//��ʼ������

void DYY_InputStudentNumberScreen(void);											//ѧ���������

void DYY_InputStudentNumber(void);													//����ѧ��

void DYY_TemperatureSettingScreen(void);											//�¶����ý���

void DYY_TemperatureSetting(void);													//�¶�����

void DYY_BuildBalanceScreen(void);													//�����Ⱥ�̬����

void DYY_BuildBalance(void);														//�����Ⱥ�̬

void DYY_HeatingScreen(void);														//���½���

void DYY_Heating(void);																//����

void DYY_RecordingScreen(void);														//���ݼ�¼����

void DYY_Recording(void);															//��¼����

void DYY_ShowDataScreen(void);														//��ʾ���ݽ���

void DYY_ShowData(void);															//��ʾ����

void DYY_EnterDialogScreen(void);													//ȷ�϶Ի������

uint8_t DYY_EnterDialog(void);														//ȷ�϶Ի���

void DYY_CompleteScreen(void);														//ʵ����ɶԻ������

void DYY_Complete(void);															//ʵ����ɶԻ���

void DYY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//���Ƽ�����

void DYY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//���Ʋ����ȵ���

void DYY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color);			//����ɢ����

void DYY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color);		//��ʾ����

void DYY_TemperatureControl(float temperature);										//�¶ȿ���

void DYY_UplaodData(uint8_t command);												//�ϴ�����

#endif
