/****************** ***************************
*             ������֧�ֺ���                 *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/2                      *
*********************************************/

#include	"support.h"

TEMP_Control temperatureControl;

/*******************************************
*�������ƣ�	DYY_SystemSettingScreen
*���ܣ�		��ʾ�������ý���
*������		��
*����ֵ��	��
*******************************************/
void DYY_SystemSettingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayString(176, 16, WHITE, FONT16X32, "Settings");								//��ʾ��Settings��
	
	QPYLCD_SetBackColor(CYAN);																	//��ʾ��Machine Number��
	QPYLCD_DrawRectangle(0, 64, 480, 47, CYAN);
	QPYLCD_DisplayString(5, 76, BLACK, FONT16X24, "Machine Number");
	QPYLCD_DisplayInt(320, 76, BLACK, FONT16X24, experimentalData.machineNumber);				//��ʾ������
	QPYLCD_DrawLine(0, 111, 479, 111, QPYLCD_NewColor(1, 1, 1));
	
	QPYLCD_SetBackColor(WHITE);																	//��ʾ"Brightness"
	QPYLCD_DisplayString(5, 124, BLACK, FONT16X24, "Brightness");
	QPYLCD_DisplayInt(320, 124, BLACK, FONT16X24, screenBrightness);							//��ʾ���ȵȼ�
	QPYLCD_DrawLine(0, 159, 479, 159, QPYLCD_NewColor(1, 1, 1));
	
	QPYLCD_DisplayString(5, 172, BLACK, FONT16X24, "Software Version");							//��ʾ��Software Version��
	QPYLCD_DisplayString(320, 172, BLACK, FONT16X24, SOFTWAREVERSION);							//��ʾ�汾
	QPYLCD_DrawLine(0, 207, 479, 207, QPYLCD_NewColor(1, 1, 1));
}


/*******************************************
*�������ƣ�	DYY_SystemSetting
*���ܣ�		��������
*������		��
*����ֵ��	��
*******************************************/
void DYY_SystemSetting(void)
{
	uint8_t scanData;																			//������ֵ
	uint8_t tempData[3];																		//��������
	int8_t i = 0;
	const uint8_t str[3][20] = {"Machine Number", "Brightness", "Software Version"};			//����ѡ��
	
	tempData[0] = experimentalData.machineNumber;												//�������ݳ�ʼ��
	tempData[1] = screenBrightness;
	
	QPYLCD_SetBackColor(CYAN);
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_RIGHT:																	//�����ң��ƶ����ù��
					if (i < 2)
					{
						QPYLCD_SetBackColor(WHITE);
						QPYLCD_DrawRectangle(0, 64 + i * 48, 480, 47, WHITE);
						QPYLCD_DisplayString(5, 76 + i * 48, BLACK, FONT16X24, str[i]);
						QPYLCD_DisplayInt(320, 76 + i * 48, BLACK, FONT16X24, tempData[i]);
						i++;
						QPYLCD_SetBackColor(CYAN);
						QPYLCD_DrawRectangle(0, 64 + i * 48, 480, 47, CYAN);
					}
					break;
				
				case KEY_LEFT:																	//�������ƶ����ù��
					if (i > 0)
					{
						QPYLCD_SetBackColor(WHITE);
						QPYLCD_DrawRectangle(0, 64 + i * 48, 480, 47, WHITE);
						QPYLCD_DisplayString(5, 76 + i * 48, BLACK, FONT16X24, str[i]);
						QPYLCD_DisplayInt(320, 76 + i * 48, BLACK, FONT16X24, tempData[i]);
						i--;
						QPYLCD_SetBackColor(CYAN);
						QPYLCD_DrawRectangle(0, 64 + i * 48, 480, 47, CYAN);
					}
					break;
				
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						readFlash[0] = experimentalData.machineNumber = tempData[0];			//������������
						readFlash[1] = screenBrightness = tempData[1];
						MemWriteByte(readFlash, 2);												//д��Flash
						return;
					}
					break;
				
				case EC11_UP:																	//������˳ʱ��
					tempData[i]++ ;																//������������
					break;
				
				case EC11_DOWN:																	//��������ʱ��
					tempData[i]-- ;																//�������ݼ���
					break;
				
				default:
					break;
			}
			if (tempData[i] < 1 || tempData[0] > 30 || tempData[1] > 7)							//�ж��������ݷ�Χ
			{
				tempData[i] = 1;
			}
			QPYLCD_SetBackColor(CYAN);
			QPYLCD_DisplayString(5, 76 + i * 48, BLACK, FONT16X24, str[i]);						//ˢ����ʾ
			QPYLCD_DrawRectangle(320, 76 + i * 48, 32, 24, CYAN);
			QPYLCD_DisplayInt(320, 76 + i * 48, BLACK, FONT16X24, tempData[i]);
			
			if (i != 2)																			//���ְ汾����ʾ����
			{
				QPYLCD_SetBackColor(WHITE);
			}
			QPYLCD_DisplayString(320, 172, BLACK, FONT16X24, SOFTWAREVERSION);					//���ְ汾����ʾ����
			
		}
	}
}


