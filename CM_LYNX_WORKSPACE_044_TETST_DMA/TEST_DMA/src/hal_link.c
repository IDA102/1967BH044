/*
 *
 *	File/Файл: 		hal_link.c
 *	Description/Описание: 	HAL for ARINC/HAL для модуля контроллера интерфейса ARINC
 *
 *	History/История:
 *					Zatonsky Pavel 		- 	reated/создан
 *
 */
#include <sysreg.h>
#include <builtins.h>
#include "hal_1967VN044.h"

int32_t HAL_LinkRx_GetStat( uint32_t link_num )
{
	switch ( link_num )
	{
		case 0:
			return ( uint32_t ) __builtin_sysreg_read( __LRSTAT0 );
		case 1:
			return ( uint32_t ) __builtin_sysreg_read( __LRSTAT1 );
		default:
			return -1;
	}
}

int32_t HAL_LinkTx_GetStat( uint32_t link_num )
{
	switch ( link_num )
	{
		case 0:
			return ( uint32_t ) __builtin_sysreg_read( __LTSTAT0 );
		case 1:
			return ( uint32_t ) __builtin_sysreg_read( __LTSTAT1 );
		default:
			return -1;
	}
}

int32_t HAL_LinkRx_GetStatCl( uint32_t link_num )
{
	switch ( link_num )
	{
		case 0:
			return ( uint32_t ) __builtin_sysreg_read( __LRSTATC0 );
		case 1:
			return ( uint32_t ) __builtin_sysreg_read( __LRSTATC1 );
		default:
			return -1;
	}
}

int32_t HAL_LinkTx_GetStatCl( uint32_t link_num )
{
	switch ( link_num )
	{
		case 0:
			return ( uint32_t ) __builtin_sysreg_read( __LTSTATC0 );
		case 1:
			return ( uint32_t ) __builtin_sysreg_read( __LTSTATC1 );
		default:
			return -1;
	}
}

int32_t HAL_LinkRx_Enable( uint32_t link_num, LinkRx_Init_type *LRxInit, LinkRxEx_type *LRxExInit )
{
	unsigned int tmpreg = 0;

	tmpreg |= LRxInit->CheckSum;																	// Check Sum
	tmpreg |= LRxInit->CheckBCMPI;																	// Check BCMPI
	tmpreg |= LRxInit->TOIntEn;																		// Time Out Interrupt
	tmpreg |= LRxInit->OVRIntEn;																	// Overwrite Interrupt
	tmpreg |= LRxInit->DataSize;																	// Data Size

	if ( LRxExInit )
	{
		tmpreg |= LRxExInit->GpsClk;
		tmpreg |= LRxExInit->RxDest;
		tmpreg |= LRxExInit->AdcDataSize;
		tmpreg |= LRxExInit->Rcode;
		tmpreg |= LRxExInit->BitOrder;
	}

	switch ( link_num )																				// Link Num
	{
		case 0:
			//*(unsigned *)GPC_ALT_LOAD_LOC |= (1<<24); //ALT Func for GPC_24 = ACKO 0
			__builtin_sysreg_write( __LRCTL0, tmpreg );
			tmpreg |= LRX_EN;
			__builtin_sysreg_write( __LRCTL0, tmpreg );
			break;
		case 1:
			//*(unsigned *)GPC_ALT_LOAD_LOC |= (1<<28); //ALT Func for GPC_28 = ACKO 1
			tmpreg |= ( 1 << 16 ); 																	//	CLK1 Source for Link1
			__builtin_sysreg_write( __LRCTL1, tmpreg );
			tmpreg |= LRX_EN;
			__builtin_sysreg_write( __LRCTL1, tmpreg );
			break;
		default:
			return -1;
	}
	return 0;
}

void HAL_LinkRx_Disable( uint32_t link_num )
{
	switch ( link_num )
	{
		case 0:
			__builtin_sysreg_write( __LRCTL0, 0 );
			break;
		case 1:
			__builtin_sysreg_write( __LRCTL1, 0 );
			break;
	}
}

