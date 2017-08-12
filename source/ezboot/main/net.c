//------------------------------------------------------------------------------
// ȭ�ϸ� : net.c
// ��  �� : ezBoot�� Network ������ ó���ϴ� ��ƾ�̴�. 
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
// �ۼ��� : 2001�� 10�� 8��
// ���۱� : (��)���̴��Ƽ 
//          
// ��  �� : 
//------------------------------------------------------------------------------

#include <pxa255.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <cs8900.h>
#include <net.h>


//******************************************************************************
//
// ���� ���� ����
//
//******************************************************************************
static short __IP_ID    = 0;
static Word32 __BootpID = 0;                      // Bootp Transation ID 

//------------------------------------------------------------------------------
// ���� : ������ packet�� checksum�� check.
// �Ű� : ptr : IP header�� �ּ�.
//        len : IP header�� ����.
// ��ȯ : IP header���� ���� checksum ��.
// ���� : 
//------------------------------------------------------------------------------
unsigned IPChecksum(char *ptr, int len)
{
	Word32		xsum;

	xsum = 0;
	while (len-- > 0) xsum += *((Word16 *)ptr)++;

	xsum = (xsum & 0xffff) + (xsum >> 16);
	xsum = (xsum & 0xffff) + (xsum >> 16);

	return xsum & 0xffff;
}	

//------------------------------------------------------------------------------
// ���� : ������ packet�� checksum�� check.
// �Ű� : ptr : IP header�� �ּ�.
//        len : IP header�� ����.
// ��ȯ : 0 : failure,		1 : success
// ���� : 
//------------------------------------------------------------------------------
int IPChksumOK( char *ptr, int len )
{
	return !((IPChecksum(ptr, len) + 1) & 0xfffe);
}	

//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� ������ � ���������ΰ��� ���´�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
// ��ȯ : �������� Ÿ�� ��ȣ 
// ���� : 
//------------------------------------------------------------------------------
Word16 GetProtocolFromRxPacket( EthernetIP_t *rxPktBuf, int len )
{
        return SWAP16( rxPktBuf->et_protocol ); 
}
//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� ������ Bootp ���������ΰ��� ���´�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
//        ClientIP : �Ҵ�� IP 
// ��ȯ : ������ ���ٸ� TRUE �ִٸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL CheckBootpRxPacket( BOOTP_Packet_t *rxPktBuf, int len, Word32 *ClientIP )
{

        // UDP�� ������ ũ�⺸�� �������� �˻� 

	if(len < ( sizeof( EthernetIP_t ) + sizeof( IP_t ) + sizeof( UDP_t ) ) ) return FALSE;	
        if(len < ( sizeof( EthernetIP_t ) + SWAP16( rxPktBuf->IP.ip_len)             ) ) return FALSE;
	
        // ip version�� 4�ΰ��� �˻��Ѵ�.  
	if( ( rxPktBuf->IP.ip_hl_v & 0xf0 ) != 0x40 ) return FALSE; 
        
	// Can't deal fragments.
	if( rxPktBuf->IP.ip_off & SWAP16(0x1fff)) return FALSE;
	
    // check checksum.
	if(!IPChksumOK((char *) &rxPktBuf->IP, sizeof( rxPktBuf->IP ) / 2) ) return FALSE ; 
	
    // client�� ip�� ���� packet�� ip ��.
	if( *ClientIP && memcmp(&rxPktBuf->IP.ip_dst, ClientIP, 4) ) return FALSE; 
	
        // udp���� check. 17�� udp�� ��ȣ. 
	if( rxPktBuf->IP.ip_p != IPPROTO_UDP )                   return FALSE; 

        // BOOTP ��Ʈ ��ȣ �˻� 	
	if( rxPktBuf->UDP.udp_src != SWAP16( PORT_BOOTPS ) )     return FALSE;
        if( rxPktBuf->UDP.udp_dest!= SWAP16( PORT_BOOTPC ) ) return FALSE; 

        // BOOTP ���ϰ� ���� ũ���ΰ��� Ȯ���Ѵ�. 
	if (len != sizeof( BOOTP_Packet_t ) )		             return FALSE;

	// BOOTP ���� �����ΰ��� �˻� 
	if( rxPktBuf->BOOTP.bh_opcode  !=2 )		             return FALSE;
	if( rxPktBuf->BOOTP.bh_htype   !=1 )			         return FALSE;
	if( rxPktBuf->BOOTP.bh_hlen    !=6 )			         return FALSE;
	if( memcmp(&rxPktBuf->BOOTP.bh_tid, &__BootpID, 4))      return FALSE;

	// get infomation from bootp packet.

        return TRUE;
}

