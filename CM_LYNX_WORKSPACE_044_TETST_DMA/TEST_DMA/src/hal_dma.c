/*
 *
 *	File/Файл: 		hal_dma.c
 *	Description/Описание: 	HAL for the DMA unit/HAL для модуля прямого доступа к памяти
 *
 *	History/История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	changed for the second revision/изменен под вторую ревизию
 *						    Zatonsky Pavel 		-   created/создан
 *
 */
#include <sysreg.h>
#include <builtins.h>
#include "hal_1967VN044.h"

#define TCB_INTMEM16  			0x40000000
#define TCB_INTMEM22			0x60000000
#define TCB_EXTMEM16			0x80000000
#define TCB_EXTMEM22			0xE0000000

// Writing to DMA channel configuration registers, channel 4-12/Запись конфигурационных регистров DMA канала 4 - 12
void HAL_DMA_WriteDC( uint32_t ch_number, void *qw_tcb )
{
	switch ( ch_number )
	{
		case 4:  __builtin_sysreg_write4( __DC4, *( __builtin_quad * ) qw_tcb );	break;
		case 5:  __builtin_sysreg_write4( __DC5, *( __builtin_quad * ) qw_tcb );	break;
		case 6:  __builtin_sysreg_write4( __DC6, *( __builtin_quad * ) qw_tcb );	break;
		case 7:  __builtin_sysreg_write4( __DC7, *( __builtin_quad * ) qw_tcb );	break;
		case 8:  __builtin_sysreg_write4( __DC8, *( __builtin_quad * ) qw_tcb );	break;
		case 9:  __builtin_sysreg_write4( __DC9, *( __builtin_quad * ) qw_tcb );	break;
		case 10: __builtin_sysreg_write4( __DC10, *( __builtin_quad * ) qw_tcb );	break;
		case 11: __builtin_sysreg_write4( __DC11, *( __builtin_quad * ) qw_tcb );	break;
		case 12: __builtin_sysreg_write4( __DC12, *( __builtin_quad * ) qw_tcb );	break;
		default: break;
	}
}

// Writing to the peripheral device address setting register, DMA channels 4-11/Запись регистра задания адреса периферийного устройства DMA канала 4 - 11
void HAL_DMA_WriteDCA( uint32_t ch_number, uint64_t *data )
{
	switch ( ch_number )
	{
		case 4:  *( uint64_t* )0x801F0030 = *data; 	break;
		case 5:  *( uint64_t* )0x801F0032 = *data; 	break;
		case 6:  *( uint64_t* )0x801F0034 = *data; 	break;
		case 7:  *( uint64_t* )0x801F0036 = *data; 	break;
		case 8:  *( uint64_t* )0x801F0050 = *data; 	break;
		case 9:  *( uint64_t* )0x801F0052 = *data; 	break;
		case 10: *( uint64_t* )0x801F0054 = *data; 	break;
		case 11: *( uint64_t* )0x801F0056 = *data; 	break;
		default: break;
	}
}

// Writing to the data source configuration register, DMA channels 0 - 3/Запись конфигурационного регистра ИСТОЧНИКА данных для каналов 0 - 3
void HAL_DMA_WriteDCS( uint32_t ch_number, void *qw_tcb )
{
	switch ( ch_number )
	{
		case 0: __builtin_sysreg_write4( __DCS0, *( __builtin_quad * ) qw_tcb ); break;
		case 1: __builtin_sysreg_write4( __DCS1, *( __builtin_quad * ) qw_tcb ); break;
		case 2: __builtin_sysreg_write4( __DCS2, *( __builtin_quad * ) qw_tcb ); break;
		case 3: __builtin_sysreg_write4( __DCS3, *( __builtin_quad * ) qw_tcb ); break;
		default: break;
	}
}

// Writing to the data destination configuration register, DMA channels 0 - 3/Запись конфигурационного регистра ПРИЕМНИКА данных для каналов 0 - 3
void HAL_DMA_WriteDCD( uint32_t ch_number, void *qw_tcb )
{
	switch ( ch_number )
	{
		case 0: __builtin_sysreg_write4( __DCD0, *( __builtin_quad * ) qw_tcb ); break;
		case 1: __builtin_sysreg_write4( __DCD1, *( __builtin_quad * ) qw_tcb ); break;
		case 2: __builtin_sysreg_write4( __DCD2, *( __builtin_quad * ) qw_tcb ); break;
		case 3: __builtin_sysreg_write4( __DCD3, *( __builtin_quad * ) qw_tcb ); break;
		default: break;
	}
}

