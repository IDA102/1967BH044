/*
 *
 *	File/Файл: 		hal_arinc.с
 *	Description/Описание: 	HAL for ARINC/ HAL для модуля контроллера интерфейса ARINC
 *
 *	History/История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	frequency of the peripheral bus is received out of PLL_Freq/частота шины периферии беретс¤ из PLL_Freq
 *						    Zatonsky Pavel 		- 	created/создан
 *
 */

#include "hal_1967VN044.h"

void HAL_ARINC_RX_Init( ARINC_RX_type *LX_ARINC_RX_CHx, ARINC_RX_Init_type *InitStruct )
{
	LX_ARINC_RX_CHx->RX_CONTROL.b.DIV = PLL_Freq.SocClk / 1000;
	LX_ARINC_RX_CHx->RX_CONTROL.b.CLK = InitStruct->Clk;
	LX_ARINC_RX_CHx->RX_CONTROL.b.LB_EN = InitStruct->LB;
	LX_ARINC_RX_CHx->RX_CONTROL.b.SD_EN = InitStruct->SD;
	LX_ARINC_RX_CHx->RX_CONTROL.b.DA = InitStruct->DA;
	LX_ARINC_RX_CHx->RX_CONTROL.b.SDI1 = InitStruct->SDI1;
	LX_ARINC_RX_CHx->RX_CONTROL.b.SDI2 = InitStruct->SDI2;
	LX_ARINC_RX_CHx->RX_CONTROL.b.ENSYNC = InitStruct->Sync;
	LX_ARINC_RX_CHx->RX_CONTROL.b.INTEDR = InitStruct->ITnEmptyFIFO;
	LX_ARINC_RX_CHx->RX_CONTROL.b.INTEER = InitStruct->ITErr;
	LX_ARINC_RX_CHx->RX_CONTROL.b.INTEFF = InitStruct->ITFullFIFO;
	LX_ARINC_RX_CHx->RX_CONTROL.b.INTEHF = InitStruct->ITHalfFIFO;
	switch ( InitStruct->Parity )
	{
		case ARINC_Parity_Dis:
			LX_ARINC_RX_CHx->RX_CONTROL.b.ENPAR = 0;
			break;
		case ARINC_Parity_Odd:
			LX_ARINC_RX_CHx->RX_CONTROL.b.ENPAR = 1;
			LX_ARINC_RX_CHx->RX_CONTROL.b.ODD = 1;
			break;
		case ARINC_Parity_Even:
			LX_ARINC_RX_CHx->RX_CONTROL.b.ENPAR = 1;
			LX_ARINC_RX_CHx->RX_CONTROL.b.ODD = 0;
			break;
	}
}

void HAL_ARINC_RX_DeInit( ARINC_RX_type *LX_ARINC_RX_CHx )
{
	LX_ARINC_RX_CHx->RX_CONTROL.word = 0;
}

void HAL_ARINC_RX_ITEn( ARINC_RX_type *LX_ARINC_RX_CHx, ARINC_RX_IT ITName )
{
	switch ( ITName )
	{
		case ARINC_RX_IT_nEmptyFIFO:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEDR = 1;
			break;
		case ARINC_RX_IT_Err:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEER = 1;
			break;
		case ARINC_RX_IT_HalfFIFO:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEHF = 1;
			break;
		case ARINC_RX_IT_FullFIFO:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEFF = 1;
			break;
	}
}

void HAL_ARINC_RX_ITDis( ARINC_RX_type *LX_ARINC_RX_CHx, ARINC_RX_IT ITName )
{
	switch ( ITName )
	{
		case ARINC_RX_IT_nEmptyFIFO:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEDR = 0;
			break;
		case ARINC_RX_IT_Err:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEER = 0;
			break;
		case ARINC_RX_IT_HalfFIFO:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEHF = 0;
			break;
		case ARINC_RX_IT_FullFIFO:
			LX_ARINC_RX_CHx->RX_CONTROL.b.INTEFF = 0;
			break;
	}
}

uint32_t HAL_ARINC_RX_AllDataRead( ARINC_RX_type *LX_ARINC_RX_CHx, void *PtrRxArr )
{
	uint32_t *RxArr;
	RxArr = ( uint32_t * ) PtrRxArr;
	while( LX_ARINC_RX_CHx->RX_STATUS.b.DR )
	{
		*RxArr++ = LX_ARINC_RX_CHx->RX_DATA;
	}
	return ( RxArr - ( uint32_t * ) PtrRxArr );
}