//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� ������ ECHO ���������ΰ��� ���´�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
//        ClientIP : �Ҵ�� IP 
// ��ȯ : ������ ���ٸ� TRUE �ִٸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL CheckECHORxPacket( ECHO_Packet_t *rxPktBuf, int len, Word32 *ClientIP )
{

    // UDP�� ������ ũ�⺸�� �������� �˻� 

	if(len < sizeof( ECHO_Packet_t ) ) return FALSE;	
	
    // ip version�� 4�ΰ��� �˻��Ѵ�.  
	if( ( rxPktBuf->IP.ip_hl_v & 0xf0 ) != 0x40 ) return FALSE; 
	
        
	// Can't deal fragments.
	if( rxPktBuf->IP.ip_off & SWAP16(0x1fff)) return FALSE;
	
	
    // check checksum.
	if(!IPChksumOK((char *) &rxPktBuf->IP, sizeof( rxPktBuf->IP ) / 2) ) return FALSE ; 
	
	
    // client�� ip�� ���� packet�� ip ��.
	if( *ClientIP && memcmp(&rxPktBuf->IP.ip_dst, ClientIP, 4) ) return FALSE; 
	
	
    // udp���� check. 17�� udp�� ��ȣ. 
	if( rxPktBuf->IP.ip_p != IPPROTO_ICMP )                  return FALSE; 
	

	// ECHO Request �ΰ��� �˻� 
	if( rxPktBuf->ICMP.icmp_type  != ECHO_REPLY )		             return FALSE;
	

    return TRUE;
}


//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� ������ Tftp ���������ΰ��� ���´�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
//        ClientIP : �Ҵ�� IP 
// ��ȯ : ������ ���ٸ� TRUE �ִٸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL CheckTftpRxPacket( TFTP_Packet_t *rxPktBuf, int len, Word32 *ClientIP )
{

    // UDP�� ������ ũ�⺸�� �������� �˻� 

	if(len < ( sizeof( EthernetIP_t ) + sizeof( IP_t ) + sizeof( UDP_t ) ) ) return FALSE;	
        if(len < ( sizeof( EthernetIP_t ) + SWAP16( rxPktBuf->IP.ip_len)             ) ) return FALSE;
    // ip version�� 4�ΰ��� �˻��Ѵ�.  
	if( ( rxPktBuf->IP.ip_hl_v & 0xf0 ) != 0x40 ) return FALSE; 

	// Can't deal fragments.
	if( rxPktBuf->IP.ip_off & SWAP16(0x1fff)) return FALSE;

    // check checksum.
	if(!IPChksumOK((char *) &rxPktBuf->IP, sizeof( rxPktBuf->IP ) / 2) ) return FALSE ; 

    // client�� ip�� ���� packet�� ip ��.
	if( *ClientIP && memcmp(&rxPktBuf->IP.ip_dst, ClientIP, 4) ) return FALSE; 
        // udp���� check. 17�� udp�� ��ȣ. 
	if( rxPktBuf->IP.ip_p != IPPROTO_UDP ) return FALSE; 

    // TFTP ��Ʈ �ΰ��� �˻��Ѵ�. 
    // � ��Ʈ�� Listen��Ʈ�� ����Ÿ ��Ʈ�� �ٸ��� ������ ������ �ʴ´�. 
	if( rxPktBuf->UDP.udp_dest != SWAP16( TFTP_PORT_CLIENT ) ) return FALSE;

    // TFTP ������ �ּ� ũ�� ���� ū���� Ȯ���Ѵ�. 
	if (len < sizeof( TFTP_Packet_t ) )		        return FALSE;

        return TRUE;
}


