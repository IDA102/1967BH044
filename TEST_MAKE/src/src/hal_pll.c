/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	File/Файл: 		hal_pll.c
 *	Description/Описание: 	HAL for PLL/HAL для модуля PLL
 *
 *	History/История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	changed for the second revision, the macros which defines the frequency of the external clock has been added/изменен под вторую ревизию, добавлен макрос определяющий частоту внешнего источника тактирования
 *				08-May-2015 Andrei Abramchuk 	- 	created/создан
 *
 */
#include <hal_1967VN044.h>

#define FREQ_KHZ_RANGE1         ( 10000 )
#define FREQ_KHZ_RANGE2         ( 16000 )
#define FREQ_KHZ_RANGE3         ( 25000 )
#define FREQ_KHZ_RANGE4         ( 40000 )
#define FREQ_KHZ_RANGE5         ( 65000 )
#define FREQ_KHZ_RANGE6         ( 100000 )

#define PLL_FREQ_KHZ_MIN        ( 400000 )
#define PLL_FREQ_KHZ_MAX        ( 800000 )

#define RESULT_ERROR            ( -1 )

volatile PLL_Freq_type PLL_Freq = { XTI_KHZ, XTI_KHZ/2, XTI_KHZ };

int32_t HAL_PLL_ConfigCalc( PLL_Config_type* pll_cfg, uint32_t ref_freq, uint32_t pll_freq )
{
	uint32_t divr = 1;
	uint32_t divf;
	uint32_t divq = 0;
	uint32_t range;
	uint32_t ivco = 0;
	uint32_t bypass = 0;

	float tmp;

	if ( ( ref_freq < FREQ_KHZ_RANGE1 ) || ( ref_freq / 16 > FREQ_KHZ_RANGE6 ) )
		return RESULT_ERROR;

	if ( ( pll_freq > PLL_FREQ_KHZ_MAX ) || ( pll_freq * 32 < PLL_FREQ_KHZ_MIN ) )
		return RESULT_ERROR;

	while( pll_freq < PLL_FREQ_KHZ_MIN )
	{
		pll_freq *= 2;
		divq += 1;
	}

	while( ( ref_freq / divr >= FREQ_KHZ_RANGE1 ) && ( divr < 16 ) )
	{
		divr += 1;
	}

	divr -= 1;
	tmp = ( float ) ref_freq / ( float ) divr;
	divr -= 1;

	if ( tmp < ( float ) FREQ_KHZ_RANGE2 )
		range = 1;
	else if ( tmp < ( float ) FREQ_KHZ_RANGE3 )
		range = 2;
	else if ( tmp < ( float ) FREQ_KHZ_RANGE4 )
		range = 3;
	else if ( tmp < ( float ) FREQ_KHZ_RANGE5 )
		range = 4;
	else
		range = 5;

	tmp = ( float ) pll_freq / tmp + ( float ) 0.5;
	divf = ( uint32_t ) tmp - 2;

	tmp = ( float ) ref_freq * ( float ) ( divf + 2 ) / ( float ) ( divr + 1 )
			/ ( float ) ( 1 << divq ) + ( float ) 0.5;

	pll_cfg->divr = divr;
	pll_cfg->divf = divf;
	pll_cfg->divq = divq;
	pll_cfg->range = range;
	pll_cfg->ivco = ivco;
	pll_cfg->bypass = bypass;

	pll_cfg->ref_freq_khz = ref_freq;
	pll_cfg->pll_freq_khz = ( uint32_t ) tmp;

	return ( int32_t ) tmp;
}

void HAL_PLL_CoreSwitch( PLL_Config_type* pll_cfg )
{
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) &= ~( CPU_CPLL_SEL );
	*( ( uint32_t* ) PLL_CORE_CFG_LOC ) = pll_cfg->value;
	HAL_SYS_WaitClk( 500 );
	*( ( uint32_t* ) CMU_CFG4_LOC ) |= CPU_CPLL_SEL;												// Switching to the PLL/Переключение на PLL
	HAL_SYS_WaitClk( 500 );
}

