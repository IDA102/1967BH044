/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_link.h
 *	Описание: 	HAL для модуля контроллера интерфейса ARINC
 *
 *	История:
 *						    Zatonsky Pavel 		- 	создан
 *
 */
#ifndef __HAL_LINK_H__
#define __HAL_LINK_H__

#include <stdint.h>

typedef enum {Link_CheckSum_Dis = 0,	Link_CheckSum_En = LRX_VEN} 	Link_CheckSum_type;
typedef enum {Link_CheckBCMP_Dis = 0,	Link_CheckBCMP_En = LRX_BCMP} 	Link_CheckBCMP_type;
typedef enum {Link_TOIT_Dis = 0,		Link_TOIT_En = LRX_TOEN} 		Link_TOIT_type;
typedef enum {Link_OvrIT_Dis = 0,		Link_OvrIT_En = LRX_OVRN} 		Link_OvrIT_type;
typedef enum {Link_DataSize_1bit = 0,	Link_DataSize_4bit = LRX_DSIZE_4BIT,	Link_DataSize_8bit = LRX_DSIZE_8BIT, Link_DataSize_16bit = LRX_DSIZE_16BIT} 	Link_DataSize_type;
typedef enum {Link_TxClk_Master = 0, 	Link_TxClk_Slave = (1<<9), 				Link_TxClk_InvSlave = (3<<9)} 			Link_TxClk_type;

typedef enum {Link_GpsClk_Dis = 0,     	Link_GpsClk_En = LRX_GPSCLKEN } Link_GpsClk_type;
typedef enum {Link_RxDest_Buff = 0,		Link_RxDest_UpDown = ( 1 << 11 ) } Link_RxDest_type;
typedef enum {Link_AdcDataSize_10b = ( 3 << 12 ),  Link_AdcDataSize_12b = ( 2 << 12 ), 	Link_AdcDataSize_14b = ( 1 << 12 ), Link_AdcDataSize_16b = 0 } Link_AdcDataSize_type;
typedef enum {Link_Rcode_Dis = 0,		Link_Rcode_En = ( 1 << 14 ) } Link_Rcode_type;
typedef enum {Link_BitOrder_LoFrontHiCut = 0,		Link_BitOrder_OddFrontEvenCut = ( 1 << 15 )	} Link_BitOrder_type;
typedef enum {Link_TxDest_Buff = 0, 	Link_TxDest_UpDown = ( 1 << 11 ) } Link_TxDest_type;
typedef enum {Link_Tcode_Dis = 0,		Link_Tcode_En = ( 1 << 12 ) } Link_Tcode_type;
typedef enum {Link_CheckACKI_Dis = ( 1 << 13 ),	Link_CheckACKI_En = 0 } Link_CheckACKI_type;

typedef volatile struct
{
	Link_CheckSum_type 			CheckSum;
	Link_CheckBCMP_type 		CheckBCMPI;
	Link_DataSize_type  		DataSize;
	Link_TOIT_type 				TOIntEn;
	Link_OvrIT_type 			OVRIntEn;
}LinkRx_Init_type;

typedef volatile struct
{
	Link_GpsClk_type 			GpsClk;																// Разрешение генерации клока GPS
	Link_RxDest_type 			RxDest;																// Выбор приемника данных
	Link_AdcDataSize_type 		AdcDataSize;														// Режим для ADC 8 бит шины данных
	Link_Rcode_type 			Rcode;																// Преобразование прямого кода в дополнительный
	Link_BitOrder_type 			BitOrder;															// Порядок выдачи бит
} LinkRxEx_type;

typedef volatile struct
{
	Link_CheckSum_type 			CheckSum;
	Link_CheckBCMP_type 		CheckBCMPO;
	Link_DataSize_type  		DataSize;
	Link_TOIT_type 				TOIntEn;
	Link_TxClk_type 			ClkSource;
}LinkTx_Init_type;

typedef volatile struct
{
	Link_TxDest_type 			TxSource;															// Выбор источника данных для передачи
	Link_Tcode_type 			Tcode;																// Преобразование прямого кода в дополнительный
	Link_CheckACKI_type 		CheckACKI;															// Анализ сигнала LxACKI
} LinkTxEx_type;

#ifdef __cplusplus
extern "C"
{
#endif
	int32_t 	HAL_LinkRx_GetStat		( uint32_t link_num );
	int32_t 	HAL_LinkRx_GetStatCl	( uint32_t link_num );
	int32_t 	HAL_LinkTx_GetStat		( uint32_t link_num );
	int32_t 	HAL_LinkTx_GetStatCl	( uint32_t link_num );

	int32_t 	HAL_LinkRx_Enable		( uint32_t link_num, LinkRx_Init_type *LRxInit, LinkRxEx_type *LRxExInit );
	void 		HAL_LinkRx_Disable		( uint32_t link_num );
	int32_t 	HAL_LinkCore_Receive	( uint32_t link_num, void *BuffPtr, uint32_t size );
	void 		HAL_LinkDMA_Receive		( uint32_t link_num, void *tcb );

	int32_t 	HAL_LinkTx_Enable		( uint32_t link_num, LinkTx_Init_type *LTxInit, LinkTxEx_type *LTxExInit );
	void 		HAL_LinkTx_Disable		( uint32_t link_num );
	int32_t 	HAL_LinkCore_Transmit	( uint32_t link_num, void *BuffPtr, uint32_t size );
	void 		HAL_LinkDMA_Transmit	( uint32_t link_num, void *tcb );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif
