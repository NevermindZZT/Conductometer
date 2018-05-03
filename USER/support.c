/**********************************************
*             导热仪支持函数                  *
*         中南大学物理与电子学院              *
*                张克强	                      *
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
*函数名称：	DRY_SystemSetting
*功能：		仪器设置
*参数：		无
*返回值：	无
*******************************************/

void DRY_SystemSetting(void)
{
	uint8_t scanData;
	int8_t cursorLocation = 0;
	int8_t cursorOffset = 0;
	
	DRY_SettingItem settingItem[] =															 //设置项数组，可自由增减
	{
		{(uint8_t *)"Machine Number", 1, 30, ITEM_COUNT, 0},
		{(uint8_t *)"Brightness", 1, 7, ITEM_COUNT, 0},
		{(uint8_t *)"Software Version", 0, 0, ITEM_STRING, 0},
		{(uint8_t *)"Build Date", 0, 0, ITEM_STRING, 0},
		{(uint8_t *)"Romer", 0, 0, ITEM_STRING, 0},
	};
	
	settingItem[0].itemData.countData = experimentalData.machineNumber;						 //设置项数据初值
	settingItem[1].itemData.countData = screenBrightness;
	settingItem[2].itemData.stringData = (uint8_t *)SOFTWAREVERSION;
	settingItem[3].itemData.stringData = (uint8_t *)BUILDDATE;
	settingItem[4].itemData.stringData = (uint8_t *)"Letter";
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayString(176, 16, WHITE, FONT16X32, "Settings");								//显示“Settings”
	
	QPYLCD_SetBackColor(WHITE);
	
	for (uint8_t i = 0; (i < sizeof(settingItem) / sizeof(DRY_SettingItem)) && (64 + 48 * i <= 272); i++)				 //显示设置项
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
*函数名称：	DRY_DisplaySettingItem
*功能：		显示仪器设置的条目
*参数：		location		位置(此处为y轴坐标)
*			color		   背景色
*			settingItem	 设置条目，使用结构体定义
*返回值：	无
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
*函数名称：	DRY_DataSearchScreen
*功能：		显示查找界面
*参数：		无
*返回值：	无
*******************************************/
void DRY_DataSearchScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(176, 16, WHITE, FONT32X32, 4, sjcx);								//显示“数据查询”
	
	QPYLCD_SetBackColor(WHITE);
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(124, 168, BLACK, FONT24X24, 3, xuehao);							//显示“学号：”
	QPYLCD_DrawLine(196, 194, 356, 194, BLACK);
}



/*******************************************
*函数名称：	DRY_DataSearch
*功能：		数据查找
*参数：		无
*返回值：	无
*******************************************/
void DRY_DataSearch(void)
{
	uint8_t scanData;																			//按键键值
	uint8_t tempData = 0;																		//当前光标位置数据
	uint8_t studentNumber[12];
	uint16_t xLabel = 196;																		//光标x坐标
	uint8_t i = 0;
	int8_t searchDirection = 0;
	uint32_t searchAddress = DATA_START_ADDRESS;
	uint32_t lastAimAddress;																	//最近一次查找到数据的地址
	DRY_DataSaveController dataSaveController;
	
	DRY_DataSearchScreen();
	
	QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);									//默认刷新一次数据
	
	if (W25X16_Read((uint8_t *)&dataSaveController,
					DATA_CONTROLLER_ADDRESS,
					sizeof(DRY_DataSaveController)) != W25X16_OK)
	{
		DRY_DataHandlerDialog(DATA_READ_FAILED);
		return;
	}
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//按键左，光标左移
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
				
				case KEY_RIGHT:																	//按键右，保存当前位置数据并光标右移
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
					studentNumber[i++] = tempData + 48;									//保存最后一次数据
					studentNumber[i] = NULL;											//学号结束
					
					
					searchAddress = dataSaveController.startAddress;
					searchDirection = 1;
					lastAimAddress = 0x00000000;
					while(1)
					{
						uint8_t enterFlag = 0;
						
						DRY_DataHandlerDialog(DATA_LOOKUP);											//正在查找数据
						
						if (DRY_ReadExperimentalData(&experimentalData, searchAddress) != DRY_OK)
						{
							//TODO 未读取到数据处理
							DRY_DataHandlerDialog(DATA_READ_FAILED);								//读取失败
							
							DRY_DataSearchScreen();
							QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);			//默认刷新一次数据
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
										//TODO 向后查找到尽头
										DRY_DataHandlerDialog(DATA_LOOKUP_END);					//没有更多数据
									}
									else if (enterFlag == 0)
									{
										//TODO 没有查找到数据
										DRY_DataHandlerDialog(DATA_LOOKUP_FAILED);				//查找失败
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
										//TODO 向前查找到尽头
										DRY_DataHandlerDialog(DATA_LOOKUP_END);					//没有更多数据
									}
									else if (enterFlag == 0)
									{
										//TODO 没有查找到数据
									}
								}
								break;
								
							default:
								break;
						}
						
						if (enterFlag == 1)														//按下确认键，返回查找界面
						{
							for (uint8_t i = 0; i < 12; i++)
							{
								studentNumber[i] = 0;
							}
							tempData = 0;
							xLabel = 196;
							i = 0;
							DRY_DataSearchScreen();
							QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);			//默认刷新一次数据
							break;
						}
					}
					break;
				
				case EC11_UP:																	//编码器顺时针
					tempData++;																	//光标位置数据增加
					if (tempData > 9)															//数据范围限制
						tempData = 0;
					break;
					
				case EC11_DOWN:																	//编码器逆时针
					tempData--;																    //光标位置数据减少
					if (tempData > 9)														    //数据范围限制
						tempData = 9;
					break;
				
				default:
					break;
				
			}
			QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);							//刷新显示
		}
	}
}


