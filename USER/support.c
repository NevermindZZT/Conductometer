/**********************************************
*             ������֧�ֺ���                  *
*         ���ϴ�ѧ���������ѧԺ              *
*                �ſ�ǿ	                      *
*                2018/4                       *
**********************************************/

#include	"support.h"
#include	"string.h"

TEMP_Control temperatureControl = 
{
	.isHeating = FALSE,
#if PID_CONTROL != 0
	.pidKP = 20,
	.pidKI = 0.445,
	.pidKD = 0,
	#if PID_CONTROL == 1
	.pidErr = 0,
	#endif
#endif
};


/*******************************************
*�������ƣ�	DRY_SystemSetting
*���ܣ�		��������
*������		��
*����ֵ��	��
*******************************************/

void DRY_SystemSetting(void)
{
	uint8_t scanData;
	int8_t cursorLocation = 0;
	int8_t cursorOffset = 0;
	
	DRY_SettingItem settingItem[] =															 //���������飬����������
	{
		{(uint8_t *)"Machine Number", 1, 30, ITEM_COUNT, 0},
		{(uint8_t *)"Brightness", 1, 7, ITEM_COUNT, 0},
		{(uint8_t *)"Software Version", 0, 0, ITEM_STRING, 0},
		{(uint8_t *)"Build Date", 0, 0, ITEM_STRING, 0},
		{(uint8_t *)"Romer", 0, 0, ITEM_STRING, 0},
	};
	
	settingItem[0].itemData.countData = experimentalData.machineNumber;						 //���������ݳ�ֵ
	settingItem[1].itemData.countData = screenBrightness;
	settingItem[2].itemData.stringData = (uint8_t *)SOFTWAREVERSION;
	settingItem[3].itemData.stringData = (uint8_t *)BUILDDATE;
	settingItem[4].itemData.stringData = (uint8_t *)"Letter";
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayString(176, 16, WHITE, FONT16X32, "Settings");								//��ʾ��Settings��
	
	QPYLCD_SetBackColor(WHITE);
	
	for (uint8_t i = 0; (i < sizeof(settingItem) / sizeof(DRY_SettingItem)) && (64 + 48 * i <= 272); i++)				 //��ʾ������
	{
		if (i == 0)
		{
			DRY_DisplaySettingItem(64 + 48 * i, CYAN, settingItem[i]);
		}
		else
		{
			DRY_DisplaySettingItem(64 + 48 * i, WHITE, settingItem[i]);
		}
	}
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:
					if (cursorLocation > 0)
					{
						if (cursorLocation - cursorOffset <= 0)
						{
							for (uint8_t i = 0; 64 + 48 * i <= 272; i++)
							{
								DRY_DisplaySettingItem(64 + 48 * i, WHITE, 
									settingItem[cursorLocation - 1 + i]);
							}
							cursorOffset--;
						}
						else
						{
							
							DRY_DisplaySettingItem(64 + 48 * (cursorLocation - cursorOffset), 
														WHITE, settingItem[cursorLocation]);
							
						}
						cursorLocation--;
					}
					break;
				
				case KEY_RIGHT:
					if (cursorLocation < sizeof(settingItem) / sizeof(DRY_SettingItem) - 1)
					{
						if (cursorLocation - cursorOffset >= 3)
						{
							for (uint8_t i = 0; 64 + 48 * i <= 272; i++)
							{
								DRY_DisplaySettingItem(64 + 48 * i, WHITE, 
									settingItem[cursorLocation - 2 + i]);
							}
							cursorOffset++;
						}
						else
						{
							DRY_DisplaySettingItem(64 + 48 * (cursorLocation - cursorOffset), 
														WHITE, settingItem[cursorLocation]);
						}
						cursorLocation++;
					}
					break;
				
				case KEY_ENTER_LONG:
						readFlash[0] = experimentalData.machineNumber = settingItem[0].itemData.countData;
						readFlash[1] = screenBrightness = settingItem[1].itemData.countData;
						MemWriteByte(readFlash, 2);					
					return;
//					break;
				
				case EC11_UP:
					switch ((uint8_t)settingItem[cursorLocation].type)
					{
						case ITEM_COUNT:
							if (++settingItem[cursorLocation].itemData.countData
								> settingItem[cursorLocation].maxCount)
							{
								settingItem[cursorLocation].itemData.countData
								 = settingItem[cursorLocation].minCount;
							}
							break;
						case ITEM_BOOL:
							settingItem[cursorLocation].itemData.isTrue = TRUE;
							break;
						case ITEM_STRING:
							break;
						default:
							break;
					}
					break;
				
				case EC11_DOWN:
					switch ((uint8_t)settingItem[cursorLocation].type)
					{
						case ITEM_COUNT:
							if (--settingItem[cursorLocation].itemData.countData
								< settingItem[cursorLocation].minCount)
							{
								settingItem[cursorLocation].itemData.countData
								 = settingItem[cursorLocation].maxCount;
							}
							break;
						case ITEM_BOOL:
							settingItem[cursorLocation].itemData.isTrue = FALSE;
							break;
						case ITEM_STRING:
							break;
						default:
							break;
					}
					break;
				
				default :
					break;
			}
			DRY_DisplaySettingItem(64 + 48 * (cursorLocation - cursorOffset), 
									CYAN, settingItem[cursorLocation]);
		}
	}
}


