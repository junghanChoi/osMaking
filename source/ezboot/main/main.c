//------------------------------------------------------------------------------
// ȭ�ϸ� : main.c
// ��  �� : ezBoot�� �������� ���� ��ƾ�̴�. 
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
//          ����� (��)���̴��Ƽ freefrug@falinux.com -- Ǫ��
// �ۼ��� : 2001�� 10�� 08��
// ������ : 2003�� 06�� 07�� 	ez_x5 ������ �����Ǿ���.
//          2003�� 08�� 01�� 	eztiny �� �߰�    			        -- Ǫ��
//	    2003�� 08�� 02��	nand.c Ÿ�̹� ����, ���ӵ� bad-block ���� ����  -- Ǫ��
//	    2004�� 08�� 14��	AM29LV400BT �÷��� �߰�  			-- Ǫ��	
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
//------------------------------------------------------------------------------

#include <pxa255.h>
#include <serial.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

#include <mem_map.h>
#include <config.h>

#include <cs8900.h>
#include <flash.h>
#include <main.h>

/* �ܺ��Լ� ���� --------------------------------------------------------------*/

extern int RamDump      (int argc, char **argv);      // �޸� ����Ÿ ���� 
extern int RamWriteChar (int argc, char **argv);      // �޸𸮿� ����Ÿ ���� ( 1 Byte )
extern int RamWriteWord (int argc, char **argv);      // �޸𸮿� ����Ÿ ���� ( 2 Byte ) 
extern int RamWriteLong (int argc, char **argv);      // �޸𸮿� ����Ÿ ���� ( 3 Byte ) 

extern int Flash_Memory (int argc, char **argv);      // Flash�� �޸� ����Ÿ�� ����. 

extern int ZModem_Memory(int argc, char **argv);      // ZMODEM ���������� �̿��� ȭ�� �ٿ�ε� ( �޸𸮿� ����. )
extern int ZModem_FlashBoot(int argc, char **argv);   // ZMODEM ���������� �̿��� ȭ�� �ٿ�ε����� �÷��� ��Ʈ�� ����.
extern int ZModem_Kernel(int argc, char **argv); 
extern int ZModem_RamDisk(int argc, char **argv); 
extern unsigned ZModem_Baudrate;

extern int Arp(int argc, char **argv);                 // Arp   ���������� �̿��Ͽ� HOST Mac Address�� ��� �´�. 
extern int Ping(int argc, char **argv);                // ICMP  ���������� �̿��Ͽ� Ping �׽�Ʈ�� �Ѵ�. 

extern int Tftp (int argc, char **argv);               // Tftp ���������� �̿��Ͽ� ����Ÿ�� ��� �´�.                                                     
extern int Tftp_FlashBoot(int argc, char **argv);      // Tftp ���������� �̿��Ͽ� ��Ʈ ȭ�� �ٿ�ε�����  �÷����� ����. 
extern int Tftp_Kernel(int argc, char **argv);         // Tftp ���������� �̿��Ͽ� Ŀ�� ȭ�� �ٿ�ε��Ѵ�. 
extern int Tftp_RamDisk(int argc, char **argv);        // Tftp ���������� �̿��Ͽ� Ram Disk �̹��� ȭ����  �ٿ�ε��Ѵ�. 
extern int Tftp_AppImage(int argc, char **argv);       // Tftp ���������� �̿��Ͽ� AppImage ȭ����  �ٿ�ε��Ѵ�. 

extern int GoKernel(int argc, char **argv);            // Ŀ�η� �����Ѵ�. 
extern int GoFunction(int argc, char **argv);	       // �޸𸮿� �ٿ�ε�� ����� �Լ��� �����Ѵ�.

extern int Help(int argc, char **argv);                // help

extern int  gets_his(char *s);
extern void MemoryHexDump( void *address, int size );
extern int  getc_timed( char cmpKey, int mTimeOver );

/* �����Լ� ���� --------------------------------------------------------------*/

int Soft_Reset(int argc, char **argv);
int TestFunction(int argc, char **argv);        		// ����� 