void HAL_ARINC_RX_MDataRead( uint32_t *LX_ARINC_RX_MEMx, void *PtrRxArr, uint32_t Size )
{
	uint32_t *RxArr, i;
	RxArr = ( uint32_t * ) PtrRxArr;
	for ( i = 0; i < Size; i++ )
	{
		RxArr[ i ] = LX_ARINC_RX_MEMx[ i ];
	}
}

void HAL_ARINC_RX_DataRead( ARINC_RX_type *LX_ARINC_RX_CHx, void *PtrRxArr, uint32_t Size )
{
	uint32_t *RxArr, i;
	RxArr = ( uint32_t * ) PtrRxArr;
	for ( i = 0; i < Size; i++ )
	{
		RxArr[ i ] = LX_ARINC_RX_CHx->RX_DATA;
	}
}

void HAL_ARINC_RX_Enable( ARINC_RX_type *LX_ARINC_RX_CHx )
{
	LX_ARINC_RX_CHx->RX_CONTROL.b.CH_EN = 1;
}

void HAL_ARINC_RX_Disable( ARINC_RX_type *LX_ARINC_RX_CHx )
{
	LX_ARINC_RX_CHx->RX_CONTROL.b.CH_EN = 0;
}

void HAL_ARINC_RX_LabelSet( uint32_t *LX_ARINC_RX_LABELx, uint32_t *CorrectLabel, uint32_t Size )
{
	int32_t i;
	for ( i = 0; i < Size; i++ )
	{
		if ( CorrectLabel[ i ] < 32 )
			LX_ARINC_RX_LABELx[ 0 ] |= ( 1 << CorrectLabel[ i ] );
		else if ( CorrectLabel[ i ] < 64 )
			LX_ARINC_RX_LABELx[ 1 ] |= ( 1 << ( CorrectLabel[ i ] - 32 ) );
		else if ( CorrectLabel[ i ] < 96 )
			LX_ARINC_RX_LABELx[ 2 ] |= ( 1 << ( CorrectLabel[ i ] - 64 ) );
		else if ( CorrectLabel[ i ] < 128 )
			LX_ARINC_RX_LABELx[ 3 ] |= ( 1 << ( CorrectLabel[ i ] - 96 ) );
		else if ( CorrectLabel[ i ] < 160 )
			LX_ARINC_RX_LABELx[ 4 ] |= ( 1 << ( CorrectLabel[ i ] - 128 ) );
		else if ( CorrectLabel[ i ] < 192 )
			LX_ARINC_RX_LABELx[ 5 ] |= ( 1 << ( CorrectLabel[ i ] - 160 ) );
		else if ( CorrectLabel[ i ] < 224 )
			LX_ARINC_RX_LABELx[ 6 ] |= ( 1 << ( CorrectLabel[ i ] - 192 ) );
		else if ( CorrectLabel[ i ] < 256 )
			LX_ARINC_RX_LABELx[ 7 ] |= ( 1 << ( CorrectLabel[ i ] - 224 ) );
	}
}

void HAL_ARINC_RX_LabelReset( uint32_t *LX_ARINC_RX_LABELx, uint32_t *IncorrectLabel, uint32_t Size )
{
	int32_t i;
	for ( i = 0; i < Size; i++ )
	{
		if ( IncorrectLabel[ i ] < 32 )
			LX_ARINC_RX_LABELx[ 0 ] &= ( 1 << IncorrectLabel[ i ] );
		else if ( IncorrectLabel[ i ] < 64 )
			LX_ARINC_RX_LABELx[ 1 ] &= ~( 1 << ( IncorrectLabel[ i ] - 32 ) );
		else if ( IncorrectLabel[ i ] < 96 )
			LX_ARINC_RX_LABELx[ 2 ] &= ~( 1 << ( IncorrectLabel[ i ] - 64 ) );
		else if ( IncorrectLabel[ i ] < 128 )
			LX_ARINC_RX_LABELx[ 3 ] &= ~( 1 << ( IncorrectLabel[ i ] - 96 ) );
		else if ( IncorrectLabel[ i ] < 160 )
			LX_ARINC_RX_LABELx[ 4 ] &= ~( 1 << ( IncorrectLabel[ i ] - 128 ) );
		else if ( IncorrectLabel[ i ] < 192 )
			LX_ARINC_RX_LABELx[ 5 ] &= ~( 1 << ( IncorrectLabel[ i ] - 160 ) );
		else if ( IncorrectLabel[ i ] < 224 )
			LX_ARINC_RX_LABELx[ 6 ] &= ~( 1 << ( IncorrectLabel[ i ] - 192 ) );
		else if ( IncorrectLabel[ i ] < 256 )
			LX_ARINC_RX_LABELx[ 7 ] &= ~( 1 << ( IncorrectLabel[ i ] - 224 ) );
	}
}

