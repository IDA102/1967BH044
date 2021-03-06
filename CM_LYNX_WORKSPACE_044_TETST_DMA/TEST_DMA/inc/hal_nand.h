﻿/*
 *
 *	Файл: 		hal_nand.h
 *	Описание: 	HAL для контроллера NAND
 *
 *	История:
 *				26-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *						    Zatonsky Pavel 		- 	создан
 *
 */
#ifndef __HAL_NAND_H__
#define __HAL_NAND_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CSCA 		:3;																	// Число периодов(+1) CLK с момента активизации NF_CS [2:0] до момента активизации сигналов nRE или nWE
			uint32_t CA 		:5;																	// Число периодов (+1) активности сигнала nRE во время чтения памяти
			uint32_t BWD 		:3;																	// Число периодов (+1) активности nWE во время записи
			uint32_t BRT 		:4;																	// Число периодов (+1) от момента снятия одного NF_CSx до момента активизации NF_CSx другого банка, с целью избежать конфликт на шине данных
			uint32_t BHT 		:3;																	// Число периодов (+1) CLK в течение которых NF_CSx находится в неактивном состоянии между последовательными выборками. А также число периодов (+1) после снятия nRE или nWE до следующего активного сигнала
			uint32_t MDT 		:1;																	// Тип интерфейса. Используется совместно с VGA битом
			uint32_t RESERVED1 	:2;																	//
			uint32_t VGA 		:1;																	// Тип интерфейса. Имеет значение при установленном MDT
			uint32_t RESERVED2 	:1;																	//
			uint32_t CSKPL 		:4;																	// Управление линиями NF_CSx между доступами к флэш-памяти: 0 – линии NF_CSх становятся неактивными между выборками, 1 – линии NF_CSx активны (=0) между выборками
			uint32_t RESERVED3 	:5;																	//
		} b;
	};
} NAND_IO_CFG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t ENWT 		:4;																	// Разрешение анализа входа RnB во время обмена с одним из банков NF_CS[3:0].
			uint32_t CCSE 		:4;																	// Управление активностью сигнала выборки банка. Эти биты позволяют снять выборку банка во время RnB = 0
			uint32_t RESERVED 	:13;																//
			uint32_t WTOC 		:10;																// Длительность периода “time-out”
			uint32_t TOE 		:1;																	// Разрешение контроля ситуации “time-out”
		} b;
	};
} NAND_WCT_CFG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t VOLCOL 	:3;																	// Размер страницы
			uint32_t ROWBT 		:1;																	// Число байт адреса для выборки строки
			uint32_t COMCLW 	:2;																	// Число команд для записи
			uint32_t COMCLR 	:2;																	// Число команд для чтения
			uint32_t ADRCL 		:2;																	// Число адресных циклов
			uint32_t RESERVED1 	:2;																	//
			uint32_t ADRSC 		:1;																	// Управление пропуском адресного цикла
			uint32_t DATSC		:1;																	// Управление пропуском цикла данных
			uint32_t RESERVED2 	:1;																	//
			uint32_t COMSC 		:1;																	// Управление пропуском цикла передачи команд
			uint32_t RESBF 		:5;																	// Время задержки до анализа активности RnB
			uint32_t RESAF 		:7;																	// Время задержки после завершения активности RnB
			uint32_t ADDRH 		:4;																	// Биты адреса расширения
		} b;
	};
} NAND_CFG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t COMW1 		:8;																	// Команда 1
			uint32_t COMW2 		:8;																	// Команда 2
			uint32_t COMW3 		:8;																	// Команда 3
			uint32_t RESERVED 	:8;																	//
		} b;
	};
} NAND_WR_CFG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t COMR1 		:8;																	// Команда 1
			uint32_t COMR2 		:8;																	// Команда 2
			uint32_t COMR3 		:8;																	// Команда 3
			uint32_t RESERVED 	:8;																	//
		} b;
	};
} NAND_RD_CFG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t EN 		:1;																	// Разрешение работы
			uint32_t RW 		:1;																	// Чтение (0) или запись (1)
			uint32_t RESERVED1 	:1;																	//
			uint32_t SQE 		:1;																	// Разрешение последовательного доступа (когда 0)
			uint32_t RIM 		:1;																	// Разрешение прерывания при чтении (1-разрешение)
			uint32_t TIM 		:1;																	// Разрешение прерывания при записи (1-разрешение)
			uint32_t CIM 		:1;																	// Разрешение прерывания при достижении счетчика значения 0 (1-разрешение)
			uint32_t RESERVED2 	:1;																	//
			uint32_t SZ 		:2;																	// Размер передаваемых данных
			uint32_t RESERVED3 	:1;																	//
			uint32_t DBSZ 		:1;																	// Размер шины данных FIFO контроллера
			uint32_t ERI_EN 	:1;																	// Разрешение прерывания в случае обнаружения ошибочной ситуации в работе контроллера, т.е. когда обнаруживается ситуация тайм-аут
			uint32_t RNBI_EN 	:1;																	// Разрешение прерывания в случае обнаружения положительного фронта на входе RnB, т.е. когда флаг RnB_Fрегистра состояния равен 1
			uint32_t EIM 		:1;																	// 1 – разрешение прерывания при обнаружении ошибки ЕСС
			uint32_t ECC_ON 	:1;																	// 1 – включение режима ЕСС
			uint32_t RESERVED4 	:16;																//
		} b;
	};
} NAND_CR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t EMPTY 		:1;																	// FIFO данных пусто (1)
			uint32_t FULL 		:1;																	// FIFO данных заполнено полностью (1)
			uint32_t TFS 		:1;																	// Запрос от FIFO данных на запись новых данных процессором или контроллером прямого доступа \ FIFO наполовину пусто (1)
			uint32_t RFS 		:1;																	// Запрос от FIFO данных на чтение новых данных процессором или контроллером прямого доступа \ FIFO наполовину заполнено (1)
			uint32_t IRQ 		:1;																	// Общий запрос прерывания от контроллера
			uint32_t RESERVED1 	:3;																	//
			uint32_t TOEX 		:4;																	// Флаги состояния Time-Out
			uint32_t RESERVED2 	:1;																	//
			uint32_t RNB_F 		:1;																	// Флаг изменения входа RnB из низкого уровня «0» в высокий «1»
			uint32_t RESERVED3 	:2;																	//
			uint32_t RFS_I 		:1;																	// Запрос на прерывание когда RFS==1 и бит RIM==1. Запрос может быть очищен посредством чтения буфера данных
			uint32_t TFS_I 		:1;																	// Запрос на прерывание когда ТFS==1 и бит ТIM==1. Запрос может быть очищен посредством записи в буфер данных.
			uint32_t CNTZ_I 	:1;																	// Запрос на прерывание когда значение счетчика становится равным нулю и бит СIM==1. Запрос может быть очищен посредством записи нового значения счетчика
			uint32_t TO_I 		:1;																	// Запрос на прерывание когда ТОЕХ==1 и бит ERI_EN ==1. Запрос может быть очищен посредством записи 1 в бит ТОЕХ
			uint32_t RNBF_I 	:1;																	// Запрос на прерывание когда RnB_F ==1 и бит RnBI_EN ==1. Запрос может быть очищен посредством записи 1 в бит RnB_F
			uint32_t RESERVED4 	:10;																//
			uint32_t RNB 		:1;																	// Вход готовности NAND флэш-памяти. Отражает состояние линии RnB
		} b;
	};
} NAND_SR_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef struct
{
	NAND_IO_CFG_type IO_CFG;
	NAND_WCT_CFG_type WCT_CFG;
	NAND_CFG_type NAND_CFG;
	NAND_WR_CFG_type WR_CFG;
	NAND_RD_CFG_type RD_CFG;
	uint32_t RESERV0;
	uint32_t RESERV1;
	uint32_t RESERV2;
	NAND_CR_type CR;
	NAND_SR_type SR;
	uint32_t AR;
	uint32_t CNTR;
	uint32_t DR;
	uint32_t ERR12;
	uint32_t ERR32;
	uint32_t SP_BUF;
} NAND_type;

#define LX_NAND           ( ( volatile NAND_type* ) base_NAND )

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_NAND_H__ */