/* ���� ���� ���� -------------------------------------------------------------*/

extern TConfig Cfg;		// config.c �� ����


/* ��� ����Ʈ ----------------------------------------------------------------*/

TCommand Cmds[] = { 
        
        { "HELP" , Help            },
        { "?"    , Help            },
        
        { "RST"  , Soft_Reset      },	// main.c
        { "TST"  , TestFunction    },	
        
        { "ARP"  , Arp             },	// arp_cmd.c
        { "PING" , Ping            },
        
        { "GK"   , GoKernel        },	// go_cmd.c
        { "GO"   , GoFunction      },
        
        { "MD"   , RamDump         },   // ram_cmd.c
        { "MWB"  , RamWriteChar    },
        { "MWW"  , RamWriteWord    },
        { "MWL"  , RamWriteLong    },

        { "NEK"  , ErasePartiotion },	// flash_cmd.c
        { "NER"  , ErasePartiotion },
        { "NEA"  , ErasePartiotion },

        { "ZMK"  , ZModem_Kernel   },	// zmodem_cmd.c
        { "ZFK"  , ZModem_Kernel   },
        { "ZMR"  , ZModem_RamDisk  },
        { "ZFR"  , ZModem_RamDisk  },
        { "ZFB"  , ZModem_FlashBoot},
        { "ZM"   , ZModem_Memory   },
        { "ZF"   , ZModem_Memory   },

        { "TMK"  , Tftp_Kernel     },	// tftp_cmd.c
        { "TFK"  , Tftp_Kernel     },
        { "TMR"  , Tftp_RamDisk    },
        { "TFR"  , Tftp_RamDisk    },
        { "TFB"  , Tftp_FlashBoot  },
        { "TM"   , Tftp            },
        { "TF"   , Tftp            },

        { "SET"  , ModifyCfg       },	// config.c
        
        { NULL   , NULL            }
};

/* implimantation =============================================================*/


//------------------------------------------------------------------------------
// ���� : ����Ʈ ������ �����Ѵ�. 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ��ġ�� Ÿ�̸Ӹ� �̿��Ͽ� �������� �Ѵ�. 
//------------------------------------------------------------------------------
int 	Soft_Reset(int argc, char **argv)
{
	printf("System Soft Reset.......\n");

	SetWatchdog(10);	// 10msec
	
    	return 0;
}
//------------------------------------------------------------------------------
// ���� : ��ɹ��ڸ� �� ��ū���� �и��Ͽ� ��ȯ�Ѵ�. 
// �Ű� : cmdline : ��ɶ���
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ��ū�� 
// ���� : 
//------------------------------------------------------------------------------
static const char *delim = " \f\n\r\t\v";
static int parse_args(char *cmdline, char **argv)
{
	char *tok;
	int argc = 0;

	argv[argc] = NULL;
                   
	for (tok = strtok(cmdline, delim); tok; tok = strtok(NULL, delim)) 
        {
		argv[argc++] = tok;
	}

	return argc;
}

//------------------------------------------------------------------------------
// ���� : �׽�Ʈ �Լ�
// �Ű� : 
// ��ȯ : 
// ���� : 
//------------------------------------------------------------------------------
int 	TestFunction(int argc, char **argv)     // ����� 
{
    
}

//------------------------------------------------------------------------------
// ���� : �׽�Ʈ �Լ�
// �Ű� : 
// ��ȯ : 
// ���� : 
//------------------------------------------------------------------------------
int 	LedBlink( void )
{
	int  delay = (Cfg.AutoBootWaitTime >= 2) ? 250:50;

	GPIO_SetLED( 0, LED_ON  ); GPIO_SetLED( 1, LED_OFF ); GPIO_SetLED( 2, LED_ON  ); GPIO_SetLED( 3, LED_OFF );
	
	msleep(delay);
	
	GPIO_SetLED( 0, LED_OFF ); GPIO_SetLED( 1, LED_ON  ); GPIO_SetLED( 2, LED_OFF ); GPIO_SetLED( 3, LED_ON  );
	
	msleep(delay);
	
	GPIO_SetLED( 0, LED_OFF ); GPIO_SetLED( 1, LED_OFF ); GPIO_SetLED( 2, LED_OFF ); GPIO_SetLED( 3, LED_OFF );
}