void HAL_ARINC_RX_LabelAllReset( uint32_t *LX_ARINC_RX_LABELx )
{
	int32_t i;
	for ( i = 0; i < 8; i++ )
	{
		LX_ARINC_RX_LABELx[ i ] = 0;
	}
}

void HAL_ARINC_TX_Init( ARINC_TX_type *LX_ARINC_TX_CHx, ARINC_TX_Init_type *InitStruct )
{
	LX_ARINC_TX_CHx->TX_CONTROL.b.DIV = PLL_Freq.SocClk / 1000;
	LX_ARINC_TX_CHx->TX_CONTROL.b.CLK = InitStruct->Clk;
	LX_ARINC_TX_CHx->TX_CONTROL.b.ENSYNC = InitStruct->Sync;
	LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_TXR = InitStruct->ITEmptyFIFO;
	LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_FFT = InitStruct->ITFullFIFO;
	LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_HFT = InitStruct->ITHalfFIFO;
	switch ( InitStruct->Parity )
	{
		case ARINC_Parity_Dis:
			LX_ARINC_TX_CHx->TX_CONTROL.b.EN_PAR = 0;
			break;
		case ARINC_Parity_Odd:
			LX_ARINC_TX_CHx->TX_CONTROL.b.EN_PAR = 1;
			LX_ARINC_TX_CHx->TX_CONTROL.b.ODD = 1;
			break;
		case ARINC_Parity_Even:
			LX_ARINC_TX_CHx->TX_CONTROL.b.EN_PAR = 1;
			LX_ARINC_TX_CHx->TX_CONTROL.b.ODD = 0;
			break;
	}
}

void HAL_ARINC_TX_DeInit( ARINC_TX_type *LX_ARINC_TX_CHx )
{
	LX_ARINC_TX_CHx->TX_CONTROL.word = 0;
}

void HAL_ARINC_TX_ITEn( ARINC_TX_type *LX_ARINC_TX_CHx, ARINC_TX_IT ITName )
{
	switch ( ITName )
	{
		case ARINC_TX_IT_EmptyFIFO:
			LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_TXR = 1;
			break;
		case ARINC_TX_IT_HalfFIFO:
			LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_HFT = 1;
			break;
		case ARINC_TX_IT_FullFIFO:
			LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_FFT = 1;
			break;
	}
}

void HAL_ARINC_TX_ITDis( ARINC_TX_type *LX_ARINC_TX_CHx, ARINC_TX_IT ITName )
{
	switch ( ITName )
	{
		case ARINC_TX_IT_EmptyFIFO:
			LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_TXR = 0;
			break;
		case ARINC_TX_IT_HalfFIFO:
			LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_HFT = 0;
			break;
		case ARINC_TX_IT_FullFIFO:
			LX_ARINC_TX_CHx->TX_CONTROL.b.INTE_FFT = 0;
			break;
	}
}

void HAL_ARINC_TX_DataWrite( ARINC_TX_type *LX_ARINC_TX_CHx, void *PtrTxArr, uint32_t Size )
{
	uint32_t i, *TxArr;
	TxArr = ( uint32_t * ) PtrTxArr;
	for ( i = 0; i < Size; i++ )
	{
		while( LX_ARINC_TX_CHx->TX_STATUS.b.BUSY | LX_ARINC_TX_CHx->TX_STATUS.b.FFT )
			;
		LX_ARINC_TX_CHx->TX_DATA = TxArr[ i ];
	}
}

void HAL_ARINC_TX_Enable( ARINC_TX_type *LX_ARINC_TX_CHx )
{
	LX_ARINC_TX_CHx->TX_CONTROL.b.CH_EN = 1;
}

void HAL_ARINC_TX_Disable( ARINC_TX_type *LX_ARINC_TX_CHx )
{
	LX_ARINC_TX_CHx->TX_CONTROL.b.CH_EN = 0;
}
