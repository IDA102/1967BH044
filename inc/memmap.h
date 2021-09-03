#include "DMA.h"

#ifndef memmap_H_
#define memmap_H_

/*MEMORY_MAP*/
#define INTERNAL_BANK_0(addr)     ((0x000000<=addr) && (addr<=0x00FFFF))
#define INTERNAL_BANK_1(addr)     ((0x040000<=addr) && (addr<=0x04FFFF))
#define INTERNAL_BANK_2(addr)     ((0x080000<=addr) && (addr<=0x08FFFF))
#define INTERNAL_BANK_3(addr)     ((0x0C0000<=addr) && (addr<=0x0CFFFF))
#define INTERNAL_BANK_4(addr)     ((0x100000<=addr) && (addr<=0x1FFFFF))
#define INTERNAL_BANK_5(addr)     ((0x140000<=addr) && (addr<=0x14FFFF))

#define INTERNAL_REGISTERS(addr)  ((0x1E0000<=addr) && (addr<=0x1E03FF))
#define PERIPHERY_REGISTERS(addr) ((0x1F0000<=addr) && (addr<=0x1F03FF))

#define SRAM_BANK_0(addr)         ((0x30000000<=addr) && (addr<=0x37FFFFFF))
#define SRAM_BANK_1(addr)         ((0x38000000<=addr) && (addr<=0x3FFFFFFF))

#define SDRAM_BANK_0(addr)        ((0x40000000<=addr) && (addr<=0x43FFFFFF))
#define SDRAM_BANK_1(addr)        ((0x50000000<=addr) && (addr<=0x53FFFFFF))
#define SDRAM_BANK_2(addr)        ((0x60000000<=addr) && (addr<=0x63FFFFFF))
#define SDRAM_BANK_3(addr)        ((0x70000000<=addr) && (addr<=0x73FFFFFF))

/*ОПРЕДЕЛЕНИЯ ТИПА ПАМЯТИ И ТИПА ПЕРЕДАЧИ*/
static int32_t HAL_DMA_INIT_MEM_TYPE( const void* addr )
{
  if(INTERNAL_BANK_0(addr) ||
     INTERNAL_BANK_1(addr) ||
     INTERNAL_BANK_2(addr) ||
     INTERNAL_BANK_3(addr) ||
     INTERNAL_BANK_4(addr) ||
     INTERNAL_BANK_5(addr))
    { 
      puts("INTERNAL BANK");
      return DP_DMA_TYPE_EXCHANGE_INT_MEM_16_16;
    }
  else if( INTERNAL_REGISTERS(addr) )
    { 
      puts("INTERNAL REGISTERS");  
      return -1; 
    }
  else if( PERIPHERY_REGISTERS(addr) )
    { 
      puts("PERIPHERY REGISTERS"); 
      return -1; 
    }
  else if(  SRAM_BANK_0(addr) || SRAM_BANK_1(addr) )
    { 
      puts("SRAM BANK");
      return DP_DMA_TYPE_EXCHANGE_EXT_MEM_16_16;
    }
  else if( SDRAM_BANK_0(addr) ||
           SDRAM_BANK_1(addr) ||
           SDRAM_BANK_2(addr) ||
           SDRAM_BANK_3(addr))
    {
      puts("SRAM BANK");
      return DP_DMA_TYPE_EXCHANGE_EXT_MEM_16_16;
    }
}

static void SYSCON_BusConfig(int BusSize)
{
	int tmp;
	switch(BusSize)
  {
    case 16:
      tmp = __builtin_sysreg_read(__SYSCON);
      tmp |= (1<<19);
      __builtin_sysreg_write(__SYSCON,tmp);
      break;
    case 32:
      tmp = __builtin_sysreg_read(__SYSCON);
      tmp &= ~(1<<19);
      __builtin_sysreg_write(__SYSCON,tmp);
      break;
	}
}

static void ErrorStop(unsigned int testNumber)
{
	uint32_t errFlag = testNumber;
	if (testNumber) __builtin_sysreg_write(__FLAGREG,0xCF);
	while(1);
}

/*ОПРЕДЕЛЕНИЯ НОМЕРА ИСПОЛЬЗУЕМОГО БАНКА ДЛЯ "INTERNAL BANK"*/
static int32_t num_bank( const void* addr )
{BRD_init();
       if(INTERNAL_BANK_0(addr)){ puts("BANK 0"); return 0; }
  else if(INTERNAL_BANK_1(addr)){ puts("BANK 1"); return 0; }
  else if(INTERNAL_BANK_2(addr)){ puts("BANK 2"); return 0; }
  else if(INTERNAL_BANK_3(addr)){ puts("BANK 3"); return 0; }
  else if(INTERNAL_BANK_4(addr)){ puts("BANK 4"); return 0; }
  else if(INTERNAL_BANK_5(addr)){ puts("BANK 5"); return 0; }
  else return -1;
}
#endif
