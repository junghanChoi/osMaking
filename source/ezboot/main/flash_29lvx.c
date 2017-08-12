//------------------------------------------------------------------------------
// ȭ�ϸ�   : flash_29lvx.c
// ������Ʈ : ezboot_x5
// ��  ��   : ezboot�� FB29LVxflash ó�� ��ƾ�̴�. 
// 
// �ۼ���   : ����� (��)���̴���Ƽ freefrug@falinux.com --Ǫ��
// �ۼ���   : 2003�� 5�� 23��
//            2003�� 6�� 05��
//	      2004�� 8�� 14��   AMD,MXIC �÷��� �� 29LV Ÿ���� �����Ѵ�.
// ���۱�   : FA������
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
#include <time.h>
#include <flash_29lvx.h>
#include <mem_map.h>

// flash block
const int FB29LV400T_block_addr[] = 
		{ 0x00000, 0x10000, 0x20000, 0x30000, 
		  0x40000, 0x50000, 0x60000, 0x70000,
		  0x78000, 0x7A000, 0x7C000,
		  -1
		};

const int FB29LV400B_block_addr[] = 
		{ 0x00000, 0x04000, 0x06000, 0x08000, 
		  0x10000, 0x20000, 0x30000, 0x40000,
		  0x50000, 0x60000, 0x70000,
		  -1
		};
		
const int FB29LV800T_block_addr[] = 
		{ 0x00000, 0x10000, 0x20000, 0x30000, 
		  0x40000, 0x50000, 0x60000, 0x70000,
		  0x80000, 0x90000, 0xA0000, 0xB0000,
		  0xC0000, 0xD0000, 0xE0000, 0xF0000,
		  0xF8000, 0xFA000, 0xFC000,
		  -1
		};

const int FB29LV800B_block_addr[] = 
		{ 0x00000, 0x04000, 0x06000, 0x08000, 
		  0x10000, 0x20000, 0x30000, 0x40000,
		  0x50000, 0x60000, 0x70000, 0x80000,
		  0x90000, 0xA0000, 0xB0000, 0xC0000,
		  0xD0000, 0xE0000, 0xF0000,
		  -1
		};

const int FB29LV160T_block_addr[] = 
		{ 0x000000, 0x010000, 0x020000, 0x030000, 
		  0x040000, 0x050000, 0x060000, 0x070000,
		  0x080000, 0x090000, 0x0A0000, 0x0B0000,
		  0x0C0000, 0x0D0000, 0x0E0000, 0x0F0000,
		  0x100000, 0x110000, 0x120000, 0x130000, 
		  0x140000, 0x150000, 0x160000, 0x170000,
		  0x180000, 0x190000, 0x1A0000, 0x1B0000,
		  0x1C0000, 0x1D0000, 0x1E0000, 0x1F0000,
		  0x1F8000, 0x1FA000, 0x1FC000,
		  -1
		};

const int FB29LV160B_block_addr[] = 
		{ 0x000000, 0x004000, 0x006000, 0x008000,
		  0x010000, 0x020000, 0x030000, 0x040000,
		  0x050000, 0x060000, 0x070000, 0x080000,
		  0x090000, 0x0A0000, 0x0B0000, 0x0C0000,
		  0x0D0000, 0x0E0000, 0x0F0000, 0x100000,
		  0x110000, 0x120000, 0x130000, 0x140000, 
		  0x150000, 0x160000, 0x170000, 0x180000,
		  0x190000, 0x1A0000, 0x1B0000, 0x1C0000,
		  0x1D0000, 0x1E0000, 0x1F0000,
		  -1
		};		

//-----------------------------------------------------------------------------
// �÷��� ���� ����
//-----------------------------------------------------------------------------

int	Flash_PID   = 0;	// �÷��ø� �����Ѵ�.
int	BlockCount  = 0;
int	FlashSize   = 0;
int	*pBlockAddr = (int *)FB29LV400B_block_addr;