//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� Bootp ���Ͽ��� LocalIP�� HostIP�� ���´�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        LocalIP  : �Ҵ�� ���� IP 
//        HostIP   : �Ҵ�� ȣ��Ʈ IP
// ��ȯ : ������ ���ٸ� TRUE �ִٸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL  GetIPFromBootpPacket( BOOTP_Packet_t *rxPktBuf, Word32 *LocalIP, Word32 *HostIP )
{

        *LocalIP = rxPktBuf->BOOTP.bh_yiaddr;
        *HostIP  = rxPktBuf->BOOTP.bh_siaddr;
        return TRUE;
}
//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� ���Ͽ��� �۽� �� �� ��巹���� ���´�. 
// �Ű� : rxPktBuf       : ���ŵ� Packet ���� �ּ� 
//        HostMacAddress : �Ҵ�� ���� IP 
//        HostIP   : �Ҵ�� ȣ��Ʈ IP
// ��ȯ : ������ ���ٸ� TRUE �ִٸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL  GetHostMacAddressFromBootpPacket( BOOTP_Packet_t *rxPktBuf, char *HostMacAddress )
{
        // Client �� ��巹���� �����Ѵ�.
	memcpy( HostMacAddress , rxPktBuf->ETHERNET.et_src, sizeof( rxPktBuf->ETHERNET.et_src ) );
        return TRUE;
}

//------------------------------------------------------------------------------
// ���� : ���ŵ� �̴��� ���Ͽ��� �۽� �� �� ��巹���� ���´�. 
// �Ű� : ptrTFTP : ���ŵ� Packet ���� �ּ� 
// ��ȯ : ȣ��Ʈ�� ��Ʈ ��ȣ�� ��´�. 
// ���� : 
//------------------------------------------------------------------------------
Word16 GetHostPortNumberFromTftpPacket( TFTP_Packet_t *ptrTFTP )
{
       return  SWAP16( ptrTFTP->UDP.udp_src );
}


//------------------------------------------------------------------------------
// ���� : Ethernet Header�� IP ���� �������� ����..
// �Ű� : ptrEthernetIP : EthenrIP ���� �ּ� 
//        srcMAC   : �۽� �� MAC ��巹�� 
//        destMAC  : ���� �� MAC ��巹��  
// ��ȯ : ����. 
// ���� : ����.
//------------------------------------------------------------------------------
void SetEthernetIPHeader( EthernetIP_t *ptrEthernetIP, char *srcMAC, char *destMAC )
{
	memcpy((char *) ptrEthernetIP->et_dest, destMAC, sizeof( ptrEthernetIP->et_dest ) );
	memcpy((char *) ptrEthernetIP->et_src , srcMAC , sizeof( ptrEthernetIP->et_src  ) );

	ptrEthernetIP->et_protocol = SWAP16( PROT_IP );	// set 0x0800 (ip).
}	