// Reading of the DMA configuration registers, DMA channels 4 - 12/Чтение конфигурационных регистров DMA канала 4 - 12
void HAL_DMA_ReadDC( uint32_t ch_number, __builtin_quad *qw_tcb )
{
	switch ( ch_number )
	{
		case 4:  *qw_tcb = __builtin_sysreg_read4( __DC4 );		break;
		case 5:  *qw_tcb = __builtin_sysreg_read4( __DC5 );		break;
		case 6:  *qw_tcb = __builtin_sysreg_read4( __DC6 );		break;
		case 7:  *qw_tcb = __builtin_sysreg_read4( __DC7 );		break;
		case 8:  *qw_tcb = __builtin_sysreg_read4( __DC8 );		break;
		case 9:  *qw_tcb = __builtin_sysreg_read4( __DC9 );		break;
		case 10: *qw_tcb = __builtin_sysreg_read4( __DC10 );	break;
		case 11: *qw_tcb = __builtin_sysreg_read4( __DC11 );	break;
		case 12: *qw_tcb = __builtin_sysreg_read4( __DC12 );	break;
		default: break;
	}
}

// Reading of the peripheral device address setting register, DMA channels 4 - 11/Чтение регистра задания адреса периферийного устройства DMA канала 4 - 11
void HAL_DMA_ReadDCA( uint32_t ch_number, uint64_t *data )
{
	switch ( ch_number )
	{
		case 4:  *data = *( uint64_t* )0x801F0030; 	break;
		case 5:  *data = *( uint64_t* )0x801F0032; 	break;
		case 6:  *data = *( uint64_t* )0x801F0034; 	break;
		case 7:  *data = *( uint64_t* )0x801F0036; 	break;
		case 8:  *data = *( uint64_t* )0x801F0050; 	break;
		case 9:  *data = *( uint64_t* )0x801F0052; 	break;
		case 10: *data = *( uint64_t* )0x801F0054; 	break;
		case 11: *data = *( uint64_t* )0x801F0056; 	break;
		default: break;
	}
}

// Reading of the data source configuration register, DMA channels 0 - 3/Чтение конфигурационного регистра ИСТОЧНИКА данных для каналов 0 - 3
void HAL_DMA_ReadDCS( uint32_t ch_number, __builtin_quad *qw_tcb )
{
	switch ( ch_number )
	{
		case 0: *qw_tcb = __builtin_sysreg_read4( __DCS0 ); break;
		case 1: *qw_tcb = __builtin_sysreg_read4( __DCS1 ); break;
		case 2: *qw_tcb = __builtin_sysreg_read4( __DCS2 ); break;
		case 3: *qw_tcb = __builtin_sysreg_read4( __DCS3 ); break;
		default: break;
	}
}

// Reading of the data destination configuration register, DMA channels 0 - 3/Чтение конфигурационного регистра ПРИЕМНИКА данных для каналов 0 - 3
void HAL_DMA_ReadDCD( uint32_t ch_number, __builtin_quad *qw_tcb )
{
	switch ( ch_number )
	{
		case 0: *qw_tcb = __builtin_sysreg_read4( __DCD0 ); break;
		case 1: *qw_tcb = __builtin_sysreg_read4( __DCD1 ); break;
		case 2: *qw_tcb = __builtin_sysreg_read4( __DCD2 ); break;
		case 3: *qw_tcb = __builtin_sysreg_read4( __DCD3 ); break;
		default: break;
	}
}

// Disconnection of the DMA channel/Выключение канала DMA
void HAL_DMA_Stop( uint32_t ch_number )
{
	__builtin_quad zero_tcb = __builtin_compose_128( 0, 0 );
	if ( ch_number > 3 )
	{
		HAL_DMA_WriteDC( ch_number, &zero_tcb );
	}
	else
	{
		HAL_DMA_WriteDCS( ch_number, &zero_tcb );
		HAL_DMA_WriteDCD( ch_number, &zero_tcb );
	}

}