/*******************************************
*�������ƣ�	DYY_WelcomeScreen
*���ܣ�		��ʾ������ʼ������
*������		��
*����ֵ��	��
*******************************************/
void DYY_WelcomeScreen(void)
{
	uint8_t i;
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
//	QPYLCD_DrawColorImage(0, 60, 480, 150, logo1);
	QPYLCD_DrawColorImage(0, 60, 150, 150, logo);												//��ʾlogo(Ժ��)
	QPYLCD_DisplayCharacters(155, 90, QPYLCD_NewColor(0, 2, 2), FONT32X32, 10, zndxwlsyzx);		//��ʾ�����ϴ�ѧ����ʵ�����ġ�
	QPYLCD_DisplayCharacters(171, 150, QPYLCD_NewColor(0, 2, 2), FONT24X24, 12, blrdt);			//��ʾ�������ȵ��嵼��ϵ�������ǡ�
	QPYLCD_DisplayCharacters(168, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 8, cshwdcgq);		//��ʾ����ʼ���¶ȴ�������
	QPYLCD_DisplayString(296, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "...");					//
	if ((!DS18B20_Init(DS18B20A)) || (!DS18B20_Init(DS18B20B)) || (!DS18B20_Init(DS18B20C)))	//�ж��¶ȴ������Ƿ�ɹ�����	
	{
		QPYLCD_DrawRectangle(168, 242, 152, 16, WHITE);
		QPYLCD_DisplayCharacters(112, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 16, wdcgqcshsb);//��ʾ���¶ȴ�������ʼ��ʧ�ܡ�

#ifndef		DEBUG																				//DEBUGģʽ�رձ���
		BEEP_ON;																				//����������
		while(1);																				//��ʼ��ʧ�ܣ���������
#endif
	}
	delay_s(1);
	
	QPYLCD_DrawRectangle(112, 242, 256, 16, WHITE);
	QPYLCD_DisplayString(184, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "WiFi");
	QPYLCD_DisplayCharacters(216, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 3, wifilj);
	QPYLCD_DisplayString(264, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "...");
	
	i = 10;
	while (i--)
	{
		if(espState.isConnect == FALSE)
		{
			ESP8266_Cmd(ESP8266_AT);
			espState.currentCommand = ESP8266_AT;
			delay_ms(100);
		}
		else
		{
			break;
		}
	}
	if (espState.isConnect == FALSE)
	{
		QPYLCD_DisplayString(136, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "WiFi");
		QPYLCD_DisplayCharacters(168, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 2, wifilj);
		QPYLCD_DisplayCharacters(200, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 9, wifilj + 32 * 3);
		delay_ms(500);
	}
	delay_s(1);
}


/*******************************************
*�������ƣ�	DYY_InputStudentNumberScreen
*���ܣ�		��ʾ��������ѧ�Ž���
*������		��
*����ֵ��	��
*******************************************/
void DYY_InputStudentNumberScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, qsrxh);								//��ʾ��������ѧ�š�
	
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(124, 168, BLACK, FONT24X24, 3, xuehao);							//��ʾ��ѧ�ţ���
	QPYLCD_DrawLine(196, 194, 356, 194, BLACK);
}


/*******************************************
*�������ƣ�	DYY_InputStudentNumber
*���ܣ�		ѧ������
*������		��
*����ֵ��	��
*******************************************/
void DYY_InputStudentNumber(void)
{
	uint8_t scanData;																			//������ֵ
	uint8_t tempData = 0;																		//��ǰ���λ������
	uint16_t xLabel = 196;																		//���x����
	uint8_t i = 0;
	uint8_t j;
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//����һ������ȷ�ϼ�����˳����ж�ȷ�ϼ����ɿ�
	
	QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);									//Ĭ��ˢ��һ������
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//�����󣬹������
					if (i > 0)
					{
						i--;
						QPYLCD_DrawRectangle(xLabel, 168, 16, 24, WHITE);
						xLabel -= 16;
						tempData = 0;
					}
					break;
				
				case KEY_RIGHT:																	//�����ң����浱ǰλ�����ݲ��������
					if (i < 9)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData);
						experimentalData.studentNumber[i++] = tempData + 48;
						xLabel += 16;
						tempData = 0;
					}
					break;
				
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						DYY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DYY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.studentNumber[i++] = tempData + 48;				//�������һ������
							experimentalData.studentNumber[i] = NULL;							//ѧ�Ž���
							experimentalData.progress = TEMPERATURESETTING;						//ѧ��������������̽�����һ��
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DYY_InputStudentNumberScreen();										//ˢ����ʾ
							xLabel = 196;
							for (j = 0; j < i; j++)												//�ָ�һ����ѧ��������ʾ
							{
								QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, experimentalData.studentNumber[j] - 48);
								xLabel += 16;
							}
						}
						
					}
					break;
				
				case EC11_UP:																	//������˳ʱ��
					tempData++;																	//���λ����������
					if (tempData > 9)															//���ݷ�Χ����
						tempData = 0;
					break;
					
				case EC11_DOWN:																	//��������ʱ��
					tempData--;                                                                 //���λ�����ݼ���
					if (tempData > 9)                                                           //���ݷ�Χ����
						tempData = 9;
					break;
				
				default:
					break;
				
			}
			QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);							//ˢ����ʾ
		}
	}
}


/*******************************************
*�������ƣ�	DYY_TemperatureSettingScreen
*���ܣ�		��ʾ�����¶����ý���
*������		��
*����ֵ��	��
*******************************************/
void DYY_TemperatureSettingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(176, 16, WHITE, FONT32X32, 4, wdsz);								//��ʾ���¶����á�
	
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(124, 168, BLACK, FONT24X24, 2, jrpwd + 72 * 7);					//��ʾ�����ü������¶ȣ���
	QPYLCD_DisplayCharacters(172, 168, BLACK, FONT24X24, 6, jrpwd);	
	QPYLCD_DrawLine(316, 194, 348, 194, BLACK);
}


/*******************************************
*�������ƣ�	DYY_TemperatureSetting
*���ܣ�		�����¶�����
*������		��
*����ֵ��	��
*******************************************/
void DYY_TemperatureSetting(void)
{
	uint8_t scanData;																			//������ֵ
	uint8_t tempData[2] = {0, 0};																//�����¶�ʮλ����λ
	uint16_t xLabel = 316;																		//���x����
	uint8_t i = 0;
	
	QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[0]);								//Ĭ��ˢ��һ������
	QPYLCD_DisplayInt(xLabel + 16, 168, BLACK, FONT16X24, tempData[1]);
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//�жϰ����ɿ�
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//�����󣬹������
					if (i > 0)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData[i]);
						i--;
						xLabel -= 16;
					}
