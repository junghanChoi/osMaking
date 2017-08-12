//------------------------------------------------------------------------------
// ȭ�ϸ� : cs8900.c
// ��  �� : �̴���Ĩ cs8900�� �����Ѵ�.
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
// �ۼ��� : 2001�� 10�� 8��
// ��  �� : 2003�� 05�� 20��  pxa255 �� ���� ����
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
#include <time.h>
#include <mem_map.h>
#include <cs8900.h>
#include <config.h>
//******************************************************************************
//
// ���� ���� ����
//
//******************************************************************************


//------------------------------------------------------------------------------
// ���� : ������ �� ��巹���� �ּҸ� ��´�. 
// �Ű� : ����
// ��ȯ : �� ��巹�� 
// ���� : ����
//------------------------------------------------------------------------------
Byte *CS8900_GetMacAddress( void ) 
{
  	return Cfg.Local_MacAddress;
}
//------------------------------------------------------------------------------
// ���� : IO mode�� packet page point�� addr(packet page address)�� ����, 
//        IO mode�� packet page data�� value(packet page data)�� �Ἥ ���� �ٲ۴�.
// �Ű� : addr  : packet page �ּ�.
//        value : packet page �ּ��� ��ġ�� �� ��.
// ��ȯ : ����.
// ���� : ����.
//------------------------------------------------------------------------------
static void CS8900_WriteToPPR(short addr, short value)
{
	CS8900_PPPTR = addr;
	CS8900_PPDATA = value;
	return;
}	
//------------------------------------------------------------------------------
// ���� : IO mode�� packet page point�� addr(packet page address)�� ����, 
//        IO mode�� packet page data���� ���� �о� return�Ѵ�.
// �Ű� : addr  : packet page �ּ�.
// ��ȯ : packet page addr�� �ִ� ��.
// ���� : ���� 
//------------------------------------------------------------------------------
static short CS8900_ReadFromPPR( short addr )
{
	CS8900_PPPTR = addr;
	return CS8900_PPDATA;
}
//------------------------------------------------------------------------------
// ���� : packet�� ����.
//        ���� ������ �������� Ȯ���� �� IO_RTX_DATA�� packet�� �ݺ������� ���.
// �Ű� : txPktBuf : ������ ���� ��巹�� 
//        len      : ���� ũ��  
// ��ȯ : packet page addr�� �ִ� ��.
// ���� : ���� 
//------------------------------------------------------------------------------
BOOL CS8900_Transmit( void *txPktBuf, int len )
{
	short   *endPtr, *s;
	unsigned int tick = 0;

	// Ethernet Tx on.
	CS8900_PPPTR = PP_LineCTL;
	CS8900_PPDATA |= SERIAL_TX_ON;

	// packet ��ü�� CS8900A�� ������ �� ���۽���.
	CS8900_TXCMD = TX_AFTER_ALL;
	CS8900_TXLEN = len;

	// Tx�� �غ�� ������ ���.
//printf( "TX-Enter ");
	while ((CS8900_ReadFromPPR(PP_BusST) & READY_FOR_TX_NOW)==0)
	{
	        tick++;
	        if ( 0 == tick )
	        {
	                printf( " Error TX BIT\n");
	                return FALSE;
	        }
	}
//printf( " TX-Exit\n");

	// CS8900���� packet ����. �̰��� ���� �� CS8900A�� RJ45�� packet ������ ������.
	for (s=txPktBuf, endPtr=txPktBuf+len; s<endPtr; s++)
        {
		CS8900_RTX_DATA = *s;
                // printf( " %04X", SWAP16(*s) & 0xFFFF );
	}

	return TRUE;
}	
//------------------------------------------------------------------------------
// ���� : �� ������ ��Ŷ�� �����Ѵ�. 
//        packet ������ ���� ���� IO_RTX_DATA���� state, length, packet�� ������ �о� ��.
// �Ű� : 
// ��ȯ : ���� ���� ���� ���� / ������ 0 
// ���� : ip�� ��� udp���� check�� �� ������ ó���� �� �ִ� receive �Լ� ȣ��.
//------------------------------------------------------------------------------
int CS8900_Resive(void *rxPktBuf, int max_size )
{
	int	i;
	Word16	*s;
	short	isq, status, len, cnt;

        isq=CS8900_ISQ;

	// ���� ������ �߻��Ͽ��°��� �����Ѵ�. 
	if( isq & ISQ_RX_MISS_EVENT )
	{
		 printf( "Miss RX\n" );
		 return 0;
	}	 

	// ���� �̺�Ʈ�� �߻��Ͽ��°��� �����Ѵ�. 
	if( isq & ISQ_RECEIVER_EVENT )
        {
            
	    // ���� ���¸� ��´�. 
	    status = CS8900_RTX_DATA;
            len = CS8900_RTX_DATA;  // ���ŵ� ���̸� ��´�. 
            
                // printf( "RESIVE EVENT [%04X]\n", status );  
                // printf( "RESIVE LENGTH %d \n", len ); 
           
            // ����Ÿ�� Ʋ������ �ϴ� �о� �´�. 
	    cnt = len/2 + len%2;
	    for (i=0, s=rxPktBuf; i<cnt; i++)
            {
                 *s = CS8900_RTX_DATA;
                  s++;
                        // printf( " [%04X]", SWAP16(*s) );
            }
            if( status & RX_OK ) return len;
	}
	return 0;

}

