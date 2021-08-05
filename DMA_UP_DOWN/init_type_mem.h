/*DMA_TYPE_MEMOTY*/
#define TCB_LINK        (0x20000000)
#define TCB_INTMEM16_16 (0x40000000)
#define TCB_INTMEM22_10 (0x60000000)
#define TCB_EXTMEM16_16 (0x80000000)
#define TCB_EXTMEM22_10 (0xE0000000)

/*MEMORY_MAP*/
#define INTERNAL_BANK_0(addr)      ((0x000000<=addr) && (addr<=0x00FFFF))
#define INTERNAL_BANK_1(addr)      ((0x040000<=addr) && (addr<=0x04FFFF))
#define INTERNAL_BANK_2(addr)      ((0x080000<=addr) && (addr<=0x08FFFF))
#define INTERNAL_BANK_3(addr)      ((0x0C0000<=addr) && (addr<=0x0CFFFF))
#define INTERNAL_BANK_4(addr)      ((0x100000<=addr) && (addr<=0x1FFFFF))
#define INTERNAL_BANK_5(addr)      ((0x140000<=addr) && (addr<=0x14FFFF))

#define INTERNAL_REGISTERS(addr)   ((0x1E0000<=addr) && (addr<=0x1E03FF))
#define PERIPHERY_REGISTERS(addr)  ((0x1F0000<=addr) && (addr<=0x1F03FF))

#define SRAM_BANK_0(addr)   ((0x30000000<=addr) && (addr<=0x37FFFFFF))
#define SRAM_BANK_1(addr)   ((0x38000000<=addr) && (addr<=0x3FFFFFFF))

#define SDRAM_BANK_0(addr)  ((0x40000000<=addr) && (addr<=0x43FFFFFF))
#define SDRAM_BANK_1(addr)  ((0x50000000<=addr) && (addr<=0x53FFFFFF))
#define SDRAM_BANK_2(addr)  ((0x60000000<=addr) && (addr<=0x63FFFFFF))
#define SDRAM_BANK_3(addr)  ((0x70000000<=addr) && (addr<=0x73FFFFFF))

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
      return TCB_INTMEM16_16; 
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
      return TCB_EXTMEM16_16; 
    }
  else if( SDRAM_BANK_0(addr) ||
           SDRAM_BANK_1(addr) ||
           SDRAM_BANK_2(addr) ||
           SDRAM_BANK_3(addr))
    {
      puts("SRAM BANK");
      return TCB_EXTMEM16_16;
    }
}