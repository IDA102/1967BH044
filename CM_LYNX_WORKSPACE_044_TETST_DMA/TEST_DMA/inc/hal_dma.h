/*
 *
 *	Файл: 		hal_dma.h
 *	Описание: 	HAL для модуля прямого доступа к памяти
 *
 *	История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *						    Zatonsky Pavel 		- 	создан
 *
 */

/*
	Каналы 0 - 3  позволяют выполнять пересылки: память - память, память - устройство, устройство - память
    Каналы 4 - 7  позволяют выполнять только пересылки память - устройство
	Каналы 8 - 11 позволяют выполнять только пересылки устройство - память
	Канал 12 используется совместно с цифровым коррелятором
 */
#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

#include <stdint.h>

typedef enum
{
	dmaSTD = 0,
	dmaUART0,
	dmaUART1,
	dmaSPI0,
	dmaVIDEO,
	dmaSSI0,
	dmaSSI1,
	dmaNAND,
	dmaUPDOWN1,
	dmaUPDOWN0,
	dmaUPDOWN2,
	dmaUPDOWN3,
	dmaSPI1,
	dmaSPI2,
	dmaH264RQ1,
	dmaH264RQ0,
	dmaDMAR_4_8,
	dmaDMAR_5_9,
	dmaDMAR_6_10,
	dmaDMAR_7_11,
	dmaTIMER0_PWM0,																					// Запрос от DMA_RE таймера 0
	dmaTIMER0_PWM1,																					// Запрос от DMA_RE1 таймера 0
	dmaTIMER0_PWM2,																					// Запрос от DMA_RE2 таймера 0
	dmaTIMER0_PWM3,																					// Запрос от DMA_RE3 таймера 0
	dmaTIMER0_PWM4,																					// Запрос от DMA_RE4 таймера 0
	dmaTIMER1_PWM0,																					// Запрос от DMA_RE таймера 1
	dmaTIMER1_PWM1,																					// Запрос от DMA_RE1 таймера 1
	dmaTIMER1_PWM2,																					// Запрос от DMA_RE2 таймера 1
	dmaTIMER1_PWM3,																					// Запрос от DMA_RE3 таймера 1
	dmaTIMER1_PWM4,																					// Запрос от DMA_RE4 таймера 1
	dmaTIMER0,
	dmaTIMER1,
} DMA_Requester_type;

typedef enum
{
	dmaCopyValid = 0,
	dmaCopyAddrErr,
	dmaCopyLengthErr,
	dmaCopyChNumErr,
	dmaCopyInitActCh,
	dmaCopyCfgErr
} DMA_Return_type;

#define DMA_DCA_INT_MEM						0x2000000000000000ULL
#define DMA_DCA_EXT_MEM						0x4000000000000000ULL

#ifdef __cplusplus
extern "C"
{
#endif
	void 			HAL_DMA_WriteDC					( uint32_t ch_number, void *qw_tcb );
	void 			HAL_DMA_WriteDCA				( uint32_t ch_number, uint64_t *data );
	void 			HAL_DMA_WriteDCS				( uint32_t ch_number, void *qw_tcb );
	void			HAL_DMA_WriteDCD				( uint32_t ch_number, void *qw_tcb );

	void 			HAL_DMA_ReadDC					( uint32_t ch_number, __builtin_quad *qw_tcb );
	void 			HAL_DMA_ReadDCA					( uint32_t ch_number, uint64_t *data );
	void 			HAL_DMA_ReadDCS					( uint32_t ch_number, __builtin_quad *qw_tcb );
	void 			HAL_DMA_ReadDCD					( uint32_t ch_number, __builtin_quad *qw_tcb );

	void 			HAL_DMA_Stop					( uint32_t ch_number );
	uint32_t 		HAL_DMA_GetChannelStatus		( uint32_t channel );
	uint32_t 		HAL_DMA_GetChannelStatusClear	( uint32_t );
	uint32_t 		HAL_DMA_WaitForChannel			( uint32_t channel );
	uint32_t		HAL_DMA_GetDcCountX				( uint32_t ch_number );
	DMA_Return_type HAL_DMA_MemCopy32				( uint32_t ch_number, const void *src, const void *dst, uint32_t size );
	uint32_t 		HAL_DMA_GetTCBChannelDest		( uint32_t channel );
	void 			HAL_DMA_CreateChannelDest		( uint32_t channel, void *tcb_current, void *tcb_next );

	void 			HAL_DMA_RqstSet					( uint32_t ch_number, DMA_Requester_type dmaRqster );
	void 			HAL_DMA_RqstClr					( uint32_t ch_number );

	void 			HAL_DMA_SourceDestRqstReset		( void );
	void 			HAL_DMA_PrimaryPriority			( void );
	uint32_t HAL_DMA_GetDcAddr( uint32_t ch_number );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_DMA_H__ */
