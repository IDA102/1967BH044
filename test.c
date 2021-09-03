/*Описание что делает*/

#include <hal_1967VN044.h>
#include "hal_1967VN044.h"
#include <builtins.h>
#include <sysreg.h>
#include <stddef.h>
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <driver1967VN044_NANDSamsung.h>
#include "../inc/memmap.h"
#include "../inc/_dduc.h"
#include "5101HB015.h"
#include "5101HB015_SPI_1967VC3.h"
#include <stdint.h>

#define NOP asm("nop;;")

/*ПОДКЛЮЧЕНИЕ SDRAM*/
#define BUS_CLK_kHz		80000      //25000 //-------------------------------------------------
#define CORE_CLK_kHz	200000     //200000//210000

#define X2_SDRAM_MODEL_48LC16M16
#ifdef X2_SDRAM_MODEL_48LC16M16
  #define SDRCON_PG         SDRCON_PG256
  #define DATA_BUS_WIDTH    32
  #define SYS_BUS           SYS_BUS_32b
#endif

#define N_PI_PO       1020*3//65532 //КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВАХ PING & PONG, кратные 4.
#define S             100
#define N             N_PI_PO*S//65532 //КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВАХ PING & PONG, кратные 4.

#define DMA_CH_OUTPUT 8   //НОМЕР DMA ПРИЁМНИКА ( УСТРОЙСТВО -> ПАМЯТЬ )
#define OUTPUTFNAME   "w:/MILANDR/MSO/CURRENT_FOLDER/1967BH044/1967BH044/oddc.bin"
#define OUTPUTFNAME2  "w:/MILANDR/MSO/CURRENT_FOLDER/1967BH044/1967BH044/oddc2.bin"

static int32_t HAL_DMA_INIT_MEM_TYPE( const void* addr );// ОПРЕДЕЛЕНИЯ ТИПА ПАМЯТИ И ТИПА ПЕРЕДАЧИ
static int32_t num_bank( const void* addr );//ОПРЕДЕЛЕНИЯ НОМЕРА ИСПОЛЬЗУЕМОГО БАНКА ДЛЯ "INTERNAL BANK"

__attribute__((aligned(4))) uint32_t otcb_ping_1[4];//PING
__attribute__((aligned(4))) uint32_t otcb_pong_1[4];//PONG

__attribute__((aligned(4)))
__attribute__((section("datasectionping"))) uint32_t samples_ping_mem[N];// max 1'107'296'256 по 32(размер одного банка)
//__attribute__((aligned(4)))
//__attribute__((section("datasectionpong"))) uint32_t samples_pong_mem[N_PI_PO*2];// max 1'107'296'256‬ по 32(размер одного банка)

static uint32_t flag,cnt;
//static uint32_t ADDR = (0x10000000+N_PI_PO);
static uint32_t* ADDR = (0x10000000+N_PI_PO);

//Функция обработчика прерывания DMA PING PONG
__attribute((interrupt))
static void dma_handler()
{
  if(cnt >= S)
  {
	  HAL_DMA_Stop( DMA_CH_OUTPUT );
	  return;
  }
  cnt++;
  ADDR += N_PI_PO;
  flag = flag ^ 0x00000001;
  if(flag == 1)
  {
	otcb_ping_1[0] = (void*)(ADDR);
  }
  if(flag == 0)
  {
	otcb_pong_1[0] = (void*)(ADDR);
  }
}