//------------------------------------------------------------------------------
// ���� : Ethernet Header�� IP ���� �������� ����..
// �Ű� : ptrEthernetIP : EthenrIP ���� �ּ� 
//        srcMAC   : �۽� �� MAC ��巹�� 
//        destMAC  : ���� �� MAC ��巹��  
// ��ȯ : ����. 
// ���� : ����.
//------------------------------------------------------------------------------
void SetEthernetARPHeader( EthernetIP_t *ptrEthernetIP, char *srcMAC, char *destMAC )
{
	memcpy((char *) ptrEthernetIP->et_dest, destMAC, sizeof( ptrEthernetIP->et_dest ) );
	memcpy((char *) ptrEthernetIP->et_src , srcMAC , sizeof( ptrEthernetIP->et_src  ) );

	ptrEthernetIP->et_protocol = SWAP16( PROT_ARP );	// set 0x0806 (ip).
}	
//------------------------------------------------------------------------------
// ���� : ARP �䱸 ������ �����. 
// �Ű� : 
// ��ȯ : ����. 
// ���� : ����.
//------------------------------------------------------------------------------
void SetArpRequestHeader( ARP_Packet_t *ptrARP_Packet, char *srcMAC, Word32 srcIP, Word32 hostIP )
{

    ptrARP_Packet->ar_hrd = SWAP16( HWT_ETHER    );       // Format of hardware address	
    ptrARP_Packet->ar_pro = SWAP16( PROT_IP      );         // Format of protocol address	
    ptrARP_Packet->ar_hln =         6             ;               // Length of hardware address	
    ptrARP_Packet->ar_pln =         4             ;               // Length of protocol address	
    ptrARP_Packet->ar_op  = SWAP16( ARPOP_REQUEST);   // Operation
                                             
    memcpy( ptrARP_Packet->SenderMac, srcMAC, sizeof( ptrARP_Packet->SenderMac ) );  // Sender MAC Address
    memset( ptrARP_Packet->TargetMac,     0 , sizeof( ptrARP_Packet->TargetMac ) );  // Target MAC Address
    ptrARP_Packet->SenderIP  = srcIP;                                                // Sender IP  Address
    ptrARP_Packet->TargetIP = hostIP;                                                // Target IP  Address

}
//------------------------------------------------------------------------------
// ���� : ���ŵ� ARP ������ �̻��� �������� Ȯ���ϰ� ������ ������ �ϴ����� 
//        �˻� �Ѵ�. 
// �Ű� : rxPktBuf : ���ŵ� Packet ���� �ּ� 
//        len      : ���ŵ� ���� 
//        ClientIP : �Ҵ�� IP 
// ��ȯ : �۽� �ʸ��� �䱸 �ǳ� TRUE �ƴϸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL CheckARPRxPacket( void *rxPktBuf, int len, Word32 *HostIP, Word32 *ClientIP )
{

	ARP_Packet_t *ptrARP = (ARP_Packet_t *)( rxPktBuf );

 
   	if( len < sizeof( ARP_Packet_t )              ) return FALSE;

	if( SWAP16( ptrARP->ar_op  ) != ARPOP_REPLY   ) return FALSE;
	if( SWAP16( ptrARP->ar_hrd ) != ARP_ETHER     ) return FALSE;
	if( SWAP16( ptrARP->ar_pro ) != PROT_IP       ) return FALSE;
	if(         ptrARP->ar_hln   != 6             ) return FALSE;
	if(         ptrARP->ar_pln   != 4             ) return FALSE;

    if( ptrARP->SenderIP != *HostIP   ) return FALSE;
    if( ptrARP->TargetIP != *ClientIP ) return FALSE;

    return TRUE;

}

//------------------------------------------------------------------------------
// ���� : ���ŵ� ARP ���Ͽ��� �۽� �� �� ��巹���� ���´�. 
// �Ű� : rxPktBuf       : ���ŵ� Packet ���� �ּ� 
//        HostMacAddress : �Ҵ�� ���� IP 
//        HostIP   : �Ҵ�� ȣ��Ʈ IP
// ��ȯ : ������ ���ٸ� TRUE �ִٸ� FALSE;
// ���� : 
//------------------------------------------------------------------------------
BOOL  GetHostMacAddressFromArpPacket( ARP_Packet_t *rxPktBuf, char *HostMacAddress )
{

	memcpy( HostMacAddress , rxPktBuf->SenderMac, sizeof( rxPktBuf->SenderMac ) );
    	return TRUE;

}