/*******************************************
*函数名称：	DRY_ShowSearchResult
*功能：		显示数据查找结果
*参数：		无
*返回值：	数据查找界面触发的按键键值
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
*函数名称：	DRY_Booting
*功能：		仪器启动
*参数：		无
*返回值：	无
*******************************************/
void DRY_Booting(void)
{
	uint8_t i;
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawColorImage(0, 60, 150, 150, logo);												//显示logo(院徽)
	QPYLCD_DisplayCharacters(155, 90, QPYLCD_NewColor(0, 2, 2), FONT32X32, 10, zndxwlsyzx);		//显示“中南大学物理实验中心”
	QPYLCD_DisplayCharacters(171, 150, QPYLCD_NewColor(0, 2, 2), FONT24X24, 12, blrdt);			//显示“不良热导体导热系数测量仪”
	QPYLCD_DisplayCharacters(168, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 8, cshwdcgq);		//显示“初始化温度传感器”
	QPYLCD_DisplayString(296, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "...");					//
	if ((!DS18B20_Init(DS18B20A)) || (!DS18B20_Init(DS18B20B)) || (!DS18B20_Init(DS18B20C)))	//判断温度传感器是否成功连接	
	{
		QPYLCD_DrawRectangle(168, 242, 152, 16, WHITE);
		QPYLCD_DisplayCharacters(112, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 16, wdcgqcshsb);//显示“温度传感器初始化失败”

#ifndef		DEBUG_TEST																				//DEBUG模式关闭报警
		BEEP_ON;																				//蜂鸣器报警
		while(1);																				//初始化失败，程序死机
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
*函数名称：	DRY_InputStudentNumberScreen
*功能：		显示仪器输入学号界面
*参数：		无
*返回值：	无
*******************************************/
void DRY_InputStudentNumberScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, qsrxh);								//显示“请输入学号”
	
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(124, 168, BLACK, FONT24X24, 3, xuehao);							//显示“学号：”
	QPYLCD_DrawLine(196, 194, 356, 194, BLACK);
}


/*******************************************
*函数名称：	DRY_InputStudentNumber
*功能：		学号输入
*参数：		无
*返回值：	无
*******************************************/
void DRY_InputStudentNumber(void)
{
	uint8_t scanData;																			//按键键值
	uint8_t tempData = 0;																		//当前光标位置数据
	uint16_t xLabel = 196;																		//光标x坐标
	uint8_t i = 0;
	uint8_t j;
	
	DRY_InputStudentNumberScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//由上一步长按确认键进入此程序，判断确认键被松开
	
	QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);									//默认刷新一次数据
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//按键左，光标左移
					if (i > 0)
					{
						i--;
						QPYLCD_DrawRectangle(xLabel, 168, 16, 24, WHITE);
						xLabel -= 16;
						tempData = 0;
					}
					break;
				
				case KEY_RIGHT:																	//按键右，保存当前位置数据并光标右移
					if (i < 9)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData);
						experimentalData.studentNumber[i++] = tempData + 48;
						xLabel += 16;
						tempData = 0;
					}
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//按键确定
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
//					{
						//DRY_EnterDialogScreen();												//显示确认弹窗
						if (DRY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.studentNumber[i++] = tempData + 48;				//保存最后一次数据
							experimentalData.studentNumber[i] = NULL;							//学号结束
							experimentalData.progress = TEMPERATURESETTING;						//学号输入结束，进程进入下一步
							return;
						}
						else																	//取消，刷新显示
						{
							DRY_InputStudentNumberScreen();										//刷新显示
							xLabel = 196;
							for (j = 0; j < i; j++)												//恢复一输入学号数据显示
							{
								QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, experimentalData.studentNumber[j] - 48);
								xLabel += 16;
							}
						}
						
//					}
					break;
				
				case EC11_UP:																	//编码器顺时针
					tempData++;																	//光标位置数据增加
					if (tempData > 9)															//数据范围限制
						tempData = 0;
					break;
					
				case EC11_DOWN:																	//编码器逆时针
					tempData--;																    //光标位置数据减少
					if (tempData > 9)														    //数据范围限制
						tempData = 9;
					break;
				
				default:
					break;
				
			}
			QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData);							//刷新显示
		}
	}
}


/*******************************************
*函数名称：	DRY_TemperatureSettingScreen
*功能：		显示仪器温度设置界面
*参数：		无
*返回值：	无
*******************************************/
void DRY_TemperatureSettingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(176, 16, WHITE, FONT32X32, 4, wdsz);								//显示“温度设置”
	
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(124, 168, BLACK, FONT24X24, 2, jrpwd + 72 * 7);					//显示“设置加热盘温度：”
	QPYLCD_DisplayCharacters(172, 168, BLACK, FONT24X24, 6, jrpwd);	
	QPYLCD_DrawLine(316, 194, 348, 194, BLACK);
}


