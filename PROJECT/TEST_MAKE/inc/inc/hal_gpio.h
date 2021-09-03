/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_gpio.h
 *	Описание: 	HAL для модуля портов ввода/вывода
 *
 *	История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	изменено оформление и функция HAL_GPIO_Px_AltDeInit
 *	 			14-Mar-2017 Ivan Osadchy 	 	- 	добавлено LX_GPIO_...
 *				02-Mar-2017 Ivan Osadchy 	  	- 	создан
 *
 */

#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#include <stdint.h>
#include "def1967VN044.h"

/* ----------------------------------------- Типы регистров ------------------------------------- */
// Набор из 4х регистров:
typedef volatile struct
{
	uint32_t LD;
	uint32_t SET;
	uint32_t CLR;
	uint32_t INV;
} GPIO_REGSET;

/* ------------------------------------ Структура периферии ------------------------------------- */
// Набор регистров для портов PA, PB и PC:
typedef volatile struct
{
	GPIO_REGSET DR;
	GPIO_REGSET DDR;
	GPIO_REGSET PEIE;
	GPIO_REGSET NEIE;
	GPIO_REGSET INVR;
	GPIO_REGSET IMR;
	GPIO_REGSET ALT;
	GPIO_REGSET PUR;
	uint32_t 	PXD;
	uint32_t 	INTREQ;
	uint32_t 	rsv;
	uint32_t 	ECLR;
} GPIO_PortPi;


// Набор регистров для порта PxA:
typedef volatile struct
{
	uint32_t DR;
	uint32_t SET;
	uint32_t CLR;
} GPIO_PortPxA;

// Набор регистров для порта PxD:
typedef volatile struct
{
	uint32_t DR;
	uint32_t SET;
	uint32_t CLR;
	uint32_t DIR;
	uint32_t res[5];
	uint32_t PXD;
	uint32_t PUR;
} GPIO_PortPxD;

// Регистр PxALT:
typedef volatile struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t PDB 	: 4;
			uint32_t PAB 	: 3;
			uint32_t PDXF 	: 2;
			uint32_t res 	: 23;
		} b;
	};
} GPIO_RegPxALT;

#define 	LX_GPIO_PA 			( ( volatile GPIO_PortPi 	* ) base_GPA )
#define 	LX_GPIO_PB 			( ( volatile GPIO_PortPi 	* ) base_GPB )
#define 	LX_GPIO_PC 			( ( volatile GPIO_PortPi 	* ) base_GPC )
#define 	LX_GPIO_PxA 		( ( volatile GPIO_PortPxA  * ) PXA_LOAD_LOC )
#define 	LX_GPIO_PxD 		( ( volatile GPIO_PortPxD  * ) PXD_LOAD_LOC )
#define 	LX_GPIO_PxALT 		( ( volatile GPIO_RegPxALT * ) PX_ALT_LOC )

/* -------------------------------------- Специфические типы ------------------------------------ */
// Набор пинов порта:
typedef enum
{
	GPIO_PIN_0 =  0x00000001UL,
	GPIO_PIN_1 =  0x00000002UL,
	GPIO_PIN_2 =  0x00000004UL,
	GPIO_PIN_3 =  0x00000008UL,
	GPIO_PIN_4 =  0x00000010UL,
	GPIO_PIN_5 =  0x00000020UL,
	GPIO_PIN_6 =  0x00000040UL,
	GPIO_PIN_7 =  0x00000080UL,
	GPIO_PIN_8 =  0x00000100UL,
	GPIO_PIN_9 =  0x00000200UL,
	GPIO_PIN_10 = 0x00000400UL,
	GPIO_PIN_11 = 0x00000800UL,
	GPIO_PIN_12 = 0x00001000UL,
	GPIO_PIN_13 = 0x00002000UL,
	GPIO_PIN_14 = 0x00004000UL,
	GPIO_PIN_15 = 0x00008000UL,
	GPIO_PIN_16 = 0x00010000UL,
	GPIO_PIN_17 = 0x00020000UL,
	GPIO_PIN_18 = 0x00040000UL,
	GPIO_PIN_19 = 0x00080000UL,
	GPIO_PIN_20 = 0x00100000UL,
	GPIO_PIN_21 = 0x00200000UL,
	GPIO_PIN_22 = 0x00400000UL,
	GPIO_PIN_23 = 0x00800000UL,
	GPIO_PIN_24 = 0x01000000UL,
	GPIO_PIN_25 = 0x02000000UL,
	GPIO_PIN_26 = 0x04000000UL,
	GPIO_PIN_27 = 0x08000000UL,
	GPIO_PIN_28 = 0x10000000UL,
	GPIO_PIN_29 = 0x20000000UL,
	GPIO_PIN_30 = 0x40000000UL,
	GPIO_PIN_31 = 0x80000000UL,
} GPIO_PIN;