/*******************************************
*�������ƣ�	DRY_DisplaySettingItem
*���ܣ�		��ʾ�������õ���Ŀ
*������		location		λ��(�˴�Ϊy������)
*			color		   ����ɫ
*			settingItem	 ������Ŀ��ʹ�ýṹ�嶨��
*����ֵ��	��
*******************************************/
void DRY_DisplaySettingItem(uint16_t location, uint8_t color, DRY_SettingItem settingItem)
{
	QPYLCD_SetBackColor(color);
	QPYLCD_DrawRectangle(0, location, 480, 47, color);
	QPYLCD_DisplayString(5, location + 12, BLACK, FONT16X24, settingItem.id);
	switch ((uint8_t)settingItem.type)
	{
		case ITEM_COUNT:
			QPYLCD_DisplayInt(320, location + 12, BLACK, FONT16X24, settingItem.itemData.countData);
			break;
		
		case ITEM_BOOL:
			if (settingItem.itemData.isTrue == TRUE)
			{
				QPYLCD_DisplayString(320, location + 12, BLACK, FONT16X24, (uint8_t *)"ON");
			}
			else
			{
				QPYLCD_DisplayString(320, location + 12, BLACK, FONT16X24, (uint8_t *)"OFF");
			}
			break;
			
		case ITEM_STRING:
			if (strlen((const char *)settingItem.itemData.stringData) <= 10)
			{
				QPYLCD_DisplayString(320, location + 12, BLACK, FONT16X24, settingItem.itemData.stringData);
			}
			else if (strlen((const char *)settingItem.itemData.stringData) <= 20)
			{
				QPYLCD_DisplayString(320, location + 16, BLACK, FONT8X16, settingItem.itemData.stringData);
			}
			else
			{
				QPYLCD_DisplayString(320, location + 8, BLACK, FONT8X16, settingItem.itemData.stringData);
			}
			break;
	}
	QPYLCD_DrawLine(0, location + 47, 479, location + 47, QPYLCD_NewColor(1, 1, 1));
}



/*******************************************
*�������ƣ�	DRY_DataSearchScreen
*���ܣ�		��ʾ���ҽ���
*������		��
*����ֵ��	��
*******************************************/
void DRY_DataSearchScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(176, 16, WHITE, FONT32X32, 4, sjcx);								//��ʾ�����ݲ�ѯ��
	
	QPYLCD_SetBackColor(WHITE);
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(124, 168, BLACK, FONT24X24, 3, xuehao);							//��ʾ��ѧ�ţ���
	QPYLCD_DrawLine(196, 194, 356, 194, BLACK);
}



/*******************************************
*�������ƣ�	DRY_DataSearch
*���ܣ�		���ݲ���
*������		��
*����ֵ��	��
*******************************************/
void DRY_DataSearch(void)
{
	uint8_t scanData;																			//������ֵ
	uint8_t tempData = 0;																		//��ǰ���λ������
	uint8_t studentNumber[12];
	uint16_t xLabel = 196;																		//���x����
	uint8_t i = 0;
	int8_t searchDirection = 0;
	uint32_t searchAddress = DATA_START_ADDRESS;
	uint32_t lastAimAddress;																	//���һ�β��ҵ����ݵĵ�ַ
	DRY_DataSaveController dataSaveController;
	
	DRY_DataSearchScreen();
	
	QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);									//Ĭ��ˢ��һ������
	
	if (W25X16_Read((uint8_t *)&dataSaveController,
					DATA_CONTROLLER_ADDRESS,
					sizeof(DRY_DataSaveController)) != W25X16_OK)
	{
		DRY_DataHandlerDialog(DATA_READ_FAILED);
		return;
	}
	
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
						studentNumber[i] = NULL;
						i--;
						QPYLCD_DrawRectangle(xLabel, 168, 16, 24, WHITE);
						xLabel -= 16;
						tempData = 0;
					}
					break;
					
				case KEY_LEFT_LONG:
					return;
					//break;
				
				case KEY_RIGHT:																	//�����ң����浱ǰλ�����ݲ��������
					if (i < 9)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData);
						studentNumber[i++] = tempData + 48;
						xLabel += 16;
						tempData = 0;
					}
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:		
					studentNumber[i++] = tempData + 48;									//�������һ������
					studentNumber[i] = NULL;											//ѧ�Ž���
					
					
					searchAddress = dataSaveController.startAddress;
					searchDirection = 1;
					lastAimAddress = 0x00000000;
					while(1)
					{
						uint8_t enterFlag = 0;
						
						DRY_DataHandlerDialog(DATA_LOOKUP);											//���ڲ�������
						
						if (DRY_ReadExperimentalData(&experimentalData, searchAddress) != DRY_OK)
						{
							//TODO δ��ȡ�����ݴ���
							DRY_DataHandlerDialog(DATA_READ_FAILED);								//��ȡʧ��
							
							DRY_DataSearchScreen();
							QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);			//Ĭ��ˢ��һ������
							break;
						}
						if (strstr((const char*)experimentalData.studentNumber, (const char *)studentNumber) != NULL)
						{
							lastAimAddress = searchAddress;
							switch (DRY_ShowSearchResult())
							{
								case KEY_LEFT:
									searchDirection = -1;
									break;
								
								case KEY_RIGHT:
									searchDirection = 1;
									break;
								
								case KEY_ENTER:
									enterFlag = 1;
									break;
								
								default:
									
									break;
							}
						}
						
						switch (searchDirection)
						{
							case 1:
								searchAddress += 256;
								if (searchAddress >= dataSaveController.endAddress)
								{
									if (lastAimAddress != 0x00000000 && enterFlag == 0)
									{
										searchAddress = lastAimAddress;
										//TODO �����ҵ���ͷ
										DRY_DataHandlerDialog(DATA_LOOKUP_END);					//û�и�������
									}
									else if (enterFlag == 0)
									{
										//TODO û�в��ҵ�����
										DRY_DataHandlerDialog(DATA_LOOKUP_FAILED);				//����ʧ��
										enterFlag = 1;
									}
								}
								break;
							
							case -1:
								searchAddress -= 256;
								if (searchAddress < dataSaveController.startAddress)
								{
									if (lastAimAddress != 0x00000000 && enterFlag == 0)
									{
										searchAddress = lastAimAddress;
										//TODO ��ǰ���ҵ���ͷ
										DRY_DataHandlerDialog(DATA_LOOKUP_END);					//û�и�������
									}
									else if (enterFlag == 0)
									{
										//TODO û�в��ҵ�����
									}
								}
								break;
								
							default:
								break;
						}
						
						if (enterFlag == 1)														//����ȷ�ϼ������ز��ҽ���
						{
							for (uint8_t i = 0; i < 12; i++)
							{
								studentNumber[i] = 0;
							}
							tempData = 0;
							xLabel = 196;
							i = 0;
							DRY_DataSearchScreen();
							QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);			//Ĭ��ˢ��һ������
							break;
						}
					}
					break;
				
				case EC11_UP:																	//������˳ʱ��
					tempData++;																	//���λ����������
					if (tempData > 9)															//���ݷ�Χ����
						tempData = 0;
					break;
					
				case EC11_DOWN:																	//��������ʱ��
					tempData--;																    //���λ�����ݼ���
					if (tempData > 9)														    //���ݷ�Χ����
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
*�������ƣ�	DRY_ShowSearchResult
*���ܣ�		��ʾ���ݲ��ҽ��
*������		��
*����ֵ��	���ݲ��ҽ��津���İ�����ֵ
*******************************************/
uint8_t DRY_ShowSearchResult(void)
{
	uint8_t scanData;
	
	DRY_ShowDataScreen();
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();
		
		if (scanData == KEY_LEFT || scanData == KEY_RIGHT || scanData == KEY_ENTER)
		{
			return scanData;
		}
	}
	
}




