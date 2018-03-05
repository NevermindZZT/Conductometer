/*********************************************
*             EC11����������                 *
*         ���ϴ�ѧ���������ѧԺ             *
*                �ſ�ǿ                      *
*                2018/1                      *
*********************************************/

#include "ec11.h"

int8_t ec11ALast = 0;
int8_t ec11BLast = 0;
int8_t ec11Count = 0;

/****************************************
*-----------EC11��ʼ������--------------*
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
*----------EC11������ɨ�����-----------*
*����ֵ		0	�޶���
*			1	˳ʱ��
*			-1	��ʱ��
*			2	����
*			3	��ס����˳ʱ��
*			-3	��ס������ʱ��
****************************************/

int8_t EC11_Scan(void)
{
	static int8_t lastScanData;
	int8_t scanResult;
	
//	while (EC11A_NOW == ec11ALast);					//�ж��Ƿ���������ʹ��ʱ��ע�͵�
	if (EC11A_NOW == ec11ALast)
		return 0;
	
	if(EC11TYPE == 0)								//�ж�EC11����
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

    if(EC11KEY_NOW == 0)								//�жϰ����Ƿ񱻰���
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
