/*
 * H.h
 *
 *  Created on: 16 июля 2021 г.
 *      Author: GuRu78
 */

#ifndef H_H_
#define H_H_


#include <hal_1967VN044.h>
#include "hal_1967VN044.h"
#include <builtins.h>
#include <sysreg.h>
#include <stddef.h>
#include "board.h"
#include <stdio.h>
#include <stdlib.h>

/*DMA*/
#define TCB_LINK        (0x20000000)
#define TCB_INTMEM16_16 (0x40000000)
#define TCB_INTMEM22_10 (0x60000000)
#define TCB_EXTMEM16_16 (0x80000000)
#define TCB_EXTMEM22_10 (0xE0000000)

/*#define HAL_DMA_INIT_MEM_TYPE(addr,TYPE) \
	if ((addr >= (0x00000000) && addr <= (0x0000FFFF)) || (addr >= 0x00040000 && addr <= (0x04FFFF)) ||\
		(addr >= 0x080000 && addr <= (0x08FFFF)) || (addr >= 0x0C0000 && addr <= (0x0CFFFF)) ||\
		(addr >= 0x100000 && addr <= (0x10FFFF)) || (addr >= 0x140000 && addr <= (0x14FFFF)))\
		TYPE = TCB_INTMEM16_16;\
	else if((addr >= 0x30000000 && addr <= (0x44000000) ) || (addr >= 0x50000000 && addr <= (0x54000000)) ||\
			(addr >= 0x60000000 && addr <= (0x64000000)) || (addr >= 0x70000000 && addr <= (0x74000000)) || (addr >= 0x80000000 && addr <= (0xFFFFFFFF)))\
		 TYPE = TCB_EXTMEM16_16;\
	else return -1;*/

/*OTHER*/
#define NOP asm("nop;;")

#endif /* H_H_ */