/*******************************************
*�������ƣ�	DRY_Booting
*���ܣ�		��������
*������		��
*����ֵ��	��
*******************************************/
void DRY_Booting(void)
{
	uint8_t i;
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawColorImage(0, 60, 150, 150, logo);												//��ʾlogo(Ժ��)
	QPYLCD_DisplayCharacters(155, 90, QPYLCD_NewColor(0, 2, 2), FONT32X32, 10, zndxwlsyzx);		//��ʾ�����ϴ�ѧ����ʵ�����ġ�
	QPYLCD_DisplayCharacters(171, 150, QPYLCD_NewColor(0, 2, 2), FONT24X24, 12, blrdt);			//��ʾ�������ȵ��嵼��ϵ�������ǡ�
	QPYLCD_DisplayCharacters(168, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 8, cshwdcgq);		//��ʾ����ʼ���¶ȴ�������
	QPYLCD_DisplayString(296, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "...");					//
	if ((!DS18B20_Init(DS18B20A)) || (!DS18B20_Init(DS18B20B)) || (!DS18B20_Init(DS18B20C)))	//�ж��¶ȴ������Ƿ�ɹ�����	
	{
		QPYLCD_DrawRectangle(168, 242, 152, 16, WHITE);
		QPYLCD_DisplayCharacters(112, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 16, wdcgqcshsb);//��ʾ���¶ȴ�������ʼ��ʧ�ܡ�

#ifndef		DEBUG_TEST																				//DEBUGģʽ�رձ���
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
*�������ƣ�	DRY_InputStudentNumberScreen
*���ܣ�		��ʾ��������ѧ�Ž���
*������		��
*����ֵ��	��
*******************************************/
void DRY_InputStudentNumberScreen(void)
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
*�������ƣ�	DRY_InputStudentNumber
*���ܣ�		ѧ������
*������		��
*����ֵ��	��
*******************************************/
void DRY_InputStudentNumber(void)
{
	uint8_t scanData;																			//������ֵ
	uint8_t tempData = 0;																		//��ǰ���λ������
	uint16_t xLabel = 196;																		//���x����
	uint8_t i = 0;
	uint8_t j;
	
	DRY_InputStudentNumberScreen();
	
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
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//����ȷ��
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
//					{
						//DRY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DRY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.studentNumber[i++] = tempData + 48;				//�������һ������
							experimentalData.studentNumber[i] = NULL;							//ѧ�Ž���
							experimentalData.progress = TEMPERATURESETTING;						//ѧ��������������̽�����һ��
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DRY_InputStudentNumberScreen();										//ˢ����ʾ
							xLabel = 196;
							for (j = 0; j < i; j++)												//�ָ�һ����ѧ��������ʾ
							{
								QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, experimentalData.studentNumber[j] - 48);
								xLabel += 16;
							}
						}
						
//					}
					break;
				
				case EC11_UP:																	//������˳ʱ��
					tempData++;																	//���λ����������
					if (tempData > 9)															//���ݷ�Χ����
						tempData = 0;
					break;
					
				case EC11_DOWN:																	//��������ʱ��
					tempData--;																    //���λ�����ݼ���
					if (tempData > 9)														    //���ݷ�Χ����
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
*�������ƣ�	DRY_TemperatureSettingScreen
*���ܣ�		��ʾ�����¶����ý���
*������		��
*����ֵ��	��
*******************************************/
void DRY_TemperatureSettingScreen(void)
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
*�������ƣ�	DRY_TemperatureSetting
*���ܣ�		�����¶�����
*������		��
*����ֵ��	��
*******************************************/
void DRY_TemperatureSetting(void)
{
	uint8_t scanData;																			//������ֵ
	uint8_t tempData[2] = {0, 0};																//�����¶�ʮλ����λ
	uint16_t xLabel = 316;																		//���x����
	uint8_t i = 0;
	
	DRY_TemperatureSettingScreen();
	
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
					break;
					
#ifdef		ALLOWBACK																			//������������һ����
				case KEY_LEFT_LONG:
//					QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[i]);				//ˢ����ʾ���������ڳ���������ɵ���ʾ�ӳ�
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
//					{
						experimentalData.progress = INPUTSTUDENTNUMBER;							//������һ����
						return;
//					}
//					break;
#endif
				
				case KEY_RIGHT:																	//�����ң��������
					if (i < 1)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData[i]);
						i++;
						xLabel += 16;
					}	
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//����ȷ��
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
//					{
						//DRY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DRY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.settedTemperature = tempData[0] * 10 + tempData[1];//��������
							experimentalData.progress = BUILDBALANCE;							//������һ����
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DRY_TemperatureSettingScreen();										//ˢ����ʾ
							
							QPYLCD_DisplayInt(316, 168, BLACK, FONT16X24, tempData[0]);			//�ָ�������ʾ
							QPYLCD_DisplayInt(332, 168, BLACK, FONT16X24, tempData[1]);
						}
