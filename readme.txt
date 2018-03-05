说明文档：

1. 此程序适用于中南大学物理实验中心热学试验仪

2. 源文件说明
	文件名				说明									备注
	main.c				主程序所在文件							外设初始化，主循环
	qpylcd.c			液晶驱动
	delay.c				延时函数
	asciifont.c			液晶驱动支持文件，提供ASCII字符字库
	ds18b20.c			温度传感器驱动							提供三条ds18b20总线
	usart.c				串口驱动								
	tim.c				定时器驱动								定时器2,3,4，其中定时器4用作PWM
	esp8266.c			ESP8266WiFi模块驱动						实现WiFi通信，通信逻辑
	font.c				程序所用到的汉字字模及LOGO图片			汉字未实现统一字库
	support.c			导热仪支持驱动							实现仪器的逻辑处理，显示UI
	key.c				按键驱动								添加KEYANDEC11_Scan函数，实现同时扫描按键和编码器
	ec11.c				编码器驱动
	pwm.c				PWM驱动									通过PWM调节加热功率
	eeprom.c			模拟EEPROM驱动							使用STM32ROM最后1K作为EEPROM
	beep.c				蜂鸣器驱动
	
3. 部分可自定义选项位于相关头文件宏定义区域，可自行修改，说明如下：
	宏					功能							位置
	PRINTTOUSART		实现printf函数打印到串口1		usart.h
	PRINTTILCD			实现printf函数打印到液晶屏		qpylcd.h
	ALLOWBACK			实验中允许返回上一步骤			support.h
	
4. WiFi通信中，热点ssid和password以及服务器ip和port位于esp8266.c文件中，修改时留意相关代码


5. 软件更新日志：

2018/2/23	0.1Beta
	a. 初版完整软件，各项功能基本实现

2018/2/27	0.2Beta
	a. 修复编码器概率性不可控
	b. 增加“确认进入下一步”对话框
	c. 增加“实验数据”界面，方便实验完成的数据记录
	d. 增加“实验完成”对话框，提示实验已完成


------中南大学物理与电子学院------
--------------张克强--------------
--------------2018/2--------------