int32_t HAL_LinkCore_Receive( uint32_t link_num, void *BuffPtr, uint32_t size )
{
	__builtin_quad bq;
	uint32_t i, *bqPtr, *bp, LRStat;
	bp = ( uint32_t * ) BuffPtr;
	size >>= 2; 																					// size/4
	switch ( link_num )
	{
		case 0:
			LRStat = __builtin_sysreg_read( __LRSTAT0 );
			if ( !( LRStat & 0x40 ) ) return -1;
			for ( i = 0; i < size; i++ )
			{
				while( ( LRStat & 0x41 ) != 0x41 )
				{
					LRStat = __builtin_sysreg_read( __LRSTAT0 );
					if ( LRStat & 0x3C ) return -1;
				}
				bq = __builtin_sysreg_read4( __LBUFRX0 );
				bqPtr = (uint32_t*) ( &bq );
				*( bp + 0 ) = *( bqPtr + 0 );
				*( bp + 1 ) = *( bqPtr + 1 );
				*( bp + 2 ) = *( bqPtr + 2 );
				*( bp + 3 ) = *( bqPtr + 3 );
				bp += 4;
			}
			break;
		case 1:
			LRStat = __builtin_sysreg_read( __LRSTAT1 );
			if ( !( LRStat & 0x40 ) ) return -1;
			for ( i = 0; i < size; i++ )
			{
				while( ( LRStat & 0x41 ) != 0x41 )
				{
					LRStat = __builtin_sysreg_read( __LRSTAT1 );
					if ( LRStat & 0x3C ) return -1;
				}
				bq = __builtin_sysreg_read4( __LBUFRX1 );
				bqPtr = (uint32_t*) ( &bq );
				*( bp + 0 ) = *( bqPtr + 0 );
				*( bp + 1 ) = *( bqPtr + 1 );
				*( bp + 2 ) = *( bqPtr + 2 );
				*( bp + 3 ) = *( bqPtr + 3 );
				bp += 4;
			}
			break;
	}
	return 0;
}

void HAL_LinkDMA_Receive( uint32_t link_num, void *tcb )
{
	HAL_DMA_RqstSet( 8 + link_num, dmaSTD );
	HAL_DMA_WriteDC( 8 + link_num, tcb );
}

int32_t HAL_LinkTx_Enable( uint32_t link_num, LinkTx_Init_type *LTxInit, LinkTxEx_type *LTxExInit )
{
	uint32_t tmpreg = 0;

	tmpreg |= LTxInit->CheckSum;																	// Check Sum
	tmpreg |= LTxInit->CheckBCMPO;																	// Check BCMPI
	tmpreg |= LTxInit->TOIntEn;																		// Time Out Interrupt
	tmpreg |= LTxInit->DataSize;																	// Data Size
	tmpreg |= LTxInit->ClkSource;																	// Link Clk Source

	if ( LTxExInit )
	{
		tmpreg |= LTxExInit->TxSource;
		tmpreg |= LTxExInit->Tcode;
		tmpreg |= LTxExInit->CheckACKI;
	}

	switch ( link_num )																				// Link Num
	{
		case 0:
			__builtin_sysreg_write( __LTCTL0, tmpreg );
			tmpreg |= LTX_EN;
			__builtin_sysreg_write( __LTCTL0, tmpreg );
			break;
		case 1:
			__builtin_sysreg_write( __LTCTL1, tmpreg );
			tmpreg |= LTX_EN;
			__builtin_sysreg_write( __LTCTL1, tmpreg );
			break;
		default:
			return -1;
	}
	return 0;
}

void HAL_LinkTx_Disable( uint32_t link_num )
{
	switch ( link_num )
	{
		case 0:
			//	*(unsigned *)GPC_ALT_LOAD_LOC &= ~(0xA<<24); //ALT Func for GPC_24,26 = Disable
			__builtin_sysreg_write( __LTCTL0, 0 );
			break;
		case 1:
			//	*(unsigned *)GPC_ALT_LOAD_LOC &= ~(0xA<<28); //ALT Func for GPC_28,30 = Disable
			__builtin_sysreg_write( __LTCTL1, 0 );
			break;
	}
}

int32_t HAL_LinkCore_Transmit( uint32_t link_num, void *BuffPtr, uint32_t size )
{
	int *bp, i, LTStat = 0;
	bp = ( int * ) BuffPtr;
	size >>= 2;
	switch ( link_num )
	{
		case 0:
			for ( i = 0; i < size; i++ )
			{
				while( LTStat != 3 )
				{
					LTStat = __builtin_sysreg_read( __LTSTAT0 );
					if ( LTStat & 0xC ) return -1;
				}
				__builtin_sysreg_write4( __LBUFTX0, *( __builtin_quad * ) bp );
				bp += 4;
			}
			break;
		case 1:
			for ( i = 0; i < size; i++ )
			{
				while( LTStat != 3 )
				{
					LTStat = __builtin_sysreg_read( __LTSTAT1 );
					if ( LTStat & 0xC ) return -1;
				}
				__builtin_sysreg_write4( __LBUFTX1, *( __builtin_quad * ) bp );
				bp += 4;
			}
			break;
		default:
			return -1;
	}
	return 0;
}

void HAL_LinkDMA_Transmit( uint32_t link_num, void *tcb )
{
	HAL_DMA_RqstSet( 4 + link_num, dmaSTD );
	HAL_DMA_WriteDC( 4 + link_num, tcb );
}