//					}
					break;
				
				case EC11_UP:																	//������˳ʱ��
					tempData[i]++;															    //���λ����������
					if (tempData[i] > 9)														//���ݷ�Χ����
						tempData[i] = 0;														
					break;																	  
																								
				case EC11_DOWN:																    //��������ʱ��
					tempData[i]--;															    //���λ�����ݼ���
					if (tempData[i] > 9)														//���ݷ�Χ����
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
*�������ƣ�	DRY_BuildBalanceScreen
*���ܣ�		��ʾ���������Ⱥ�̬����
*������		��
*����ֵ��	��
*******************************************/
void DRY_BuildBalanceScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, jlwht);								//��ʾ�������Ⱥ�̬��
	
	DRY_DrawEquipment1(320, 80, YELLOW);														//����ͼ��
	DRY_DrawEquipment2(320, 144, YELLOW);
	DRY_DrawEquipment3(320, 160, YELLOW);
	
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
*�������ƣ�	DRY_BuildBalance
*���ܣ�		�����Ⱥ�̬
*������		��
*����ֵ��	��
*******************************************/
void DRY_BuildBalance(void)
{
	float tempA, tempB, tempT;																			//�����̣�ɢ�����¶�
	uint8_t str[10];
	uint8_t scanData;																			//������ֵ
	
	DRY_BuildBalanceScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//�жϰ����ɿ�
	
	tim3Count = 0;																				//��ʱ������
	
	while (DS18B20_ReadTemp(DS18B20B) == 85.0)
		;
	tempB = DS18B20_ReadTemp(DS18B20B);															//Ĭ��ˢ��һ������
	sprintf((char *)str, "%-5.1f", tempB);
	QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
	
	HeatingEnable();																			//��������
	temperatureControl.heatingAimTemperature = experimentalData.settedTemperature;				//���ü���Ŀ���¶�
	
	while (1)
	{
//		DRY_TemperatureControl(experimentalData.settedTemperature);								//�¶ȿ���

		do
		{
			tempA = DS18B20_ReadTemp(DS18B20A);													//��ȡ�¶�
			tempT = DS18B20_ReadTemp(DS18B20A);												    //�����Σ����˴���
		} while ((((tempA - tempT) > 0) ? (tempA - tempT) : (tempT - tempA)) > 10);
		
		do 
		{
			tempB = DS18B20_ReadTemp(DS18B20B);
			tempT = DS18B20_ReadTemp(DS18B20B);
		} while ((((tempB - tempT) > 0) ? (tempB - tempT) : (tempT - tempB)) > 10);
		
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
#ifdef		ALLOWBACK																			//������������һ����
				case KEY_LEFT_LONG:																	//������
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
//					{
						HeatingDisable();														//�رռ���
						experimentalData.progress = TEMPERATURESETTING;							//������һ����
						return;
//					}
#endif
//					break;
				
				case KEY_COUNT:																	//������ʱ�����¼�ʱ
					tim3Count = 0;																//��ʱ������
					sprintf((char *)str, "%-5.1f", tempB);										//ˢ�¼�ʱʱ�¶�
//					QPYLCD_DrawRectangle(149, 216, 80, 24, WHITE);
					QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
					QPYLCD_DrawRectangle(149, 176, 96, 24, WHITE);								//���ʱ����ʾ����
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//����ȷ��
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
//					{
						//DRY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DRY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.balanceTempeatrue = tempB;							//��¼����
							experimentalData.progress = HEATTING;								//������һ����
							
							HeatingDisable();													//�رռ���
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DRY_BuildBalanceScreen();											//ˢ����ʾ
							
//							tempB = DS18B20_ReadTemp(DS18B20B);									//ˢ������
							sprintf((char *)str, "%-5.1f", tempB);
							QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
						}
//					}
					break;
				
				default:
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DRY_HeatingScreen
*���ܣ�		��ʾ����ɢ�������½���
*������		��
*����ֵ��	��
*******************************************/
void DRY_HeatingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, srpsw);								//��ʾ��ɢ�������¡�
	
	DRY_DrawEquipment1(320, 100, YELLOW);														//����ͼ��
	DRY_DrawEquipment3(320, 164, YELLOW);
	
	QPYLCD_SetBackColor(WHITE);	
	
	QPYLCD_DisplayCharacters(5, 124, BLACK, FONT24X24, 6, jrpwd);								//��ʾ���������¶ȡ�
	
	QPYLCD_DisplayCharacters(5, 188, BLACK, FONT24X24, 1, jrpwd + 6 * 72);						//��ʾ��ɢ�����¶ȡ�
	QPYLCD_DisplayCharacters(29, 188, BLACK, FONT24X24, 5, jrpwd + 72);
}