/*******************************************
*函数名称：	DRY_TemperatureSetting
*功能：		加热温度设置
*参数：		无
*返回值：	无
*******************************************/
void DRY_TemperatureSetting(void)
{
	uint8_t scanData;																			//按键键值
	uint8_t tempData[2] = {0, 0};																//设置温度十位，个位
	uint16_t xLabel = 316;																		//光标x坐标
	uint8_t i = 0;
	
	DRY_TemperatureSettingScreen();
	
	QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[0]);								//默认刷新一次数据
	QPYLCD_DisplayInt(xLabel + 16, 168, BLACK, FONT16X24, tempData[1]);
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//判断按键松开
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//按键左，光标左移
					if (i > 0)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData[i]);
						i--;
						xLabel -= 16;
					}
					break;
					
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
				case KEY_LEFT_LONG:
//					QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[i]);				//刷新显示，消除由于长按返回造成的显示延迟
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
//					{
						experimentalData.progress = INPUTSTUDENTNUMBER;							//进入上一步骤
						return;
//					}
//					break;
#endif
				
				case KEY_RIGHT:																	//按键右，光标右移
					if (i < 1)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData[i]);
						i++;
						xLabel += 16;
					}	
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//按键确认
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
//					{
						//DRY_EnterDialogScreen();												//显示确认弹窗
						if (DRY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.settedTemperature = tempData[0] * 10 + tempData[1];//保存数据
							experimentalData.progress = BUILDBALANCE;							//进入下一步骤
							return;
						}
						else																	//取消，刷新显示
						{
							DRY_TemperatureSettingScreen();										//刷新显示
							
							QPYLCD_DisplayInt(316, 168, BLACK, FONT16X24, tempData[0]);			//恢复数据显示
							QPYLCD_DisplayInt(332, 168, BLACK, FONT16X24, tempData[1]);
						}
//					}
					break;
				
				case EC11_UP:																	//编码器顺时针
					tempData[i]++;															    //光标位置数据增加
					if (tempData[i] > 9)														//数据范围限制
						tempData[i] = 0;														
					break;																	  
																								
				case EC11_DOWN:																    //编码器逆时针
					tempData[i]--;															    //光标位置数据减少
					if (tempData[i] > 9)														//数据范围限制
						tempData[i] = 9;
					break;
				
				default:
					break;
				
			}
			QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[i]);						//刷新显示
		}
	}
}


/*******************************************
*函数名称：	DRY_BuildBalanceScreen
*功能：		显示仪器建立稳恒态界面
*参数：		无
*返回值：	无
*******************************************/
void DRY_BuildBalanceScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, jlwht);								//显示“建立稳恒态”
	
	DRY_DrawEquipment1(320, 80, YELLOW);														//绘制图形
	DRY_DrawEquipment2(320, 144, YELLOW);
	DRY_DrawEquipment3(320, 160, YELLOW);
	
	QPYLCD_SetBackColor(WHITE);
	
	QPYLCD_DisplayCharacters(5, 96, BLACK, FONT24X24, 6, jrpwd);								//显示“加热盘温度”
	
	QPYLCD_DisplayCharacters(5, 136, BLACK, FONT24X24, 1, jrpwd + 6 * 72);						//显示“散热盘温度”
	QPYLCD_DisplayCharacters(29, 136, BLACK, FONT24X24, 5, jrpwd + 72);
	
	QPYLCD_DisplayCharacters(5, 176, BLACK, FONT24X24, 1, jishi);								//显示“计时：”
	QPYLCD_DisplayCharacters(5 + 24*4, 176, BLACK, FONT24X24, 2, jishi + 72);
	
	QPYLCD_DisplayCharacters(5, 216, BLACK, FONT24X24, 2, jishi);								//显示“计时时温度：”
	QPYLCD_DisplayCharacters(5 + 24*2, 216, BLACK, FONT24X24, 1, jishi + 72);
	QPYLCD_DisplayCharacters(5 + 24*3, 216, BLACK, FONT24X24, 3, jrpwd + 72*3);
	
	QPYLCD_DisplayCharacters(280, 216, BLACK, FONT24X24, 2, dangqian);							//显示“当前温度”
	QPYLCD_DisplayCharacters(280 + 24*2, 216, BLACK, FONT24X24, 3, jrpwd + 72*3);
}


