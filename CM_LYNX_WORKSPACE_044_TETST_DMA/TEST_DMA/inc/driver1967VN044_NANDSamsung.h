/**********************************************************************************************************************
  Copyright (c) 2017 Milandr Corporation. All rights reserved.

  History:
  	  Zatonsky Pavel - Created.
**********************************************************************************************************************/
#ifndef __HAL_NAND_SAMSUNG_K9F8G08UXM_H__
#define __HAL_NAND_SAMSUNG_K9F8G08UXM_H__

#include "hal_nand.h"

typedef enum { K9F8G08UXM, K9F2G08X0B} NAND_Samsung_type;

#define	WRITE_COMM 	0x100080
#define	READ_COMM	0x003000
#define	ERASE_COMM	0x00D060

/*******************Include Function*********************/
#ifdef __cplusplus
extern "C"
{
#endif

void Driver_NANDSamsung_WaitWrite();
void Driver_NANDSamsung_WaitRead();
void Driver_NANDSamsung_WaitErase();

void Driver_NANDSamsung_Enable(NAND_Samsung_type numModel);
void Driver_NANDSamsung_Disable();

void Driver_NANDSamsung_reset();
unsigned int Driver_NANDSamsung_Read_id();
unsigned char Driver_NANDSamsung_Read_status();

void Driver_NANDSamsung_WriteInsidePage(void *src, void *dst, int size);
void Driver_NANDSamsung_WriteWord (int data, void *dst);
void Driver_NANDSamsung_WriteBurst(void *src, void *dst, int size);
void Driver_DMANANDSamsung_WriteInsidePage(int dmaCh, void *tcb, void *dst);

void Driver_NANDSamsung_ReadInsidePage(void *src, void *dst, int size);
void Driver_NANDSamsung_ReadBurst(void *src, void *dst, int size);
int  Driver_NANDSamsung_ReadWord(void *src);
void Driver_DMANANDSamsung_ReadInsidePage(int dmaCh, void *tcb, void *src);

void Driver_NANDSamsung_EraseBlocks(int numBlock, int amount);
void Driver_NANDSamsung_EraseMem(int memSize);

int Driver_NANDSamsung_GetPageSize(void);

#ifdef __cplusplus
}
#endif // __cplusplus

/********************************************************/



#endif