/*******************************************
*�������ƣ�	DRY_Heating
*���ܣ�		ɢ��������
*������		��
*����ֵ��	��
*******************************************/
void DRY_Heating(void)
{
	float tempA, tempB, tempT;																			//�����̣�ɢ�����¶�
	uint8_t str[10];																			
	uint8_t scanData;																		   //������ֵ
						
	DRY_HeatingScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);													 //�жϰ����ɿ�
	
	HeatingEnable();																			//��������
	temperatureControl.heatingAimTemperature = experimentalData.settedTemperature;			//���ü���Ŀ���¶�
	
	while (1)
	{
//		DRY_TemperatureControl(80);																//�¶ȿ���
																								//����PID�㷨���ɶ�ʱ�������¶ȿ���
		do
		{
			tempA = DS18B20_ReadTemp(DS18B20A);														//��ȡ�¶�
			tempT = DS18B20_ReadTemp(DS18B20A);													 //�����Σ����˴���
		} while ((((tempA - tempT) > 0) ? (tempA - tempT) : (tempT - tempA)) > 10);
		
		do 
		{
			tempB = DS18B20_ReadTemp(DS18B20B);
			tempT = DS18B20_ReadTemp(DS18B20B);
		} while ((((tempB - tempT) > 0) ? (tempB - tempT) : (tempT - tempB)) > 10);
		
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
#ifdef		ALLOWBACK																			//������������һ����
				case KEY_LEFT_LONG:																	//������
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
//					{
						HeatingDisable();														//�رռ���
						experimentalData.progress = BUILDBALANCE;								//������һ����
						return;
//					}
#endif
//					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//����ȷ��
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
//					{
						//DRY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DRY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							experimentalData.heatingTempeatrue = tempB;							//��������
							experimentalData.progress = RECORDING;								//������һ����
							
							HeatingDisable();													//�رռ���
							
							return;
						}
						else																	//ȡ����ˢ����ʾ
						{
							DRY_HeatingScreen();												//ˢ����ʾ
						}
//					}
					break;
				
				default:
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DRY_RecordingScreen
*���ܣ�		��ʾ������¼���ݽ���
*������		��
*����ֵ��	��
*******************************************/
void DRY_RecordingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(144, 16, WHITE, FONT32X32, 6, srslcl);								//��ʾ��ɢ�����ʲ�����
	
	DRY_DrawEquipment3(352, 66, YELLOW);														//����ͼ��
	
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
	QPYLCD_DisplayCharacters(176, 69, BLACK, FONT16X16, 1, sjwd);
	QPYLCD_DisplayCharacters(208, 69, BLACK, FONT16X16, 1, sjwd + 32);
	
	QPYLCD_DisplayCharacters(96, 69, BLACK, FONT16X16, 1, sjwd + 64);								//��ʾ��ͷ���¶ȡ�
	QPYLCD_DisplayCharacters(128, 69, BLACK, FONT16X16, 1, sjwd + 96);
	QPYLCD_DisplayCharacters(256, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(288, 69, BLACK, FONT16X16, 1, sjwd + 96);
}