//#define CFG_FILE
int main(void)
{
  FILE* ofid;
  int iqcnt,ErrFlag;
  uint32_t TCB_TYPE_MEMORY;
  uint32_t status=0,sdrconValue;

  ADC5101HB015_config_t    ADC_CFG;          //Cтруктура для записи конфигурации АЦП
  ADC5101HB015_hw_config_t ADC_SPI_CFG;	     //Cтруктура для настройки SPI АЦП
  LinkRx_Init_type         LINK_ADC_INI_STR; //Структура для описания внешенего устройства с LINK интерфейсом (от кого принимаем)
  LinkRxEx_type            LINK_RX_INI_STR;  //Структура для инициализации приемника LINK порта

  /*SDRAM*/
  HAL_PLL_CoreSetup(CORE_CLK_kHz);
  HAL_PLL_BusSetup(BUS_CLK_kHz);

  HAL_SYS_FlagEnable();
  __builtin_sysreg_write(__FLAGREG, 0x01); //START//0xEF

  LX_CMU->CFG1.b.SYS_WE = 1; //ВКЛЮЧИТЬ МНОЖЕСТВЕННУЮ ЗАПИСЬ В SYSCON/SDRCON - Multiple write to SYSCON/SDRCON enable

  //SDRCON Configure
  sdrconValue = SDRCON_ENBL    |
                SDRCON_CLAT2   |
                SDRCON_PG256   |
                SDRCON_REF3700 |
                SDRCON_PC2RAS2 |
                SDRCON_RAS2PC2 |
                SDRCON_EMRS    |
                SDRCON_INIT    ;
  //SYSCON Bus and GPIO
  SYSCON_BusConfig(DATA_BUS_WIDTH);
  HAL_SYS_ExtBusEnable(SYS_BUS);
  HAL_SYS_SDRAMEnable(sdrconValue);
  //BRD_init();-------------------------------------------------------------------------------------
  for( uint32_t i = 0 ; i < N ; i++) {*(uint32_t*)(0x10000000+i) = 0;}

/* Инициализация АЦП 0 по spi*/
// Задание выводов SPI интерфейса АЦП 0 и 1
  ADC_SPI_CFG.port           = LX_GPIO_PC;
  ADC_SPI_CFG.cs_bit         = (1 << 2); // FLAG[2]
  ADC_SPI_CFG.clk_bit        = (1 << 0); // FLAG[0]
  ADC_SPI_CFG.mosi_bit       = (1 << 3); // FLAG[3]
  ADC_SPI_CFG.miso_bit       = (1 << 3);
  ADC_SPI_CFG.power_down_bit = (1 << 24);// L0ACKO
  ADC_SPI_CFG.core_freq_khz  = CORE_FREQ_KHZ;

//Задание значений регистров АЦП
  ADC_CFG.reference_level   = ADC5101HB015_REF_1P0;
  ADC_CFG.output_format     = ADC5101HB015_OUTPUT_LVDS;
  ADC_CFG.lvdsen_pin_state  = ADC5101HB015_LVDSEN_PIN_HIGH;
  ADC_CFG.lvds_current_mode = ADC5101HB015_LVDS_CURRENT_NORMAL;
  ADC_CFG.oen_pin_override  = ADC5101HB015_OEN_OVERRIDE;
  ADC_CFG.common_mode_sel   = ADC5101HB015_COMMON_MODE_0P75;

//Инициализация АЦП 0 и 1 (сброс, калибровка, отправка конфигурации регистров по spi)
  ADC5101HB015_init(1, 1, &ADC_SPI_CFG);
  ADC5101HB015_config(&ADC_CFG);

/* Инициализация LINK-интерфейса для работы с АЦП 0*/
  LINK_RX_INI_STR.AdcDataSize = Link_AdcDataSize_14b;
  LINK_RX_INI_STR.Rcode       = Link_Rcode_En;//Link_Rcode_En
  LINK_RX_INI_STR.BitOrder    = Link_BitOrder_OddFrontEvenCut;
  LINK_RX_INI_STR.GpsClk      = Link_GpsClk_Dis;
  LINK_RX_INI_STR.RxDest      = Link_RxDest_Buff;

  LINK_ADC_INI_STR.DataSize   = Link_DataSize_8bit;
  LINK_ADC_INI_STR.CheckBCMPI = Link_CheckBCMP_Dis;
  LINK_ADC_INI_STR.CheckSum   = Link_CheckSum_Dis;
  LINK_ADC_INI_STR.OVRIntEn   = Link_OvrIT_Dis;
  LINK_ADC_INI_STR.TOIntEn    = Link_TOIT_Dis;

  HAL_LinkRx_Disable(0);                                        //Деинициализация приемника LINK порта  0
  HAL_LinkRx_Enable(0, &LINK_ADC_INI_STR,  &LINK_RX_INI_STR);   //Инициализация приемника LINK порта  0

//Разрешение прерывания от канла DMA 8, передача указателя на функцию обработчика прерывания
  HAL_Interrupt_GlobalEnable();
  HAL_Interrupt_Enable( intDMA8 , dma_handler );

/*OUTPUT DMA*/
  HAL_DMA_RqstClr(DMA_CH_OUTPUT);                        //ОЧИСТКА ИСТОЧНИКА ЗАПРОСА DMA
  HAL_DMA_RqstSet(DMA_CH_OUTPUT, dmaSTD);                //УСТАНОВКА НОВОГО ИСТОЧНИКА ОПРОСА ДЛЯ КАНАЛА DMA_CH_OUTPUT
  TCB_TYPE_MEMORY = DP_DMA_TYPE_EXCHANGE_EXT_MEM_16_16;;//ОПРЕДЕЛЕНИЕ ТИПА ИСПОЛЬЗУЕМОЙ ПАМЯТИ !!!!!!!!!!должна быть перед заполнением структуры!!!!!
  if(TCB_TYPE_MEMORY == -1){ puts("ERROR TYPE MEMORY - TX");}

/*TCB_PING*/
  otcb_ping_1[0] = (void*)(0x10000000);   //DI
  otcb_ping_1[1] = ((N_PI_PO<<16) | 4);     //DX
  otcb_ping_1[2] = 0;                       //DY
  otcb_ping_1[3] |= TCB_TYPE_MEMORY        |
                    DP_DMA_LENGTH_DATA_128 |
					          DP_DMA_HIGH_PRIORITY   |
					          DP_DMA_INT_EN          ;//DP

  otcb_pong_1[0] = (void*)(0x10000000+N_PI_PO);   //DI
  otcb_pong_1[1] = ((N_PI_PO<<16) | 4);     //DX
  otcb_pong_1[2] = 0;                       //DY
  otcb_pong_1[3] |= TCB_TYPE_MEMORY        |
                    DP_DMA_LENGTH_DATA_128 |
					          DP_DMA_HIGH_PRIORITY   |
                    DP_DMA_INT_EN          ;//DP

//Создание цепочки PING PONG
  HAL_DMA_CreateChannelDest(DMA_CH_OUTPUT , otcb_ping_1 , otcb_pong_1);
  HAL_DMA_CreateChannelDest(DMA_CH_OUTPUT , otcb_pong_1 , otcb_ping_1);

  HAL_DMA_Stop( DMA_CH_OUTPUT );
  HAL_DMA_GetChannelStatusClear( DMA_CH_OUTPUT );

/*Включение PING PONG*/
  HAL_DMA_WriteDC(DMA_CH_OUTPUT, otcb_ping_1);
  HAL_SYS_WaitClk( 300000000 );//600000000
  printf("int - %i\n",cnt);

  ofid = fopen(OUTPUTFNAME,"w+b") ; if(ofid == 0){ puts("Unable open oddc.bin.");}
  fwrite((void*)(0x10000000),sizeof(uint32_t),(N),ofid);
  fclose(ofid);

  puts("END");
  return(0);
}