#ifdef		ALLOWBACK																			//������������һ����
					QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[i]);				//ˢ����ʾ���������ڳ���������ɵ���ʾ�ӳ�
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
					{
						experimentalData.progress = INPUTSTUDENTNUMBER;							//������һ����
						return;
					}
#endif
					break;
				
				case KEY_RIGHT:																	//�����ң��������
					if (i < 1)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData[i]);
						i++;
						xLabel += 16;
					}	
					break;
				
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						DYY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DYY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.settedTemperature = tempData[0] * 10 + tempData[1];//��������
							experimentalData.progress = BUILDBALANCE;							//������һ����
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DYY_TemperatureSettingScreen();										//ˢ����ʾ
							
							QPYLCD_DisplayInt(316, 168, BLACK, FONT16X24, tempData[0]);			//�ָ�������ʾ
							QPYLCD_DisplayInt(332, 168, BLACK, FONT16X24, tempData[1]);
						}
					}
					break;
				
				case EC11_UP:																	//������˳ʱ��
					tempData[i]++;                                                              //���λ����������
					if (tempData[i] > 9)                                                        //���ݷ�Χ����
						tempData[i] = 0;                                                        
					break;                                                                      
					                                                                            
				case EC11_DOWN:                                                                 //��������ʱ��
					tempData[i]--;                                                              //���λ�����ݼ���
					if (tempData[i] > 9)                                                        //���ݷ�Χ����
						tempData[i] = 9;
					break;
				
				default:
					break;
				
			}
			QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[i]);						//ˢ����ʾ
		}
	}
}


/*******************************************
*�������ƣ�	DYY_BuildBalanceScreen
*���ܣ�		��ʾ���������Ⱥ�̬����
*������		��
*����ֵ��	��
*******************************************/
void DYY_BuildBalanceScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, jlwht);								//��ʾ�������Ⱥ�̬��
	
	DYY_DrawEquipment1(320, 80, YELLOW);														//����ͼ��
	DYY_DrawEquipment2(320, 144, YELLOW);
	DYY_DrawEquipment3(320, 160, YELLOW);
	
	QPYLCD_SetBackColor(WHITE);
	
	QPYLCD_DisplayCharacters(5, 96, BLACK, FONT24X24, 6, jrpwd);								//��ʾ���������¶ȡ�
	
	QPYLCD_DisplayCharacters(5, 136, BLACK, FONT24X24, 1, jrpwd + 6 * 72);						//��ʾ��ɢ�����¶ȡ�
	QPYLCD_DisplayCharacters(29, 136, BLACK, FONT24X24, 5, jrpwd + 72);
	
	QPYLCD_DisplayCharacters(5, 176, BLACK, FONT24X24, 1, jishi);								//��ʾ����ʱ����
	QPYLCD_DisplayCharacters(5 + 24*4, 176, BLACK, FONT24X24, 2, jishi + 72);
	
	QPYLCD_DisplayCharacters(5, 216, BLACK, FONT24X24, 2, jishi);								//��ʾ����ʱʱ�¶ȣ���
	QPYLCD_DisplayCharacters(5 + 24*2, 216, BLACK, FONT24X24, 1, jishi + 72);
	QPYLCD_DisplayCharacters(5 + 24*3, 216, BLACK, FONT24X24, 3, jrpwd + 72*3);
	
	QPYLCD_DisplayCharacters(280, 216, BLACK, FONT24X24, 2, dangqian);							//��ʾ����ǰ�¶ȡ�
	QPYLCD_DisplayCharacters(280 + 24*2, 216, BLACK, FONT24X24, 3, jrpwd + 72*3);
}


