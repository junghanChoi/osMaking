//------------------------------------------------------------------------------
// ȭ�ϸ� : info_cmd.c
// ��  �� : ������ ���¸� ǥ���Ѵ�. 
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
// �ۼ��� : 2003�� 05�� 20��
//          
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
//------------------------------------------------------------------------------

//******************************************************************************
//
// ��� ����
//
//******************************************************************************
#include <pxa255.h>
#include <config.h>
#include <serial.h>
#include <time.h>
#include <stdio.h>
#include <gpio.h>


#define	EZ_X5_RAM_BOOT_END	0xA0F20000      // ��Ʈ ������ ��

//******************************************************************************
//
// ���� ���� ����
//
//******************************************************************************

//******************************************************************************
//
// �Լ� ����
//
//******************************************************************************

//------------------------------------------------------------------------------
// ���� : �ý��� ���¸� ǥ���Ѵ�. 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Info_cmd(int argc, char **argv)
{
       // �޸� ȯ�� ���¸� ǥ���Ѵ�. 
       printf( "-[MEMORY INFO]-------------------------------------------------------------\n" );
       printf( " FLASH ROM START        : 0x%08X\n",  DEFAULT_FLASH_BOOT           );
       printf( " BOOT START             : 0x%08X\n",  DEFAULT_FLASH_BOOT           );
       printf( " BOOT PARAM             : 0x%08X  - 0x%08X \n",  
                                                      DEFAULT_FLASH_BOOT
                                                    + DEFAULT_BOOT_SIZE,
                                                      DEFAULT_FLASH_BOOT
                                                    + DEFAULT_BOOT_SIZE
                                                    + DEFAULT_BOOT_PARAM_SIZE
                                                                )  
       printf( "\n" );
       printf( " CS8900 BASE            : 0x%08X\n",  DEFAULT_nCS_CS8900_BASE      );
       printf( " NAND   BASE            : 0x%08X\n",  DEFAULT_nCS_NAND_BASE        );
       printf( " MK712  BASE            : 0x%08X\n",  DEFAULT_nCS_MK712_BASE       );
       printf( "\n" );
       printf( " SDRAM START            : 0x%08X\n" , BANK0_START                  ); 
       printf( " KERNEL ZERO PAGE       : 0x%08X\n",  DEFAULT_RAM_KERNEL_ZERO_PAGE );
       printf( " KERNEL START           : 0x%08X\n",  DEFAULT_RAM_KERNEL_START     );
       printf( " RAMDISK START          : 0x%08X\n",  DEFAULT_RAM_RAMDISK_START    );
       printf( " BOOTLOADER START       : 0x%08X\n",  DEFAULT_RAM_BOOT_START       );
       printf( " STACK                  : 0x%08X\n",  EZ_X5_RAM_BOOT_END           );
       printf( " WORKING AREA START     : 0x%08X\n",  DEFAULT_RAM_WORK_START       );
       printf( "\n" );
       printf( "-[MEMORY INFO]-------------------------------------------------------------\n" );
       printf( " BOOT FLASH SIZE        : 512 K byte\n" );
       printf( " SDRAM SIZE             :  64 M Byte\n" );
       printf( " NAND FLASH SIZE        :  64 M Byte\n" );
       printf( " KERNEL  SIZE           : 0x%08X\n",                     );
       printf( " RAMDISK SIZE           : 0x%08X\n",                     );
       printf( "---------------------------------------------------------------------------\n" );
       
        
       return 0;
}

