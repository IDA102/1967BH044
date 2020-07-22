/**********************************************************************************************************************
  Copyright (c) 2017 Milandr Corporation. All rights reserved.

  History:
  	  Zatonsky Pavel - Created.
**********************************************************************************************************************/
#include "driver1967VN044_NANDSamsung.h"
#include "hal_1967VN044.h"

void Driver_NANDSamsung_WaitWrite(){
	while(!(LX_NAND->SR.word & 1));
}

void Driver_NANDSamsung_WaitRead(){
	while(!(LX_NAND->SR.word & 8));
}

void Driver_NANDSamsung_WaitErase(){
	while(!(LX_NAND->SR.word & 1<<13));
}

void Driver_NANDSamsung_reset()
{
	unsigned int trash;
	LX_NAND->RD_CFG.word = 0xff;
	LX_NAND->NAND_CFG.word &= 0xFFFF000F;
	LX_NAND->NAND_CFG.word |= 0x3000;
	LX_NAND->CR.word = 0x301;
	LX_NAND->CNTR = 1;
	while( LX_NAND->SR.b.RFS == 0 );
	while( LX_NAND->SR.b.RNB == 0 );
	trash =  LX_NAND->DR & 0xff;
}


void Driver_NANDSamsung_Enable(NAND_Samsung_type numModel){
	//NAND
	LX_NAND->CR.word = 0x0;
	LX_NAND->WCT_CFG.word = 0xFF;
	LX_NAND->IO_CFG.word = 0x00A3FFFE;
	switch(numModel){
	case K9F8G08UXM:
		LX_NAND->NAND_CFG.word = 0x00E5035C;	//page size = 4kB
		break;
	case K9F2G08X0B:
		LX_NAND->NAND_CFG.word = 0x00E5035B;	//page size = 2kB
		break;
	}
	//GPIO
	*(unsigned *)PX_ALT_LOC = 0xFC;
	LX_NAND->CR.word = 0x1;
}

void Driver_NANDSamsung_Disable(void){
	//NAND Disable
	LX_NAND->CR.word = 0x0;
	//GPIO ALT FUNC Disable
	*(unsigned *)PX_ALT_LOC &= ~0xFC;
}

unsigned int Driver_NANDSamsung_Read_id()
{
	unsigned int tmp;
	LX_NAND->RD_CFG.word = 0x90;
	tmp = LX_NAND->NAND_CFG.word ;
	LX_NAND->NAND_CFG.word = 0x1;
	LX_NAND->CR.word = 0x1;
	LX_NAND->AR = 0;
	LX_NAND->CNTR = 1;
	while( LX_NAND->SR.b.RFS == 0 );
	//while( LX_NAND->SR.b.RNB == 0 );
	LX_NAND->NAND_CFG.word = tmp;
	return LX_NAND->DR;
}


unsigned char Driver_NANDSamsung_Read_status()
{
	LX_NAND->RD_CFG.word = 0x70;
	LX_NAND->NAND_CFG.word &= 0xFFFF000F;
	LX_NAND->NAND_CFG.word |= 0x1000;
	LX_NAND->CR.word = 0x301;
	LX_NAND->CNTR = 1;

	while( LX_NAND->SR.b.RFS == 0 );
	while( LX_NAND->SR.b.RNB == 0 );
	return LX_NAND->DR & 0xff;
}


void Driver_NANDSamsung_WriteInsidePage(void *src, void *dst, int size){
	int tmp, i, *pSrc;
	pSrc = (int *)src;
	LX_NAND->WR_CFG.word = WRITE_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int)dst;
	LX_NAND->CR.word = 3;
	LX_NAND->CNTR = size;
	for(i=0;i<size;i++){
		while(!(LX_NAND->SR.word & 1));
		LX_NAND->DR = *pSrc;
		pSrc++;
	}
	while(!(LX_NAND->SR.word & 1));
}

void Driver_NANDSamsung_WriteWord (int data, void *dst){
	int tmp;
	LX_NAND->WR_CFG.word = WRITE_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) dst;
	LX_NAND->CR.word = 3;
	LX_NAND->CNTR = 1;
	while(!(LX_NAND->SR.word & 1));
	LX_NAND->DR = data;
	while(!(LX_NAND->SR.word & 1));
}

void Driver_NANDSamsung_WriteBurst(void *src, void *dst, int size){
	int tmp, i, j, *pSrc, pageSizeW, amountPageWrite;
	pSrc = (int *)src;
	pageSizeW = (256<<(LX_NAND->NAND_CFG.word & 7))/4;
	if (!pageSizeW) pageSizeW = 128;
	LX_NAND->WR_CFG.word = WRITE_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) dst;
	LX_NAND->CR.word = 3;
	tmp = pageSizeW - ((int)dst & (pageSizeW-1));
	amountPageWrite = (size-tmp)/(pageSizeW);
	tmp = (tmp<size)?tmp:size;
	LX_NAND->CNTR = tmp;
	for(i=0;i<tmp;i++){
		while(!(LX_NAND->SR.word & 1));
		LX_NAND->DR = *pSrc;
		pSrc++;
	}
	while(!(LX_NAND->SR.word & 1));
	for(j=0;j<amountPageWrite;j++){
		LX_NAND->CNTR = pageSizeW;
		for(i=0;i<(pageSizeW);i++){
			while(!(LX_NAND->SR.word & 1));
			LX_NAND->DR = *pSrc;
			pSrc++;
		}
		while(!(LX_NAND->SR.word & 1));
	}
	i = amountPageWrite*(pageSizeW)+tmp;
	LX_NAND->CNTR = size - i;
	for(;i<size;i++){
		while(!(LX_NAND->SR.word & 1));
		LX_NAND->DR = *pSrc;
		pSrc++;
	}
	while(!(LX_NAND->SR.word & 1));
}