/*******************************************
*�������ƣ�	DYY_BuildBalance
*���ܣ�		�����Ⱥ�̬
*������		��
*����ֵ��	��
*******************************************/
void DYY_BuildBalance(void)
{
	float tempA, tempB;																			//�����̣�ɢ�����¶�
	uint8_t str[10];
	uint8_t scanData;																			//������ֵ
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//�жϰ����ɿ�
	
	tim3Count = 0;																				//��ʱ������
	tempB = DS18B20_ReadTemp(DS18B20B);															//Ĭ��ˢ��һ������
	tempB = DS18B20_ReadTemp(DS18B20B);															//�ض��¶ȣ���֤������
	sprintf((char *)str, "%-5.1f", tempB);
	QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
	
	HeatingEnable();																			//��������
	temperatureControl.heatingAimTemperature = 60;												//���ü���Ŀ���¶�
	
	while (1)
	{
//		DYY_TemperatureControl(experimentalData.settedTemperature);								//�¶ȿ���
																								//����PID�㷨���¶ȿ����ɶ�ʱ������	
		
		tempA = DS18B20_ReadTemp(DS18B20A);														//��ȡ�¶�
		tempB = DS18B20_ReadTemp(DS18B20B);
		
		sprintf((char *)str, "%-5.1f", tempA);													//��ʾ�������¶�
//		QPYLCD_DrawRectangle(149, 96, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 96, BLACK, FONT16X24, str);
		
		sprintf((char *)str, "%-5.1f", tempB);													//��ʾɢ�����¶�
//		QPYLCD_DrawRectangle(149, 136, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 136, BLACK, FONT16X24, str);
		QPYLCD_DisplayString(400, 216, BLACK, FONT16X24, str);									//��ǰ�¶�
		
		sprintf((char *)str, "%d:%02d", tim3Count / 60, tim3Count % 60);						//��ʾ��ʱʱ��
//		QPYLCD_DrawRectangle(149, 176, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 176, BLACK, FONT16X24, str);
		
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//������
#ifdef		ALLOWBACK																			//������������һ����
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
					{
						experimentalData.progress = TEMPERATURESETTING;							//������һ����
						return;
					}
#endif
					break;
				
				case KEY_COUNT:																	//������ʱ�����¼�ʱ
					tim3Count = 0;																//��ʱ������
					sprintf((char *)str, "%-5.1f", tempB);										//ˢ�¼�ʱʱ�¶�
//					QPYLCD_DrawRectangle(149, 216, 80, 24, WHITE);
					QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
					break;
				
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						DYY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DYY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.balanceTempeatrue = tempB;							//��¼����
							experimentalData.progress = HEATTING;								//������һ����
							
							HeatingDisable();													//�رռ���
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DYY_BuildBalanceScreen();											//ˢ����ʾ
							
							tempB = DS18B20_ReadTemp(DS18B20B);									//ˢ������
							sprintf((char *)str, "%-5.1f", tempB);
							QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
						}
					}
					break;
				
				default:
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DYY_HeatingScreen
*���ܣ�		��ʾ����ɢ�������½���
*������		��
*����ֵ��	��
*******************************************/
void DYY_HeatingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, srpsw);								//��ʾ��ɢ�������¡�
	
	DYY_DrawEquipment1(320, 100, YELLOW);														//����ͼ��
	DYY_DrawEquipment3(320, 164, YELLOW);
	
	QPYLCD_SetBackColor(WHITE);	
	
	QPYLCD_DisplayCharacters(5, 124, BLACK, FONT24X24, 6, jrpwd);								//��ʾ���������¶ȡ�
	
	QPYLCD_DisplayCharacters(5, 188, BLACK, FONT24X24, 1, jrpwd + 6 * 72);						//��ʾ��ɢ�����¶ȡ�
	QPYLCD_DisplayCharacters(29, 188, BLACK, FONT24X24, 5, jrpwd + 72);
}