//------------------------------------------------------------------------------
// ���� : IP Header  ������ �����Ѵ�. ( UDP ����)
// �Ű� : ptrIP      : IP ���� ���� ���� �ּ� 
//        srcIP      : �۽� �� IP �ּ� 
//        destIP     : ���� �� IP �ּ� 
//        PacketSize : ���� packet�� ����.
// ��ȯ : ����.
// ���� : ����.
//------------------------------------------------------------------------------
void SetIPHeaderByUDP( IP_t *ptrIP, Word32 destIP, Word32 srcIP, Word16 PacketSize )
{

	// ���� ũ�Ⱑ Ȧ����� üũ�� ����� ���Ͽ� ������ ��ġ�� 0�� �ִ´�. 
	if ( PacketSize & 1) ptrIP->Data[PacketSize] = 0;

	// IP�� ���� ������ �����Ѵ�. 
	ptrIP->ip_hl_v                   = 0x45;			// ���� ip version 4, IP_HDR_SIZE / 4 (not including UDP)
	ptrIP->ip_tos                    = 0;				// type of service. ���� network���� ��������. �׻� 0.
	ptrIP->ip_len                    = SWAP16( sizeof( IP_t ) + PacketSize );	// total length of ip packet.
	ptrIP->ip_id                     = SWAP16( __IP_ID );		// identifier.
	ptrIP->ip_off                    = SWAP16( 0x0000 );		// No fragmentation.
	ptrIP->ip_ttl                    = 64;				// time to live.
	ptrIP->ip_p                      = IPPROTO_UDP;			// UDP.
	ptrIP->ip_sum                    = 0;

	memcpy( (char *)&(ptrIP->ip_src) , &(srcIP ), 4 );
	memcpy( (char *)&(ptrIP->ip_dst) , &(destIP), 4 );

	ptrIP->ip_sum = ~IPChecksum( (char *) ptrIP, sizeof( IP_t ) / 2);

        __IP_ID++;

}

//------------------------------------------------------------------------------
// ���� : IP Header  ������ �����Ѵ�. ( UDP ����)
// �Ű� : ptrIP      : IP ���� ���� ���� �ּ� 
//        srcIP      : �۽� �� IP �ּ� 
//        destIP     : ���� �� IP �ּ� 
//        PacketSize : ���� packet�� ����.
// ��ȯ : ����.
// ���� : ����.
//------------------------------------------------------------------------------
void SetIPHeaderByICMP( IP_t *ptrIP, Word32 destIP, Word32 srcIP, Word16 PacketSize )
{

	// ���� ũ�Ⱑ Ȧ����� üũ�� ����� ���Ͽ� ������ ��ġ�� 0�� �ִ´�. 
	if ( PacketSize & 1) ptrIP->Data[PacketSize] = 0;

	// IP�� ���� ������ �����Ѵ�. 
	ptrIP->ip_hl_v                   = 0x45;			// ���� ip version 4, IP_HDR_SIZE / 4 (not including UDP)
	ptrIP->ip_tos                    = 0;				// type of service. ���� network���� ��������. �׻� 0.
	ptrIP->ip_len                    = SWAP16( sizeof( IP_t ) + PacketSize );	// total length of ip packet.
	ptrIP->ip_id                     = SWAP16( __IP_ID );		// identifier.
	ptrIP->ip_off                    = SWAP16( 0x0000 );		// No fragmentation.
	ptrIP->ip_ttl                    = 64;			         	// time to live.
	ptrIP->ip_p                      = IPPROTO_ICMP;			// UDP.
	ptrIP->ip_sum                    = 0;

	memcpy( (char *)&(ptrIP->ip_src) , &(srcIP ), 4 );
	memcpy( (char *)&(ptrIP->ip_dst) , &(destIP), 4 );

	ptrIP->ip_sum = ~IPChecksum( (char *) ptrIP, sizeof( IP_t ) / 2);

    __IP_ID++;

}
//------------------------------------------------------------------------------
// ���� : ICMP üũ���� ���Ѵ�. 
// �Ű� : ptrIP      : IP ���� ���� ���� �ּ� 
//        srcIP      : �۽� �� IP �ּ� 
//        destIP     : ���� �� IP �ּ� 
//        PacketSize : ���� packet�� ����.
// ��ȯ : ����.
// ���� : ����.
//------------------------------------------------------------------------------
Word16 GetCheckSumICMP( Word16 *Data, int Size )
{
	Word16  *w      = Data;	// address of next 16-bit word 
	int      nleft  = Size;	// remaining 16-bit words 
	int      sum    = 0;	// 32-bit accumulator 
	Word16   answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(Byte *)(&answer) = *(Byte *)w;
		sum += answer;
	}

    // Add back carry outs from top 16 bits to low 16 bits.
 
	sum = (sum >> 16) + (sum & 0xFFFF);	// add hi 16 to low 16 
	sum += (sum >> 16);			        // add carry 
	answer = ~sum;				        // truncate to 16 bits 
	return(answer);
}