// Extraction of the channel status bits/Извлечение статусных битов канала
static uint32_t HAL_DMA_DecodeStatus( uint64_t dmaStatus, uint32_t channel )
{
	switch ( channel )
	{
		case 0:  return ( uint32_t ) ( ( dmaStatus >> 0 ) & 0x07 );
		case 1:  return ( uint32_t ) ( ( dmaStatus >> 3 ) & 0x07 );
		case 2:  return ( uint32_t ) ( ( dmaStatus >> 6 ) & 0x07 );
		case 3:  return ( uint32_t ) ( ( dmaStatus >> 9 ) & 0x07 );
		case 4:  return ( uint32_t ) ( ( dmaStatus >> 12 ) & 0x07 );
		case 5:  return ( uint32_t ) ( ( dmaStatus >> 15 ) & 0x07 );
		case 6:  return ( uint32_t ) ( ( dmaStatus >> 18 ) & 0x07 );
		case 7:  return ( uint32_t ) ( ( dmaStatus >> 21 ) & 0x07 );
		case 8:  return ( uint32_t ) ( ( dmaStatus >> 32 ) & 0x07 );
		case 9:  return ( uint32_t ) ( ( dmaStatus >> 35 ) & 0x07 );
		case 10: return ( uint32_t ) ( ( dmaStatus >> 38 ) & 0x07 );
		case 11: return ( uint32_t ) ( ( dmaStatus >> 41 ) & 0x07 );
		case 12: return ( uint32_t ) ( ( dmaStatus >> 50 ) & 0x07 );
		default: return 0x05;																		// An error/ошибка
	}
}

// Reading of the DMA channel status/Получение статуса DMA канала
uint32_t HAL_DMA_GetChannelStatus( uint32_t channel )
{
	uint64_t dmaStatus = __builtin_sysreg_read2( __DSTAT );
	return HAL_DMA_DecodeStatus( dmaStatus, channel );
}

// Waiting for the end of the DMA channel transmission/ Ожидание завершения передачи DMA канала
// Returned value/Возвращаемое значение:
// 0 - transmission has been completed successfully/успешное завершение передачи;
// 1 - an error has occurred/возникла ошибка;
// 2 - the cannel is off/канал отключен.
uint32_t HAL_DMA_WaitForChannel( uint32_t channel )
{
	uint32_t status;

	do
		status = HAL_DMA_GetChannelStatus( channel );
	while( status == DSTAT_ACT );

	switch ( status )
	{
		case DSTAT_IDLE: return 2;
		case DSTAT_DONE: return 0;
		default:         return 1;
	}
}

// Reading of the number of words which are to be transferred, channels 4 - 12/Чтение количества слов для передачи каналом 4 - 12
uint32_t HAL_DMA_GetDcCountX( uint32_t ch_number )
{
	__builtin_quad temp_tcb;
	uint32_t *ptr = ( uint32_t* ) &temp_tcb;
	uint32_t temp;

	switch ( ch_number )
	{
		case 4:  temp_tcb = __builtin_sysreg_read4( __DC4 ); 	break;
		case 5:  temp_tcb = __builtin_sysreg_read4( __DC5 ); 	break;
		case 6:  temp_tcb = __builtin_sysreg_read4( __DC6 ); 	break;
		case 7:  temp_tcb = __builtin_sysreg_read4( __DC7 ); 	break;
		case 8:  temp_tcb = __builtin_sysreg_read4( __DC8 ); 	break;
		case 9:  temp_tcb = __builtin_sysreg_read4( __DC9 ); 	break;
		case 10: temp_tcb = __builtin_sysreg_read4( __DC10 ); 	break;
		case 11: temp_tcb = __builtin_sysreg_read4( __DC11 );	break;
		case 12: temp_tcb = __builtin_sysreg_read4( __DC12 );	break;
		default: break;
	}

	temp = *( ptr + 1 );
	return ( temp >> 16 );
}