/*******************************************
*�������ƣ�	DYY_Heating
*���ܣ�		ɢ��������
*������		��
*����ֵ��	��
*******************************************/
void DYY_Heating(void)
{
	float tempA, tempB;																			//�����̣�ɢ�����¶�
	uint8_t str[10];                                                                            
	uint8_t scanData;                                                                           //������ֵ
	                                                                                            
	while (KEYANDEC11_Scan() == KEY_ENTER);                                                     //�жϰ����ɿ�
	
	HeatingEnable();																			//��������
	temperatureControl.heatingAimTemperature = 80;												//���ü���Ŀ���¶�
	
	while (1)
	{
//		DYY_TemperatureControl(80);																//�¶ȿ���
																								//����PID�㷨���ɶ�ʱ�������¶ȿ���
		
		tempA = DS18B20_ReadTemp(DS18B20A);														//��ȡ�¶�
		tempB = DS18B20_ReadTemp(DS18B20B);
		
		sprintf((char *)str, "%-5.1f", tempA);													//��ʾ�������¶�
//		QPYLCD_DrawRectangle(149, 124, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 124, BLACK, FONT16X24, str);
		
		sprintf((char *)str, "%-5.1f", tempB);													//��ʾɢ�����¶�
//		QPYLCD_DrawRectangle(149, 188, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 188, BLACK, FONT16X24, str);

		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//������
#ifdef		ALLOWBACK																			//������������һ����
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
					{
						experimentalData.progress = BUILDBALANCE;								//������һ����
						return;
					}
#endif
					break;
				
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						DYY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DYY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.heatingTempeatrue = tempB;							//��������
							experimentalData.progress = RECORDING;								//������һ����
							
							HeatingDisable();													//�رռ���
							
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DYY_HeatingScreen();												//ˢ����ʾ
						}
					}
					break;
				
				default:
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DYY_RecordingScreen
*���ܣ�		��ʾ������¼���ݽ���
*������		��
*����ֵ��	��
*******************************************/
void DYY_RecordingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(144, 16, WHITE, FONT32X32, 6, srslcl);								//��ʾ��ɢ�����ʲ�����
	
	DYY_DrawEquipment3(352, 66, YELLOW);														//����ͼ��
	
	QPYLCD_DrawLine(0, 64, 320, 64, BLACK);														//�������ݱ��
	QPYLCD_DrawLine(0, 90, 320, 90, BLACK);
	QPYLCD_DrawLine(0, 116, 479, 116, BLACK);
	QPYLCD_DrawLine(0, 142, 479, 142, BLACK);
	QPYLCD_DrawLine(0, 168, 479, 168, BLACK);
	QPYLCD_DrawLine(0, 194, 479, 194, BLACK);
	QPYLCD_DrawLine(0, 220, 479, 220, BLACK);
	QPYLCD_DrawLine(0, 246, 479, 246, BLACK);
	
	

	QPYLCD_DrawLine(80, 64, 80, 271, BLACK);
	QPYLCD_DrawLine(160, 64, 160, 271, BLACK);
	QPYLCD_DrawLine(240, 64, 240, 271, BLACK);
	QPYLCD_DrawLine(320, 64, 320, 271, BLACK);
	QPYLCD_DrawLine(400, 116, 400, 271, BLACK);
	
	QPYLCD_SetBackColor(WHITE);
	
	QPYLCD_DisplayCharacters(16, 69, BLACK, FONT16X16, 1, sjwd);								//��ʾ��ͷ��ʱ�䡱
	QPYLCD_DisplayCharacters(48, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(176, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(208, 69, BLACK, FONT16X16, 1, sjwd + 96);
	
	QPYLCD_DisplayCharacters(96, 69, BLACK, FONT16X16, 1, sjwd);								//��ʾ��ͷ���¶ȡ�
	QPYLCD_DisplayCharacters(128, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(256, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(288, 69, BLACK, FONT16X16, 1, sjwd + 96);
}


/*******************************************
*�������ƣ�	DYY_Recording
*���ܣ�		��¼����
*������		��
*����ֵ��	��
*******************************************/
void DYY_Recording(void)
{
	uint8_t group = 1;																			//���ݱ��
	uint8_t scanData;																			//������ֵ
	float tempB;
	uint8_t j;
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//�жϰ����ɿ�
	
	tim3Count = 0;																				//��ʱ������
	
	while(1)
	{
		tempB = DS18B20_ReadTemp(DS18B20B);														//��ȡɢ�����¶�
		DYY_DisplayData(group, tim3Count, tempB, RED);											//��ʾһ������
		
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//������
#ifdef		ALLOWBACK																			//������������һ����
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
					{
						experimentalData.progress = HEATTING;									//������һ����
						return;
					}
#endif
					break;
				
				case KEY_COUNT:																	//��������
					if (group <= 20)
					{
						experimentalData.measuredData[group - 1].time = tim3Count;				//��¼һ������
						experimentalData.measuredData[group - 1].temperature = tempB;
						DYY_DisplayData(group++, tim3Count, tempB, BLACK);						//ˢ��������ʾ
					}
					break;
				
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						DYY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DYY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							if (group <= 20)													//��¼���һ������
							{
								experimentalData.measuredData[group - 1].time = tim3Count;
								experimentalData.measuredData[group - 1].temperature = tempB;
								DYY_DisplayData(group++, tim3Count, tempB, BLACK);
							}
							experimentalData.progress = SHOWDATA;								//������һ����
							return;
						}
						else																	//ȡ����ˢ����ʾ	
						{
							DYY_RecordingScreen();												//ˢ����ʾ
							
							for (j = 0; j < group - 1; j++)										//ˢ������
							{
								DYY_DisplayData(j + 1, experimentalData.measuredData[j].time,
									experimentalData.measuredData[j].temperature, BLACK);
							}
						}
					}
					break;
				
				default:
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DYY_ShowDataScreen
*���ܣ�		��ʾ����ʵ������
*������		��
*����ֵ��	��
*******************************************/
void DYY_ShowDataScreen(void)
{
	uint8_t str[5];
	uint8_t i;
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(176, 16, WHITE, FONT32X32, 4, sysj);								//��ʾ��ɢ�����ʲ�����
	
	
	QPYLCD_DrawLine(0, 64, 344, 64, BLACK);														//�������ݱ��
	QPYLCD_DrawLine(0, 90, 344, 90, BLACK);
	QPYLCD_DrawLine(0, 116, 479, 116, BLACK);
	QPYLCD_DrawLine(72, 142, 479, 142, BLACK);
	QPYLCD_DrawLine(0, 168, 479, 168, BLACK);
	QPYLCD_DrawLine(0, 194, 479, 194, BLACK);
	QPYLCD_DrawLine(72, 220, 479, 220, BLACK);
	QPYLCD_DrawLine(0, 246, 479, 246, BLACK);
	
	
	QPYLCD_DrawLine(72, 64, 72, 271, BLACK);
	QPYLCD_DrawLine(140, 64, 140, 271, BLACK);
	QPYLCD_DrawLine(208, 64, 208, 271, BLACK);
	QPYLCD_DrawLine(276, 64, 276, 271, BLACK);
	QPYLCD_DrawLine(344, 64, 344, 271, BLACK);
	QPYLCD_DrawLine(414, 116, 414, 271, BLACK);
	
	QPYLCD_SetBackColor(WHITE);
	
	QPYLCD_DisplayCharacters(82, 69, BLACK, FONT16X16, 1, sjwd);								//��ʾ��ͷ��ʱ�䡱
	QPYLCD_DisplayCharacters(114, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(218, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(250, 69, BLACK, FONT16X16, 1, sjwd + 96);
	
	QPYLCD_DisplayCharacters(150, 69, BLACK, FONT16X16, 1, sjwd);								//��ʾ��ͷ���¶ȡ�
	QPYLCD_DisplayCharacters(182, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(286, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(318, 69, BLACK, FONT16X16, 1, sjwd + 96);
	
	QPYLCD_DisplayCharacters(4, 69, BLACK, FONT16X16, 4, jrwdwht);								//��ʾ�������¶ȡ���
	QPYLCD_DisplayCharacters(12, 121, BLACK, FONT16X16, 3, jrwdwht + 32 *4);
	QPYLCD_DisplayCharacters(20, 147, BLACK, FONT16X16, 2, jrwdwht + 32 * 2);
	QPYLCD_DisplayCharacters(12, 199, BLACK, FONT16X16, 3, jrwdwht + 32 * 7);
	QPYLCD_DisplayCharacters(20, 225, BLACK, FONT16X16, 2, jrwdwht + 32 * 2);
	
	QPYLCD_DisplayCharacters(348, 82, BLACK, FONT16X16, 3, jrwdwht + 32 * 10);					//��ʾ��ѧ�š�
	QPYLCD_DisplayString(396, 82, BLACK, FONT8X16, experimentalData.studentNumber);				//��ʾѧ��
	
	sprintf((char *)str, "%.1f", experimentalData.settedTemperature);							//��ʽ�������¶Ȳ���ʾ
	QPYLCD_DisplayString(20, 95, BLACK, FONT8X16, str);	
	
	sprintf((char *)str, "%.1f", experimentalData.balanceTempeatrue);							//��ʽ���Ⱥ�̬�¶Ȳ���ʾ
	QPYLCD_DisplayString(20, 173, BLACK, FONT8X16, str);	
	
	sprintf((char *)str, "%.1f", experimentalData.heatingTempeatrue);							//��ʽ�����º��¶Ȳ���ʾ
	QPYLCD_DisplayString(20, 251, BLACK, FONT8X16, str);	
	
	for (i = 0; i < 7; i ++)
	{
		sprintf((char *)str, "%02d:%02d", experimentalData.measuredData[i].time / 60,
				experimentalData.measuredData[i].time % 60);									//��ʽ��ʱ��
		QPYLCD_DisplayString(86, 95 + 26 * i, BLACK, FONT8X16, str);							//��ʾʱ��
		sprintf((char *)str, "%.1f", experimentalData.measuredData[i].temperature);				//��ʽ���¶�
		QPYLCD_DisplayString(158, 95 + 26 * i, BLACK, FONT8X16, str);							//��ʾ�¶�
		
		sprintf((char *)str, "%02d:%02d", experimentalData.measuredData[i + 7].time / 60,
				experimentalData.measuredData[i + 7].time % 60);								//��ʽ��ʱ��
		QPYLCD_DisplayString(222, 95 + 26 * i, BLACK, FONT8X16, str);							//��ʾʱ��
		sprintf((char *)str, "%.1f", experimentalData.measuredData[i+ 7].temperature);			//��ʽ���¶�
		QPYLCD_DisplayString(294, 95 + 26 * i, BLACK, FONT8X16, str);							//��ʾ�¶�
		if (i + 14 < 20)
		{
			sprintf((char *)str, "%02d:%02d", experimentalData.measuredData[i + 14].time / 60,
				experimentalData.measuredData[i + 14].time % 60);								//��ʽ��ʱ��
			QPYLCD_DisplayString(359, 121 + 26 * i, BLACK, FONT8X16, str);						//��ʾʱ��
			sprintf((char *)str, "%.1f", experimentalData.measuredData[i + 14].temperature);	//��ʽ���¶�
			QPYLCD_DisplayString(430, 121 + 26 * i, BLACK, FONT8X16, str);						//��ʾ�¶�
		}
	}
	
}


void DYY_ShowData(void)
{
	uint8_t scanData;
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//������
#ifdef		ALLOWBACK																			//������������һ����
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
					{
						experimentalData.progress = RECORDING;									//������һ����
						return;
					}
#endif
					break;
					
				case KEY_ENTER:																	//����ȷ��
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
					{
						experimentalData.progress = COMPLETE;									//������һ����
						return;
					}
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DYY_EnterDialogScreen
*���ܣ�		��ʾȷ�϶Ի���
*������		��
*����ֵ��	��
*******************************************/
void DYY_EnterDialogScreen(void)
{
	QPYLCD_DrawRectangle(132, 88, 216, 96, WHITE);												//����Ի�������
	
	QPYLCD_DrawLine(132, 88, 348, 88, QPYLCD_NewColor(4, 4, 2));								//���ƶԻ������
	QPYLCD_DrawLine(132, 184, 348, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(132, 88, 132, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(348, 88, 348, 184, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_DisplayCharacters(144, 100, BLACK, FONT24X24, 8, qdjrxyb);							//��ʾ��ȷ�Ͻ�����һ������
	
	QPYLCD_DrawLine(144, 148, 193, 148, QPYLCD_NewColor(4, 4, 2));								//���ơ�ȷ�ϡ���ť�߿�
	QPYLCD_DrawLine(144, 172, 193, 172, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(144, 148, 144, 172, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(193, 148, 193, 172, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_DrawLine(288, 148, 337, 148, QPYLCD_NewColor(4, 4, 2));								//���ơ�ȡ������ť�߿�
	QPYLCD_DrawLine(288, 172, 337, 172, QPYLCD_NewColor(4, 4, 2));	
	QPYLCD_DrawLine(288, 148, 288, 172, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(337, 148, 337, 172, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_DrawRectangle(145, 149, 48, 23, CYAN);												//ȷ�ϰ�ť��ɫ
	
	QPYLCD_SetBackColor(CYAN);
	QPYLCD_DisplayCharacters(152, 152, BLACK, FONT16X16, 2, qdqx);								//��ʾ��ȷ�ϡ�
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(296, 152, BLACK, FONT16X16, 2, qdqx + 32 * 2);						//��ʾ��ȡ����
}


/*******************************************
*�������ƣ�	DYY_EnterDialog
*���ܣ�		ȷ�϶Ի������
*������		��
*����ֵ��	0			ȷ��
*			1			ȡ��
*******************************************/
uint8_t DYY_EnterDialog(void)
{
	uint8_t scanData;
	uint8_t i = 0;
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//�������ƶ����
					if (i > 0)
					{
						i--;
						
						QPYLCD_DrawRectangle(145, 149, 48, 23, CYAN);
						QPYLCD_DrawRectangle(289, 149, 48, 23, WHITE);
						
						QPYLCD_SetBackColor(CYAN);
						QPYLCD_DisplayCharacters(152, 152, BLACK, FONT16X16, 2, qdqx);
						QPYLCD_SetBackColor(WHITE);
						QPYLCD_DisplayCharacters(296, 152, BLACK, FONT16X16, 2, qdqx + 32 * 2);
					}
					break;
				
				case KEY_RIGHT:																	//�����ң��ƶ����
					if (i < 1)
					{
						i++;
						
						QPYLCD_DrawRectangle(145, 149, 48, 23, WHITE);
						QPYLCD_DrawRectangle(289, 149, 48, 23, CYAN);
						
						QPYLCD_SetBackColor(WHITE);
						QPYLCD_DisplayCharacters(152, 152, BLACK, FONT16X16, 2, qdqx);
						QPYLCD_SetBackColor(CYAN);
						QPYLCD_DisplayCharacters(296, 152, BLACK, FONT16X16, 2, qdqx + 32 * 2);
					}
					break;
				
				case KEY_ENTER:																	//����ȷ�ϣ�����ѡ��ֵ
					return i;
//					break;
					
				default :
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DYY_CompleteScreen
*���ܣ�		��ʾʵ�������ʾ
*������		��
*����ֵ��	��
*******************************************/
void DYY_CompleteScreen(void)
{
	QPYLCD_DrawRectangle(156, 88, 168, 96, WHITE);												//����Ի�������
	
	QPYLCD_DrawLine(156, 88, 324, 88, QPYLCD_NewColor(4, 4, 2));								//���ƶԻ������
	QPYLCD_DrawLine(156, 184, 324, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(156, 88, 156, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(324, 88, 324, 184, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 6, nywcsy);							//��ʾ���������ʵ�顱
	QPYLCD_DisplayCharacters(168, 148, BLACK, FONT24X24, 6, nywcsy + 72 * 6);					//��ʾ������������桱
}


/*******************************************
*�������ƣ�	DYY_Complete
*���ܣ�		ʵ�����
*������		��
*����ֵ��	��
*******************************************/
void DYY_Complete(void)
{
	uint8_t scanData;
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//������ⰴ������
					
				case KEY_RIGHT:
					
				case KEY_COUNT:
				
				case KEY_ENTER:
					experimentalData.progress = SHOWDATA;										//�ص���һ��
				
					return;
//					break;
				
				default:
					break;
			}
		}
	}
}
	


/*******************************************
*�������ƣ�	DYY_DrawEquipment1
*���ܣ�		��ʾ����ͼ��������
*������		xLabel			x����
*			yLabel			y����
*			color			�����ɫ
*����ֵ��	��
*******************************************/
void DYY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color)
{
	QPYLCD_DrawRectangle(xLabel +32, yLabel, 32, 8, color);
	QPYLCD_DrawRectangle(xLabel + 40, yLabel + 8, 16, 8, color);
	QPYLCD_DrawRectangle(xLabel, yLabel + 16, 96, 48, color);
	
	QPYLCD_DrawLine(xLabel + 32, yLabel, xLabel + 64, yLabel, BLACK);
	QPYLCD_DrawLine(xLabel + 32, yLabel, xLabel + 32, yLabel + 8, BLACK);
	QPYLCD_DrawLine(xLabel + 64, yLabel, xLabel + 64, yLabel + 8, BLACK);
	QPYLCD_DrawLine(xLabel + 32, yLabel + 8, xLabel + 40, yLabel + 8, BLACK);
	QPYLCD_DrawLine(xLabel + 56, yLabel + 8, xLabel + 64, yLabel + 8, BLACK);
	QPYLCD_DrawLine(xLabel + 40, yLabel + 8, xLabel + 40, yLabel + 16, BLACK);
	QPYLCD_DrawLine(xLabel + 56, yLabel + 8, xLabel + 56, yLabel + 16, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel + 16, xLabel + 40, yLabel + 16, BLACK);
	QPYLCD_DrawLine(xLabel + 56, yLabel + 16, xLabel + 96, yLabel + 16, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel + 16, xLabel, yLabel + 64, BLACK);
	QPYLCD_DrawLine(xLabel + 96, yLabel + 16, xLabel + 96, yLabel + 64, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel + 64, xLabel + 96, yLabel + 64, BLACK);
	
	
	QPYLCD_SetBackColor(color);
	QPYLCD_DisplayCharacters(xLabel + 24, yLabel + 32, BLACK, FONT16X16, 3, jrpsrp);
	
}


/*******************************************
*�������ƣ�	DYY_DrawEquipment2
*���ܣ�		��ʾ����ͼ�������ȵ���
*������		xLabel			x����
*			yLabel			y����
*			color			�����ɫ
*����ֵ��	��
*******************************************/
void DYY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color)
{
	QPYLCD_DrawRectangle(xLabel, yLabel, 96, 16, color);
	QPYLCD_SetBackColor(color);
	QPYLCD_DisplayCharacters(xLabel + 8, yLabel, BLACK, FONT16X16, 5, jrpsrp + 32*6);
	QPYLCD_DrawLine(xLabel, yLabel, xLabel + 96, yLabel, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel, xLabel, yLabel + 16, BLACK);
	QPYLCD_DrawLine(xLabel + 96, yLabel, xLabel + 96, yLabel + 16, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel + 16, xLabel + 96, yLabel + 16, BLACK);	
}


/*******************************************
*�������ƣ�	DYY_DrawEquipment3
*���ܣ�		��ʾ����ɢ����
*������		xLabel			x����
*			yLabel			y����
*			color			�����ɫ
*����ֵ��	��
*******************************************/
void DYY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color)
{
	QPYLCD_DrawRectangle(xLabel, yLabel, 96, 48, color);
	QPYLCD_DrawLine(xLabel, yLabel, xLabel + 96, yLabel, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel, xLabel, yLabel + 48, BLACK);
	QPYLCD_DrawLine(xLabel + 96, yLabel, xLabel + 96, yLabel + 48, BLACK);
	QPYLCD_DrawLine(xLabel, yLabel + 48, xLabel + 96, yLabel + 48, BLACK);
	
	QPYLCD_SetBackColor(color);
	QPYLCD_DisplayCharacters(xLabel + 24, yLabel + 16, BLACK, FONT16X16, 3, jrpsrp + 3*32);
}


/*******************************************
*�������ƣ�	DYY_DisplayData
*���ܣ�		��ʾʵ������
*������		group			���ݱ��
*			tempB			�¶�
*			color			������ɫ
*����ֵ��	��
*******************************************/
void DYY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color)
{
	uint8_t str[10];

	if (group < 8)																				//ǰ7������
	{
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);									//��ʽ��ʱ��
		QPYLCD_DisplayString(5, 69 + 26 * group, color, FONT8X16, str);							//��ʾʱ��
		sprintf((char *)str, "%.1f", tempB);													//��ʽ���¶�
		QPYLCD_DisplayString(85, 69 + 26 * group, color, FONT8X16, str);						//��ʾ�¶�
	}
	else if (group < 15)																		//7-14������
	{                                                                                           
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);                      			//��ʽ��ʱ��
		QPYLCD_DisplayString(165, 69 + 26 * (group - 7), color, FONT8X16, str);                 //��ʾʱ��
		sprintf((char *)str, "%.1f", tempB);                                                    //��ʽ���¶�
		QPYLCD_DisplayString(245, 69 + 26 * (group - 7), color, FONT8X16, str);                 //��ʾ�¶�
	}
	else if (group < 21)																		//15-20������
	{                                                                                           
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);									//��ʽ��ʱ��
		QPYLCD_DisplayString(325, 69 + 26 * (group - 13), color, FONT8X16, str);                //��ʾʱ��
		sprintf((char *)str, "%.1f", tempB);                                                    //��ʽ���¶�
		QPYLCD_DisplayString(405, 69 + 26 * (group - 13), color, FONT8X16, str);                //��ʾ�¶�
	}
}


/*******************************************
*�������ƣ�	DYY_TemperatureControl
*���ܣ�		�¶ȿ���
*������		temperature			����Ŀ���¶�
*����ֵ��	��
*******************************************/
void DYY_TemperatureControl(void)
{
	float tempC;
	
#ifdef		PID_CONTROL																			//PID�㷨�����¶�
	static float dutyCycle = 100;																	//ռ�ձ�
	float err[3];
	
#else																							//�����㷨�����¶�
	float tempA;
	
	tempA = DS18B20_ReadTemp(DS18B20A);															//��ȡ�¶�
#endif
	
	tempC = DS18B20_ReadTemp(DS18B20C);															//�޹��¶ȿ����㷨���¶ȹ���ֹͣ����
	if (tempC > 100)																			//�������¶Ȳ��ܳ���100��
	{
		PWM_SetDutyCycle(0);
		return;
	}
	
#ifdef		PID_CONTROL																			//PID�㷨�����¶�
	err[0] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[0];
	err[1] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[1];
	err[2] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[2];
	
	if (temperatureControl.pidTemperature[2] < (temperatureControl.heatingAimTemperature - 5))
	{
		PWM_SetDutyCycle(100);
	}
	else if (temperatureControl.pidTemperature[2] < (temperatureControl.heatingAimTemperature + 5))
	{
		dutyCycle += PID_KP * (err[2] - err[1]) + PID_KI * err[2]
				+ PID_KP * (err[2] - 2 * err[1] + err[0]);										//PID�㷨
		if (dutyCycle > 100)
		{
			dutyCycle = 100;
		}
		else if (dutyCycle < 0)
		{
			dutyCycle = 0;
		}
		PWM_SetDutyCycle((int)dutyCycle);
	}
	else
	{
		PWM_SetDutyCycle(0);
	}
	
#else																							//�����㷨�����¶�
	if (tempA < (temperatureControl.heatingAimTemperature - 5))									//�Ƚϼ����̵�ǰ�¶���Ŀ���¶�	
	{
		PWM_SetDutyCycle(100);
	}
	else if (tempA < temperatureControl.heatingAimTemperature + 3)
	{
		PWM_SetDutyCycle((uint8_t)(35 + (temperatureControl.heatingAimTemperature - tempA) * 13));
	}
	else if (tempA > temperatureControl.heatingAimTemperature + 3)
	{
		PWM_SetDutyCycle(0);
	}
#endif
}


/*******************************************
*�������ƣ�	DYY_UplaodData
*���ܣ�		�ϴ�������������
*������		command			����
*����ֵ��	��
*******************************************/
void DYY_UplaodData(uint8_t command)
{
	char str[20];
	uint8_t i = 0;
	
	if (espState.currentCommand != ESP8266_CIPSEND)												//ESP8266��ʼ��δ���
		return;
	
	sprintf(str, "send-%d-%d", command, experimentalData.machineNumber);						//��ʽ������ͻ����Ų�����
	ESP8266_SendString(str);
	
	switch (command)																			//�ж�����
	{
		case DEVICE_CONNECT:																	//��������
			
			break;
		
		case DEVICE_REGISTER:																	//����ע��
			sprintf(str, "-%s", experimentalData.studentNumber);								//��ʽ��ѧ�Ų�����
			ESP8266_SendString(str);
			break;
		
		case DEVICE_DATA:																		//������������
			if (experimentalData.progress != INPUTSTUDENTNUMBER)								//�жϵ�ǰ����
			{
				sprintf(str, "-%.1f", experimentalData.settedTemperature);						//��ʽ�������¶Ȳ�����
				ESP8266_SendString(str);
				if (experimentalData.progress == TEMPERATURESETTING)
					break;
				
				sprintf(str, "-%.1f", experimentalData.balanceTempeatrue);						//��ʽ���Ⱥ�̬�¶Ȳ�����
				ESP8266_SendString(str);
				if (experimentalData.progress == BUILDBALANCE)
					break;
				
				sprintf(str, "-%.1f", experimentalData.heatingTempeatrue);						//��ʽ�������¶Ȳ�����
				ESP8266_SendString(str);
				if (experimentalData.progress == HEATTING)
					break;
				
				while (experimentalData.measuredData[i].time)									//��ʽ��ʵ�����ݲ�����
				{
					if (i > 19)
						break;
					sprintf(str, "-%d-%.1f", experimentalData.measuredData[i].time, experimentalData.measuredData[i].temperature);
					ESP8266_SendString(str);
					i++;
				}
			}
			break;
		
		default:
			break;
	}
	ESP8266_SendString("\r\n");																	//����β
}