/*******************************************
*�������ƣ�	DRY_Recording
*���ܣ�		��¼����
*������		��
*����ֵ��	��
*******************************************/
void DRY_Recording(void)
{
	uint8_t group = 1;																			//���ݱ��
	uint8_t scanData;																			//������ֵ
	float tempB;
	uint8_t j;
	
	DRY_RecordingScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//�жϰ����ɿ�
	
	tim3Count = 0;																				//��ʱ������
	
	while(1)
	{
		tempB = DS18B20_ReadTemp(DS18B20B);														//��ȡɢ�����¶�
		DRY_DisplayData(group, tim3Count, tempB, RED);											//��ʾһ������
		
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData)
		{
			switch (scanData)
			{
#ifdef		ALLOWBACK																			//������������һ����
				case KEY_LEFT_LONG:																	//������
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
//					{
						experimentalData.progress = HEATTING;									//������һ����
						return;
//					}
#endif
//					break;
				
				case KEY_LEFT:
					if (group > 1)
					{
						DRY_DisplayData(group--, tim3Count, tempB, WHITE);						//ɾ��һ����¼(ʹ�ñ���ɫ��ʾ�����ٴ�����)
					}
					break;
				
				case KEY_COUNT:																	//��������
					if (group <= 20)
					{
						experimentalData.measuredData[group - 1].time = tim3Count;				//��¼һ������
						experimentalData.measuredData[group - 1].temperature = tempB;
						DRY_DisplayData(group++, tim3Count, tempB, BLACK);						//ˢ��������ʾ
					}
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//����ȷ��
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
//					{
						//DRY_EnterDialogScreen();												//��ʾȷ�ϵ���
						if (DRY_EnterDialog() == 0)												//�жϵ�������������0Ϊȷ��
						{
							if (group <= 20)													//��¼���һ������
							{
								experimentalData.measuredData[group - 1].time = tim3Count;
								experimentalData.measuredData[group - 1].temperature = tempB;
//								DRY_DisplayData(group++, tim3Count, tempB, BLACK);				//�˴�ע�ͣ�û�б�Ҫˢ��
							}
							
							if (W25X16_Check() == TRUE)
							{
								DRY_DataHandlerDialog(DATA_SAVE);
								DRY_SaveExperimentalData(experimentalData);
//								AT24CXX_Write(0, (uint8_t *)(&experimentalData), sizeof(DRY_ExperimentalData));	//��������ʵ��������AT24C02
							}
							experimentalData.progress = SHOWDATA;								//������һ����
							return;
						}
						else																	//ȡ����ˢ����ʾ	
						{
							DRY_RecordingScreen();												//ˢ����ʾ
							
							for (j = 0; j < group - 1; j++)										//ˢ������
							{
								DRY_DisplayData(j + 1, experimentalData.measuredData[j].time,
									experimentalData.measuredData[j].temperature, BLACK);
							}
						}
//					}
					break;
				
				default:
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DRY_ShowDataScreen
*���ܣ�		��ʾ����ʵ������
*������		��
*����ֵ��	��
*******************************************/
void DRY_ShowDataScreen(void)
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
	QPYLCD_DisplayCharacters(218, 69, BLACK, FONT16X16, 1, sjwd);
	QPYLCD_DisplayCharacters(250, 69, BLACK, FONT16X16, 1, sjwd + 32);
	
	QPYLCD_DisplayCharacters(150, 69, BLACK, FONT16X16, 1, sjwd + 64);								//��ʾ��ͷ���¶ȡ�
	QPYLCD_DisplayCharacters(182, 69, BLACK, FONT16X16, 1, sjwd + 96);
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


void DRY_ShowData(void)
{
	uint8_t scanData;
	
	DRY_ShowDataScreen();
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//ɨ�谴���ͱ�����
		if (scanData != 0)
		{
			switch (scanData)
			{
#ifdef		ALLOWBACK																			//������������һ����
				case KEY_LEFT_LONG:																	//������
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//��������,������һ����
//					{
						experimentalData.progress = RECORDING;									//������һ����
						return;
//					}
#endif
//					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//����ȷ��
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//����
//					{
						experimentalData.progress = COMPLETE;									//������һ����
						return;
//					}
//					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DRY_EnterDialogScreen
*���ܣ�		��ʾȷ�϶Ի���
*������		��
*����ֵ��	��
*******************************************/
void DRY_EnterDialogScreen(void)
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
*�������ƣ�	DRY_EnterDialog
*���ܣ�		ȷ�϶Ի������
*������		��
*����ֵ��	0			ȷ��
*			1			ȡ��
*******************************************/
uint8_t DRY_EnterDialog(void)
{
	uint8_t scanData;
	uint8_t i = 0;
	
	DRY_EnterDialogScreen();
	
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
				case KEY_ENTER_LONG:
					return i;
//					break;
					
				default :
					break;
			}
		}
	}
}


/*******************************************
*�������ƣ�	DRY_CompleteScreen
*���ܣ�		��ʾʵ�������ʾ
*������		��
*����ֵ��	��
*******************************************/
void DRY_CompleteScreen(void)
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
*�������ƣ�	DRY_Complete
*���ܣ�		ʵ�����
*������		��
*����ֵ��	��
*******************************************/
void DRY_Complete(void)
{
	uint8_t scanData;
	
	DRY_CompleteScreen();
	
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
*�������ƣ�	DRY_DataHandlerDialog
*���ܣ�		���ݱ��棬��ȡ�Ի���
*������		mode		0			��ȡ
*						1			����
*����ֵ��	��
*******************************************/
void DRY_DataHandlerDialog(DRY_DataHandlerType mode)
{
	QPYLCD_DrawRectangle(156, 88, 168, 96, WHITE);												//����Ի�������
	
	QPYLCD_DrawLine(156, 88, 324, 88, QPYLCD_NewColor(4, 4, 2));								//���ƶԻ������
	QPYLCD_DrawLine(156, 184, 324, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(156, 88, 156, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(324, 88, 324, 184, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_SetBackColor(WHITE);
	
	switch ((uint8_t)mode)
	{
		case DATA_READ:
			QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 2, zzbcsj);					//��ʾ�����ڶ�ȡ���ݡ�
			QPYLCD_DisplayCharacters(216, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 9);
			QPYLCD_DisplayCharacters(264, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 4);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 3, zzbcsj + 72 * 6);			//��ʾ����ȴ���
			break;
		
		case DATA_SAVE:
			QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 6, zzbcsj);					//��ʾ"���ڱ�������"
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 3, zzbcsj + 72 * 6);			//��ʾ����ȴ���
			break;
		
		case DATA_LOOKUP:
			QPYLCD_DisplayCharacters(192, 100, BLACK, FONT24X24, 2, zzbcsj);					//��ʾ�����ڲ��ҡ�
			QPYLCD_DisplayCharacters(240, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 11);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 3, zzbcsj + 72 * 6);			//��ʾ����ȴ���
			break;
		
		case DATA_READ_FAILED:
			QPYLCD_DisplayCharacters(192, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 9);			//��ʾ����ȡʧ�ܡ�
			QPYLCD_DisplayCharacters(240, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 13);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 1, zzbcsj + 72 * 6);			//��ʾ�������ԡ�
			QPYLCD_DisplayCharacters(228, 148, BLACK, FONT24X24, 2, zzbcsj + 72 * 15);
			break;
		
		case DATA_LOOKUP_FAILED:
			QPYLCD_DisplayCharacters(192, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 11);			//��ʾ������ʧ�ܡ�
			QPYLCD_DisplayCharacters(240, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 13);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 1, zzbcsj + 72 * 6);			//��ʾ�������ԡ�
			QPYLCD_DisplayCharacters(228, 148, BLACK, FONT24X24, 2, zzbcsj + 72 * 15);
			break;
		
		case DATA_LOOKUP_END:
			QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 4, zzbcsj + 72 * 17);			//��ʾ��û�и������ݡ�
			QPYLCD_DisplayCharacters(264, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 4);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 1, zzbcsj + 72 * 6);			//��ʾ�������ԡ�
			QPYLCD_DisplayCharacters(228, 148, BLACK, FONT24X24, 2, zzbcsj + 72 * 15);
			break;
	}
	if (mode == DATA_READ_FAILED
		|| mode == DATA_LOOKUP_FAILED
		|| mode == DATA_LOOKUP_END)
	{
		uint16_t i = 50;
		while (KEYANDEC11_Scan() == 0 && i-- > 0)
		{
		}
	}
}



