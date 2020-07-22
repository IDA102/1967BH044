/*
 *
 *	����: 		hal_sys.h
 *	��������: 	HAL ��� ������� ���� ������
 *
 *	�������:
 *						28-Jun-2017 Dmitriy Sharaviev 	- 	������� ��� ������ �������, ��������� ������� ������� ������� �����������
 *										Zatonsky Pavel 	- 	������
 *
 */
#ifndef __HAL_SYS_H__
#define __HAL_SYS_H__

#include <sysreg.h>
#include <builtins.h>
#include <stdint.h>

#define __NOP			asm("nop;;")
#define __IDLE			asm("idle;;")
#define __RDS			asm("rds;;")
#define __BTBEN			asm("btben;;")
#define SysregWrite		__builtin_sysreg_write
#define SysregRead		__builtin_sysreg_read
#define FIX_IT_BUG() \
       asm("JL0 = J27 + 0x100;;"); \
       asm("JB0 = 8;;"); \
       asm("KL0 = JL0;;"); \
       asm("KB0 = JB0;;")

typedef enum
{
	SYS_BUS_16b = 16,
	SYS_BUS_32b = 32
}SYS_BusSize_type;

#ifdef __cplusplus
extern "C"
{
#endif
	void HAL_SYS_ExtBusEnable		( SYS_BusSize_type size );
	void HAL_SYS_ExtBusDisable		( void );
	void HAL_SYS_SDRAMEnable		( uint32_t sdrconValue );
	void HAL_SYS_SDRAMRegenPer		( uint16_t RegenPer );
	void HAL_SYS_SDRAMDisable		( void );
	void HAL_SYS_SRAMEnable			( uint32_t sysconValue );
	void HAL_SYS_SRAMDisable		( void );
	void HAL_SYS_FlagEnable			( void );
	void HAL_SYS_FlagDisable		( void );
	void HAL_SYS_CorePrimariPriority( void );
	void HAL_SYS_WaitMs				( uint32_t ms );

	static inline void HAL_SYS_WaitClk( uint32_t clk )
	{
		uint64_t stop = __read_ccnt();
		stop += clk;
		while( ( ( ( uint64_t ) ( __read_ccnt( ) ) ) - stop ) & ( ( uint64_t ) 1 << 63 ) );
	}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_SYS_H__ */
