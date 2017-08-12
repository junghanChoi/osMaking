//------------------------------------------------------------------------------
// ȭ�ϸ� : arp_cmd.c
// ��  �� : arp ���������� ó���Ѵ�.
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
// �ۼ��� : 2001�� 10�� 08��
// ������ : 2003�� 06�� 18��  
//          bootp ������ �����ϰ� �����̸��� �����Ͽ���.  -- Ǫ��
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
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

#include <cs8900.h>
#include <net.h>
#include <config.h>


extern int Cfg_parse_args(char *cmdline, char **argv);
extern Byte StrToByte( char *ptr, int hex );

//******************************************************************************
//
// ���� ���� ����
//
//******************************************************************************
static short __ICMP_SEQ    = 1;

//------------------------------------------------------------------------------
// ���� : ȣ��Ʈ Mac Address �� ���Ѵ�. 
// �Ű� : 
// ��ȯ : ���� IP
// ���� : 
//------------------------------------------------------------------------------
char *GetHostMacAddress( void )
{
        return Cfg.Host_MacAddress;
}

//------------------------------------------------------------------------------
// ���� : �����Ǿ� �ִ� Local IP�� ���Ѵ�. 
// �Ű� : 
// ��ȯ : ���� IP
// ���� : 
//------------------------------------------------------------------------------
Word32 GetLocalIP( void )
{
        return Cfg.Local_IP;
}
//------------------------------------------------------------------------------
// ���� : �����Ǿ� �ִ� Host IP�� ���Ѵ�. 
// �Ű� : 
// ��ȯ : ȣ��Ʈ IP
// ���� : 
//------------------------------------------------------------------------------
Word32 GetHostIP( void )
{
        return Cfg.Host_IP;   
}

//------------------------------------------------------------------------------
// ���� : IP ��û ARP ������ �����Ѵ�. 
// �Ű� : 
// ��ȯ : 
// ���� : 
//------------------------------------------------------------------------------
BOOL SendArpPacket( void )
{

    Byte	          BroadcastMAC[6] =  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } ; // ��ε� ĳ���� �� ��巹�� 
    ARP_Packet_t      ARP_Packet; 
    //int lp;

    // �޸� ������ �ʱ�ȭ �Ѵ�. 
    memset( &ARP_Packet, 0, sizeof( ARP_Packet ) );

    // BOOTP ������ �����Ѵ�. 
    SetArpRequestHeader     ( &ARP_Packet, CS8900_GetMacAddress(), GetLocalIP(), GetHostIP() );
    
    // �̴��� ���� ���� ������ �����Ѵ�. 
    SetEthernetARPHeader( ( EthernetIP_t * ) &ARP_Packet , CS8900_GetMacAddress(), BroadcastMAC );

    // ������ �����Ѵ�. 
    CS8900_Transmit( (char * ) &ARP_Packet, sizeof( ARP_Packet ) ); 
    return TRUE;
  
}

