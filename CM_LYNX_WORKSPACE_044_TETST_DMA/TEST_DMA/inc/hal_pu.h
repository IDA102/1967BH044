#ifndef __HAL_PU_H__
#define __HAL_PU_H__


#include "def1967VN044.h"
#include <stdint.h>

//PUx = j5;;
asm ("#define PUx_Write(reg_name)\
         __INSTR  0x89851F00 + (reg_name << 0)");

//j1 = PUx;;
asm ("#define PUx_Read(reg_name)\
      __INSTR     0x8BE00C02 + (reg_name << 16)");

//PU_CR = j5;;
asm ("#define PU_CR_Write()\
         __INSTR  0x89851F38");

//j1 = CR_CR;;
asm ("#define PU_CR_Read()\
        __INSTR  0x8BFC0C02");


typedef union{
	uint32_t word;
	struct{
		uint32_t STA			: 11;		//PUx[10:0]
		uint32_t RES0			: 1;		//PUx[11]
		uint32_t ENDA			: 11;		//PUx[22:12]
		uint32_t RES1			: 1;	    //PUx[23]
		uint32_t JK_AP			: 2;		//PUx[25:24]
		uint32_t I_AP			: 2;		//PUx[27:26]
		uint32_t H_AP 			: 2;		//PUx[29:28]
		uint32_t RES2			: 2;		//PUx[31:30]
	}b;
}PUx_reg;

typedef union {
	uint32_t word;
	struct {
		uint32_t DC_ON			: 1;		//PU_CR[0]
		uint32_t IC_ON	 		: 1;		//PU_CR[1]
		uint32_t EN_2dQW 		: 1;		//PU_CR[2]
		uint32_t EN_2iQW		: 1;		//PU_CR[3]
		uint32_t SOC_speed_up 	: 1;		//PU_CR[4]
		uint32_t RES0			: 3;		//PU_CR[7:5]
		uint32_t PSD_fun		: 1;		//PU_CR[8]
		uint32_t SP_byte		: 1;		//PU_CR[9]
		uint32_t MIN_MAX		: 1;		//PU_CR[10]
		uint32_t LC_sup			: 1;		//PU_CR[11]
		uint32_t RES1			: 4;		//PU_CR[15:12]
		uint32_t PU0_EN			: 1;		//PU_CR[16]
		uint32_t PU1_EN			: 1;		//PU_CR[17]
		uint32_t PU2_EN			: 1;		//PU_CR[18]
		uint32_t PU3_EN			: 1;		//PU_CR[19]
		uint32_t PU4_EN			: 1;		//PU_CR[20]
		uint32_t PU5_EN			: 1;		//PU_CR[21]
		uint32_t PU6_EN			: 1;		//PU_CR[22]
		uint32_t PU7_EN			: 1;		//PU_CR[23]
		uint32_t RES2			: 6;		//PU_CR[29:24]
		uint32_t USOC_EN		: 2;		//PU_CR[31:30]
	}b;
}PU_CR_reg;

typedef enum {
	PU0,
	PU1,
	PU2,
	PU3,
	PU4,
	PU5,
	PU6,
	PU7,
	PU_CR
}PUx_num;

/*******************Include Function**********************/
#ifdef __cplusplus
extern "C"
{
#endif

	void HAL_PUx_Write(PUx_num PU_reg, uint32_t *InitStruct);
	uint32_t HAL_PUx_Read (PUx_num PU_reg);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_PU_H__ */