/*******************************************
*函数名称：	DRY_BuildBalance
*功能：		建立稳恒态
*参数：		无
*返回值：	无
*******************************************/
void DRY_BuildBalance(void)
{
	float tempA, tempB, tempT;																			//加热盘，散热盘温度
	uint8_t str[10];
	uint8_t scanData;																			//按键键值
	
	DRY_BuildBalanceScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//判断按键松开
	
	tim3Count = 0;																				//计时器归零
	
	while (DS18B20_ReadTemp(DS18B20B) == 85.0)
		;
	tempB = DS18B20_ReadTemp(DS18B20B);															//默认刷新一次数据
	sprintf((char *)str, "%-5.1f", tempB);
	QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
	
	HeatingEnable();																			//开启加热
	temperatureControl.heatingAimTemperature = experimentalData.settedTemperature;				//设置加热目标温度
	
	while (1)
	{
//		DRY_TemperatureControl(experimentalData.settedTemperature);								//温度控制

		do
		{
			tempA = DS18B20_ReadTemp(DS18B20A);													//读取温度
			tempT = DS18B20_ReadTemp(DS18B20A);												    //读两次，过滤错误
		} while ((((tempA - tempT) > 0) ? (tempA - tempT) : (tempT - tempA)) > 10);
		
		do 
		{
			tempB = DS18B20_ReadTemp(DS18B20B);
			tempT = DS18B20_ReadTemp(DS18B20B);
		} while ((((tempB - tempT) > 0) ? (tempB - tempT) : (tempT - tempB)) > 10);
		
		sprintf((char *)str, "%-5.1f", tempA);													//显示加热盘温度
//		QPYLCD_DrawRectangle(149, 96, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 96, BLACK, FONT16X24, str);
		
		sprintf((char *)str, "%-5.1f", tempB);													//显示散热盘温度
//		QPYLCD_DrawRectangle(149, 136, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 136, BLACK, FONT16X24, str);
		QPYLCD_DisplayString(400, 216, BLACK, FONT16X24, str);									//当前温度
		
		sprintf((char *)str, "%d:%02d", tim3Count / 60, tim3Count % 60);						//显示计时时间
//		QPYLCD_DrawRectangle(149, 176, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 176, BLACK, FONT16X24, str);
		
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData)
		{
			switch (scanData)
			{
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
				case KEY_LEFT_LONG:																	//按键左
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
//					{
						HeatingDisable();														//关闭加热
						experimentalData.progress = TEMPERATURESETTING;							//进入上一步骤
						return;
//					}
#endif
//					break;
				
				case KEY_COUNT:																	//按键计时，重新计时
					tim3Count = 0;																//计时器归零
					sprintf((char *)str, "%-5.1f", tempB);										//刷新计时时温度
//					QPYLCD_DrawRectangle(149, 216, 80, 24, WHITE);
					QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
					QPYLCD_DrawRectangle(149, 176, 96, 24, WHITE);								//清除时间显示区域
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//按键确认
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
//					{
						//DRY_EnterDialogScreen();												//显示确认弹窗
						if (DRY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.balanceTempeatrue = tempB;							//记录数据
							experimentalData.progress = HEATTING;								//进入下一步骤
							
							HeatingDisable();													//关闭加热
							return;
						}
						else																	//取消，刷新显示
						{
							DRY_BuildBalanceScreen();											//刷新显示
							
//							tempB = DS18B20_ReadTemp(DS18B20B);									//刷新数据
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
*函数名称：	DRY_HeatingScreen
*功能：		显示仪器散热盘升温界面
*参数：		无
*返回值：	无
*******************************************/
void DRY_HeatingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, srpsw);								//显示“散热盘升温”
	
	DRY_DrawEquipment1(320, 100, YELLOW);														//绘制图形
	DRY_DrawEquipment3(320, 164, YELLOW);
	
	QPYLCD_SetBackColor(WHITE);	
	
	QPYLCD_DisplayCharacters(5, 124, BLACK, FONT24X24, 6, jrpwd);								//显示“加热盘温度”
	
	QPYLCD_DisplayCharacters(5, 188, BLACK, FONT24X24, 1, jrpwd + 6 * 72);						//显示“散热盘温度”
	QPYLCD_DisplayCharacters(29, 188, BLACK, FONT24X24, 5, jrpwd + 72);
}


/*******************************************
*函数名称：	DRY_Heating
*功能：		散热盘升温
*参数：		无
*返回值：	无
*******************************************/
void DRY_Heating(void)
{
	float tempA, tempB, tempT;																			//加热盘，散热盘温度
	uint8_t str[10];																			
	uint8_t scanData;																		   //按键键值
						
	DRY_HeatingScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);													 //判断按键松开
	
	HeatingEnable();																			//开启加热
	temperatureControl.heatingAimTemperature = experimentalData.settedTemperature;			//设置加热目标温度
	
	while (1)
	{
//		DRY_TemperatureControl(80);																//温度控制
																								//加入PID算法，由定时器进行温度控制
		do
		{
			tempA = DS18B20_ReadTemp(DS18B20A);														//读取温度
			tempT = DS18B20_ReadTemp(DS18B20A);													 //读两次，过滤错误
		} while ((((tempA - tempT) > 0) ? (tempA - tempT) : (tempT - tempA)) > 10);
		
		do 
		{
			tempB = DS18B20_ReadTemp(DS18B20B);
			tempT = DS18B20_ReadTemp(DS18B20B);
		} while ((((tempB - tempT) > 0) ? (tempB - tempT) : (tempT - tempB)) > 10);
		
		sprintf((char *)str, "%-5.1f", tempA);													//显示加热盘温度
//		QPYLCD_DrawRectangle(149, 124, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 124, BLACK, FONT16X24, str);
		
		sprintf((char *)str, "%-5.1f", tempB);													//显示散热盘温度
//		QPYLCD_DrawRectangle(149, 188, 80, 24, WHITE);
		QPYLCD_DisplayString(149, 188, BLACK, FONT16X24, str);

		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData)
		{
			switch (scanData)
			{
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
				case KEY_LEFT_LONG:																	//按键左
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
//					{
						HeatingDisable();														//关闭加热
						experimentalData.progress = BUILDBALANCE;								//进入上一步骤
						return;
//					}
#endif
//					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//按键确认
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
//					{
						//DRY_EnterDialogScreen();												//显示确认弹窗
						if (DRY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.heatingTempeatrue = tempB;							//保存数据
							experimentalData.progress = RECORDING;								//进入下一步骤
							
							HeatingDisable();													//关闭加热
							
							return;
						}
						else																	//取消，刷新显示
						{
							DRY_HeatingScreen();												//刷新显示
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
*函数名称：	DRY_RecordingScreen
*功能：		显示仪器记录数据界面
*参数：		无
*返回值：	无
*******************************************/
void DRY_RecordingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(144, 16, WHITE, FONT32X32, 6, srslcl);								//显示“散热速率测量”
	
	DRY_DrawEquipment3(352, 66, YELLOW);														//绘制图形
	
	QPYLCD_DrawLine(0, 64, 320, 64, BLACK);														//绘制数据表格
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
	
	QPYLCD_DisplayCharacters(16, 69, BLACK, FONT16X16, 1, sjwd);								//显示表头“时间”
	QPYLCD_DisplayCharacters(48, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(176, 69, BLACK, FONT16X16, 1, sjwd);
	QPYLCD_DisplayCharacters(208, 69, BLACK, FONT16X16, 1, sjwd + 32);
	
	QPYLCD_DisplayCharacters(96, 69, BLACK, FONT16X16, 1, sjwd + 64);								//显示表头“温度”
	QPYLCD_DisplayCharacters(128, 69, BLACK, FONT16X16, 1, sjwd + 96);
	QPYLCD_DisplayCharacters(256, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(288, 69, BLACK, FONT16X16, 1, sjwd + 96);
}


/*******************************************
*函数名称：	DRY_Recording
*功能：		记录数据
*参数：		无
*返回值：	无
*******************************************/
void DRY_Recording(void)
{
	uint8_t group = 1;																			//数据编号
	uint8_t scanData;																			//按键键值
	float tempB;
	uint8_t j;
	
	DRY_RecordingScreen();
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//判断按键松开
	
	tim3Count = 0;																				//计时器归零
	
	while(1)
	{
		tempB = DS18B20_ReadTemp(DS18B20B);														//读取散热盘温度
		DRY_DisplayData(group, tim3Count, tempB, RED);											//显示一组数据
		
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData)
		{
			switch (scanData)
			{
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
				case KEY_LEFT_LONG:																	//按键左
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
//					{
						experimentalData.progress = HEATTING;									//进入上一步骤
						return;
//					}
#endif
//					break;
				
				case KEY_LEFT:
					if (group > 1)
					{
						DRY_DisplayData(group--, tim3Count, tempB, WHITE);						//删除一条记录(使用背景色显示，减少代码量)
					}
					break;
				
				case KEY_COUNT:																	//按键计数
					if (group <= 20)
					{
						experimentalData.measuredData[group - 1].time = tim3Count;				//记录一组数据
						experimentalData.measuredData[group - 1].temperature = tempB;
						DRY_DisplayData(group++, tim3Count, tempB, BLACK);						//刷新数据显示
					}
					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//按键确认
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
//					{
						//DRY_EnterDialogScreen();												//显示确认弹窗
						if (DRY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							if (group <= 20)													//记录最后一组数据
							{
								experimentalData.measuredData[group - 1].time = tim3Count;
								experimentalData.measuredData[group - 1].temperature = tempB;
//								DRY_DisplayData(group++, tim3Count, tempB, BLACK);				//此处注释，没有必要刷新
							}
							
							if (W25X16_Check() == TRUE)
							{
								DRY_DataHandlerDialog(DATA_SAVE);
								DRY_SaveExperimentalData(experimentalData);
//								AT24CXX_Write(0, (uint8_t *)(&experimentalData), sizeof(DRY_ExperimentalData));	//保存所有实验数据至AT24C02
							}
							experimentalData.progress = SHOWDATA;								//进入下一步骤
							return;
						}
						else																	//取消，刷新显示	
						{
							DRY_RecordingScreen();												//刷新显示
							
							for (j = 0; j < group - 1; j++)										//刷新数据
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
*函数名称：	DRY_ShowDataScreen
*功能：		显示所有实验数据
*参数：		无
*返回值：	无
*******************************************/
void DRY_ShowDataScreen(void)
{
	uint8_t str[5];
	uint8_t i;
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(176, 16, WHITE, FONT32X32, 4, sysj);								//显示“散热速率测量”
	
	
	QPYLCD_DrawLine(0, 64, 344, 64, BLACK);														//绘制数据表格
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
	
	QPYLCD_DisplayCharacters(82, 69, BLACK, FONT16X16, 1, sjwd);								//显示表头“时间”
	QPYLCD_DisplayCharacters(114, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(218, 69, BLACK, FONT16X16, 1, sjwd);
	QPYLCD_DisplayCharacters(250, 69, BLACK, FONT16X16, 1, sjwd + 32);
	
	QPYLCD_DisplayCharacters(150, 69, BLACK, FONT16X16, 1, sjwd + 64);								//显示表头“温度”
	QPYLCD_DisplayCharacters(182, 69, BLACK, FONT16X16, 1, sjwd + 96);
	QPYLCD_DisplayCharacters(286, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(318, 69, BLACK, FONT16X16, 1, sjwd + 96);
	
	QPYLCD_DisplayCharacters(4, 69, BLACK, FONT16X16, 4, jrwdwht);								//显示“加热温度”等
	QPYLCD_DisplayCharacters(12, 121, BLACK, FONT16X16, 3, jrwdwht + 32 *4);
	QPYLCD_DisplayCharacters(20, 147, BLACK, FONT16X16, 2, jrwdwht + 32 * 2);
	QPYLCD_DisplayCharacters(12, 199, BLACK, FONT16X16, 3, jrwdwht + 32 * 7);
	QPYLCD_DisplayCharacters(20, 225, BLACK, FONT16X16, 2, jrwdwht + 32 * 2);
	
	QPYLCD_DisplayCharacters(348, 82, BLACK, FONT16X16, 3, jrwdwht + 32 * 10);					//显示“学号”
	QPYLCD_DisplayString(396, 82, BLACK, FONT8X16, experimentalData.studentNumber);				//显示学号
	
	sprintf((char *)str, "%.1f", experimentalData.settedTemperature);							//格式化加热温度并显示
	QPYLCD_DisplayString(20, 95, BLACK, FONT8X16, str);	
	
	sprintf((char *)str, "%.1f", experimentalData.balanceTempeatrue);							//格式化稳恒态温度并显示
	QPYLCD_DisplayString(20, 173, BLACK, FONT8X16, str);	
	
	sprintf((char *)str, "%.1f", experimentalData.heatingTempeatrue);							//格式化升温后温度并显示
	QPYLCD_DisplayString(20, 251, BLACK, FONT8X16, str);	
	
	for (i = 0; i < 7; i ++)
	{
		sprintf((char *)str, "%02d:%02d", experimentalData.measuredData[i].time / 60,
				experimentalData.measuredData[i].time % 60);									//格式化时间
		QPYLCD_DisplayString(86, 95 + 26 * i, BLACK, FONT8X16, str);							//显示时间
		sprintf((char *)str, "%.1f", experimentalData.measuredData[i].temperature);				//格式化温度
		QPYLCD_DisplayString(158, 95 + 26 * i, BLACK, FONT8X16, str);							//显示温度
		
		sprintf((char *)str, "%02d:%02d", experimentalData.measuredData[i + 7].time / 60,
				experimentalData.measuredData[i + 7].time % 60);								//格式化时间
		QPYLCD_DisplayString(222, 95 + 26 * i, BLACK, FONT8X16, str);							//显示时间
		sprintf((char *)str, "%.1f", experimentalData.measuredData[i+ 7].temperature);			//格式化温度
		QPYLCD_DisplayString(294, 95 + 26 * i, BLACK, FONT8X16, str);							//显示温度
		if (i + 14 < 20)
		{
			sprintf((char *)str, "%02d:%02d", experimentalData.measuredData[i + 14].time / 60,
				experimentalData.measuredData[i + 14].time % 60);								//格式化时间
			QPYLCD_DisplayString(359, 121 + 26 * i, BLACK, FONT8X16, str);						//显示时间
			sprintf((char *)str, "%.1f", experimentalData.measuredData[i + 14].temperature);	//格式化温度
			QPYLCD_DisplayString(430, 121 + 26 * i, BLACK, FONT8X16, str);						//显示温度
		}
	}
	
}


void DRY_ShowData(void)
{
	uint8_t scanData;
	
	DRY_ShowDataScreen();
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
				case KEY_LEFT_LONG:																	//按键左
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
//						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
//					{
						experimentalData.progress = RECORDING;									//进入上一步骤
						return;
//					}
#endif
//					break;
				
				case KEY_ENTER:
				case KEY_ENTER_LONG:																	//按键确认
//					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
//					{
						experimentalData.progress = COMPLETE;									//进入下一步骤
						return;
//					}
//					break;
			}
		}
	}
}


/*******************************************
*函数名称：	DRY_EnterDialogScreen
*功能：		显示确认对话框
*参数：		无
*返回值：	无
*******************************************/
void DRY_EnterDialogScreen(void)
{
	QPYLCD_DrawRectangle(132, 88, 216, 96, WHITE);												//清除对话框区域
	
	QPYLCD_DrawLine(132, 88, 348, 88, QPYLCD_NewColor(4, 4, 2));								//绘制对话框外框
	QPYLCD_DrawLine(132, 184, 348, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(132, 88, 132, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(348, 88, 348, 184, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_DisplayCharacters(144, 100, BLACK, FONT24X24, 8, qdjrxyb);							//显示“确认进入下一步？”
	
	QPYLCD_DrawLine(144, 148, 193, 148, QPYLCD_NewColor(4, 4, 2));								//绘制“确认”按钮边框
	QPYLCD_DrawLine(144, 172, 193, 172, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(144, 148, 144, 172, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(193, 148, 193, 172, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_DrawLine(288, 148, 337, 148, QPYLCD_NewColor(4, 4, 2));								//绘制“取消”按钮边框
	QPYLCD_DrawLine(288, 172, 337, 172, QPYLCD_NewColor(4, 4, 2));	
	QPYLCD_DrawLine(288, 148, 288, 172, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(337, 148, 337, 172, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_DrawRectangle(145, 149, 48, 23, CYAN);												//确认按钮填色
	
	QPYLCD_SetBackColor(CYAN);
	QPYLCD_DisplayCharacters(152, 152, BLACK, FONT16X16, 2, qdqx);								//显示“确认”
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(296, 152, BLACK, FONT16X16, 2, qdqx + 32 * 2);						//显示“取消”
}


/*******************************************
*函数名称：	DRY_EnterDialog
*功能：		确认对话框操作
*参数：		无
*返回值：	0			确认
*			1			取消
*******************************************/
uint8_t DRY_EnterDialog(void)
{
	uint8_t scanData;
	uint8_t i = 0;
	
	DRY_EnterDialogScreen();
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//按键左，移动光标
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
				
				case KEY_RIGHT:																	//按键右，移动光标
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
				
				case KEY_ENTER:																	//按键确认，返回选中值
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
*函数名称：	DRY_CompleteScreen
*功能：		显示实验完成提示
*参数：		无
*返回值：	无
*******************************************/
void DRY_CompleteScreen(void)
{
	QPYLCD_DrawRectangle(156, 88, 168, 96, WHITE);												//清除对话框区域
	
	QPYLCD_DrawLine(156, 88, 324, 88, QPYLCD_NewColor(4, 4, 2));								//绘制对话框外框
	QPYLCD_DrawLine(156, 184, 324, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(156, 88, 156, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(324, 88, 324, 184, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 6, nywcsy);							//显示“你已完成实验”
	QPYLCD_DisplayCharacters(168, 148, BLACK, FONT24X24, 6, nywcsy + 72 * 6);					//显示“请整理好桌面”
}


/*******************************************
*函数名称：	DRY_Complete
*功能：		实验完成
*参数：		无
*返回值：	无
*******************************************/
void DRY_Complete(void)
{
	uint8_t scanData;
	
	DRY_CompleteScreen();
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//检测任意按键按下
					
				case KEY_RIGHT:
					
				case KEY_COUNT:
				
				case KEY_ENTER:
					experimentalData.progress = SHOWDATA;										//回到上一步
				
					return;
//					break;
				
				default:
					break;
			}
		}
	}
}
	

/*******************************************
*函数名称：	DRY_DataHandlerDialog
*功能：		数据保存，读取对话框
*参数：		mode		0			读取
*						1			保存
*返回值：	无
*******************************************/
void DRY_DataHandlerDialog(DRY_DataHandlerType mode)
{
	QPYLCD_DrawRectangle(156, 88, 168, 96, WHITE);												//清除对话框区域
	
	QPYLCD_DrawLine(156, 88, 324, 88, QPYLCD_NewColor(4, 4, 2));								//绘制对话框外框
	QPYLCD_DrawLine(156, 184, 324, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(156, 88, 156, 184, QPYLCD_NewColor(4, 4, 2));
	QPYLCD_DrawLine(324, 88, 324, 184, QPYLCD_NewColor(4, 4, 2));
	
	QPYLCD_SetBackColor(WHITE);
	
	switch ((uint8_t)mode)
	{
		case DATA_READ:
			QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 2, zzbcsj);					//显示“正在读取数据”
			QPYLCD_DisplayCharacters(216, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 9);
			QPYLCD_DisplayCharacters(264, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 4);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 3, zzbcsj + 72 * 6);			//显示“请等待”
			break;
		
		case DATA_SAVE:
			QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 6, zzbcsj);					//显示"正在保存数据"
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 3, zzbcsj + 72 * 6);			//显示“请等待”
			break;
		
		case DATA_LOOKUP:
			QPYLCD_DisplayCharacters(192, 100, BLACK, FONT24X24, 2, zzbcsj);					//显示“正在查找”
			QPYLCD_DisplayCharacters(240, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 11);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 3, zzbcsj + 72 * 6);			//显示“请等待”
			break;
		
		case DATA_READ_FAILED:
			QPYLCD_DisplayCharacters(192, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 9);			//显示“读取失败”
			QPYLCD_DisplayCharacters(240, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 13);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 1, zzbcsj + 72 * 6);			//显示“请重试”
			QPYLCD_DisplayCharacters(228, 148, BLACK, FONT24X24, 2, zzbcsj + 72 * 15);
			break;
		
		case DATA_LOOKUP_FAILED:
			QPYLCD_DisplayCharacters(192, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 11);			//显示“查找失败”
			QPYLCD_DisplayCharacters(240, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 13);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 1, zzbcsj + 72 * 6);			//显示“请重试”
			QPYLCD_DisplayCharacters(228, 148, BLACK, FONT24X24, 2, zzbcsj + 72 * 15);
			break;
		
		case DATA_LOOKUP_END:
			QPYLCD_DisplayCharacters(168, 100, BLACK, FONT24X24, 4, zzbcsj + 72 * 17);			//显示“没有更多数据”
			QPYLCD_DisplayCharacters(264, 100, BLACK, FONT24X24, 2, zzbcsj + 72 * 4);
			QPYLCD_DisplayCharacters(204, 148, BLACK, FONT24X24, 1, zzbcsj + 72 * 6);			//显示“请重试”
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
*函数名称：	DRY_DrawEquipment1
*功能：		显示仪器图案加热盘
*参数：		xLabel			x坐标
*			yLabel			y坐标
*			color			填充颜色
*返回值：	无
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
*函数名称：	DRY_DrawEquipment2
*功能：		显示仪器图案不良热导体
*参数：		xLabel			x坐标
*			yLabel			y坐标
*			color			填充颜色
*返回值：	无
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
*函数名称：	DRY_DrawEquipment3
*功能：		显示仪器散热盘
*参数：		xLabel			x坐标
*			yLabel			y坐标
*			color			填充颜色
*返回值：	无
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
*函数名称：	DRY_DisplayData
*功能：		显示实验数据
*参数：		group			数据编号
*			tempB			温度
*			color			字体颜色
*返回值：	无
*******************************************/
void DRY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color)
{
	uint8_t str[10];

	if (group < 8)																				//前7组数据
	{
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);									//格式化时间
		QPYLCD_DisplayString(5, 69 + 26 * group, color, FONT8X16, str);							//显示时间
		sprintf((char *)str, "%.1f", tempB);													//格式化温度
		QPYLCD_DisplayString(85, 69 + 26 * group, color, FONT8X16, str);						//显示温度
	}
	else if (group < 15)																		//7-14组数据
	{																						   
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);					  			//格式化时间
		QPYLCD_DisplayString(165, 69 + 26 * (group - 7), color, FONT8X16, str);				 //显示时间
		sprintf((char *)str, "%.1f", tempB);													//格式化温度
		QPYLCD_DisplayString(245, 69 + 26 * (group - 7), color, FONT8X16, str);				 //显示温度
	}
	else if (group < 21)																		//15-20组数据
	{																						   
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);									//格式化时间
		QPYLCD_DisplayString(325, 69 + 26 * (group - 13), color, FONT8X16, str);				//显示时间
		sprintf((char *)str, "%.1f", tempB);													//格式化温度
		QPYLCD_DisplayString(405, 69 + 26 * (group - 13), color, FONT8X16, str);				//显示温度
	}
}


/*******************************************
*函数名称：	DRY_TemperatureControl
*功能：		温度控制
*参数：		temperature			加热目标温度
*返回值：	无
*******************************************/
void DRY_TemperatureControl(void)
{
	float tempC;
	
#if		PID_CONTROL != 0 																		//PID算法控制温度
	static float dutyCycle = 100;																//占空比
	float err[3];
	
#else	/*- PID_CONTROL != 0 -*/																//比例算法控制温度
	float tempA;
	
	tempA = DS18B20_ReadTemp(DS18B20A);															//读取温度
#endif	/*- PID_CONTROL != 0 -*/
	
	tempC = DS18B20_ReadTemp(DS18B20C);															//无关温度控制算法，温度过高停止加热
	if (tempC > temperatureControl.heatingAimTemperature + 10)									//加热盘温度不能超过目标值10度
	{
		PWM_SetDutyCycle(0);
		return;
	}
	
#if		PID_CONTROL != 0																		//PID算法控制温度
	err[0] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[0];
	err[1] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[1];
	err[2] = temperatureControl.heatingAimTemperature - temperatureControl.pidTemperature[2];
	
	if (temperatureControl.pidTemperature[2] < (temperatureControl.heatingAimTemperature - 5))
	{
		PWM_SetDutyCycle(100);
	}
	else if (temperatureControl.pidTemperature[2] < (temperatureControl.heatingAimTemperature + 5))
	{
	#if PID_CONTROL == 1																		//位置式PID算法
		temperatureControl.pidErr += err[2];
		dutyCycle = temperatureControl.pidKP * err[2]
					+ temperatureControl.pidKI * temperatureControl.pidErr
					+ temperatureControl.pidKD * (err[2] - err [1]);
	#else /*- PID_CONTROL == 1 -*/																//增量式PID算法
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
	
	/*-----------------输出PID调试信息--------------*/
	DEBUG_LOG("%d:\t", tim3Count);
	#if	PID_CONTROL == 1
		DEBUG_LOG("err:%Lf, ", temperatureControl.pidErr);
	#endif
	
	DEBUG_LOG("temp[k]:%.1f, temp[k-1]:%.1f, temp[k-2]:%.1f; PWM_DutyCycle:%f\r\n",
				temperatureControl.pidTemperature[2],
				temperatureControl.pidTemperature[1],
				temperatureControl.pidTemperature[0],
				dutyCycle);
	
#else	/*- PID_CONTROL != 0 - */																//比例算法控制温度
	if (tempA < (temperatureControl.heatingAimTemperature - 5))									//比较加热盘当前温度与目标温度	
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
*函数名称：	DRY_UplaodData
*功能：		上传数据至服务器
*参数：		command			命令
*返回值：	无
*******************************************/
void DRY_UplaodData(uint8_t command)
{
	char str[20];
	uint8_t i = 0;
	
	if (espState.currentCommand != ESP8266_CIPSEND)												//ESP8266初始化未完成
		return;
	
	sprintf(str, "send-%d-%d", command, experimentalData.machineNumber);						//格式化命令和机器号并发送
	ESP8266_SendString(str);
	
	switch (command)																			//判断命令
	{
		case DEVICE_CONNECT:																	//仪器连接
			
			break;
		
		case DEVICE_REGISTER:																	//仪器注册
			sprintf(str, "-%s", experimentalData.studentNumber);								//格式化学号并发送
			ESP8266_SendString(str);
			break;
		
		case DEVICE_DATA:																		//仪器发送数据
			if (experimentalData.progress != INPUTSTUDENTNUMBER)								//判断当前步骤
			{
				sprintf(str, "-%.1f", experimentalData.settedTemperature);						//格式化设置温度并发送
				ESP8266_SendString(str);
				if (experimentalData.progress == TEMPERATURESETTING)
					break;
				
				sprintf(str, "-%.1f", experimentalData.balanceTempeatrue);						//格式化稳恒态温度并发送
				ESP8266_SendString(str);
				if (experimentalData.progress == BUILDBALANCE)
					break;
				
				sprintf(str, "-%.1f", experimentalData.heatingTempeatrue);						//格式化升温温度并发送
				ESP8266_SendString(str);
				if (experimentalData.progress == HEATTING)
					break;
				
				while (experimentalData.measuredData[i].time)									//格式化实验数据并发送
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
	ESP8266_SendString("\r\n");																	//数据尾
}



/*******************************************
*函数名称：	DRY_SaveExperimentalData
*功能：		保存数据至Flash
*参数：		experimentalData	实验数据
*返回值：	DRY_OK				保存成功
*			DRY_ERROR			保存失败
*说明：		一个扇区大小4K(0x1000 Byte)，每组实验数据储存在256字节的空间里，一个
*			扇区可以储存16组数据，地址0x00000000至0x000010000(第一个扇区)用于储存
*			数据数量等信息
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
*函数名称：	DRY_ReadExperimentalData
*功能：		从Flash读取实验数据
*参数：		experimentalDataPointer		实验数据
*			dataAddress					数据地址
*返回值：	DRY_OK				读取成功
*			DRY_ERROR			读取失败
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
	else if (dataAddress == 0x00000000)								//地址为0时，读取最后一组实验数据
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


