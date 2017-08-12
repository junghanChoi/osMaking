//------------------------------------------------------------------------------
// ȭ�ϸ�   : flash_cmd.c
// ������Ʈ : ezboot_x5
// ��  ��   : NOR, NABD �÷����� ���� ó�� ��ƾ�̴�.
//            ������ƾ�� ȣ���Ͽ� ����� ���ð� ��Ī�Ѵ�.
// 
// �ۼ���   : ����� (��)���̴���Ƽ freefrug@falinux.com   -- Ǫ��
// �ۼ���   : 2003�� 6�� 7��
//
// ���۱�   : (��)���̴���Ƽ 
//
//------------------------------------------------------------------------------

//******************************************************************************
//
// ��� ����
//
//******************************************************************************
#include <pxa255.h>
#include <stdio.h>
#include <string.h>
#include <config.h>
#include <mem_map.h>
#include <flash.h>


/* ���� ���� ���� -------------------------------------------------------------*/

extern TConfig Cfg;		// config.c �� ����

//------------------------------------------------------------------------------
// ���� : �̹����� �����Ѵ�.
// ���� : 
//------------------------------------------------------------------------------
void    CopyImage( void )
{
        printf("Copy Kernel Image .....\n" );
	CopyTo_SDRAM_Kernel ( DEFAULT_RAM_KERNEL_START  );

	if ( ( 'Y' == Cfg.UseRamdisk ) || ( 'y' == Cfg.UseRamdisk ) )
	{
        	printf("Copy Ramdisk Image .....\n" );
		CopyTo_SDRAM_Ramdisk( DEFAULT_RAM_RAMDISK_START );
	}
}

//------------------------------------------------------------------------------
// ���� : ��Ƽ���� �����.
// ���� : 
//------------------------------------------------------------------------------
int    ErasePartiotion( int argc, char **argv)
{
	int	ee = TRUE;
	
	if ( ( argc >= 2 ) && ( toupper(argv[1][0]) == 'E')  ) ee = FALSE;
	
	switch ( argv[0][2] )
	{
	case 'K' : Nand_ErasePartition( PARTITION_NUM_KERNEL ,	ee    ); return 0;
	case 'R' : Nand_ErasePartition( PARTITION_NUM_RAMDISK,  ee    ); return 0;
	case 'A' : Nand_ErasePartition( PARTITION_NUM_APPIMAGE, FALSE ); return 0;
	}
	
	return -1;
}



