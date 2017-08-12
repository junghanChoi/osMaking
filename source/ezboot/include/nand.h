//------------------------------------------------------------------------------
// �� �� �� : nand.h
// ������Ʈ : ezboot
// ��    �� : ezboot���� ����ϴ� nand�� ���õ� ��� 
// �� �� �� : ����â
// �� �� �� : 2003��  5�� 23��
// ��    �� : 2003�� 6�� 9��
//            ����� (��)���̴��Ƽ freefrug@falinux.com  --Ǫ��
// �� �� �� : (��)���̴��Ƽ 
// ��    �� : 
//------------------------------------------------------------------------------

#ifndef _NAND_HEADER_
#define _NAND_HEADER_

#define 	NAND_BASE	DEFAULT_nCS_NAND_BASE

#define NAND_ACCESS_START       *((volatile short *)(NandDev->BaseAddress + 0x000 ))
#define NAND_DATA               *((volatile short *)(NandDev->BaseAddress + 0x000 ))
#define NAND_CMD                *((volatile short *)(NandDev->BaseAddress + 0x100 ))  
#define NAND_ADDR               *((volatile short *)(NandDev->BaseAddress + 0x200 ))  
#define NAND_ACCESS_END         *((volatile short *)(NandDev->BaseAddress + 0x300 ))  

#define	PARTITION_NUM_KERNEL	0
#define	PARTITION_NUM_RAMDISK	1
#define	PARTITION_NUM_APPIMAGE	2


// ����Ƽ���� 0 ���������� ������������ ����Ѵ�.
typedef struct 
{
	int	data_size;
} __attribute__ ((packed)) TParPageInfo;

// ��Ƽ���� ����
typedef struct 
{
    unsigned char  Name[128];        
    int            BaseBlock;         // ���� ���� ��� 
    int            BlockCount;        // ���� ��� �� 
} __attribute__ ((packed)) TNandPartition;

#define NAND_MAX_PATITION           8

#define NAND_PARTITON_REST_SIZE    -1 
#define NAND_PARTITON_END          -1 

typedef struct 
{
    unsigned long  Type;        
    unsigned long  BaseAddress;         // ��ġ �ּ� 
    unsigned long  TotalSize;           // �� ��� ���� �뷮
    unsigned long  EraseSize;           // ���� ���� ���

    char           BadBlock[1024*8];    // ��� ��� ǥ�� 'X' : ����� / 'O' : ����
    unsigned short VirBlock[1024*8];    //
    int            BadBlockCount;

    TNandPartition *pPartition;         // ��Ƽ�� ���� �ּ� 
    int            PartitionNR;         // ��Ƽ�� ����  
} __attribute__ ((packed)) TNandInfo;


#define NAND_CMD_READ_ID           0x90             // ID �б� Ŀ�ǵ� 
#define NAND_CMD_READ_STATE        0x70             // ���� �б� Ŀ�ǵ� 
#define NAND_CMD_ERASE_SETUP       0x60             // ����� �¾� Ŀ�ǵ�
#define NAND_CMD_ERASE_RUN         0xD0             // ����� ���� Ŀ�ǵ�
#define NAND_CMD_WRITE_START       0x80             // ���� ���� Ŀ�ǵ�
#define NAND_CMD_WRITE_RUN         0x10             // ���� ���� Ŀ�ǵ�
#define NAND_CMD_READ_A            0x00             // A ���� �б� 
#define NAND_CMD_READ_B            0x01             // B ���� �б� 
#define NAND_CMD_READ_C            0x50             // C ���� �б� 

#define NAND_STATUS_ERASE_OK       0x01             // bit = 0 �ϰ�� OK 
#define NAND_STATUS_PROGRAM_OK     0x01             // bit = 0 �ϰ�� OK 
#define NAND_STATUS_BUSY           0x40             // bit = 0 �ϰ�� BUSY 
#define NAND_STATUS_PROTECTED      0x80             // bit = 0 �ϰ�� LOCK 

#define NAND_STATUS_NORMAL_MASK    ( NAND_STATUS_ERASE_OK || NAND_STATUS_PROGRAM_OK || NAND_STATUS_BUSY || NAND_STATUS_PROTECTED )

#define NAND_TYPE_UNKNOW             0
#define NAND_TYPE_SAMSUNG_16M      0x73
#define NAND_TYPE_SAMSUNG_32M      0x75
#define NAND_TYPE_SAMSUNG_64M      0x76
#define NAND_TYPE_SAMSUNG_128M     0x79

#ifndef NAND_VAR
  extern TNandInfo Nand1;
  extern TNandPartition  Nand1Partition[NAND_MAX_PATITION];
#endif


extern int  NandFlash_Init( void );
extern BOOL Nand_ErasePartition( int pnb, BOOL NoEraseBadBlock );
extern int  CopyTo_NandFlash_Kernel ( unsigned int src, int size );
extern int  CopyTo_NandFlash_Ramdisk( unsigned int src, int size );
extern int  CopyTo_NandFlash_AppImage( unsigned int src, int size );
extern int  CopyTo_SDRAM_Kernel( unsigned int dst );
extern int  CopyTo_SDRAM_Ramdisk( unsigned int dst );
#endif //_NAND_HEADER_

