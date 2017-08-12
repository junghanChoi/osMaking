//------------------------------------------------------------------------------
// �� �� �� : config.h
// ������Ʈ : ezboot
// ��    �� : ezboot���� ����ϴ� ȯ�� ������ ���õ� ����
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
//          ����� (��)���̴��Ƽ freefrug@falinux.com  -- Ǫ��
// �ۼ��� : 2002�� 1�� 19��
// ��  �� : 2003-06-07	
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
//------------------------------------------------------------------------------

#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#define CONFIG_MAGIC                    0x89880017
#define CONFIG_CMD_MAGIC                0x20030702
#define DEFAULT_BOOT_WAIT               3
#define DEFAULT_ENTRY_KEY               ' '
#define DEFAULT_SERIAL                  2  

#define DEFAULT_LOCAL_MAC				{0x00, 0xA2, 0x55, 0xF2, 0x26, 0x25}
#define DEFAULT_LOCAL_IP				(192<<0)|(168<<8)|(10<<16)|(155<<24)
#define DEFAULT_HOST_IP                 (192<<0)|(168<<8)|(10<<16)|(24<<24)

#define DEFAULT_TFTP_DIR				""
#define DEFAULT_KERNEL_FILENAME			"zImage.x5"
#define DEFAULT_RAMDISK_FILENAME		"ramdisk.x5-12M.gz"
#define DEFAULT_EZBOOT_FILENAME			"ezboot.x5"

#define CMD_ARCH_NAME                   "EZ-X5"

#define	DEFAULT_NAND_PAR_KERNEL_SZ		1
#define	DEFAULT_NAND_PAR_RAMDISK_SZ		5
#define	DEFAULT_NAND_PAR_APP_SZ			58

#define DEFAULT_KERNEL_COMMAND_A		"initrd=0xa0800000,5M root=/dev/ram ramdisk=12288"
#define DEFAULT_KERNEL_COMMAND_B  		"console=ttyS02,115200"
#define DEFAULT_KERNEL_COMMAND_C  		""

#define	KCMD_BUFF_MAX					80
#define	KCMD_COUNT						3

typedef struct 
{
	Word32	CMD_MagicNumber;        // Ŀ�� Ŀ�ǵ� ������ȣ 
	char    CMD_Tag[8]; 		// Ŀ�� ��� ���� ����Ʈ ""
	char    cmd_A[KCMD_BUFF_MAX];
	char    cmd_B[KCMD_BUFF_MAX];
	char    cmd_C[KCMD_BUFF_MAX];	
} __attribute__ ((packed)) TKernelCommandLine;

/*
typedef struct 
{
	Word32	MagicNumber;        	// ������ȣ 
	char    Buff[512]; 		// �Ķ���� ���ڿ�
        
} __attribute__ ((packed)) TParam;
*/

typedef struct SConfig
{
	Word32	MagicNumber;           	// ȯ�� ����Ÿ�� ������ ������ ���� ���� ��ȣ 0x8988000x
    
	Word32	AutoBootWaitTime;      	// �ڵ� ��Ʈ ��� �ð� ( �� ���� )
	
	char    BootMenuKey;    	// ��Ʈ �δ� ���� Ű �⺻ ' '
	char	UseRamdisk;		// ����ũ �������
	char	Watchdog;		// ��ġ��
	char    Rev1;               	// ���� ����     

	Byte    Local_MacAddress[8];   	// ����   MAC Address  [xx:xx:xx:xx:xx:xx] 
	Byte    Host_MacAddress[8];    	// ȣ��Ʈ MAC Address  [xx:xx:xx:xx:xx:xx] 
                               	    	// 6 ����Ʈ�� ����Ѵ�.
	Word32  Local_IP;              	// ����   IP  = 0
	Word32  Host_IP;               	// ȣ��Ʈ IP  = 0

	char    TFTP_Directory      [64]; // TFTP ���丮 ��         ����Ʈ ""
	char    TFTP_zImageFileName [64]; // Ŀ�� �̹���      ȭ�� �� ����Ʈ "zImage" 
	char    TFTP_RamDiskFileName[64]; // �� ��ũ �̹��� ȭ�� �� ����Ʈ "ramdisk.gz" 
	char    TFTP_LoaderFileName [64]; // ��Ʈ�δ�         ȭ�� �� ����Ʈ "ezboot_x5" 

	Word32	SeriaNumber; 		// ����� �ø��� ��Ʈ ��ȣ  
	Word32  Kernel_ArchNumber;	// Ŀ�ο� ���޵� Ŀ�� ��Ű���� ��ȣ  ����Ʈ 303

	Word16	NandPar[4];		// NAND-Partition ũ��

        TKernelCommandLine KCmd; 	// Ŀ�� Ŀ�ǵ� 

} __attribute__ ((packed))TConfig;

#ifndef CONFIG_VAR
extern TConfig Cfg;
#endif


extern void   LoadConfig( void );
extern void   SaveConfig( void );
extern int    ModifyCfg(int argc, char **argv);
extern int    GetKCmdStr( char *dest );

#endif // _CONFIG_HEADER_



