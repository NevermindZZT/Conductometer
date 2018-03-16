/****************** ***************************
*             导热仪支持函数                 *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/2                      *
*********************************************/

#include	"support.h"

TEMP_Control temperatureControl;

/*******************************************
*函数名称：	DYY_SystemSettingScreen
*功能：		显示仪器设置界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_SystemSettingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayString(176, 16, WHITE, FONT16X32, "Settings");								//显示“Settings”
	
	QPYLCD_SetBackColor(CYAN);																	//显示“Machine Number”
	QPYLCD_DrawRectangle(0, 64, 480, 47, CYAN);
	QPYLCD_DisplayString(5, 76, BLACK, FONT16X24, "Machine Number");
	QPYLCD_DisplayInt(320, 76, BLACK, FONT16X24, experimentalData.machineNumber);				//显示机器号
	QPYLCD_DrawLine(0, 111, 479, 111, QPYLCD_NewColor(1, 1, 1));
	
	QPYLCD_SetBackColor(WHITE);																	//显示"Brightness"
	QPYLCD_DisplayString(5, 124, BLACK, FONT16X24, "Brightness");
	QPYLCD_DisplayInt(320, 124, BLACK, FONT16X24, screenBrightness);							//显示亮度等级
	QPYLCD_DrawLine(0, 159, 479, 159, QPYLCD_NewColor(1, 1, 1));
	
	QPYLCD_DisplayString(5, 172, BLACK, FONT16X24, "Software Version");							//显示“Software Version”
	QPYLCD_DisplayString(320, 172, BLACK, FONT16X24, SOFTWAREVERSION);							//显示版本
	QPYLCD_DrawLine(0, 207, 479, 207, QPYLCD_NewColor(1, 1, 1));
}


/*******************************************
*函数名称：	DYY_SystemSetting
*功能：		仪器设置
*参数：		无
*返回值：	无
*******************************************/
void DYY_SystemSetting(void)
{
	uint8_t scanData;																			//按键键值
	uint8_t tempData[3];																		//设置数据
	int8_t i = 0;
	const uint8_t str[3][20] = {"Machine Number", "Brightness", "Software Version"};			//设置选项
	
	tempData[0] = experimentalData.machineNumber;												//设置数据初始化
	tempData[1] = screenBrightness;
	
	QPYLCD_SetBackColor(CYAN);
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_RIGHT:																	//按键右，移动设置光标
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
				
				case KEY_LEFT:																	//按键左，移动设置光标
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
				
				case KEY_ENTER:																	//按键确认
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						readFlash[0] = experimentalData.machineNumber = tempData[0];			//保存设置数据
						readFlash[1] = screenBrightness = tempData[1];
						MemWriteByte(readFlash, 2);												//写入Flash
						return;
					}
					break;
				
				case EC11_UP:																	//编码器顺时针
					tempData[i]++ ;																//设置数据增加
					break;
				
				case EC11_DOWN:																	//编码器逆时针
					tempData[i]-- ;																//设置数据减少
					break;
				
				default:
					break;
			}
			if (tempData[i] < 1 || tempData[0] > 30 || tempData[1] > 7)							//判断设置数据范围
			{
				tempData[i] = 1;
			}
			QPYLCD_SetBackColor(CYAN);
			QPYLCD_DisplayString(5, 76 + i * 48, BLACK, FONT16X24, str[i]);						//刷新显示
			QPYLCD_DrawRectangle(320, 76 + i * 48, 32, 24, CYAN);
			QPYLCD_DisplayInt(320, 76 + i * 48, BLACK, FONT16X24, tempData[i]);
			
			if (i != 2)																			//保持版本号显示不变
			{
				QPYLCD_SetBackColor(WHITE);
			}
			QPYLCD_DisplayString(320, 172, BLACK, FONT16X24, SOFTWAREVERSION);					//保持版本号显示不变
			
		}
	}
}


