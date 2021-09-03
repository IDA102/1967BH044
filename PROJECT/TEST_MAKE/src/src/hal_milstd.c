/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	File/Файл: 		hal_milstd.c
 *	Description/описание: 	HAL для модуля контроллера интерфейса по ГОСТ Р 52070-2003
 *
 *	History/История:
 *				26-Jun-2017 Dmitriy Sharaviev 	- 	changed for the second revision/изменен под вторую ревизию
 *						    Zatonsky Pavel 		- 	created/создан
 *
 */
#include "hal_1967VN044.h"

void HAL_MILSTD_Reset( MILSTD_type *MILSTDx )
{
	MILSTDx->CONTROL.b.MR = 1;
}

void HAL_MILSTD_Init( MILSTD_type *MILSTDx, MILSTD_Init_type *InitStruct )
{
	int tmp;
	MILSTDx->CONTROL.b.MR = 0;
	tmp = PLL_Freq.SocClk / 1000;

	if ( tmp < 120 )
		MILSTDx->CONTROL.b.DIV6_0 = tmp;
	else
		MILSTDx->CONTROL.b.DIV6_0 = 0x7F;

	MILSTDx->CONTROL.b.MODE = InitStruct->Mode;
	if ( InitStruct->Mode == MILSTD_Mode_RT )
	{
		MILSTDx->CONTROL.b.TRA = 1;
		MILSTDx->CONTROL.b.TRB = 1;
		MILSTDx->CONTROL.b.RTA = InitStruct->RTA;
	}
	else
		MILSTDx->CONTROL.word |= InitStruct->Channel;

	MILSTDx->CONTROL.b.RERR = InitStruct->ResetErr;
	MILSTDx->CONTROL.b.INVTR = InitStruct->InvPRD;
	MILSTDx->CONTROL.word |= InitStruct->Filter;
	MILSTDx->CONTROL.b.AUTOTUNE = InitStruct->AutoTune;
}

void HAL_MILSTD_ITEnable( MILSTD_type *MILSTDx, MILSTD_IT_type ITName )
{
	MILSTDx->INTEN.word |= ITName;
}

void HAL_MILSTD_ITDisable( MILSTD_type *MILSTDx, MILSTD_IT_type ITName )
{
	MILSTDx->INTEN.word &= ~ITName;
}

void HAL_MILSTD_DeInit( MILSTD_type *MILSTDx )
{
	MILSTDx->CONTROL.b.MR = 1;
	MILSTDx->CONTROL.word = 0;
}

void HAL_MILSTD_CWInit( MILSTD_type *MILSTDx, uint32_t NumCW, uint32_t TxRxMode, uint32_t RTA, uint32_t amount, uint32_t SubAddr )
{
	uint32_t tmp;
	tmp = ( RTA << 11 ) | ( TxRxMode << 10 ) | ( SubAddr << 5 ) | ( amount << 0 );
	switch ( NumCW )
	{
		case 1:
			MILSTDx->COMMWORD1.word = tmp;
			break;
		case 2:
			MILSTDx->COMMWORD2.word = tmp;
			break;
		default:
			break;
	}

}

void HAL_MILSTD_DTWrite( MILSTD_type *MILSTDx, void *Buff, uint32_t amount, uint32_t subaddr )
{
	uint32_t i, *bptr, AddrWD;
	AddrWD = subaddr * 32;
	bptr = ( uint32_t * ) Buff;

	if( amount > 32 )
		amount = 32;

	for ( i = 0; i < amount; i++ )
	{
		MILSTDx->DATA[ AddrWD++ ] = bptr[ i ];
	}
}

void HAL_MILSTD_StartCWTransmit( MILSTD_type *MILSTDx )
{
	MILSTDx->CONTROL.b.BCSTART = 1;
}

void HAL_MILSTD_DRRead( MILSTD_type *MILSTDx, void *Buff, uint32_t amount, uint32_t subaddr )
{
	uint32_t i, *bptr, AddrWD;
	AddrWD = subaddr * 32;
	bptr = ( uint32_t * ) Buff;

	if( amount > 32 )
		amount = 32;

	for ( i = 0; i < amount; i++ )
	{
		bptr[ i ] = MILSTDx->DATA[ AddrWD++ ];
	}
}
