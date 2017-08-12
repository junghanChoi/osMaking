//------------------------------------------------------------------------------
// ȭ�ϸ� : tftp_cmd.c
// ��  �� : ezBoot�� TFTP�� �̿��Ͽ� ȭ�� ����Ÿ�� ������ ��ƾ�̴�. 
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
//          ����� (��)���̴���Ƽ freefrug@falinux.com
// �ۼ��� : 2001�� 10�� 8��
// ��  �� : 2003�� 6�� 05��
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
// 
//------------------------------------------------------------------------------

//******************************************************************************
//
// ��� ����
//
//******************************************************************************
#include <pxa255.h>
#include <serial.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

#include <mem_map.h>
#include <cs8900.h>
#include <net.h>
#include <config.h>
#include <flash.h>


//******************************************************************************
//
// ���� ���� ����
//
//******************************************************************************

Word16 __TFTPLastBlockNumber = 0; // ���������� ������ �� ��ȣ 
Word16 __TFTPHostPortNumber  = TFTP_PORT_HOST;      // ȣ��Ʈ�� ��Ʈ ��ȣ 
Word32 __TFTPResiveTotalSize = 0; // ���ŵ� �� ũ�� 

// config.c �� ����
extern TConfig Cfg;

//------------------------------------------------------------------------------
// ���� : TFTP  ������ �����Ѵ�. 
// �Ű� : ptrTFTP  : ���� Tftp ���� �ּ� 
//        HostPortNumber : ȣ��Ʈ�� ��Ʈ ��ȣ    
//        tftpsize : Tftp ����Ÿ�� ũ�� 
// ��ȯ : 
// ���� : tftpsize�� ���۵Ǵ� ������ ��ü ũ�Ⱑ �ƴ� 
//------------------------------------------------------------------------------
BOOL SendTFTPPacket( TFTP_Packet_t *ptrTFTP, Word16 HostPortNumber, int tftpsize )
{

    // UDP ���� ������ �����Ѵ�. 
    SetUdpHeader( &(ptrTFTP->UDP), HostPortNumber, TFTP_PORT_CLIENT, tftpsize  );

    // IP ���� ������ �����Ѵ�. 
    SetIPHeaderByUDP( &(ptrTFTP->IP),          
                      GetHostIP(),         // Host IP      
                      GetLocalIP(),        // Local IP 
                      sizeof( ptrTFTP->UDP ) + tftpsize 
                    );
 
    // �̴��� ���� ���� ������ �����Ѵ�. 
    SetEthernetIPHeader( &(ptrTFTP->ETHERNET), CS8900_GetMacAddress(), GetHostMacAddress() );
   
    // ������ �����Ѵ�. 
    CS8900_Transmit( ptrTFTP, sizeof( ptrTFTP->ETHERNET ) 
                            + sizeof( ptrTFTP->IP ) 
                            + sizeof( ptrTFTP->UDP ) + tftpsize 
                   ); 

    return TRUE;
}
//------------------------------------------------------------------------------
// ���� : Tftp ���� ������ ó�� �Ѵ�. 
// �Ű� : ptrTFTP   : ���ŵ� TFTP Packet ���� �ּ� 
//        len       : ���ŵ� ���� 
//        LoadAddr  : �����ؾ��� �޸� ���� ����
// ��ȯ : ���� ��� 
// ���� : ����
//------------------------------------------------------------------------------
int TftpResiveHandler( TFTP_Packet_t *ptrTFTP, int len, Word32 LoadAddr )
{
    Word16  *ptrBlockNumber;
    Word16  *ptrErrorNumber;
    char    *SrcDataStr;   // ������ ����Ÿ ���� �ּ� 
    char    *DestDataStr;  // ���� ��ġ ����Ÿ ���� �ּ� 
    int      DataSize;
    Word16   Opcode;
    int      lp,opcnt;
    

    DataSize = len - sizeof( TFTP_Packet_t ) - 2;

    ptrBlockNumber = (Word16 *) ptrTFTP->Data; 
    ptrErrorNumber = (Word16 *) ptrTFTP->Data; 

    SrcDataStr = ptrTFTP->Data + 2;  // ����Ÿ �κ� 

    Opcode = SWAP16( ptrTFTP->OPCODE );
//    printf( "OPCODE [%d]\n", Opcode ); 

    switch ( Opcode )
    { 
    case TFTP_RRQ : // ó������ ���� ���뿡 ���Ͽ� �����Ѵ�. 
                    
    case TFTP_WRQ :
    case TFTP_ACK :
    default       : 
                    break;
    
    case TFTP_ERROR : // error ó��.
		      printf( "\nTFTP Error : [ %d ] %s \n", SWAP16( *ptrErrorNumber ), SrcDataStr );

                      return TFTP_STATE_FAILURE;
    case TFTP_DATA  : // ���ŵ� ����Ÿ ó�� 
             
                      // �� ũ�Ⱑ 0�̸�  
                      if( SWAP16( *ptrBlockNumber ) == 0 )
                      {
                         printf( "\nTFTP Error : Block Number Zero\n" );
                         return TFTP_STATE_FAILURE;    
                      }
                      
                      // ����Ÿ �� ��ȣ�� �˻� �Ѵ�. 
                      // �� ��ȣ�� ū�͸� ó��  
                      if( __TFTPLastBlockNumber < SWAP16( *ptrBlockNumber ) )
                      {
                          __TFTPLastBlockNumber = SWAP16( *ptrBlockNumber );
                          // ������ �޸� ��ġ�� ����Ѵ�. 
                          DestDataStr = ( char * ) ( LoadAddr + ( ( __TFTPLastBlockNumber-1) * 512 ) );
			  // �޸𸮸� �����Ѵ�. 
			  memcpy( DestDataStr, SrcDataStr, DataSize );
	
                          __TFTPResiveTotalSize += DataSize;
                      }
                      
                      // ����Ÿ�� ũ�Ⱑ 512�� �ƴ϶�� ��� ����Ÿ�� ���� ���̴�.                       
                      if( DataSize < 512 ) return TFTP_STATE_END;
                      return TFTP_STATE_DATA_OK;
                      
    case TFTP_OACK  : // �ɼ� ���� ���� ����Ÿ ó��    // Ÿ�� �ƿ� �߰� ���� 
                      opcnt = 0; 
                      printf( "OPTION [" );
                      for( lp = 0; lp < 64; lp++ )
                      {
                        if( ptrTFTP->Data[lp] == 0 ) 
                        {
                            opcnt++;
                            if( opcnt == 4 ) break;        
                            printf( " " );
                        }         
                        else
                        {
                            printf( "%c", ptrTFTP->Data[lp] );
                        }    
                      }
                      printf( "]\n" );
                      return TFTP_STATE_DATA_OK;
                      
    } 

    return TFTP_NONE;
}
//------------------------------------------------------------------------------
// ���� : Bootp�� �̿��Ͽ� ȭ�� ����Ÿ�� ��� �´�. 
// �Ű� : mHostIP  : ������ Host IP
//        mLocalIP : ���� IP
//        LoadAddr : ������ ��巹��  
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
BOOL 	TftpProcess( Word32 mHostIP, Word32 mLocalIP, Word32 LoadAddr, char *FileName  )
{
    	Word32         IPAddr; 
    	char           TxBuff[1500];
    	char           RxBuff[1500];
    	Word16         RxSize ; 
    	Word16         Protocol;

    	TFTP_Packet_t *ptrTFTP;
    	int            size;
    
    	//CS8900_SwReset();

    	if( AutoRebuildNetworkInfo( )  == FALSE )
    	{
        	printf( "- Fail Network Infomation\n" );
        	return FALSE;
    	}

    	// HOST �� LOCAL IP�� ǥ���Ѵ�. 
    	IPAddr = SWAP32( mHostIP );
    	printf( "HOST  IP : [%d.%d.%d.%d]\n", ( IPAddr >> 24 ) & 0xff,
                                              ( IPAddr >> 16 ) & 0xff,
                                              ( IPAddr >>  8 ) & 0xff,
                                              ( IPAddr >>  0 ) & 0xff );
        
    	IPAddr = SWAP32( mLocalIP );
    	printf( "LOCAL IP : [%d.%d.%d.%d]\n", ( IPAddr >> 24 ) & 0xff,
                                              ( IPAddr >> 16 ) & 0xff,
                                              ( IPAddr >>  8 ) & 0xff,
                                              ( IPAddr >>  0 ) & 0xff );

   	// ����Ÿ�� ������ ��巹���� ǥ���Ѵ�. 
   	printf( "Resive Address  : %04X-%04X\n", ( LoadAddr >> 16 ) & 0xFFFF, LoadAddr & 0xFFFF );

   	// ���� �䱸 ������ �����Ѵ�. 
   	printf( "TFTP Request Send\n" );

   	// �޸� ������ �ʱ�ȭ �Ѵ�. 
   	memset( TxBuff, 0, sizeof( TxBuff ) );
   	ptrTFTP = ( TFTP_Packet_t * ) TxBuff;

   	size = SetTFTP_RRQPacket( ptrTFTP , FileName );
   	SendTFTPPacket( ptrTFTP, TFTP_PORT_HOST, size );

   	__TFTPLastBlockNumber = 0; // �� ��ȣ�� �ʱ�ȭ �Ѵ�. 
   	__TFTPResiveTotalSize = 0; // ���ŵ� �� ũ�� 
 
   	ReloadTimer( 0, 5000 ); // Ÿ�̸�0  5��
   	while( 1 )   
   	{
            // ���� ������ �˻�  
            RxSize = CS8900_Resive( RxBuff, 1500 );
            if( RxSize  )
            {
                // printf( "RESIVE OK [ size : %d ]\n", RxSize );
                Protocol = GetProtocolFromRxPacket( ( EthernetIP_t * ) RxBuff, RxSize );

                // printf( "Ethernet Protocol Type : [%04X]\n", Protocol );
        	if (Protocol == PROT_ARP) 
                {
                    ARP_Packet_t *pARP = (ARP_Packet_t *)RxBuff;
                    if (SWAP16(pARP->ar_op) == ARPOP_REQUEST) 
                    {                                   
                            ARP_Packet_t arp_pk;
                            Word32  local_ip = Cfg.Local_IP;
                            Word32  host_ip  = Cfg.Host_IP;
                                
                            memset(&arp_pk, 0, sizeof(ARP_Packet_t));                                 
                            arp_pk.ar_hrd = SWAP16(HWT_ETHER); 
                            arp_pk.ar_pro = SWAP16(PROT_IP);                                          
                            arp_pk.ar_hln = 6;                                                        
                            arp_pk.ar_pln = 4;                                                        
                            arp_pk.ar_op  = SWAP16(ARPOP_REPLY); // Operation                         
                            arp_pk.ar_hrd = SWAP16(HWT_ETHER);
                            memcpy(arp_pk.SenderMac, CS8900_GetMacAddress(), sizeof(arp_pk.SenderMac));            
                            memcpy(arp_pk.TargetMac, pARP->SenderMac, sizeof(arp_pk.TargetMac));                   
                            arp_pk.SenderIP = local_ip;                                               
                            arp_pk.TargetIP = host_ip;
                                
                            memcpy((char *) arp_pk.ETHERNET.et_dest, pARP->SenderMac, sizeof(arp_pk.ETHERNET.et_dest));                         
                            memcpy((char *) arp_pk.ETHERNET.et_src,  CS8900_GetMacAddress(), sizeof(arp_pk.ETHERNET.et_src));                          
                            arp_pk.ETHERNET.et_protocol = SWAP16(PROT_ARP);                           
                            CS8900_Transmit((char * )&arp_pk, sizeof(ARP_Packet_t));                  
                    }
                            
                    continue;
                }

		if( Protocol != PROT_IP ) continue;
        	
                // printf( "Resive Protocol Type : [IP]\n" );  
                if( CheckTftpRxPacket( ( TFTP_Packet_t * ) RxBuff, RxSize, &mLocalIP ) )
                {
                     __TFTPHostPortNumber = GetHostPortNumberFromTftpPacket( ( TFTP_Packet_t * ) RxBuff ); 
                     // printf( "Resive TFTP Packet [ Host Port : %d ] \n", __TFTPHostPortNumber ); 
                     // HOST�� ��Ʈ ��ȣ�� ��´�. 
                   

                     // ó���Ѵ�. 
                     switch( TftpResiveHandler( ( TFTP_Packet_t * ) RxBuff, RxSize, LoadAddr ) )
                     {
                     case TFTP_STATE_FAILURE    : // ���� �߻� 
                                                  // ��� ������ �����Ѵ�. 
                                                  printf( "FAILURE BREAK\n" ); 
                                                  break;
                     case TFTP_STATE_DATA_OK    : // ����Ÿ ���� ���� 
             
                                                  size = SetTFTP_ACKDATAPacket( ptrTFTP , __TFTPLastBlockNumber );
                                                  SendTFTPPacket( ptrTFTP, __TFTPHostPortNumber, size );
                                                  
                                                  // printf( "DATA ACK [ Block Number : %d ]\n", __TFTPLastBlockNumber ); 
                                                  if( ( __TFTPLastBlockNumber % 10 ) == 0 )
                                                      printf( "\rSize:%d KB", __TFTPResiveTotalSize / 1024 ); 
                                                  
                                                  ReloadTimer( 0, 5000 ); // Ÿ�̸�0  5��
                                                  continue;
                     case TFTP_STATE_DATA_FAILE : // ����Ÿ ���� �߻� 
                                                  printf( "DATA FAIL\n" );  
                                                  break;
                     case TFTP_STATE_END        : // ��� ����Ÿ�� ���� 
                                                  size = SetTFTP_ACKDATAPacket( ptrTFTP , __TFTPLastBlockNumber );
                                                  SendTFTPPacket( ptrTFTP, __TFTPHostPortNumber, size );
                                                  
                                                  printf( "\rALL DATA RESIVE OK [ %d bytes ]\n", __TFTPResiveTotalSize ); 
                                                  FreeTimer( 0 );
                                                  return TRUE; 
                     } 
                }  
            }

            // �ð� �ʰ� �˻� 
            if( TimeOverflow(0) )
            {
                 printf( "Time Overflow\n" );
                 break;
            }
            
   	} 

    	FreeTimer( 0 );
    	return FALSE;

}
//------------------------------------------------------------------------------
// ���� : Tftp�� �̿��Ͽ� ȭ�� ����Ÿ�� ��� �´�. 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Tftp (int argc, char **argv)
{
        unsigned long  To;
        char           *Filename;

        printf( "tftp Command\n");
        if( argc < 3 ) 
        {
                printf( "Argument Count Error!\n");
                return -1; 
        }

        // ȭ���̸��� ��´�. 
        Filename = argv[2];
        // ����� �ּҸ� ��´�.  
        // �ּҿ� ���� ������ ���� �ʾҴ�.
        To = strtoul( argv[1], NULL, 0);

        // TFTP�� ����Ÿ�� �޴´�.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, Filename ) == TRUE )
        {
                if ( 'F' == argv[0][1] ) // �÷��� ������ ����. 
                {
                        printf( "���� ���� �ʾҴ�\n" );
                        //To &= 0x0fffffff;
                        //Flash_WriteFull( To, DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
                else
                {
                        memcpy( (void *)To, (void *)DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
        } 

        return 0;
}
//------------------------------------------------------------------------------
// ���� : TFTP�� ����Ÿ�� ������ ��Ʈ�� �� �ֱ� 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Tftp_FlashBoot(int argc, char **argv)
{
        int     size = 0;
        char    FileName[256];

        // ��Ʈ�δ� �����̸�
        strcpy( FileName, Cfg.TFTP_Directory );
        strcat( FileName, Cfg.TFTP_LoaderFileName );
        printf( "Receive %s\n", FileName );

        // TFTP�� ����Ÿ�� �޴´�.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, FileName ) == TRUE )
        {
        	// ��Ʈ ������ �������� ����. 
        	CopyTo_BootFlash( DEFAULT_FLASH_BOOT, DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
        } 
        
        return size;
}

//------------------------------------------------------------------------------
// ���� : TFTP�� ����Ÿ�� ������ Ŀ�ο� �� �ֱ� 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Tftp_Kernel(int argc, char **argv)
{
        char    FileName[256];

        // Ŀ���̹��� �����̸�
        strcpy( FileName, Cfg.TFTP_Directory );
        strcat( FileName, Cfg.TFTP_zImageFileName );
        printf( "Receive %s\n", FileName );

        // TFTP�� ����Ÿ�� �޴´�.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, FileName ) == TRUE )
        {
                if ( 'F' == argv[0][1] ) // �÷��� ������ ����. 
                {
                	CopyTo_NandFlash_Kernel( DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
                else
                {
                 	memcpy( (void *)(DEFAULT_RAM_KERNEL_START), (void *)DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
        } 

        return 0;
}
//------------------------------------------------------------------------------
// ���� : TFTP�� ����Ÿ�� ������ ����ũ ������ �� �ֱ� 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Tftp_RamDisk(int argc, char **argv)
{
        char    FileName[256];

        // ����ũ�̹��� �����̸�
        strcpy( FileName, Cfg.TFTP_Directory );
        strcat( FileName, Cfg.TFTP_RamDiskFileName );
        printf( "Receive %s\n", FileName );

        // TFTP�� ����Ÿ�� �޴´�.
        if( TftpProcess( GetHostIP(), GetLocalIP(), DEFAULT_RAM_WORK_START, FileName ) == TRUE )
        {
                if ( 'F' == argv[0][1] ) // �÷��� ������ ����. 
                {
                        CopyTo_NandFlash_Ramdisk( DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
                else
                {
                        memcpy( (void *)(DEFAULT_RAM_RAMDISK_START), (void *)DEFAULT_RAM_WORK_START, __TFTPResiveTotalSize );
                }
        } 
        
        return 0;
}