//-----------------------------------------------------------------------------
// 	const, macro 
//-----------------------------------------------------------------------------
#define	RETRY_COUNT			100
#define	WAIT_LOOP_COUNT		200


#define FB29LVx_WriteMemoryShort(adr,data)   	*(unsigned short *)(adr) = ( data & 0xffff ); FB29LVx_Wait();
#define FB29LVx_ReadMemoryShort(adr)         	( *(unsigned short *)(adr) & 0xffff );



//=============================================================================

//-----------------------------------------------------------------------------
//  �� �� : Wait
//  �� ȯ :
//-----------------------------------------------------------------------------
void	dumy( void )
{
	return;	
}
void	FB29LVx_Wait( void )
{
	int	loop = WAIT_LOOP_COUNT;
	
	while( loop-- ) { dumy(); };
}
//-----------------------------------------------------------------------------
//  �� �� : RESET ��Ű�� ���
//  �� ȯ :
//------------------------------------------------------------------------------
void 	FB29LVx_CMD_RESET( Word32 vBase )
{
   	FB29LVx_WriteMemoryShort( vBase, 0xF0 ); 	
}
//------------------------------------------------------------------------------
//  �� �� : ID �� �д� ���
//  �� ȯ :
//-----------------------------------------------------------------------------
void 	FB29LVx_CMD_READID_SETUP( Word32 vBase )
{
    	FB29LVx_WriteMemoryShort( vBase + (0x555<<1), 0xAA );
    	FB29LVx_WriteMemoryShort( vBase + (0x2AA<<1), 0x55 );
    	FB29LVx_WriteMemoryShort( vBase + (0x555<<1), 0x90 );
}
//------------------------------------------------------------------------------
//  �� �� : ����Ÿ�� �� �ִ´�.
//  �� ȯ :
//-----------------------------------------------------------------------------
void 	FB29LVx_CMD_WRITE( unsigned long vBase, unsigned long vOffset, Word16 vData)
{
    	FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xAA  ); 
    	FB29LVx_WriteMemoryShort( vBase+(0x2AA<<1), 0x55  ); 
    	FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xA0  ); 
    	FB29LVx_WriteMemoryShort( vBase+vOffset   , vData ); 
}
//------------------------------------------------------------------------
// �� �� : ��� ���� ������ ����� ��´�. 
// �� ȯ : 
//------------------------------------------------------------------------
int 	FB29LVx_GetStatus( unsigned long vBase )
{
    	unsigned char status, XorStatus;
	
    	while(1)
    	{
 		status     = FB29LVx_ReadMemoryShort( vBase );
 		XorStatus  = status ^ FB29LVx_ReadMemoryShort( vBase );
 		XorStatus &= ((1<<6)|(1<<2));	// Q6, Q2
	 	
 		// �ð��ʰ��� Ȯ���Ѵ�.
 		if ( status & (1<<5) ) 		// Q5
 		{
 			return FB29LVx_STATUS_TIMEOUT;  
 		}
 		
 		// suspend 
 		if ( status & (1<<7) ) 
 		{
 			return FB29LVx_STATUS_ERSUSP; 
 		}
 		
 		// ��ȭ�� ���ٸ� �������̴�. 
 		if ( XorStatus == 0 ) 
 		{
 			return FB29LVx_STATUS_READY; 
 		}
    	}

    	return FB29LVx_STATUS_ERROR;                         // ������ ����.  
}
//------------------------------------------------------------------------
//  �� �� : Flash�� ������ ��´�. 
//  �� ȯ : 
//------------------------------------------------------------------------
int 	FB29LVx_Detect( Word32 vBase )
{
    	unsigned short	mid, pid;
    	char     mid_str[32];

    	FB29LVx_CMD_RESET( vBase ); 
    	FB29LVx_CMD_READID_SETUP(vBase);  
    	
    	// ������ ȸ���ڵ�
    	mid = FB29LVx_ReadMemoryShort( vBase + 0      );
    	// �÷��� ��ǰ�ڵ�
    	pid = FB29LVx_ReadMemoryShort( vBase + (1<<1) );

	FB29LVx_CMD_RESET( vBase ); 

    	switch (mid)
    	{
    	case MXIC_MID : strcpy( mid_str, "MX" );	break;
    	case AMD_MID  : strcpy( mid_str, "AM" );	break;
		case ES_MID   : strcpy( mid_str, "ES" );	break;
    	default       : strcpy( mid_str, "xx" );	break;
    	//default : printf( "> Error Detect Code : %02x\n", mid );
    	//	  return -1; 
    	}
    	
    	switch (pid)
    	{
    	case FB_DID_400T:

			Flash_PID   = pid;	// �÷��ø� �����Ѵ�.
			BlockCount  = FB_400x_BLOCK_COUNT;
			FlashSize   = 512*1024;
			pBlockAddr  = (int *)FB29LV400T_block_addr;
		
    			printf( "  Detect %s29LV400 (TOP)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 4M-BIT [512Kbyte]\n");
			break;

		case FB_DID_400B:

			Flash_PID   = pid;	// �÷��ø� �����Ѵ�.
			BlockCount  = FB_400x_BLOCK_COUNT;
			FlashSize   = 512*1024;
			pBlockAddr  = (int *)FB29LV400B_block_addr;
		
	    		printf( "  Detect %s29LV400 (BOTTOM)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 4M-BIT [512Kbyte]\n");
			break;
			
		case FB_DID_800T:

			Flash_PID   = pid;	// �÷��ø� �����Ѵ�.
			BlockCount  = FB_800x_BLOCK_COUNT;
			FlashSize   = 1024*1024;
			pBlockAddr  = (int *)FB29LV800T_block_addr;
		
	    		printf( "  Detect %s29LV800T (TOP)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 8M-BIT [1Mbyte]\n");
			break;

		case FB_DID_800B:

			Flash_PID   = pid;	// �÷��ø� �����Ѵ�.
			BlockCount  = FB_800x_BLOCK_COUNT;
			FlashSize   = 1024*1024;
			pBlockAddr  = (int *)FB29LV800B_block_addr;
		
	    		printf( "  Detect %s29LV800T (BOTTOM)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 8M-BIT [1Mbyte]\n");
			break;

    	case FB_DID_160T:

			Flash_PID   = pid;	// �÷��ø� �����Ѵ�.
			BlockCount  = FB_160x_BLOCK_COUNT;
			FlashSize   = 2048*1024;
			pBlockAddr  = (int *)FB29LV160T_block_addr;
		
	    		printf( "  Detect %s29LV160T (TOP)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 16M-BIT [2Mbyte]\n");
			break;

    	case FB_DID_160B:

			Flash_PID   = pid;	// �÷��ø� �����Ѵ�.
			BlockCount  = FB_160x_BLOCK_COUNT;
			FlashSize   = 2048*1024;
			pBlockAddr  = (int *)FB29LV160B_block_addr;
		
	    		printf( "  Detect %s29LV160B (BOTTOM)Flash : %04X\n", mid_str, pid );
    			printf( "  SIZE 16M-BIT [2Mbyte]\n");
			break;    	

    	default:
    		printf( "  Unknown Flash : pid=%04X\n", pid );
		return -1; 
    	} 

	return 0;
}
//------------------------------------------------------------------------------
//  �� �� : ���� �����.
//  �� ȯ : ���� 0
//  �� �� : �ݵ�� ���� �ּҸ� �ش�
//-----------------------------------------------------------------------------
int 	FB29LVx_EraseBlock( unsigned long vBase, unsigned long vBlkAddr )
{
   	Word16	status;
      	int	loop;

      	for (loop=0; loop<RETRY_COUNT; loop++)
      	{
       		FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xAA ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x2AA<<1), 0x55 ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0x80 ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x555<<1), 0xAA ); 
       		FB29LVx_WriteMemoryShort( vBase+(0x2AA<<1), 0x55 ); 
       	
       		FB29LVx_WriteMemoryShort( vBase+vBlkAddr, 0x30 );   

		msleep(100);
       	
   		status = FB29LVx_GetStatus( vBase );
   		if ( status == FB29LVx_STATUS_READY )
   		{
   			FB29LVx_CMD_RESET(vBase); 
   			return 0;  
   		}
   		else
   		{
   			FB29LVx_CMD_RESET(vBase); 	
   		}
       	}
       	
       	printf( "Erase Error [retry count]...\n" ); 		
       	return -1;
}
//------------------------------------------------------------------------
//  �� �� : Flash�� ����Ÿ�� ����.
//  �� ȯ : ���� 0
//------------------------------------------------------------------------
int 	FB29LVx_WriteWord( Word32 vBase, Word32 vOffset, Word16 data )
{
      	Word16 	status;
      	int	loop;
      	
      	for (loop=0; loop<RETRY_COUNT; loop++)
      	{
      		FB29LVx_CMD_WRITE( vBase, vOffset, data );  // ����Ÿ�� �� �ִ´�.
      
   		status = FB29LVx_GetStatus( vBase );
   		if ( status == FB29LVx_STATUS_READY )
   		{
   			FB29LVx_Wait();
   			return 0;  
   		}
   		else
   		{
   			FB29LVx_CMD_RESET(vBase); 	
   		}
	}
	printf( "Write Error [retry count]...\n" ); 
	return -1;	
}
//------------------------------------------------------------------------------
//  �� �� : offset �ּҿ��� ����ȣ�� ã�´�.
//  �� ȯ : ����ȣ
//-----------------------------------------------------------------------------
int	FB29LVx_GetBlockIndex( unsigned long vOffset )
{
	int	blk;
	
	// �÷��� ũ����� �ּҸ� �Ÿ���.
	if ( FlashSize <= vOffset ) return -1;
	
	// ����ȣ�� ã�´�.
	for (blk=0; blk<BlockCount; blk++)
	{
		if ( 0 > pBlockAddr[blk] )
		{
			return -1; // �������� �ʴ� �ּ��̴�.
		}
		
		if ( 0 < pBlockAddr[blk+1] )
		{
			if ( ( pBlockAddr[blk] <= vOffset ) &&  ( vOffset < pBlockAddr[blk+1] ) )
			{
				break; // ����ȣ�� ã�Ҵ�.
			}
		}
		else
		{
			break;	// ������ ��
		}
	}
	
	return blk;
}