//------------------------------------------------------------------------------
// ���� : ARP ���� ������ ó�� �Ѵ�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
// ��ȯ : ����
// ���� : ����
//------------------------------------------------------------------------------
BOOL ARPReply( void *rxPktBuf, int len )
{
    int lp;
    // ���ŵ� ARP ������ �̻� ������ �˻��Ѵ�. 
    if( CheckARPRxPacket( rxPktBuf, len, &(Cfg.Host_IP), &(Cfg.Local_IP) ) == TRUE )
    {  
        GetHostMacAddressFromArpPacket( ( ARP_Packet_t * ) rxPktBuf, Cfg.Host_MacAddress );
       	printf( "ARP PACKET Resive\n");
        // ȣ��Ʈ�� �� ��巹���� ǥ�� �Ѵ�. 
        printf( "HOST MAC : [ " );
        for( lp = 0; lp < 6 ; lp++ ) printf( "%02X ", Cfg.Host_MacAddress[lp] & 0xFF );
        printf( "]\n" );
         
    }
    else
    {
       printf( "ARP PACKET Error\n");
       return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
// ���� : ���� ARP �� �����Ͽ� IP�� �ش��ϴ� MAC ��巹���� ��� �´�. 
// �Ű� : ����
// ��ȯ : ���� FALSE ���� TRUE
// ���� : ���� 
//------------------------------------------------------------------------------
BOOL    ARPProcess( void )
{
        char    RxBuff[1500];
        Word16  RxSize ; 
        Word16  Protocol;
        int     lp;

        // ARP Packet�� ���� ���� �� 1�ʰ������� ������
        // Reply�� ���ؼ� MAC ��巹���� 
        for( lp = 0; lp < 10 ; lp++ )
        {
                memset( RxBuff, 0, sizeof( RxBuff ) );
                printf( "Send ARP Packet \n");
                SendArpPacket();
        
                ReloadTimer( 0, 1000 );          // set 1-sec
                while( 0 == TimeOverflow(0) )    // �ð� �ʰ� �˻� 
                {
                        // ���� ������ �˻�  
                        RxSize = CS8900_Resive( RxBuff, 1500 );
                        if( RxSize  )
                        {
                                // printf( "RESIVE OK [ size : %d ]\n", RxSize );
                                Protocol = GetProtocolFromRxPacket( ( EthernetIP_t * ) RxBuff, RxSize );

                                // printf( "Ethernet Protocol Type : [%04X]\n", Protocol );
                                if( Protocol == PROT_ARP )
                                {
                                        // printf( "Resive Protocol Type : [ARP]\n" ); 
                                        if( ARPReply( ( EthernetIP_t * ) RxBuff, RxSize ) == TRUE ) 
                                        {
                                                FreeTimer( 0 );  // Ÿ�̸� ����
                                                return TRUE;
                                        }    
                                } 
                        }
                } 
        }

        FreeTimer( 0 );  // Ÿ�̸� ����
        return FALSE;
}

//------------------------------------------------------------------------------
// ���� : ECHO ������ �����Ѵ�. 
// �Ű� : 
// ��ȯ : 
// ���� : 
//------------------------------------------------------------------------------
BOOL SendEchoPacket( void )
{

    ECHO_Packet_t      ECHO_Packet; 
    //int lp;

    // �޸� ������ �ʱ�ȭ �Ѵ�. 
    memset( &ECHO_Packet, 0, sizeof( ECHO_Packet ) );

	ECHO_Packet.ICMP.icmp_type   = ECHO_REQUEST;	
	ECHO_Packet.ICMP.icmp_code   = 0;	
	ECHO_Packet.ICMP.icmp_cksum  = 0;	
	ECHO_Packet.ICMP.icmp_id     = SWAP16(8988);	
    ECHO_Packet.ICMP.icmp_seq    = __ICMP_SEQ++;   

    ECHO_Packet.ICMP.icmp_cksum  = SWAP16(GetCheckSumICMP(( Word16 * )&(ECHO_Packet.ICMP), sizeof( ECHO_Packet.ICMP ) ));

    // IP ���� ������ �����Ѵ�. 
    SetIPHeaderByICMP( &ECHO_Packet.IP,
                      GetHostIP(),              // ������ IP      
                      GetLocalIP(),             // Local    IP 
                      sizeof( ECHO_Packet.ICMP )
                    );
    
    // �̴��� ���� ���� ������ �����Ѵ�. 
    SetEthernetIPHeader( ( EthernetIP_t * ) &ECHO_Packet , CS8900_GetMacAddress(), GetHostMacAddress() );

    // ������ �����Ѵ�. 
    CS8900_Transmit( (char * ) &ECHO_Packet, sizeof( ECHO_Packet ) ); 
    return TRUE;
  
}

//------------------------------------------------------------------------------
// ���� : ECHO ���� ������ ó�� �Ѵ�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
// ��ȯ : TRUE : ���� / FALSE : ���� 
// ���� : ����
//------------------------------------------------------------------------------
BOOL    ECHOHandler( void *rxPktBuf, int len )
{

        if( CheckECHORxPacket( rxPktBuf, len, &(Cfg.Local_IP) ) == TRUE )
        {  
                return TRUE;
        }
        return FALSE;
}


//------------------------------------------------------------------------------
// ���� : ���� Bootp �� �����Ͽ� IP�� ��� �´�. 
// �Ű� : ����
// ��ȯ : ���� FALSE ���� TRUE
// ���� : ���� 
//------------------------------------------------------------------------------
BOOL    EChoProcess( void )
{
        char    RxBuff[1500];
        Word16  RxSize ; 
        Word16  Protocol;
        int     lp;

        // Bootp Packet�� ���� ���� �� 1�ʰ������� ������
        // Reply�� ���ؼ� IP�� ��´�. 
        for( lp = 0; lp < 10 ; lp++ )
        {
                memset( RxBuff, 0, sizeof( RxBuff ) );
                printf( "Send Echo Packet \n");
                SendEchoPacket();
        
                ReloadTimer( 0, 1000 );            // set 1-sec
                while( 0 == TimeOverflow(0) )    // �ð� �ʰ� �˻� 
                {
                        // ���� ������ �˻�  
                        RxSize = CS8900_Resive( RxBuff, 1500 );
                        if( RxSize  )
                        {
                                // printf( "RESIVE OK [ size : %d ]\n", RxSize );
                                Protocol = GetProtocolFromRxPacket( ( EthernetIP_t * ) RxBuff, RxSize );

                                // printf( "Ethernet Protocol Type : [%04X]\n", Protocol );
                                if( Protocol == PROT_IP )
                                {
                                        // printf( "Resive Protocol Type : [IP]\n" ); 
                                        if( ECHOHandler( RxBuff, RxSize ) == TRUE ) 
                                        {
                                                printf( "PING TEST GOOD\n" );
                                                FreeTimer( 0 );  // Ÿ�̸� ����
                                                return TRUE;
                                        }    
                                } 
                        }
                } 
        }

        FreeTimer( 0 );  // Ÿ�̸� ����
        printf( "PING TEST FAIL\n" ); 
        return FALSE;
}

//------------------------------------------------------------------------------
// ���� : �ʿ��� ��Ʈ��ũ ������ �� �����Ѵ�. 
// �Ű� : 
// ��ȯ : 
// ���� : ���� 
//------------------------------------------------------------------------------
BOOL AutoRebuildNetworkInfo( void )
{
	if(   ( GetLocalIP() == 0          )
           || ( GetLocalIP() == 0xffffffff )
           || ( GetHostIP () == 0          ) 
           || ( GetHostIP () == 0xffffffff ) )
      	{
		printf( "- Invalid [LOCAL IP] or [HOST IP], Type [SET] command\n" );
		return FALSE;
      	}

      	return ARPProcess();
}
//------------------------------------------------------------------------------
// ���� : Arp�� �̿��Ͽ� Host�� Mac ��巹���� ���´�. 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Arp (int argc, char **argv)
{
        
        // �̴����� ��ũ �Ǿ� �ִ°��� Ȯ���Ѵ�. 
        if( CS8900_CheckLink() == FALSE )
        {
                printf( "Ethernet Link failed. Check line.\n" );
                return -1;
        }
        
        // ���� ������ ���ش�.   
        memset( Cfg.Host_MacAddress, 0, sizeof( Cfg.Host_MacAddress ) );
        ARPProcess();
        
        return TRUE; 
}
//------------------------------------------------------------------------------
// ���� : ICMPp�� ECHO ó�� 
// �Ű� : argc    : ��ū ���� 
//        argv    : �и��� ��ū ���ڿ� �ּҰ� ����� �迭 
// ��ȯ : ���� -1  ���� 0
// ���� : ���� 
//------------------------------------------------------------------------------
int     Ping( int argc, char **argv )
{
        char    *str[128];
        int     adrcnt;
        Word32  TargetIP;
        Word32  OldHostIP;

        if ( argc < 2 )
        {
                printf( "Error argument count\n" );
                return -1;
        }

        // ����ڷκ��� IP �� ���´�.
        adrcnt = Cfg_parse_args( argv[1], str );

        if ( adrcnt == 4 )
        {
          TargetIP =   ( StrToByte(str[3],0) << 24 )
                   |   ( StrToByte(str[2],0) << 16 )
                   |   ( StrToByte(str[1],0) << 8  )
                   |   ( StrToByte(str[0],0)       );
        }
        else
        {
                printf( "Error IP Address\n" );
                return -1;
        }


        OldHostIP = Cfg.Host_IP;
        Cfg.Host_IP = TargetIP;

        // �̴����� ��ũ �Ǿ� �ִ°��� Ȯ���Ѵ�. 
        if( CS8900_CheckLink() == FALSE )
        {
                printf( "Ethernet Link failed. Check Line.\n" );
                return -1;
        }

        if( AutoRebuildNetworkInfo() == FALSE ) return FALSE;

        EChoProcess();

        Cfg.Host_IP = OldHostIP;
        
        return TRUE; 
}