//------------------------------------------------------------------------------
// ���� : CS8900�� ��Ʈ��ũ�� ��ũ �Ǿ� �ִ°��� Ȯ���Ѵ�. 
// �Ű� : 
// ��ȯ : ��ũ �Ǿ� ������ TRUE �����ϸ� FALSE�� ��ȯ�Ѵ�. 
// ���� : 
//------------------------------------------------------------------------------
BOOL  CS8900_CheckLink( void )
{
        Word16 LineState;

        LineState = CS8900_ReadFromPPR( PP_LineST  );
                //printf( "CS8900 Line Link Check\n" ); // ������ 

	// check Link on.
	if( ! ( LineState & LINK_OK ) )
        {
 		return FALSE; //printf("Ethernet Link failed. Check line.\n");
 	}
        return TRUE;  //printf("Ethernet Link Ok\n");
}

//------------------------------------------------------------------------------
// ���� : CS8900�� ���ͷ�Ʈ�� ���� �ʴ� IO���� �ʱ�ȭ �Ѵ�. 
// �Ű� : 
// ��ȯ : �����ϸ� TRUE �����ϸ� FALSE�� ��ȯ�Ѵ�. 
// ���� : 
//------------------------------------------------------------------------------
BOOL CS8900_Init( void )
{

        int lp;
	
        Word32 DectectID;
        Word16 ChipIDLow;
        Word16 ChipIDHigh;
        
        printf( "CS8900 Init............................\n" );

        printf( "  Mac Address  : " );
        
        printf( "[%02X %02X %02X %02X %02X %02X]\n", Cfg.Local_MacAddress[0],Cfg.Local_MacAddress[1],Cfg.Local_MacAddress[2],
                                                     Cfg.Local_MacAddress[3],Cfg.Local_MacAddress[4],Cfg.Local_MacAddress[5] ); 

	// ������ ���������� ���� �о� CS8900 �� �����ϴ��� Ȯ���Ѵ�.
	
        CS8900_ENABLE_WORD;        // WORD �＼���� �̳��̺� ��Ų��.  SHBE pin ��Ŭ 
        
        DectectID = CS8900_PPPTR;  // ID�� ��� �´�. 

        printf( "  Detect value : [%04X:%04X]\n", CS8900_DECTECT_MASK, DectectID & CS8900_DECTECT_MASK );
        
        if( ( DectectID & CS8900_DECTECT_MASK ) != CS8900_DECTECT_MASK )
        {
		    printf( "  Can't access to Memory of CS8900A.\n");
		    return FALSE;
        }

        // CS8900�� Chip ID �� Ȯ���Ѵ�. 
        ChipIDLow  = CS8900_ReadFromPPR( PP_ChipID_LOW  );
        ChipIDHigh = CS8900_ReadFromPPR( PP_ChipID_HIGH );

        // ����Ʈ ������ �����Ѵ�. 
        ChipIDHigh  = SWAP16( ChipIDHigh  );
        ChipIDLow   = SWAP16( ChipIDLow   );

        printf( "  Chip ID      : [%04X:%04X]\n", ChipIDHigh, ChipIDLow ); // ������ 
        //printf( "       PRODUCT ID    : [%04X]\n", ChipIDHigh );                 // Ĩ EISA�� ��ϵ� ��ǰ ��ȣ  
        //printf( "       VERSION       : [%04X]\n", ChipIDLow & 0x1F );           // Ĩ ���� 

	if ( ChipIDHigh != CHECK_CHIP_ID_HIGH )
        {
		printf("  Ethernet Chip is not CS8900A. Are you use CS8900A.\n");
		return FALSE;
	}

	// �ۼ����� �Ұ����ϰ� �Ѵ�. 
	CS8900_WriteToPPR( PP_LineCTL, 0x0000 );  

        // ��� ���ͷ�Ʈ�� ���� ��Ų��. 
	CS8900_WriteToPPR(PP_CS8900_ISAINT, INTRQ_HIGH_IMPEDENCE );   // Interrupt number.  
	CS8900_WriteToPPR(PP_CS8900_ISADMA, DMRQ_HIGH_IMPEDENCE  );   // DMA Channel Number.

        // ���� ������� �ۼ����� �����ϵ��� ���� �� ���� �������͸� �ʱ�ȭ �Ѵ�. 
        CS8900_WriteToPPR(PP_RxCFG,	RX_OK_ENBL | RX_CRC_ERROR_ENBL);  
        CS8900_WriteToPPR(PP_RxCTL,	RX_OK_ACCEPT | RX_MULTICAST_ACCEPT | RX_IA_ACCEPT | RX_BROADCAST_ACCEPT);
	CS8900_WriteToPPR(PP_TxCFG,	TX_LOST_CRS_ENBL | TX_SQE_ERROR_ENBL | TX_OK_ENBL | TX_LATE_COL_ENBL | TX_JBR_ENBL | TX_ANY_COL_ENBL);
	CS8900_WriteToPPR(PP_TxCMD,	TX_START_ALL_BYTES);
	CS8900_WriteToPPR(PP_BufCFG,	0x0000);
	CS8900_WriteToPPR(PP_BusCTL,	ENABLE_IRQ | IO_CHANNEL_READY_ON);
	CS8900_WriteToPPR(PP_TestCTL,	0x0000);

        // ���� ��� �ʱ�ȭ 
	CS8900_WriteToPPR(PP_TxCommand,0x00c0);

        // MAC ��巹�� ���� ���� ����ũ ���� 
        // ���⼭�� ��� ������ �㰡�ϰ� �����Ѵ�. 
	for ( lp = 0 ; lp < 4 ; lp++ ) CS8900_WriteToPPR( PP_LAF+lp*2, 0xFFFF );

        // MAC ��巹���� �����Ѵ�.  
        for ( lp = 0 ; lp < 3 ; lp++ ) CS8900_WriteToPPR( PP_IA+lp*2, *((Word16 *)&( Cfg.Local_MacAddress[lp*2])));

        // �ۼ����� �����ϰ� �����. 
        CS8900_WriteToPPR(PP_LineCTL, SERIAL_RX_ON | SERIAL_TX_ON);

   
        //printf( "  INIT OK!\n\n" );

	return TRUE;

}	


void  CS8900_SwReset( void )
{
        CS8900_Init();
        // ���� ��� �ʱ�ȭ 
//	CS8900_WriteToPPR(PP_TxCommand,0x00c0);
}

