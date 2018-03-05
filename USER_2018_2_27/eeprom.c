
#include "eeprom.h"


#define	PAGE_ADDR	(0x08000000 + 511 * 1024)

/***************************************************************************************************
* ��������: MemReadByte()
* ��������: �Ӵ洢���ж��� num �ֽ�����
* ��ڲ���: *dat: �������ݵı����ַ
*           num : �������ֽ���
* ���ڲ���: 0������ʧ�ܣ�1�������ɹ�
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
* ��������: MemWriteByte()
* ��������: ��洢����д�� num �ֽ�����
* ��ڲ���: *dat: ��д������
*           num: д����ֽ���
* ���ڲ���: 0������ʧ�ܣ�1�������ɹ�
***************************************************************************************************/
unsigned char MemWriteByte(unsigned short *data,unsigned short num)                                
{
        FLASH_Status temp_stat;
        unsigned int temp_addr = PAGE_ADDR;
                
        FLASH_Unlock();                                                // Flash ���������������صļĴ���
        temp_stat = FLASH_ErasePage(PAGE_ADDR);                        // �����ƶ���ҳ
        
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