//------------------------------------------------------------------------------
// ���� : ����Ʈ ��Ʈ�� ����Ѵ�. 
// �Ű� : vptrMng : JTAG���� ���� �޸� �ּ� 
// ��ȯ : ���� 
// ���� : ���� ���� DLC5�� ���� ����� �� �����̴�. 
//------------------------------------------------------------------------------
int 	main( void )
{
	int  start_option = *(int *)(DEFAULT_RAM_KERNEL_ZERO_PAGE);
	char ReadBuffer[ 1024 ];
	int   argc;
	char *argv[128];
	int   cmdlp;
	
	// ȯ�氪�� �ҷ��´�.
    	LoadConfig();

    	// Ÿ�̸Ӹ� �ʱ�ȭ �Ѵ�. 
    	TimerInit();
    
    	// GPIO�� �ʱ�ȭ �Ѵ�. 
    	GPIOInit();

    	// �ø����� �ʱ�ȭ �Ѵ�. 
    	SerialInit( BAUD_115200 );
    	ZModem_Baudrate = 115200;

    	// ������Ʈ ���� �޼����� �����ش�. 
    	printf( "\n\n");
    	printf( "WELCOME EZBOOT.X5 V1.8...................for PXA255\n");
    	printf( "Program by You Young-chang, fooji (FALinux Co.,Ltd)\n");
    	printf( "Last Modify %s\n\n", __DATE__);

	// LED �� ���ڿ� �������� ǥ���Ѵ�.
	LedBlink();

    	// ��Ʈ �÷��� �ʱ�ȭ
    	BootFlash_Init();	printf("\n");

    	// NAND �÷��� �ʱ�ȭ 
	NandFlash_Init();	printf("\n");
    	
    	// CS8900�� �ʱ�ȭ �Ѵ�. 
    	CS8900_Init();		printf("\n");

	// eztiny ���� �Ѿ�°��̶�� �޸��� ������ �÷��ÿ� �����Ѵ�.
	switch ( start_option )
	{
	// [ezboot]
	case 0x0000 : 
    	     	// Autoboot =======================
    	     	printf( "Quickly Autoboot [ENTER] / " );
    	     	if( Cfg.BootMenuKey == ' ' )  printf( "Goto BOOT-MENU press [space bar]");
    	     	else                          printf( "Goto BOOT-MENU press [%c]", Cfg.BootMenuKey ); 
	     	
    	     	if ( getc_timed( Cfg.BootMenuKey, Cfg.AutoBootWaitTime*1000 ) )
    	     	{
       	     		//printf( "\r                                                                \n");  
       	     		printf( "\n");  
             		CopyImage();
             		GoKernel( 1, NULL );
             		// ����� �������� �ʴ´�.
             		// ---
    	     	}
    	     	break;
    	     	
	// [eztiny]
	case 0x0001 : SelfCopy_BootLoader(); 
		break;
	}

    	printf( "\n\n");

	// Boot propmt =====================
	// ����� ����Ѵ�. 
	while (1)
	{
		printf( "EZBOOT>");
                
        	memset( ReadBuffer, 0 , sizeof( ReadBuffer ) );
        	// �ø��󿡼� ��ɾ �Էµɶ����� ����Ѵ�.
        	gets_his( ReadBuffer );
        	printf( "\n");

		argc = parse_args( ReadBuffer, argv );
		if (argc)
        	{
			UpperStr( argv[0] );
			cmdlp = 0;
    			while( Cmds[cmdlp].CmdStr )
    			{
				if( strcmp( argv[0], Cmds[cmdlp].CmdStr ) == 0 )
                		{
					Cmds[cmdlp].func( argc, argv );
					printf( "\n");	
					break;
                		}
				cmdlp++;
    			}
		}
	}
	
}

