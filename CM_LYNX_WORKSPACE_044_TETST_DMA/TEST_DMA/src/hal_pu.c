#include "stdint.h"
#include "hal_pu.h"

void HAL_PUx_Write (PUx_num PU_reg, uint32_t *InitStruct){
	asm("j5 = %0;;" ::"j"(*InitStruct) );

	switch(PU_reg){
	case PU0 	: 	asm("PUx_Write(0);;");
			   		break;
	case PU1 	: 	asm("PUx_Write(2);;");
			   	   	break;
	case PU2 	: 	asm("PUx_Write(4);;");
			   	   	break;
	case PU3 	: 	asm("PUx_Write(6);;");
			   	    break;
	case PU4 	: 	asm("PUx_Write(8);;");
			   	    break;
	case PU5 	: 	asm("PUx_Write(10);;");
			   	   	break;
	case PU6 	: 	asm("PUx_Write(12);;");
			   	    break;
	case PU7 	: 	asm("PUx_Write(14);;");
			   	    break;
	case PU_CR  : 	asm("PU_CR_Write();;");
  	    		 	break;
	}
}

uint32_t HAL_PUx_Read (PUx_num PU_reg){
	uint32_t Value_reg;
	switch(PU_reg){
	case PU0 	:	asm("PUx_Read(0);;");
					break;
	case PU1 	:	asm("PUx_Read(1);;");
					break;
	case PU2 	:	asm("PUx_Read(2);;");
					break;
	case PU3 	:	asm("PUx_Read(3);;");
					break;
	case PU4 	:	asm("PUx_Read(4);;");
					break;
	case PU5 	:	asm("PUx_Read(5);;");
					break;
	case PU6 	:	asm("PUx_Read(6);;");
					break;
	case PU7 	:	asm("PUx_Read(7);;");
					break;
	case PU_CR	:   asm("PU_CR_Read();;");
					break;
	}
	asm("[%0+0] = j1;;" ::"j"(&Value_reg));
	return Value_reg;
}