/*******************************************
*�������ƣ�	DRY_DrawEquipment1
*���ܣ�		��ʾ����ͼ��������
*������		xLabel			x����
*			yLabel			y����
*			color			�����ɫ
*����ֵ��	��
*******************************************/
void DRY_DrawEquipment1(uint16_t xLabel, uint16_t yLabel, uint8_t color)
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
*�������ƣ�	DRY_DrawEquipment2
*���ܣ�		��ʾ����ͼ�������ȵ���
*������		xLabel			x����
*			yLabel			y����
*			color			�����ɫ
*����ֵ��	��
*******************************************/
void DRY_DrawEquipment2(uint16_t xLabel, uint16_t yLabel, uint8_t color)
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
*�������ƣ�	DRY_DrawEquipment3
*���ܣ�		��ʾ����ɢ����
*������		xLabel			x����
*			yLabel			y����
*			color			�����ɫ
*����ֵ��	��
*******************************************/
void DRY_DrawEquipment3(uint16_t xLabel, uint16_t yLabel, uint8_t color)
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
*�������ƣ�	DRY_DisplayData
*���ܣ�		��ʾʵ������
*������		group			���ݱ��
*			tempB			�¶�
*			color			������ɫ
*����ֵ��	��
*******************************************/
void DRY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color)
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
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);					  			//��ʽ��ʱ��
		QPYLCD_DisplayString(165, 69 + 26 * (group - 7), color, FONT8X16, str);				 //��ʾʱ��
		sprintf((char *)str, "%.1f", tempB);													//��ʽ���¶�
		QPYLCD_DisplayString(245, 69 + 26 * (group - 7), color, FONT8X16, str);				 //��ʾ�¶�
	}
	else if (group < 21)																		//15-20������
	{																						   
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);									//��ʽ��ʱ��
		QPYLCD_DisplayString(325, 69 + 26 * (group - 13), color, FONT8X16, str);				//��ʾʱ��
		sprintf((char *)str, "%.1f", tempB);													//��ʽ���¶�
		QPYLCD_DisplayString(405, 69 + 26 * (group - 13), color, FONT8X16, str);				//��ʾ�¶�
	}
}


/*******************************************
*�������ƣ�	DRY_TemperatureControl
*���ܣ�		�¶ȿ���
*������		temperature			����Ŀ���¶�
*����ֵ��	��
*******************************************/
void DRY_TemperatureControl(void)
{
	float tempC;
	
#if		PID_CONTROL != 0 																		//PID�㷨�����¶�
	static float dutyCycle = 100;																//ռ�ձ�
	float err[3];
	
#else	/*- PID_CONTROL != 0 -*/																//�����㷨�����¶�
	float tempA;
	
	tempA = DS18B20_ReadTemp(DS18B20A);															//��ȡ�¶�
#endif	/*- PID_CONTROL != 0 -*/
	
	tempC = DS18B20_ReadTemp(DS18B20C);															//�޹��¶ȿ����㷨���¶ȹ���ֹͣ����
	if (tempC > temperatureControl.heatingAimTemperature + 10)									//�������¶Ȳ��ܳ���Ŀ��ֵ10��
	{
		PWM_SetDutyCycle(0);
		return;
	}
	
#if		PID_CONTROL != 0																		//PID�㷨�����¶�
	err[0] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[0];
	err[1] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[1];
	err[2] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[2];
	
	if (temperatureControl.pidTemperature[2] < (temperatureControl.heatingAimTemperature - 5))
	{
		PWM_SetDutyCycle(100);
	}
	else if (temperatureControl.pidTemperature[2] < (temperatureControl.heatingAimTemperature + 5))
	{
	#if PID_CONTROL == 1																		//λ��ʽPID�㷨
		temperatureControl.pidErr += err[2];
		dutyCycle = temperatureControl.pidKP * err[2]
					+ temperatureControl.pidKI * temperatureControl.pidErr
					+ temperatureControl.pidKD * (err[2] - err [1]);
	#else /*- PID_CONTROL == 1 -*/																//����ʽPID�㷨
		dutyCycle += temperatureControl.pidKP * (err[2] - err[1])
					+ temperatureControl.pidKI * err[2]
					+ temperatureControl.pidKD * (err[2] - 2 * err[1] + err[0]);
	#endif /*- PID_CONTROL == 1 -*/
		
		if (dutyCycle > 100)
		{
			dutyCycle = 100;
		}
		else if (dutyCycle < 0)
		{
			dutyCycle = 0;
		}
		PWM_SetDutyCycle(dutyCycle);
	}
	else
	{
		PWM_SetDutyCycle(0);
	}
	
	/*-----------------���PID������Ϣ--------------*/
	DEBUG_LOG("%d:\t", tim3Count);
	#if	PID_CONTROL == 1
		DEBUG_LOG("err:%Lf, ", temperatureControl.pidErr);
	#endif
	
	DEBUG_LOG("temp[k]:%.1f, temp[k-1]:%.1f, temp[k-2]:%.1f; PWM_DutyCycle:%f\r\n",
				temperatureControl.pidTemperature[2],
				temperatureControl.pidTemperature[1],
				temperatureControl.pidTemperature[0],
				dutyCycle);
	
#else	/*- PID_CONTROL != 0 - */																//�����㷨�����¶�
	if (tempA < (temperatureControl.heatingAimTemperature - 5))									//�Ƚϼ����̵�ǰ�¶���Ŀ���¶�	
	{
		PWM_SetDutyCycle(100);
	}
	else if (tempA < temperatureControl.heatingAimTemperature + 3)
	{
		PWM_SetDutyCycle(((temperatureControl.heatingAimTemperature - 25)
						+ (temperatureControl.heatingAimTemperature - tempA) * 13));
	}
	else if (tempA > temperatureControl.heatingAimTemperature + 3)
	{
		PWM_SetDutyCycle(0);
	}
#endif	/*- PID_CONTROL != 0 - */
}