void HAL_PLL_LinkSwitch( PLL_Config_type* pll_cfg )
{
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) &= ~( 1 << 7 );
	pll_cfg->bypass = 1;
	*( ( uint32_t* ) PLL_LINK_CFG_LOC ) = pll_cfg->value;
	pll_cfg->bypass = 0;
	*( ( uint32_t* ) PLL_LINK_CFG_LOC ) = pll_cfg->value;
	HAL_SYS_WaitClk( 2000 );
}

void HAL_PLL_BusSwitch( PLL_Config_type* pll_cfg )
{
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) &= ~( CPU_BPLL_SEL );										// BPLL XTI
	pll_cfg->bypass = 1;
	*( ( uint32_t* ) PLL_BUS_CFG_LOC ) = pll_cfg->value;
	pll_cfg->bypass = 0;
	*( ( uint32_t* ) PLL_BUS_CFG_LOC ) = pll_cfg->value;
	HAL_SYS_WaitClk( 2000 );
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) |= ( CPU_BPLL_SEL );										// BPLL SET
}

void HAL_PLL_CoreSetup( uint32_t pll_freq )
{
	PLL_Config_type pll_cfg;
	PLL_Freq.CoreClk = HAL_PLL_ConfigCalc( &pll_cfg, XTI_KHZ, pll_freq );
	PLL_Freq.SocClk = PLL_Freq.CoreClk / 2;
	HAL_PLL_CoreSwitch( &pll_cfg );
}

void HAL_PLL_LinkSetup( uint32_t pll_freq )
{
	PLL_Config_type pll_cfg;
	PLL_Freq.LinkClk = HAL_PLL_ConfigCalc( &pll_cfg, XTI_KHZ, pll_freq );
	HAL_PLL_LinkSwitch( &pll_cfg );
}

void HAL_PLL_BusSetup( uint32_t pll_freq )
{
	PLL_Config_type pll_cfg;
	PLL_Freq.BusClk = HAL_PLL_ConfigCalc( &pll_cfg, XTI_KHZ, pll_freq );
	HAL_PLL_BusSwitch( &pll_cfg );
}

void HAL_XTI_CoreSwitch( void )
{
	PLL_Freq.CoreClk = XTI_KHZ;
	PLL_Freq.SocClk = PLL_Freq.CoreClk / 2;
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) &= ~( CPU_CPLL_SEL );
	HAL_SYS_WaitClk( 100 );
}

void HAL_XTI_BusSwitch( void )
{
	PLL_Freq.BusClk = XTI_KHZ;
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) &= ~( CPU_BPLL_SEL );
}

void HAL_XTI_LinkSwitch( void )
{
	PLL_Config_type pll_cfg;
	PLL_Freq.LinkClk = XTI_KHZ;
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) &= ~( 1 << 7 );											//LPLL_SEL ???? (for Rev. 1) (1<<7)
	pll_cfg.value = *( ( uint32_t* ) PLL_LINK_CFG_LOC );
	pll_cfg.bypass = 1;
	*( ( uint32_t* ) PLL_LINK_CFG_LOC ) = pll_cfg.value;
}

void HAL_PLL_BusSwitchHalfCore( void )
{
	PLL_Freq.BusClk = PLL_Freq.CoreClk / 2;
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) |= ( CPU_BPLL_SEL ) | ( 1 << 4 );							//BPLL = CPLL / 2
}

void HAL_PLL_BusSwitchFourthCore( void )
{
	PLL_Freq.BusClk = PLL_Freq.CoreClk / 4;
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) |= ( CPU_BPLL_SEL ) | ( 2 << 4 );							//BPLL = CPLL / 4
}

void HAL_PLL_BusSwitchEighthCore( void )
{
	PLL_Freq.BusClk = PLL_Freq.CoreClk / 8;
	*( ( uint32_t* ) CPU_CLK_CONFIG_LOC ) |= ( CPU_BPLL_SEL ) | ( 3 << 4 );							//BPLL = CPLL / 8
}