/*******************************************
*函数名称：	DYY_WelcomeScreen
*功能：		显示仪器初始化界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_WelcomeScreen(void)
{
	uint8_t i;
	
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
//	QPYLCD_DrawColorImage(0, 60, 480, 150, logo1);
	QPYLCD_DrawColorImage(0, 60, 150, 150, logo);												//显示logo(院徽)
	QPYLCD_DisplayCharacters(155, 90, QPYLCD_NewColor(0, 2, 2), FONT32X32, 10, zndxwlsyzx);		//显示“中南大学物理实验中心”
	QPYLCD_DisplayCharacters(171, 150, QPYLCD_NewColor(0, 2, 2), FONT24X24, 12, blrdt);			//显示“不良热导体导热系数测量仪”
	QPYLCD_DisplayCharacters(168, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 8, cshwdcgq);		//显示“初始化温度传感器”
	QPYLCD_DisplayString(296, 242, QPYLCD_NewColor(0, 2, 2), FONT8X16, "...");					//
	if ((!DS18B20_Init(DS18B20A)) || (!DS18B20_Init(DS18B20B)) || (!DS18B20_Init(DS18B20C)))	//判断温度传感器是否成功连接	
	{
		QPYLCD_DrawRectangle(168, 242, 152, 16, WHITE);
		QPYLCD_DisplayCharacters(112, 242, QPYLCD_NewColor(0, 2, 2), FONT16X16, 16, wdcgqcshsb);//显示“温度传感器初始化失败”

#ifndef		DEBUG																				//DEBUG模式关闭报警
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
*函数名称：	DYY_InputStudentNumberScreen
*功能：		显示仪器输入学号界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_InputStudentNumberScreen(void)
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
*函数名称：	DYY_InputStudentNumber
*功能：		学号输入
*参数：		无
*返回值：	无
*******************************************/
void DYY_InputStudentNumber(void)
{
	uint8_t scanData;																			//按键键值
	uint8_t tempData = 0;																		//当前光标位置数据
	uint16_t xLabel = 196;																		//光标x坐标
	uint8_t i = 0;
	uint8_t j;
	
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
				
				case KEY_ENTER:																	//按键确定
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						DYY_EnterDialogScreen();												//显示确认弹窗
						if (DYY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.studentNumber[i++] = tempData + 48;				//保存最后一次数据
							experimentalData.studentNumber[i] = NULL;							//学号结束
							experimentalData.progress = TEMPERATURESETTING;						//学号输入结束，进程进入下一步
							return;
						}
						else																	//取消，刷新显示
						{
							DYY_InputStudentNumberScreen();										//刷新显示
							xLabel = 196;
							for (j = 0; j < i; j++)												//恢复一输入学号数据显示
							{
								QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, experimentalData.studentNumber[j] - 48);
								xLabel += 16;
							}
						}
						
					}
					break;
				
				case EC11_UP:																	//编码器顺时针
					tempData++;																	//光标位置数据增加
					if (tempData > 9)															//数据范围限制
						tempData = 0;
					break;
					
				case EC11_DOWN:																	//编码器逆时针
					tempData--;                                                                 //光标位置数据减少
					if (tempData > 9)                                                           //数据范围限制
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
*函数名称：	DYY_TemperatureSettingScreen
*功能：		显示仪器温度设置界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_TemperatureSettingScreen(void)
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
*函数名称：	DYY_TemperatureSetting
*功能：		加热温度设置
*参数：		无
*返回值：	无
*******************************************/
void DYY_TemperatureSetting(void)
{
	uint8_t scanData;																			//按键键值
	uint8_t tempData[2] = {0, 0};																//设置温度十位，个位
	uint16_t xLabel = 316;																		//光标x坐标
	uint8_t i = 0;
	
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
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
					QPYLCD_DisplayInt(xLabel, 168, RED, FONT16X24, tempData[i]);				//刷新显示，消除由于长按返回造成的显示延迟
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
					{
						experimentalData.progress = INPUTSTUDENTNUMBER;							//进入上一步骤
						return;
					}
#endif
					break;
				
				case KEY_RIGHT:																	//按键右，光标右移
					if (i < 1)
					{
						QPYLCD_DisplayInt(xLabel, 168, BLACK, FONT16X24, tempData[i]);
						i++;
						xLabel += 16;
					}	
					break;
				
				case KEY_ENTER:																	//按键确认
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						DYY_EnterDialogScreen();												//显示确认弹窗
						if (DYY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.settedTemperature = tempData[0] * 10 + tempData[1];//保存数据
							experimentalData.progress = BUILDBALANCE;							//进入下一步骤
							return;
						}
						else																	//取消，刷新显示
						{
							DYY_TemperatureSettingScreen();										//刷新显示
							
							QPYLCD_DisplayInt(316, 168, BLACK, FONT16X24, tempData[0]);			//恢复数据显示
							QPYLCD_DisplayInt(332, 168, BLACK, FONT16X24, tempData[1]);
						}
					}
					break;
				
				case EC11_UP:																	//编码器顺时针
					tempData[i]++;                                                              //光标位置数据增加
					if (tempData[i] > 9)                                                        //数据范围限制
						tempData[i] = 0;                                                        
					break;                                                                      
					                                                                            
				case EC11_DOWN:                                                                 //编码器逆时针
					tempData[i]--;                                                              //光标位置数据减少
					if (tempData[i] > 9)                                                        //数据范围限制
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
*函数名称：	DYY_BuildBalanceScreen
*功能：		显示仪器建立稳恒态界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_BuildBalanceScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, jlwht);								//显示“建立稳恒态”
	
	DYY_DrawEquipment1(320, 80, YELLOW);														//绘制图形
	DYY_DrawEquipment2(320, 144, YELLOW);
	DYY_DrawEquipment3(320, 160, YELLOW);
	
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
*函数名称：	DYY_BuildBalance
*功能：		建立稳恒态
*参数：		无
*返回值：	无
*******************************************/
void DYY_BuildBalance(void)
{
	float tempA, tempB;																			//加热盘，散热盘温度
	uint8_t str[10];
	uint8_t scanData;																			//按键键值
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//判断按键松开
	
	tim3Count = 0;																				//计时器归零
	tempB = DS18B20_ReadTemp(DS18B20B);															//默认刷新一次数据
	tempB = DS18B20_ReadTemp(DS18B20B);															//重读温度，保证不出错
	sprintf((char *)str, "%-5.1f", tempB);
	QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
	
	HeatingEnable();																			//开启加热
	temperatureControl.heatingAimTemperature = 60;												//设置加热目标温度
	
	while (1)
	{
//		DYY_TemperatureControl(experimentalData.settedTemperature);								//温度控制
																								//加入PID算法，温度控制由定时器操作	
		
		tempA = DS18B20_ReadTemp(DS18B20A);														//读取温度
		tempB = DS18B20_ReadTemp(DS18B20B);
		
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
				case KEY_LEFT:																	//按键左
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
					{
						experimentalData.progress = TEMPERATURESETTING;							//进入上一步骤
						return;
					}
#endif
					break;
				
				case KEY_COUNT:																	//按键计时，重新计时
					tim3Count = 0;																//计时器归零
					sprintf((char *)str, "%-5.1f", tempB);										//刷新计时时温度
//					QPYLCD_DrawRectangle(149, 216, 80, 24, WHITE);
					QPYLCD_DisplayString(149, 216, BLACK, FONT16X24, str);
					break;
				
				case KEY_ENTER:																	//按键确认
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						DYY_EnterDialogScreen();												//显示确认弹窗
						if (DYY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.balanceTempeatrue = tempB;							//记录数据
							experimentalData.progress = HEATTING;								//进入下一步骤
							
							HeatingDisable();													//关闭加热
							return;
						}
						else																	//取消，刷新显示
						{
							DYY_BuildBalanceScreen();											//刷新显示
							
							tempB = DS18B20_ReadTemp(DS18B20B);									//刷新数据
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
*函数名称：	DYY_HeatingScreen
*功能：		显示仪器散热盘升温界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_HeatingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(160, 16, WHITE, FONT32X32, 5, srpsw);								//显示“散热盘升温”
	
	DYY_DrawEquipment1(320, 100, YELLOW);														//绘制图形
	DYY_DrawEquipment3(320, 164, YELLOW);
	
	QPYLCD_SetBackColor(WHITE);	
	
	QPYLCD_DisplayCharacters(5, 124, BLACK, FONT24X24, 6, jrpwd);								//显示“加热盘温度”
	
	QPYLCD_DisplayCharacters(5, 188, BLACK, FONT24X24, 1, jrpwd + 6 * 72);						//显示“散热盘温度”
	QPYLCD_DisplayCharacters(29, 188, BLACK, FONT24X24, 5, jrpwd + 72);
}


/*******************************************
*函数名称：	DYY_Heating
*功能：		散热盘升温
*参数：		无
*返回值：	无
*******************************************/
void DYY_Heating(void)
{
	float tempA, tempB;																			//加热盘，散热盘温度
	uint8_t str[10];                                                                            
	uint8_t scanData;                                                                           //按键键值
	                                                                                            
	while (KEYANDEC11_Scan() == KEY_ENTER);                                                     //判断按键松开
	
	HeatingEnable();																			//开启加热
	temperatureControl.heatingAimTemperature = 80;												//设置加热目标温度
	
	while (1)
	{
//		DYY_TemperatureControl(80);																//温度控制
																								//加入PID算法，由定时器进行温度控制
		
		tempA = DS18B20_ReadTemp(DS18B20A);														//读取温度
		tempB = DS18B20_ReadTemp(DS18B20B);
		
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
				case KEY_LEFT:																	//按键左
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
					{
						experimentalData.progress = BUILDBALANCE;								//进入上一步骤
						return;
					}
#endif
					break;
				
				case KEY_ENTER:																	//按键确认
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						DYY_EnterDialogScreen();												//显示确认弹窗
						if (DYY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							experimentalData.heatingTempeatrue = tempB;							//保存数据
							experimentalData.progress = RECORDING;								//进入下一步骤
							
							HeatingDisable();													//关闭加热
							
							return;
						}
						else																	//取消，刷新显示
						{
							DYY_HeatingScreen();												//刷新显示
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
*函数名称：	DYY_RecordingScreen
*功能：		显示仪器记录数据界面
*参数：		无
*返回值：	无
*******************************************/
void DYY_RecordingScreen(void)
{
	QPYLCD_SetBackColor(WHITE);
	QPYLCD_Clear();
	QPYLCD_DrawRectangle(0, 0, 480, 64, BLUE);
	QPYLCD_SetBackColor(BLUE);
	QPYLCD_DisplayCharacters(144, 16, WHITE, FONT32X32, 6, srslcl);								//显示“散热速率测量”
	
	DYY_DrawEquipment3(352, 66, YELLOW);														//绘制图形
	
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
	QPYLCD_DisplayCharacters(176, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(208, 69, BLACK, FONT16X16, 1, sjwd + 96);
	
	QPYLCD_DisplayCharacters(96, 69, BLACK, FONT16X16, 1, sjwd);								//显示表头“温度”
	QPYLCD_DisplayCharacters(128, 69, BLACK, FONT16X16, 1, sjwd + 32);
	QPYLCD_DisplayCharacters(256, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(288, 69, BLACK, FONT16X16, 1, sjwd + 96);
}


/*******************************************
*函数名称：	DYY_Recording
*功能：		记录数据
*参数：		无
*返回值：	无
*******************************************/
void DYY_Recording(void)
{
	uint8_t group = 1;																			//数据编号
	uint8_t scanData;																			//按键键值
	float tempB;
	uint8_t j;
	
	while (KEYANDEC11_Scan() == KEY_ENTER);														//判断按键松开
	
	tim3Count = 0;																				//计时器归零
	
	while(1)
	{
		tempB = DS18B20_ReadTemp(DS18B20B);														//读取散热盘温度
		DYY_DisplayData(group, tim3Count, tempB, RED);											//显示一组数据
		
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//按键左
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
					{
						experimentalData.progress = HEATTING;									//进入上一步骤
						return;
					}
#endif
					break;
				
				case KEY_COUNT:																	//按键计数
					if (group <= 20)
					{
						experimentalData.measuredData[group - 1].time = tim3Count;				//记录一组数据
						experimentalData.measuredData[group - 1].temperature = tempB;
						DYY_DisplayData(group++, tim3Count, tempB, BLACK);						//刷新数据显示
					}
					break;
				
				case KEY_ENTER:																	//按键确认
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						DYY_EnterDialogScreen();												//显示确认弹窗
						if (DYY_EnterDialog() == 0)												//判断弹窗动作，返回0为确定
						{
							if (group <= 20)													//记录最后一组数据
							{
								experimentalData.measuredData[group - 1].time = tim3Count;
								experimentalData.measuredData[group - 1].temperature = tempB;
								DYY_DisplayData(group++, tim3Count, tempB, BLACK);
							}
							experimentalData.progress = SHOWDATA;								//进入下一步骤
							return;
						}
						else																	//取消，刷新显示	
						{
							DYY_RecordingScreen();												//刷新显示
							
							for (j = 0; j < group - 1; j++)										//刷新数据
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
*函数名称：	DYY_ShowDataScreen
*功能：		显示所有实验数据
*参数：		无
*返回值：	无
*******************************************/
void DYY_ShowDataScreen(void)
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
	QPYLCD_DisplayCharacters(218, 69, BLACK, FONT16X16, 1, sjwd + 64);
	QPYLCD_DisplayCharacters(250, 69, BLACK, FONT16X16, 1, sjwd + 96);
	
	QPYLCD_DisplayCharacters(150, 69, BLACK, FONT16X16, 1, sjwd);								//显示表头“温度”
	QPYLCD_DisplayCharacters(182, 69, BLACK, FONT16X16, 1, sjwd + 32);
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


void DYY_ShowData(void)
{
	uint8_t scanData;
	
	while (1)
	{
		scanData = KEYANDEC11_Scan();															//扫描按键和编码器
		if (scanData != 0)
		{
			switch (scanData)
			{
				case KEY_LEFT:																	//按键左
#ifdef		ALLOWBACK																			//允许长按返回上一步骤
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT)
						&& KEYANDEC11_Scan() == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_LEFT))	//超级长按,返回上一步骤
					{
						experimentalData.progress = RECORDING;									//进入上一步骤
						return;
					}
#endif
					break;
					
				case KEY_ENTER:																	//按键确认
					if ((KEYANDEC11_Scan()) == KEY_DEFALUT && (KEYANDEC11_Scan() == KEY_ENTER))	//长按
					{
						experimentalData.progress = COMPLETE;									//进入下一步骤
						return;
					}
					break;
			}
		}
	}
}


/*******************************************
*函数名称：	DYY_EnterDialogScreen
*功能：		显示确认对话框
*参数：		无
*返回值：	无
*******************************************/
void DYY_EnterDialogScreen(void)
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
*函数名称：	DYY_EnterDialog
*功能：		确认对话框操作
*参数：		无
*返回值：	0			确认
*			1			取消
*******************************************/
uint8_t DYY_EnterDialog(void)
{
	uint8_t scanData;
	uint8_t i = 0;
	
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
					return i;
//					break;
					
				default :
					break;
			}
		}
	}
}


/*******************************************
*函数名称：	DYY_CompleteScreen
*功能：		显示实验完成提示
*参数：		无
*返回值：	无
*******************************************/
void DYY_CompleteScreen(void)
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
*函数名称：	DYY_Complete
*功能：		实验完成
*参数：		无
*返回值：	无
*******************************************/
void DYY_Complete(void)
{
	uint8_t scanData;
	
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
*函数名称：	DYY_DrawEquipment1
*功能：		显示仪器图案加热盘
*参数：		xLabel			x坐标
*			yLabel			y坐标
*			color			填充颜色
*返回值：	无
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
*函数名称：	DYY_DrawEquipment2
*功能：		显示仪器图案不良热导体
*参数：		xLabel			x坐标
*			yLabel			y坐标
*			color			填充颜色
*返回值：	无
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
*函数名称：	DYY_DrawEquipment3
*功能：		显示仪器散热盘
*参数：		xLabel			x坐标
*			yLabel			y坐标
*			color			填充颜色
*返回值：	无
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
*函数名称：	DYY_DisplayData
*功能：		显示实验数据
*参数：		group			数据编号
*			tempB			温度
*			color			字体颜色
*返回值：	无
*******************************************/
void DYY_DisplayData(uint8_t group, uint32_t time, float tempB, uint8_t color)
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
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);                      			//格式化时间
		QPYLCD_DisplayString(165, 69 + 26 * (group - 7), color, FONT8X16, str);                 //显示时间
		sprintf((char *)str, "%.1f", tempB);                                                    //格式化温度
		QPYLCD_DisplayString(245, 69 + 26 * (group - 7), color, FONT8X16, str);                 //显示温度
	}
	else if (group < 21)																		//15-20组数据
	{                                                                                           
		sprintf((char *)str, "%d:%02d", time / 60, time % 60);									//格式化时间
		QPYLCD_DisplayString(325, 69 + 26 * (group - 13), color, FONT8X16, str);                //显示时间
		sprintf((char *)str, "%.1f", tempB);                                                    //格式化温度
		QPYLCD_DisplayString(405, 69 + 26 * (group - 13), color, FONT8X16, str);                //显示温度
	}
}


/*******************************************
*函数名称：	DYY_TemperatureControl
*功能：		温度控制
*参数：		temperature			加热目标温度
*返回值：	无
*******************************************/
void DYY_TemperatureControl(void)
{
	float tempC;
	
#ifdef		PID_CONTROL																			//PID算法控制温度
	static float dutyCycle = 100;																	//占空比
	float err[3];
	
#else																							//比例算法控制温度
	float tempA;
	
	tempA = DS18B20_ReadTemp(DS18B20A);															//读取温度
#endif
	
	tempC = DS18B20_ReadTemp(DS18B20C);															//无关温度控制算法，温度过高停止加热
	if (tempC > 100)																			//加热盘温度不能超过100度
	{
		PWM_SetDutyCycle(0);
		return;
	}
	
#ifdef		PID_CONTROL																			//PID算法控制温度
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
				+ PID_KP * (err[2] - 2 * err[1] + err[0]);										//PID算法
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
	
#else																							//比例算法控制温度
	if (tempA < (temperatureControl.heatingAimTemperature - 5))									//比较加热盘当前温度与目标温度	
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
*函数名称：	DYY_UplaodData
*功能：		上传数据至服务器
*参数：		command			命令
*返回值：	无
*******************************************/
void DYY_UplaodData(uint8_t command)
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
					sprintf(str, "-%d-%.1f", experimentalData.measuredData[i].time, experimentalData.measuredData[i].temperature);
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