// Returns the DI register for the current channel/ Возвращает регистр DI для текущего канала
uint32_t HAL_DMA_GetDcAddr( uint32_t ch_number )
{
	__builtin_quad temp_tcb;
	uint32_t *ptr = ( uint32_t* ) &temp_tcb;
	uint32_t temp;

	switch ( ch_number )
	{
		case 4:  temp_tcb = __builtin_sysreg_read4( __DC4 ); 	break;
		case 5:  temp_tcb = __builtin_sysreg_read4( __DC5 ); 	break;
		case 6:  temp_tcb = __builtin_sysreg_read4( __DC6 ); 	break;
		case 7:  temp_tcb = __builtin_sysreg_read4( __DC7 ); 	break;
		case 8:  temp_tcb = __builtin_sysreg_read4( __DC8 ); 	break;
		case 9:  temp_tcb = __builtin_sysreg_read4( __DC9 ); 	break;
		case 10: temp_tcb = __builtin_sysreg_read4( __DC10 ); 	break;
		case 11: temp_tcb = __builtin_sysreg_read4( __DC11 );	break;
		case 12: temp_tcb = __builtin_sysreg_read4( __DC12 );	break;
		default: break;
	}

	temp = *( ptr + 0 );
	return ( temp  );
}

// Returns the DP register field for the current channel/ Возвращает поле регистра DP для текущего канала
uint32_t HAL_DMA_GetTCBChannelDest( uint32_t channel )
{
	switch ( channel )
	{
		case 4:  return TCB_DMA4DEST;
		case 5:  return TCB_DMA5DEST;
		case 6:  return TCB_DMA6DEST;
		case 7:  return TCB_DMA7DEST;
		case 8:  return TCB_DMA8DEST;
		case 9:  return TCB_DMA9DEST;
		case 10: return TCB_DMA10DEST;
		case 11: return TCB_DMA11DEST;
		default: return 0;
	}
}

// The argument of the function is the number of the DMA channel that is next in the chain/ Функция принимает номер канала DMA, который будет следующим в цепочке
void HAL_DMA_CreateChannelDest( uint32_t channel, void *tcb_current, void *tcb_next )
{
	uint32_t tmp;
	tmp = HAL_DMA_GetTCBChannelDest( channel );
	*( ( uint32_t * ) ( tcb_current ) + 3 ) |= ( ( uint32_t ) tcb_next >> 2 ) | tmp | TCB_CHAIN;
}

// Reading of the channel status and its setting to the state 'OFF'/ Чтение статуса канала и его сброс в состояние выключено
uint32_t HAL_DMA_GetChannelStatusClear( uint32_t channel )
{
	uint64_t dmaStatus = __builtin_sysreg_read2( __DSTATC );
	return HAL_DMA_DecodeStatus( dmaStatus, channel );
}

#define HAL_DMA_InitMemType( addr, DPReg )\
	if ((addr >= (0x00000000) && addr <= (0x0000FFFF)) || (addr >= 0x00040000 && addr <= (0x04FFFF)) ||\
		(addr >= 0x080000 && addr <= (0x08FFFF)) || (addr >= 0x0C0000 && addr <= (0x0CFFFF)) ||\
		(addr >= 0x100000 && addr <= (0x10FFFF)) || (addr >= 0x140000 && addr <= (0x14FFFF)))\
		DPReg |= TCB_INTMEM16;\
	else if((addr >= 0x30000000 && addr <= (0x44000000) ) || (addr >= 0x50000000 && addr <= (0x54000000)) ||\
			(addr >= 0x60000000 && addr <= (0x64000000)) || (addr >= 0x70000000 && addr <= (0x74000000)) || (addr >= 0x80000000 && addr <= (0xFFFFFFFF)))\
		DPReg |= TCB_EXTMEM16;\
	else return dmaCopyAddrErr

