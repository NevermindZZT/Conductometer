/*********************************************
*             EC11编码器驱动                 *
*         中南大学物理与电子学院             *
*                张克强                      *
*                2018/1                      *
*********************************************/

#include "ec11.h"

int8_t ec11ALast = 0;
int8_t ec11BLast = 0;
int8_t ec11Count = 0;

/****************************************
*-----------EC11初始化程序--------------*
****************************************/

void EC11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = EC11_GOIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	RCC_APB2PeriphClockCmd(EC11_GPIO_CLK, ENABLE);
	
	GPIO_Init(EC11_GPIO_PORT, &GPIO_InitStructure);
}

/****************************************
*----------EC11编码器扫描程序-----------*
*返回值		0	无动作
*			1	顺时针
*			-1	逆时针
*			2	按键
*			3	按住按键顺时针
*			-3	按住按键逆时针
****************************************/

int8_t EC11_Scan(void)
{
	static int8_t lastScanData;
	int8_t scanResult;
	
//	while (EC11A_NOW == ec11ALast);					//判断是否发生动作，使用时可注释掉
	if (EC11A_NOW == ec11ALast)
		return 0;
	
	if(EC11TYPE == 0)								//判断EC11类型
    {                       
        if(EC11A_NOW != ec11ALast) 
        {
            if(EC11A_NOW == 0)
            {
                if(EC11B_NOW ==1)                
                    scanResult = 1;   

                else                  
                    scanResult = -1;
            }
            ec11ALast = EC11A_NOW; 
            ec11BLast = EC11B_NOW; 
        }
    }   
                         
    else                 
    {                    
        if(EC11A_NOW != ec11ALast)        
        {                                   
                                            
            if(EC11A_NOW == 1)     
            {
                if((ec11BLast == 1) && (EC11B_NOW == 0))   
                    scanResult = -1;                         

                else if((ec11BLast == 0) && (EC11B_NOW == 1))    
                    scanResult = 1;                        

                
                else if((ec11BLast == EC11B_NOW) && (EC11B_NOW == 0))
                    scanResult = -1;                               

                else if((ec11BLast == EC11B_NOW) && (EC11B_NOW == 1))
                    scanResult = 1;                              
            }

            else                
            {
                if((ec11BLast == 1)&&(EC11B_NOW == 0))   
                    scanResult = 1;                        

                else if((ec11BLast == 0)&&(EC11B_NOW == 1))   
                    scanResult = -1;                         

                else if((ec11BLast == EC11B_NOW)&&(EC11B_NOW == 0))
                    scanResult = 1;                              

                else if((ec11BLast == EC11B_NOW)&&(EC11B_NOW == 1))
                    scanResult = -1;                               

            }               
            ec11ALast = EC11A_NOW;
            ec11BLast = EC11B_NOW;
        }
    }                                                                       

    if(EC11KEY_NOW == 0)								//判断按键是否被按下
    {
        if(scanResult == 0)      
            scanResult = 2;      
        else
        {
            if(scanResult == 1)  
                scanResult = 3;  
            if(scanResult == -1) 
                scanResult = -3; 
        }
    }
	
//	if (ec11Count < EC11SENSI)
//	{
//		ec11Count++;
//		scanResult = 0;
//	}
//	else
//	{
//		ec11Count = 0;
//	}
	if(scanResult != lastScanData)
	{
		lastScanData = scanResult;
		scanResult = 0;
	}
	else
	{
		lastScanData = 0;
	}
	
    return scanResult;
	
}