//------------------------------------------------------------------------------
// ���� : UDP Header�� ������ �����Ѵ�. 
// �Ű� : ptrUDP : UDP ���� ���� ���� �ּ� 
//        srcPort      : �۽� �� Port �ּ� 
//        destPort     : ���� �� Port �ּ� 
//        PacketSize   : ���� packet�� ����.
// ��ȯ : ����.
// ���� : ����.
//------------------------------------------------------------------------------
void SetUdpHeader( UDP_t *ptrUDP, Word16 descPort, Word16 srcPort, Word16 PacketSize )
{
	ptrUDP->udp_dest    = SWAP16( descPort );
	ptrUDP->udp_src     = SWAP16( srcPort  );
	ptrUDP->udp_len     = SWAP16( sizeof( UDP_t )  + PacketSize );
	ptrUDP->udp_chksum  = 0;
}

//------------------------------------------------------------------------------
// ���� : Bootp�� ���� �����Ѵ�. 
// �Ű� : ptrBOOTP : BOOTP ���� ���� ���� �ּ� 
//        srcMAC   : �۽� �� MAC ��巹�� 
// ��ȯ : ���� 
// ���� : 
//------------------------------------------------------------------------------
void SetBootpHeader( BOOTP_t *ptrBOOTP , char *srcMAC )
{
        
	ptrBOOTP->bh_opcode = OP_BOOTREQUEST;		// 1 : request		2 : reply.
	ptrBOOTP->bh_htype  = HWT_ETHER;		// 10 Base Ethernet : 1.
	ptrBOOTP->bh_hlen   = HWL_ETHER;		// 10 Base Ethernet : 6.
	ptrBOOTP->bh_hops   = 0;			// client���� 0���� setting. gateway�� ���.
	ptrBOOTP->bh_secs   = SWAP16( TimerGetTime() / TICKS_PER_SECOND ) ;

        // Client �� ��巹���� �����Ѵ�.
	memcpy( ptrBOOTP->bh_chaddr , srcMAC, sizeof( ptrBOOTP->bh_chaddr ) );

        // ���� ���� ��ȣ�� �����Ѵ�. 
        __BootpID++;  
	memcpy(&( ptrBOOTP->bh_tid ), &(__BootpID), 4);

}


