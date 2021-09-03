/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_camera.h
 *	Описание: 	HAL для интерфейса видеокамеры
 *
 *	История:
 *				19-Jun-2017 Dmitry Sharavev - изменен под вторую ревизию
 *				15-Mar-2017 Dmitry Sharavev - создан
 *
 */

#ifndef __HAL_CAMERA_H__
#define __HAL_CAMERA_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
// Регистр SR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t EMPTY	:1;																			// Буфер пуст
		uint32_t FULL	:1;																			// Буфер заполнен
		uint32_t OVERF	:1;																			// Было переполнение буфера
		uint32_t UNDERF	:1;																			// Было чтение пустого буфера
	} b;
} CAMERA_SR_type;

// Регистр CR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t VCON	:1;																			// Включение интерфейса
		uint32_t VCIE	:1;																			// Разрешение прерывания
		uint32_t SMODE	:1;																			// Режим приема информации
		uint32_t VPOL	:1;																			// Полярность сигнала VSYNC
	} b;
} CAMERA_CR_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	__builtin_quad DR;																				// 128-разрядный регистр данных. Доступен только по чтению и является выходом FIFO
	CAMERA_SR_type SR;																				// Регистр состояния
	CAMERA_CR_type CR;																				// Регистр управления
} CAMERA_type;

#define LX_CAMERA 		( ( volatile CAMERA_type * ) base_VIDEO )

/* -------------------------------------- Специфические типы ------------------------------------ */
typedef enum
{
	CAMERA_RECEIVE_MODE_CAMERA = 0,
	CAMERA_RECEIVE_MODE_MASTER
}CAMERA_Mode_type;

typedef enum
{
	CAMERA_VSYNC_LOW = 0,
	CAMERA_VSYNC_HIGH
}CAMERA_VSYNC_type;

typedef struct
{
	CAMERA_Mode_type eMode;																			// Режим работы
	CAMERA_VSYNC_type eVSYNC;																		// Полярность сигнала VSYNC
}CAMERA_Conf_type;

#ifdef __cplusplus
extern "C"
	{
#endif
	void HAL_CAMERA_SetupGpio	( void );
	void HAL_CAMERA_Config		( CAMERA_Conf_type *pxConf );
	void HAL_CAMERA_Enable		( void );
	void HAL_CAMERA_Disable		( void );
	void HAL_CAMERA_StartDma	( uint32_t ulChannel, void *pvCameraBuf, uint32_t ulXSize, uint32_t ulYSize, uint32_t ulDelta, uint8_t ucPixelsInWord, void *pvDmaIsrHandler );
#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_CAMERA_H__ */
