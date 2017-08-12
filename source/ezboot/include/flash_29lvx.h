//------------------------------------------------------------------------------
// ȭ�ϸ�   : flash_29lvx.h
// ������Ʈ : ezboot_x5
// ��  ��   : MX29LVxxxxflash ó�� ��ƾ�̴�. 
// 
// �ۼ���   : ����� (��)���̴���Ƽ freefrug@falinux.com   -- Ǫ��
// �ۼ���   : 2003�� 5�� 17��
// ��  ��   : 2003�� 6�� 07��
//
// ���۱�   : FA������
//
//------------------------------------------------------------------------------

#ifndef _MX29LVx_FLASH_HEADER_
#define _MX29LVx_FLASH_HEADER_

#define	MXIC_MID		0xC2
#define	AMD_MID			0x01
#define	ES_MID			0x4A

#define	FB_DID_400T		0x22B9
#define	FB_DID_400B		0x22BA
#define	FB_400x_BLOCK_COUNT	11
        
#define	FB_DID_800T		0x22DA
#define	FB_DID_800B		0x225B
#define	FB_800x_BLOCK_COUNT	19

#define	FB_DID_160T		0x22C4
#define	FB_DID_160B		0x2249
#define	FB_160x_BLOCK_COUNT	35

#define FB29LVx_STATUS_READY    0       // ready for action 
#define FB29LVx_STATUS_BUSY     1       // operation in progress 
#define FB29LVx_STATUS_ERSUSP   2       // erase suspended 
#define FB29LVx_STATUS_TIMEOUT  3       // operation timed out 
#define FB29LVx_STATUS_ERROR    4       // unclassified but unhappy status 

#define BootFlash_Program( a, b, c, d )  	BootFlash_ProgramEx( a, b, c, d, 1 )
#define CopyTo_BootFlash( a, b, c )			CopyTo_BootFlashEx( a, b, c, 1 )

extern	int BootFlash_Init( void );
extern void SelfCopy_BootLoader( void );

            
#endif //_MX29LVx_FLASH_HEADER_

