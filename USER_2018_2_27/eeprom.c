
#include "eeprom.h"


#define	PAGE_ADDR	(0x08000000 + 511 * 1024)

/***************************************************************************************************
* 函数名称: MemReadByte()
* 函数功能: 从存储器中读出 num 字节数据
* 入口参数: *dat: 读出数据的保存地址
*           num : 读出的字节数
* 出口参数: 0：操作失败；1：操作成功
***************************************************************************************************/
unsigned char MemReadByte(unsigned short *data,unsigned short num)                                
{
        unsigned short *temp_addr = (unsigned short *)PAGE_ADDR;
        
        while(num --)
        {
                *data ++ = *temp_addr ++;
        }
                                                                                                        
        return 1;                                                                                                        
}

/***************************************************************************************************
* 函数名称: MemWriteByte()
* 函数功能: 向存储器中写入 num 字节数据
* 入口参数: *dat: 待写入数据
*           num: 写入的字节数
* 出口参数: 0：操作失败；1：操作成功
***************************************************************************************************/
unsigned char MemWriteByte(unsigned short *data,unsigned short num)                                
{
        FLASH_Status temp_stat;
        unsigned int temp_addr = PAGE_ADDR;
                
        FLASH_Unlock();                                                // Flash 解锁，允许操作相关的寄存器
        temp_stat = FLASH_ErasePage(PAGE_ADDR);                        // 擦出制定的页
        
        if(temp_stat != FLASH_COMPLETE)
        {
                FLASH_Lock();
                return 0;
        }
        
        while(num --)
        {
                temp_stat = FLASH_ProgramHalfWord(temp_addr,*data);
                if(temp_stat != FLASH_COMPLETE)
                {
                        FLASH_Lock();
                        return 0;
                }
        
                temp_addr += 2;
                data++;
        }
        
        FLASH_Lock();        
        return 1;
}