// Copying of the data array with the use of DMA channels 0-3/ Копирование массива данных с использованием DMA канала, 0 - 3
DMA_Return_type HAL_DMA_MemCopy32( uint32_t ch_number, const void *src, const void *dst, uint32_t size )
{
	uint32_t __attribute((aligned(4))) tcb_dcs[4];
	uint32_t __attribute((aligned(4))) tcb_dcd[4];
	uint32_t chStat;

	if ( size > 0xFFFF ) return dmaCopyLengthErr;													// Check of the array dimension

	tcb_dcs[ 3 ] = 0;
	HAL_DMA_InitMemType( ( uint32_t )src, tcb_dcs[ 3 ] );											// Initialization of DCS/Инициализация DCS
	tcb_dcs[ 0 ] = ( uint32_t ) src;
	tcb_dcs[ 1 ] = ( size << 16 ) | 1;
	tcb_dcs[ 2 ] = 0;
	tcb_dcs[ 3 ] |= TCB_NORMAL;

	tcb_dcd[ 3 ] = 0;																				// Initioalization of DCD/Инициализация DCD
	HAL_DMA_InitMemType( ( uint32_t )dst, tcb_dcd[ 3 ] );
	tcb_dcd[ 0 ] = ( uint32_t ) dst;
	tcb_dcd[ 1 ] = ( size << 16 ) | 1;
	tcb_dcd[ 2 ] = 0;
	tcb_dcd[ 3 ] |= TCB_NORMAL;

	if ( ch_number < 4 )																			// Start DMA/Старт DMA
	{
		HAL_DMA_WriteDCD( ch_number, &tcb_dcd );
		HAL_DMA_WriteDCS( ch_number, &tcb_dcs );
	}
	else
		return dmaCopyChNumErr;

	chStat = HAL_DMA_GetChannelStatus( ch_number );													// Check of the DMA status/Проверка статуса DMA

	switch ( chStat )
	{
		case 4: return dmaCopyChNumErr;
		case 5: return dmaCopyInitActCh;
		case 7: return dmaCopyAddrErr;
		default: return dmaCopyValid;
	}
}

// Setting of the request sources for the DMA channels 0-11/Задание источника запросов для DMA каналов 0 - 11
void HAL_DMA_RqstSet( uint32_t ch_number, DMA_Requester_type dmaRqster )
{
	if( ch_number < 8 )
	{
		*( uint32_t * ) base_DMACFGL &= ~( 0xF << ( ch_number * 4 ) );
		*( uint32_t * ) base_DMACFGH &= ~( 0x10000 << ch_number );
		*( uint32_t * ) base_DMACFGL |= ( ( dmaRqster & 0xF ) << ( ch_number * 4 ) );
		*( uint32_t * ) base_DMACFGH |= ( ( ( dmaRqster & 0x10 ) << 12 ) << ch_number );
	}
	else if( ch_number <= 11 )
	{
		*( uint32_t * ) base_DMACFGH &= ~( 0x10000 << ch_number );
		*( uint32_t * ) base_DMACFGH |= ( ( ( dmaRqster & 0x10 ) << 12 ) << ch_number );
		ch_number *= 4;
		ch_number &= 0x1F;
		*( uint32_t * ) base_DMACFGH &= ~( 0xF << ch_number );
		*( uint32_t * ) base_DMACFGH |= ( ( dmaRqster & 0xF ) << ch_number );
	}
}

//  Reset of the request sources for DMA channels 0-11/Сброс источника запросов для DMA каналов 0 - 11
void HAL_DMA_RqstClr( uint32_t ch_number )
{
	if( ch_number < 8 )
	{
		*( uint32_t * ) base_DMACFGL &= ~( 0xF << ( ch_number * 4 ) );
		*( uint32_t * ) base_DMACFGH &= ~( 0x10000 << ch_number );
	}
	else if( ch_number <= 11 )
	{
		*( uint32_t * ) base_DMACFGH &= ~( 0x10000 << ch_number );
		ch_number *= 4;
		ch_number &= 0x1F;
		*( uint32_t * ) base_DMACFGH &= ~( 0xF << ch_number );
	}
}

// Reset of the request sources for all DMA channels/ Сброс источников запросов для всех каналов DMA
void HAL_DMA_SourceDestRqstReset( void )
{
	*( uint32_t * ) base_DMACFGL = 0;
	*( uint32_t * ) base_DMACFGH = 0;
}

// High-priority DMA channel is allowed to have external memory access priority over the processor/ Разрешение высокоприоритетному каналу DMA иметь преимущество перед процессором при доступе к внешней памяти
void HAL_DMA_PrimaryPriority( void )
{
	*( uint32_t * ) CMU_CFG1_LOC |= ( 1 << 12 );
}