//------------------------------------------------------------------------------
// ���� : ������¸� ǥ�� �Ѵ�. 
// �Ű� : title : �������� 
//        cur   : ���� ���൵ 
//        max   : �ִ밪
// ��ȯ : 
// ���� : ���� 
//------------------------------------------------------------------------------
static int dpywidth  = 50;
static char bar[]    = "===============================================================";
static char spaces[] = "                                                               "
		       "                                                               ";
static int progress_pos = 0;

void Flash_UpdateProgress(const char *title, unsigned long cur, unsigned long max)
{
	const char spinner[] = "\\|/-";
	unsigned int percent;
	int i;

	progress_pos = (progress_pos+1) & 3;
	percent = (cur * 100) / max;
	i = ((percent * dpywidth) + 50) / 100;

	printf("%s: |%s%s", title,
		bar + (sizeof(bar) - (i+1)),
		spaces + (sizeof(spaces) - (dpywidth - i + 1)));
		
	if (percent == 100)
	     putc('|');
	else putc(spinner[progress_pos & 3]);
		
	printf(" %4d%%   \r", percent);
	if (percent == 100)	// clear prog bar 
	{
		printf("%s\r", spaces + (sizeof(spaces) - 80));
	}
		
}
//------------------------------------------------------------------------------
// �� �� : �÷��ø� ã�´�.
// �� �� : ���� 
//------------------------------------------------------------------------------
int	BootFlash_Init( void )
{
	printf( "Boot Flash Check ......................\n" );
	return FB29LVx_Detect( (Word32)0x00000000 );
}
//------------------------------------------------------------------------------
// ���� : Flash�� ����Ÿ�� �� �ִ´�. 
// �Ű� : dest : �÷��� ���� ���� �ּ� 
//        src  : �� �б� ���� �ּ�  
//        size : ����� ũ�� 
// ��ȯ : 
// ���� : ���� 
//------------------------------------------------------------------------------
int 	BootFlash_ProgramEx( Word32  vBase, Word32  vOffset, char *src, int size, int with_erase )
{
	int	s_blk, e_blk;
	int	idx;
	Word16	wData;

	// ���� ���� ���۰� ���� ã�´�.
	s_blk = FB29LVx_GetBlockIndex( vOffset );
	e_blk = FB29LVx_GetBlockIndex( vOffset + size );

	if ( ( s_blk < 0 ) || ( e_blk < 0 ) ) 
	{
		printf( "Invalid Address... 0x%08x\n", vBase+vOffset );
		return -1;
	}

	if( with_erase )
	{	
		// ����� ������ ���� �����. 
		for ( idx=s_blk; idx<=e_blk; idx++ )
		{
			Flash_UpdateProgress( "    Erasing", idx-s_blk+1, e_blk-s_blk+1 );
		
			if ( 0 > FB29LVx_EraseBlock( vBase, pBlockAddr[idx] ) ) return -1;
			msleep(1000);
		}
		Flash_UpdateProgress( "    Erasing", 100, 100 );
		printf( "    Erase : OK\n" );
	}
	
	for ( idx=0; idx<size; idx+=2 )
	{
		if ( 0 == idx % 512 )
			Flash_UpdateProgress("    Writing", idx, size );
		
		wData = (src[idx]&0xff) | ( (src[idx+1]&0xff) << 8 );
		if ( 0 > FB29LVx_WriteWord( vBase, vOffset+idx, wData ) ) return -1;
		
		if ( *(Word16 *)(vBase+vOffset+idx) != wData )
		{
			if ( 0 > FB29LVx_WriteWord( vBase, vOffset+idx, wData ) ) return -1;
		}
	
	}
	Flash_UpdateProgress( "    Writing", 100, 100 );
	printf( "    Write : OK\n" );

	FB29LVx_CMD_RESET( vBase );
	
	return size;
}
//------------------------------------------------------------------------------
// ���� : Flash�� ����Ÿ�� �� �ִ´�.
// �Ű� : dest : �÷��� ���� ���� �ּ� 
//        src  : �� �б� ���� �ּ�
//        size : ����� ũ��
// ��ȯ : 
// ���� : ����
//------------------------------------------------------------------------------
int 	CopyTo_BootFlashEx( unsigned int dest, unsigned int src, int size, int with_erase )
{
	int	idx;
	unsigned short 	*pdst = (unsigned short *)dest;
	unsigned short 	*psrc = (unsigned short *)src;

	if( with_erase )	BootFlash_Program( 0, dest, (char *)src, size );
	else				BootFlash_ProgramEx( 0, dest, (char *)src, size, 0 );
	
	for(idx=0; idx<size/2; idx++)
	{
		if ( *pdst != *psrc )
  		{
  			printf( "Verify Error %p: %04x/%04x\n", pdst, *pdst, *psrc );
  			return -1;
  		}
  		pdst++; psrc++;
  	}
  		
	return 0;
}
//------------------------------------------------------------------------------
// ���� : 
// ���� : ����
//------------------------------------------------------------------------------
void	SelfCopy_BootLoader( void )
{
	CopyTo_BootFlash( DEFAULT_FLASH_BOOT, DEFAULT_RAM_BOOT_START, DEFAULT_BOOT_SIZE );
}







