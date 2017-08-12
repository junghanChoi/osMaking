//------------------------------------------------------------------------------
// ȭ�ϸ� : ram_cmd.c
// ��  �� : ezBoot�� �޸𸮿� ���� ��� ó�� ��ƾ�̴�. 
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
//          ����� (��)���̴���Ƽ freefrug@falinux.com --Ǫ��
// �ۼ��� : 2001�� 10�� 8��
// ��  �� : 2003�� 6�� 05��
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
//------------------------------------------------------------------------------

//******************************************************************************
//
// ��� ����
//
//******************************************************************************
#include <pxa255.h>
#include <stdio.h>
#include <string.h>
#include <zmodem.h>

#include <mem_map.h>
#include <config.h>
#include <flash.h>

//******************************************************************************
//
// ���� ���� ����
//
//******************************************************************************
// config.c �� ����
extern TConfig Cfg;

//******************************************************************************
//
// �Լ� ����
//
//******************************************************************************


//------------------------------------------------------------------------------
// ���� : ZMODEM���� ����Ÿ �ޱ� 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     ZModem_Memory(int argc, char **argv)
{
        unsigned long  To;
        int            size;

        if( argc != 2 ) 
        {
                printf( "Argument Count Error!\n");
                return -1; 
        }

        // ZModem���� ����Ÿ�� �޴´�.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );	
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
        }
        else
        {
                printf( "ZModem Resive Size %d\n", size );
        }


        // ����� �ּҸ� ��´�. 
        // �ּҿ� ���� ������ ���� �ʾҴ�.
        To = (unsigned long)strtoul( argv[1], NULL, 0);

        if ( 'F' == argv[0][1] )
        {
                printf( "���� ���� �ʾҴ�\n" );
                // To &= 0x0fffffff;
                // �÷��� ������ ����. 
		// Flash_WriteFull( To, DEFAULT_RAM_WORK_START, size );
        }
        else
        {
                memcpy( (void *)To, (void *)DEFAULT_RAM_WORK_START, size );
        }

        return 0;
}
//------------------------------------------------------------------------------
// ���� : ZMODEM���� ����Ÿ�� ������ ��Ʈ�� �� �ֱ� 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     ZModem_FlashBoot(int argc, char **argv)
{
        int     size;

        // ZModem���� ����Ÿ�� �޴´�.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
                return -1; 
        }

        printf( "ZModem Resive Size %d\n", size );

	    size = (size+1) & (DEFAULT_BOOT_SIZE-2);
       	// ��Ʈ ������ �������� ����. 
       	CopyTo_BootFlash( DEFAULT_FLASH_BOOT, DEFAULT_RAM_WORK_START, size );

        return 0;
}
//------------------------------------------------------------------------------
// ���� : ZMODEM���� ����Ÿ�� Kernel ������ �ִ´�. 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int ZModem_Kernel(int argc, char **argv)
{
        int     size;

        // ZModem���� ����Ÿ�� �޴´�.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );  //DEFAULT_RAM_KERNEL_START
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
                return -1; 
        }
        printf( "ZModem Resive Size %d\n", size );

        if ( 'F' == argv[0][1] ) // �÷��� ������ ����. 
        {
		CopyTo_NandFlash_Kernel( DEFAULT_RAM_WORK_START, size );
        }
        else
        {
                memcpy( (void *)(DEFAULT_RAM_KERNEL_START), (void *)DEFAULT_RAM_WORK_START, size );
        }

        return 0;
}   
//------------------------------------------------------------------------------
// ���� : ZMODEM���� ����Ÿ�� Ram Disk ������ �ִ´�. 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int ZModem_RamDisk(int argc, char **argv)
{
        int     size;

        // ZModem���� ����Ÿ�� �޴´�.
        size = rz( "test.txt",  DEFAULT_RAM_WORK_START );  //DEFAULT_RAM_RAMDISK_START
        if( size < 0 ) 
        {
                printf( "ZModem Resive Error!\n" );
                return -1; 
        }
        printf( "ZModem Resive Size %d\n", size );

        // ȯ�溯���� ũ�⸦ ���´�.
        if ( 'F' == argv[0][1] )
        {
                // �÷��� ������ ����. 
                CopyTo_NandFlash_Ramdisk( DEFAULT_RAM_WORK_START, size );
        }
        else
        {
                memcpy( (void *)(DEFAULT_RAM_RAMDISK_START), (void *)DEFAULT_RAM_WORK_START, size );
        }

        return 0;
}   