/*******************************************
*�������ƣ�	DRY_UplaodData
*���ܣ�		�ϴ�������������
*������		command			����
*����ֵ��	��
*******************************************/
void DRY_UplaodData(uint8_t command)
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
					sprintf(str, "-%d-%.1f", experimentalData.measuredData[i].time,
							experimentalData.measuredData[i].temperature);
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



/*******************************************
*�������ƣ�	DRY_SaveExperimentalData
*���ܣ�		����������Flash
*������		experimentalData	ʵ������
*����ֵ��	DRY_OK				����ɹ�
*			DRY_ERROR			����ʧ��
*˵����		һ��������С4K(0x1000 Byte)��ÿ��ʵ�����ݴ�����256�ֽڵĿռ��һ��
*			�������Դ���16�����ݣ���ַ0x00000000��0x000010000(��һ������)���ڴ���
*			������������Ϣ
*******************************************/
DRY_Status DRY_SaveExperimentalData(DRY_ExperimentalData experimentalData)
{
	DRY_DataSaveController dataSaveController;
	//DRY_DataSaveController *dataSaveControllerPointer;
	
	//dataSaveControllerPointer = &dataSaveController;
	
	if (W25X16_Read((uint8_t *)&dataSaveController,
					DATA_CONTROLLER_ADDRESS,
					sizeof(DRY_DataSaveController)) != W25X16_OK)
	{
		return DRY_ERROR;
	}
	else if ((dataSaveController.dataCount != ((dataSaveController.endAddress
				- dataSaveController.startAddress) / 256))
			|| (dataSaveController.startAddress < DATA_START_ADDRESS)
			|| (dataSaveController.endAddress > DATA_END_ADDRESS))
	{
		dataSaveController.dataCount = 0;
		dataSaveController.startAddress = DATA_START_ADDRESS;
		dataSaveController.endAddress = DATA_START_ADDRESS;
	}
	
	if (dataSaveController.endAddress % 0x1000 == 0)
	{
		if (W25X16_EraseSector(dataSaveController.endAddress) != W25X16_OK)
		{
			return DRY_ERROR;
		}
	}
	
	if (W25X16_PageWrite((uint8_t *)&experimentalData,
						dataSaveController.endAddress,
						sizeof(DRY_ExperimentalData)) != W25X16_OK)
	{
		return DRY_ERROR;
	}
	
	dataSaveController.dataCount++;
	dataSaveController.endAddress += 256;
	
	if (W25X16_EraseSector(DATA_CONTROLLER_ADDRESS) != W25X16_OK)
	{
		return DRY_ERROR;
	}
	
	if (W25X16_PageWrite((uint8_t *)&dataSaveController,
						DATA_CONTROLLER_ADDRESS,
						sizeof(DRY_DataSaveController)) != W25X16_OK)
	{
		return DRY_ERROR;
	}
	
	return DRY_OK;
}



/*******************************************
*�������ƣ�	DRY_ReadExperimentalData
*���ܣ�		��Flash��ȡʵ������
*������		experimentalDataPointer		ʵ������
*			dataAddress					���ݵ�ַ
*����ֵ��	DRY_OK				��ȡ�ɹ�
*			DRY_ERROR			��ȡʧ��
*******************************************/
DRY_Status DRY_ReadExperimentalData(DRY_ExperimentalData *experimentalDataPointer, uint32_t dataAddress)
{
	DRY_DataSaveController dataSaveController;
	//DRY_DataSaveController *dataSaveControllerPointer;
	
	//dataSaveControllerPointer = &dataSaveController;
	
	if (W25X16_Read((uint8_t *)&dataSaveController,
					DATA_CONTROLLER_ADDRESS,
					sizeof(DRY_DataSaveController)) != W25X16_OK)
	{
		return DRY_ERROR;
	}
	
	
	if (dataAddress >= dataSaveController.startAddress
		&& dataAddress < dataSaveController.endAddress)
	{
		if (W25X16_Read((uint8_t *)experimentalDataPointer,
						dataAddress,
						sizeof(DRY_ExperimentalData)) != W25X16_OK)
		{
			return DRY_ERROR;
		}
	}
	else if (dataAddress == 0x00000000)								//��ַΪ0ʱ����ȡ���һ��ʵ������
	{
		if (W25X16_Read((uint8_t *)experimentalDataPointer,
						dataSaveController.endAddress - 256,
						sizeof(DRY_ExperimentalData)) != W25X16_OK)
		{
			return DRY_ERROR;
		}
	}
	return DRY_OK;
}


