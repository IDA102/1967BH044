/*
 *
 *	File/Файл: 		hal_sys.c
 *	Description/Описание: 	HAL for the external RAM/HAL для внешней шины памяти
 *
 *	History/История:
 *						28-Jun-2017 Dmitriy Sharaviev 	- 	changed for the second revision, the function of regeneration period setting has been added/ изменен под вторую ревизию, добавлена функция задания периода регенерации
 *									Zatonsky Pavel 		- 	created/создан
 *
 */
#include <sysreg.h>
#include <builtins.h>
#include <stdint.h>
#include "hal_1967VN044.h"

#define TIC_PERIOD_GCF	16000
uint64_t ccntStart, ccntStop = 0;

// External bus enable/ Включение внешнего порта
void HAL_SYS_ExtBusEnable( SYS_BusSize_type size )
{
	uint32_t tmp = 0;
	switch ( size )
	{
		case SYS_BUS_16b:
			tmp = PX_ALT_PDB0 | PX_ALT_PDB1 | PX_ALT_PAB0 | PX_ALT_PAB1 | PX_ALT_PAB2;
			break;
		case SYS_BUS_32b:
			tmp = PX_ALT_PDB0 | PX_ALT_PDB1 | PX_ALT_PDB2 | PX_ALT_PDB3 | PX_ALT_PAB0 | PX_ALT_PAB1	| PX_ALT_PAB2;
			break;
	}
	*( uint32_t * ) PX_ALT_LOC = tmp;
}

// External bus disable/Выключение внешнего порта
void HAL_SYS_ExtBusDisable( void )
{
	*( uint32_t * ) PX_ALT_LOC = 0;
}

// SDRAM Enable/ Включение SDRAM
void HAL_SYS_SDRAMEnable( uint32_t sdrconValue )
{
	*( uint32_t * ) GPC_ALT_SET_LOC = ( 0x3FF << 8 );
	__builtin_sysreg_write( __SDRCON, sdrconValue );
	while( !( __builtin_sysreg_read( __SYSTAT ) & ( 1 << 13 ) ) );
}

// Setting of the SDRAM regeneration period/ Задание периода регенерации SDRAM
void HAL_SYS_SDRAMRegenPer( uint16_t RegenPer )
{
	*( uint16_t* ) SD_REP_LOC = RegenPer;
}

// SDRAM disbale/ Выключение SDRAM
void HAL_SYS_SDRAMDisable( void )
{
	*( uint32_t * ) GPC_ALT_CLEAR_LOC = ( 0x3FF << 8 );
	__builtin_sysreg_write( __SDRCON, 0 );
}

// SRAM enable/ Включение SRAM
void HAL_SYS_SRAMEnable( uint32_t sysconValue )
{
	*( uint32_t * ) GPC_ALT_SET_LOC = ( 0x1F << 18 );
	__builtin_sysreg_write( __SYSCON, sysconValue );
}

// SRAM disable/ Выключение SRAM
void HAL_SYS_SRAMDisable( void )
{
	*( uint32_t * ) GPC_ALT_CLEAR_LOC = ( 0x1F << 18 );
	__builtin_sysreg_write( __SYSCON, 0 );
}

// FLAG enable/ Включение FLAG
void HAL_SYS_FlagEnable( void )
{
	*( uint32_t * ) GPC_ALT_SET_LOC = 0xF;
}

// FLAG disable/ Выключение FLAG
void HAL_SYS_FlagDisable( void )
{
	*( uint32_t * ) GPC_ALT_CLEAR_LOC = 0xF;
}

// Setting of the core primary priority over DMA/ Задание наивысшего приоритета процессору перед DMA
void HAL_SYS_CorePrimariPriority( void )
{
	*( uint32_t * ) CMU_CFG1_LOC &= ~( 1 << 12 );
}

// Multiple writing in SYSCON and SDRCON registers is enabled/ Разрешение многократной записи в регистры SYSCON и SDRCON
void HAL_SYS_MultipleWriteSysconSdrconEn( void )
{
	LX_CMU->CFG1.b.SYS_WE = 1;
}

// N ms pause/ Пауза в N мс
void HAL_SYS_WaitMs( uint32_t ms )
{
	uint64_t ullStop = __read_ccnt();
	ullStop += ms * PLL_Freq.CoreClk;

	while( ( ( ( uint64_t ) ( __read_ccnt() ) ) - ullStop ) & ( ( uint64_t ) 1 << 63 ) );
}
