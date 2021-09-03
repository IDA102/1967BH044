
#include "H.h"

#define NOP asm("nop;;")
#define DDC_CR *(unsigned int*)(0x80000280)



__uint128_t data ;
__attribute__((aligned (16)))
unsigned int array[] = {0x00ff00ff,0x00ff00ff,0x00ff00ff,0x00ff00ff};

int main(void)
{
	BRD_init();
	static FILE *FP;
	//static FILE *FIN;
	size_t ind = 0;

	__uint128_t* pMASS = (__uint128_t*)calloc(16382,sizeof(__uint128_t));
	//__uint128_t* MASS = new __uint128_t[65536];
	//REG_STEP
	*(unsigned int*)(0x80000280+2) = 0x1;
	//REG_CR
	DDC_CR = 0x101;

	//FIN = fopen("W:/ML/MSO/UP_DOWN_CONVERTER/DDCIN.bin","rb");
    FP  = fopen("W:/ML/MSO/UP_DOWN_CONVERTER/UDD.bin","a+b");

  for(size_t i = 0; i < 8192; i++)
  {
    while((*(unsigned int*)(0x80000280+1) & 0x1) == 0);
   *(__uint128_t*)(0x80000280+4) = *(__uint128_t*) array ;

    if((*(unsigned int*)(0x80000280+1) & 0x2) == 2)
    {
      //pMASS[ind++] = *(__uint128_t*)(0x80000280+4) ;
      pMASS++ = *(__uint128_t*)(0x80000280+4) ;
    }
  }
  fwrite(MASS,sizeof(data),16382,FP) ;
  free(MASS);
  fclose(FP);
  //fclose(FIN);
  return 0;
}



