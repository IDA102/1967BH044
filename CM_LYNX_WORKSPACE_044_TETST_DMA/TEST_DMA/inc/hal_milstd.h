/*
 *
 *	Файл: 		hal_milstd.h
 *	Описание: 	HAL для модуля контроллера интерфейса по ГОСТ Р 52070-2003
 *
 *	История:
 *				26-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *						    Zatonsky Pavel 		- 	создан
 *
 */
#ifndef __HAL_MILSTD_H__
#define __HAL_MILSTD_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t MR 				:1;
		uint32_t BCSTART 			:1;
		uint32_t MODE 				:2;
		uint32_t TRA 				:1;
		uint32_t TRB 				:1;
		uint32_t RTA				:5;
		uint32_t DIV6_0 			:7;
		uint32_t RERR 				:1;
		uint32_t INVTR 				:1;
		uint32_t ENFILTER 			:1;
		uint32_t AUTOTUNE 			:1;
		uint32_t RSV 				:10;
	} b;
} MILSTD_CONTROL;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t IDLE 				:1;
		uint32_t RFLAGN 			:1;
		uint32_t VALMESS 			:1;
		uint32_t ERR 				:1;
		uint32_t RCVA 				:1;
		uint32_t RCVB 				:1;
		uint32_t RSV 				:26;
	} b;
} MILSTD_STATUS;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t NORCV 				:1;
		uint32_t MANERR 			:1;
		uint32_t DSYCERR 			:1;
		uint32_t CSYCERR 			:1;
		uint32_t GAPERR 			:1;
		uint32_t CONERR 			:1;
		uint32_t PROERR 			:1;
		uint32_t RSV 				:25;
	} b;
} MILSTD_ERROR;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t CODECOMM 			:5;
		uint32_t SUBADDR 			:5;
		uint32_t TXRX 				:1;
		uint32_t RTA 				:5;
		uint32_t RSV 				:16;
	} b;
} MILSTD_COMMWORD;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RT_FAIL 			:1;																// Неисправность ОУ
		uint32_t CFG_INTF_ACCEPT 	:1;																// Принято управление интерфейсом
		uint32_t SUB_FAIL 			:1;																// Неисправность абонента
		uint32_t SUB_BUSY 			:1;																// Абонент занят
		uint32_t GROUP_COMM_ACCEPT 	:1;																// Принята групповая команды
		uint32_t RSV0 				:3;																//
		uint32_t SERVICE_RQST 		:1;																// Запрос на обслуживание
		uint32_t TRAN_ANSWER_WORD 	:1;																// Передача ответного слова
		uint32_t MSG_ERR 			:1;																// Ошибка в сообщении
		uint32_t RTA 				:5;																// Адрес ОУ, от которого принято ответное слово в режиме КШ. Адрес ОУ, которое передаёт ответное слово в режиме ОУ
		uint32_t RSV1 				:16;															//
	} b;
} MILSTD_STATWORD;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t IDLEIE 			:1;
		uint32_t RFLAGNIE 			:1;
		uint32_t VALMESSIE 			:1;
		uint32_t ERRIE 				:1;
		uint32_t RSV 				:28;
	} b;
} MILSTD_INTEN;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	uint32_t DATA[ 0x400 ];
	MILSTD_CONTROL CONTROL;
	MILSTD_STATUS STATUS;
	MILSTD_ERROR ERROR;
	MILSTD_COMMWORD COMMWORD1;
	MILSTD_COMMWORD COMMWORD2;
	uint32_t MODEDATA;
	MILSTD_STATWORD STATWORD1;
	MILSTD_STATWORD STATWORD2;
	MILSTD_INTEN INTEN;
	uint32_t MSG;
} MILSTD_type;

#define LX_MILSTD0           ( ( volatile MILSTD_type    *) base_MILSTD0 )
#define LX_MILSTD1           ( ( volatile MILSTD_type    *) base_MILSTD1 )

/* -------------------------------------- Специфические типы ------------------------------------ */
#define SYNCERR					DSYCERR
#define SEQERR					CSYCERR
#define AMOUNTDATA				CODECOMM
#define MILSTD_RTA_GROUP_COMM	0x1F

typedef enum {MILSTD_Mode_BC = 1, MILSTD_Mode_RT = 2, MILSTD_Mode_Monitor = 3} MILSTD_Mode_type;
typedef enum {MILSTD_Channel_Main = (1<<4), MILSTD_Channel_Reserv = (1<<5)} MILSTD_Channel_type;
typedef enum {MILSTD_ResetErr_IDLE, MILSTD_ResetErr_MR} MILSTD_ResetErr_type;
typedef enum {MILSTD_InvPRD_Dis, MILSTD_InvPRD_En} MILSTD_InvPRD_type;
typedef enum {MILSTD_Filter_Dis = 0, MILSTD_Filter_NRZ = (1<<20), MILSTD_Filter_Interference = (1<<22), MILSTD_Filter_En = ((1<<20)|(1<<22))} MILSTD_Filter_type;
typedef enum {MILSTD_AutoTune_Dis,MILSTD_AutoTune_En} MILSTD_AutoTune_type;
typedef enum {MILSTD_IT_IDLEIE=1, MILSTD_IT_RFLAGNIE=2, MILSTD_IT_VALMESSIE=4, MILSTD_IT_ERRIE=8, MILSTD_IT_RTAIE=16} MILSTD_IT_type;

typedef struct
{
	MILSTD_Mode_type 		Mode;
	MILSTD_Channel_type 	Channel;
	MILSTD_ResetErr_type	ResetErr;
	uint32_t				RTA;
	MILSTD_InvPRD_type		InvPRD;
	MILSTD_Filter_type		Filter;
	MILSTD_AutoTune_type	AutoTune;
} MILSTD_Init_type;

#ifdef __cplusplus
extern "C"
	{
#endif
	void HAL_MILSTD_Reset			( MILSTD_type *MILSTDx );
	void HAL_MILSTD_Init			( MILSTD_type *MILSTDx, MILSTD_Init_type *InitStruct );
	void HAL_MILSTD_DeInit			( MILSTD_type *MILSTDx );
	void HAL_MILSTD_ITEnable		( MILSTD_type *MILSTDx, MILSTD_IT_type ITName );
	void HAL_MILSTD_ITDisable		( MILSTD_type *MILSTDx, MILSTD_IT_type ITName );
	void HAL_MILSTD_CWInit			( MILSTD_type *MILSTDx, uint32_t NumCW, uint32_t TxRxMode, uint32_t RTA, uint32_t amount, uint32_t SubAddr );
	void HAL_MILSTD_DTWrite			( MILSTD_type *MILSTDx, void *Buff, uint32_t amount, uint32_t subaddr );
	void HAL_MILSTD_StartCWTransmit	( MILSTD_type *MILSTDx );
	void HAL_MILSTD_DRRead			( MILSTD_type *MILSTDx, void *Buff, uint32_t amount, uint32_t subaddr );
#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_MILSTD_H__ */