// Тип порта Px:
typedef enum
{
	GPIO_PxPort_A = 0,
	GPIO_PxPort_D = 1,
} GPIO_PxPort;

// Режимы работы пина:
typedef enum
{
	GPIO_PinMode_In 		= 0,
	GPIO_PinMode_InPU		= 1,
	GPIO_PinMode_Out		= 2,
	GPIO_PinMode_OutPU		= 3,
	GPIO_PinMode_Alt		= 4
} GPIO_PinMode;


// Состояния пина:
typedef enum
{
	GPIO_PinState_Reset 	= 0,
	GPIO_PinState_Set 		= 1,
} GPIO_PinState;


// Различные варианты альтернативных функций для PxA и PxD:
typedef enum
{
	GPIO_PxAlt_Data7_0	 	= 0x1,
	GPIO_PxAlt_Data15_8	 	= 0x2,
	GPIO_PxAlt_Data23_16 	= 0x4,
	GPIO_PxAlt_Data31_24 	= 0x8,
	GPIO_PxAlt_Addr7_0	 	= 0x10,
	GPIO_PxAlt_Addr15_8	 	= 0x20,
	GPIO_PxAlt_Addr21_16 	= 0x40,
	GPIO_PxAlt_All		 	= 0x7F
} GPIO_PxAlt_type;

// Различные варианты альтернативных функций для PxA и PxD:
typedef enum
{
	GPIO_PxAlt_Mode_SDRAM  	= 0x0,
	GPIO_PxAlt_Mode_NAND   	= 0x1,
	GPIO_PxAlt_Mode_MILSTD 	= 0x3
} GPIO_PxAlt_Mode_type;

// Режимы прерываний:
typedef enum
{
	GPIO_InterruptMode_Off,
	GPIO_InterruptMode_Posedge,
	GPIO_InterruptMode_Negedge,
	GPIO_InterruptMode_LowLvl,
	GPIO_InterruptMode_HighLvl,
} GPIO_InterruptMode;

#ifdef __cplusplus
  extern "C"
	{
	#endif

  	  void 				HAL_GPIO_Init 				( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins, const GPIO_PinMode Mode );
  	  void 				HAL_GPIO_DeInit 			( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins );
  	  void 				HAL_GPIO_PxD_Init 			( const uint32_t Pins, const GPIO_PinMode Mode );
  	  void 				HAL_GPIO_PxD_DeInit 		( const uint32_t Pins );
  	  void 				HAL_GPIO_Px_AltInit 		( const GPIO_PxAlt_type PxAlt, const GPIO_PxAlt_Mode_type PxAltMode );
  	  void 				HAL_GPIO_Px_AltDeInit		( const GPIO_PxAlt_type PxAlt );

  	  void 				HAL_GPIO_WritePin 			( GPIO_PortPi * const GPIO_Pi, const GPIO_PIN Pin, const GPIO_PinState NewState );
  	  GPIO_PinState 	HAL_GPIO_ReadPin 			( GPIO_PortPi * const GPIO_Pi, const GPIO_PIN Pin );
  	  void 				HAL_GPIO_WritePins 			( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins, const uint32_t Data );
  	  void 				HAL_GPIO_TogglePins 		( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins );
  	  uint32_t 			HAL_GPIO_ReadPins 			( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins );

  	  void 				HAL_GPIO_Px_WritePins 		( const GPIO_PxPort Px, const uint32_t Pins, const uint32_t Data );
  	  uint32_t 			HAL_GPIO_Px_ReadPins 		( const GPIO_PxPort Px, const uint32_t Pins );

  	  void 				HAL_GPIO_InterruptConfig 	( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins, const GPIO_InterruptMode Mode );
  	  uint32_t 			HAL_GPIO_InterruptCheck		( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins );
  	  void 				HAL_GPIO_InterruptEdgeClear	( GPIO_PortPi * const GPIO_Pi, const uint32_t Pins );
	#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_GPIO_H__ */