void Driver_DMANANDSamsung_WriteInsidePage(int dmaCh, void *tcb, void *dst){
	int tmp, shCount;
	LX_NAND->CR.word = 0;
	HAL_DMA_RqstSet(dmaCh,dmaNAND);
	HAL_DMA_WriteDC(dmaCh,tcb);
	if ( ((int *)tcb)[0]&(~1UL<<29) ) shCount = 22;
	else shCount = 16;
	LX_NAND->WR_CFG.word = WRITE_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) dst;
	tmp = (((int *)tcb)[3] & (3<<25))>>15;	//get DMA Length
	LX_NAND->CR.word = 3 | (tmp & (2<<10));		//enable read and set Bus size for FIFO
	tmp = ((int *)tcb)[1]>>shCount * ( (((int *)tcb)[2]>>shCount) + 1 );
	LX_NAND->CNTR = tmp;
}


void Driver_NANDSamsung_ReadInsidePage(void *src, void *dst, int size){
	int tmp, i, *pDst;
	pDst = (int *)dst;
	LX_NAND->RD_CFG.word = READ_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) src;
	LX_NAND->CR.word = 1;
	LX_NAND->CNTR = size;
	for(i=0;i<size;i++){
		while(!(LX_NAND->SR.word & 8));
		pDst[i] = LX_NAND->DR;
	}
}

int Driver_NANDSamsung_ReadWord(void *src){
	int tmp;
	LX_NAND->RD_CFG.word = READ_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) src;
	LX_NAND->CR.word = 1;
	LX_NAND->CNTR = 1;
	while(!(LX_NAND->SR.word & 8));
	tmp = LX_NAND->DR;
	return tmp;
}

void Driver_NANDSamsung_ReadBurst(void *src, void *dst, int size){
	int tmp, i, j, amountPageRead, pageSizeW, *pDst;
	pDst = (int *)dst;
	pageSizeW = (256<<(LX_NAND->NAND_CFG.word & 7))/4;
	if (!pageSizeW) pageSizeW = 128/4;
	LX_NAND->RD_CFG.word = READ_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) src;
	LX_NAND->CR.word = 1;
	tmp = pageSizeW - ((int)src&(pageSizeW-1));
	amountPageRead = (size-tmp)/(pageSizeW);
	tmp = (tmp<size)?tmp:size;
	LX_NAND->CNTR = tmp;
	for(i=0;i<tmp;i++){
		while(!(LX_NAND->SR.word & 8)) {}
		*pDst = LX_NAND->DR;
		pDst++;
	}
	for(j=0;j<amountPageRead;j++){
		LX_NAND->CNTR = pageSizeW;
		for(i=0;i<(pageSizeW);i++){
			while(!(LX_NAND->SR.word & 8));
			*pDst = LX_NAND->DR;
			pDst++;
		}
	}
	i = amountPageRead*(pageSizeW)+tmp;
	LX_NAND->CNTR = size - i;
	for(;i<size;i++){
		while(!(LX_NAND->SR.word & 8)) {}
		*pDst = LX_NAND->DR;
		pDst++;
	}
}

void Driver_DMANANDSamsung_ReadInsidePage(int dmaCh, void *tcb, void *src){
	int tmp, shCount;
	LX_NAND->CR.word = 0;
	HAL_DMA_RqstSet(dmaCh,dmaNAND);
	HAL_DMA_WriteDC(dmaCh,tcb);
	if ( ((int *)tcb)[0]&(~1UL<<29) ) shCount = 22;
	else shCount = 16;
	LX_NAND->RD_CFG.word = READ_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x0350;
	LX_NAND->NAND_CFG.word = tmp;
	LX_NAND->AR = (unsigned int) src;
	tmp = (((int *)tcb)[3] & (3<<25))>>15;	//get DMA Length
	LX_NAND->CR.word = 1 | (tmp & (2<<10));		//enable read and set Bus size for FIFO
	tmp = ((int *)tcb)[1]>>shCount * ( (((int *)tcb)[2]>>shCount) + 1 );
	LX_NAND->CNTR = tmp;
}


void Driver_NANDSamsung_EraseBlocks(int numBlock, int amount){
	int tmp, shAr, i;
	LX_NAND->RD_CFG.word = ERASE_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x2240;
	LX_NAND->NAND_CFG.word = tmp;
	shAr = LX_NAND->NAND_CFG.word & 7;		//get min bit address for block
	if (shAr) shAr+=12;
	else shAr=11;
	LX_NAND->CR.word = 0x301;
	for(i=0;i<amount;i++){
		LX_NAND->SR.word = (1<<13);				//reset bit 13 from Status Register
		LX_NAND->AR = numBlock<<shAr;
		LX_NAND->CNTR = 1;
		while(!(LX_NAND->SR.word & 1<<13));		//wait Erase
		while(!(LX_NAND->SR.word & 8));
		tmp = LX_NAND->DR;					//read trash byte
		numBlock++;
	}
}

void Driver_NANDSamsung_EraseMem(int memSize){
	int amountBlocks, pageSize;
	pageSize = 256<<(LX_NAND->NAND_CFG.word & 7);
	if (pageSize == 256) pageSize = 128;
	amountBlocks = ((1024*1024)*memSize)/(64*pageSize);
	Driver_NANDSamsung_EraseBlocks(0, amountBlocks);
}


int Driver_NANDSamsung_GetPageSize(void){
	int pageSizeByte;
	pageSizeByte = (256<<(LX_NAND->NAND_CFG.word & 7));
	if (pageSizeByte == 256) pageSizeByte = 128;
	return pageSizeByte;
}





