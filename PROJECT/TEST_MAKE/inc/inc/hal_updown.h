/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_updown.h
 *	Описание: 	HAL для модуля цифровой обработки UP/DOWN
 *
 *	История:
 *	 			27-Feb-2017 Ivan Osadchy 		- 	создан
 *
 */
#ifndef __HAL_UPDOWN_H__
#define __HAL_UPDOWN_H__

#include <stdint.h>

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t EN 		:1;
			uint32_t LINK 		:1;
			uint32_t ROUND 		:1;
			uint32_t SAT 		:1;
			uint32_t ROUNDM 	:1;
			uint32_t TBD 		:1;
			uint32_t IQ_QI 		:1;
			uint32_t DAM 		:1;
			uint32_t Kdelay 	:9;
			uint32_t SHFR 		:7;
			uint32_t INT_BLK 	:1;
			uint32_t res 		:3;
			uint32_t FLEN 		:2;
			uint32_t RCNT_ON 	:1;
			uint32_t LNKUSE 	:1;
		} b;
	} CR;
	union
	{
		uint32_t word;
		struct
		{
			uint32_t T_req 		:1;
			uint32_t R_req 		:1;
			uint32_t Ferr 		:1;
			uint32_t overf 		:1;
			uint32_t res 		:28;
		} b;
	} SR;
	uint32_t STEP;
	uint32_t res;
	uint32_t DR[ 4 ];
	//__builtin_quad DR;
	uint32_t RCNT;
	uint32_t XCR;
} UPDOWN_type;

#define LX_UPDOWN0 				( ( volatile UPDOWN_type * ) base_UDDC0 )
#define LX_UPDOWN1 				( ( volatile UPDOWN_type * ) base_UDDC1 )
#define LX_UPDOWN2 				( ( volatile UPDOWN_type * ) base_UDDC2 )
#define LX_UPDOWN3 				( ( volatile UPDOWN_type * ) base_UDDC3 )

/* -------------------------------------- Специфические типы ------------------------------------ */
typedef enum
{
	UPDOWN_Mode_Down = 0,
	UPDOWN_Mode_Up = 1,
} UPDOWN_Mode_type;

typedef enum
{
	UPDOWN_Link_Test = 0x00,
	UPDOWN_Link_0 = 0x02,
	UPDOWN_Link_1 = 0x03,
} UPDOWN_Link_type;

typedef enum
{
	UPDOWN_FiltersStage_3 = 0,
	UPDOWN_FiltersStage_5 = 1,
	UPDOWN_FiltersStage_7 = 2,
} UPDOWN_FiltersStage_type;

typedef enum
{
	UPDOWN_Round_Off = 0x00,
	UPDOWN_Round_MultOnly = 0x04,
	UPDOWN_Round_OutOnly = 0x01,
	UPDOWN_Round_OutOnlyNoOverflow = 0x03,
	UPDOWN_Round_All = 0x05,
	UPDOWN_Round_AllNoOverflow = 0x07,
} UPDOWN_Round_type;

typedef enum
{
	UPDOWN_FifoMode_LowWord_IQ = 0x00,
	UPDOWN_FifoMode_LowWord_QI = 0x01,
	UPDOWN_FifoMode_HighWord_IQ = 0x02,
	UPDOWN_FifoMode_HighWord_QI = 0x03,
} UPDOWN_FifoMode_type;

typedef enum
{
	UPDOWN_CntMode_Off = 0x00,
	UPDOWN_CntMode_Run = 0x01,
	UPDOWN_CntMode_IT = 0x03,
} UPDOWN_CntMode_type;

typedef struct
{
	UPDOWN_Mode_type Mode;
	UPDOWN_Link_type LinkX;
	UPDOWN_FiltersStage_type FiltersStage;
	UPDOWN_Round_type Round;
	uint8_t Shift;
	UPDOWN_FifoMode_type FifoMode;
	uint16_t Coefficient;
	UPDOWN_CntMode_type CntMode;
	uint32_t Cnt;
	uint32_t Step;
} UPDOWN_Init_type;

typedef enum
{
	UPDOWN_Flag_Treq = 0x01,
	UPDOWN_Flag_Rreq = 0x02,
	UPDOWN_Flag_Ferr = 0x04,
	UPDOWN_Flag_Overf = 0x08,
} UPDOWN_Flag_type;

typedef enum
{
	UPDOWN_FlagState_Off = 0,
	UPDOWN_FlagState_On = 1,
} UPDOWN_FlagState_type;

#ifdef __cplusplus
extern "C"
{
#endif
	void 					HAL_UPDOWN_DefaultInitStruct	( UPDOWN_Init_type *Init );
	void 					HAL_UPDOWN_Init					( UPDOWN_type * const LX_UPDOWNx, UPDOWN_Init_type * const Init );
	void 					HAL_UPDOWN_DeInit				( UPDOWN_type * const LX_UPDOWNx );

	void 					HAL_UPDOWN_Enable				( UPDOWN_type * const LX_UPDOWNx );
	void 					HAL_UPDOWN_Disable				( UPDOWN_type * const LX_UPDOWNx );

	void 					HAL_UPDOWN_SetRcntStep			( UPDOWN_type * const LX_UPDOWNx, const uint32_t RCNT, const uint16_t Step );
	UPDOWN_FlagState_type 	HAL_UPDOWN_GetFlag				( UPDOWN_type * const LX_UPDOWNx, const UPDOWN_Flag_type Flag );

	void 					HAL_UPDOWN_ConnectDMA			( UPDOWN_type * const LX_UPDOWNx, uint32_t ch_number, void *tcb );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_UPDOWN_H__ */