//------------------------------------------------------------------------------
// ���� : TFTP ������ ����Ÿ ���ۿ䱸 �������� �����. 
// �Ű� : ptrTFTP  : ���� ���� �ּ� 
//        filename : �䱸�� ȭ�ϸ� 
// ��ȯ : int ���� ũ�� 
// ���� : ����.
//------------------------------------------------------------------------------
int  SetTFTP_RRQPacket( TFTP_Packet_t *ptrTFTP , char *filename  )
{
        int PacketSize = 0;  
 
        // �ڷ� �䱸 ��� 
        ptrTFTP->OPCODE = SWAP16(TFTP_RRQ);       
        PacketSize += 2;

        // �䱸�� ȭ�ϸ��� �����Ѵ�.  
        strcpy( ptrTFTP->Data, filename );        
        PacketSize += ( strlen( filename ) + 1 );

        // ȭ�� ��带 �����Ѵ�. 
        strcpy( ptrTFTP->Data + PacketSize -2, TFTP_FILE_MODE );  
        PacketSize += ( strlen( TFTP_FILE_MODE ) + 1 );


// �ð� �ʰ� ó�� �߰��� ������ 
        
        // Ÿ�Ӿƿ� ���� �����Ѵ�. 
        strcpy( ptrTFTP->Data + PacketSize -2, "timeout" );  
        PacketSize += ( strlen( "timeout" ) + 1 );

        strcpy( ptrTFTP->Data + PacketSize -2, "120" );  
        PacketSize += ( strlen( "120" ) + 1 );
        
#if 0	// ȣȯ�� ������ �߰����� �ʴ´�.
        // ��ũ�⸦ ����
        strcpy( ptrTFTP->Data + PacketSize -2, "blksize" );  
        PacketSize += ( strlen( "blksize" ) + 1 );

        strcpy( ptrTFTP->Data + PacketSize -2, "1024" );  
        PacketSize += ( strlen( "1024" ) + 1 );
#endif
        // ũ�⸦ �䱸�Ѵ�. 
        strcpy( ptrTFTP->Data + PacketSize -2, "tsize" );  
        PacketSize += ( strlen( "tsize" ) + 1 );

        strcpy( ptrTFTP->Data + PacketSize -2, "0" );  
        PacketSize += ( strlen( "0" ) + 1 );

// �߰��� ���볡         

	return PacketSize;
}

//------------------------------------------------------------------------------
// ���� : TFTP ������ ����Ÿ ���� ���� ���� �������� �����. 
// �Ű� : ptrTFTP     : ���� ���� �ּ� 
//        BlockNumber : ���� �� ��ȣ 
// ��ȯ : int ���� ũ�� 
// ���� : ����.
//------------------------------------------------------------------------------
int  SetTFTP_ACKDATAPacket( TFTP_Packet_t *ptrTFTP , Word16 BlockNumber  )
{
        int      PacketSize = 0;  
        Word16  *ptrBlockNumber;

        ptrBlockNumber =  (Word16  *) ptrTFTP->Data;
 
        // �ڷ� �䱸 ��� 
        ptrTFTP->OPCODE = SWAP16(TFTP_ACK);       
        PacketSize += 2;

        // �� �ѹ��� �����Ѵ�. 
        *ptrBlockNumber = SWAP16( BlockNumber );
        PacketSize += 2; 

	return PacketSize;
}

//------------------------------------------------------------------------------
// ���� : TFTP ������ ���� ���� �������� �����. 
// �Ű� : ptrTFTP  : ���� ���� �ּ� 
//        errornum : �䱸�� ȭ�ϸ� 
// ��ȯ : int ���� ũ�� 
// ���� : ����.
//------------------------------------------------------------------------------
int  SetTFTP_ErrorPacket( TFTP_Packet_t *ptrTFTP , int errornum )
{
        int      PacketSize = 0;  
        Word16  *ptrErrorNumber;
        char    *ptrErrStr;

        ptrErrorNumber =  (Word16  *) ptrTFTP->Data;
 
        // �ڷ� �䱸 ��� 
        ptrTFTP->OPCODE = SWAP16(TFTP_ERROR);       
        PacketSize += 2;

        // �� �ѹ��� �����Ѵ�. 
        *ptrErrorNumber = SWAP16(errornum);
        PacketSize += 2; 

        // ���� ��Ʈ���� �ִ´�. 
        ptrErrStr = NULL;

        switch( errornum )
        {
        case 2 : ptrErrStr = "File has bad magic";  break;
        case 3 : ptrErrStr = "File too large" ;      break;
        }  
        
        if( ptrErrStr != NULL )
        {
           strcpy( ptrTFTP->Data + 2, ptrErrStr );
           PacketSize += strlen( ptrErrStr ) + 1; 
        } 

	return PacketSize;
}


