/*Описание что делает*/
//[+] bank 0 & 1 
//[+] ping pong
#include <hal_1967VN044.h>
#include "hal_1967VN044.h"
#include <builtins.h>
#include <sysreg.h>
#include <stddef.h>
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include "../inc/memmap.h"
#include "../inc/_dduc.h"
#include "5101HB015.h"
#include "5101HB015_SPI_1967VC3.h"

#define N         32768  //КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВЕ IN
#define N_PI_PO   0xFFFC //65532 //КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВАХ PING & PONG
#define DMA_CH_OUTPUT 8 //НОМЕР DMA ПРИЁМНИКА ( УСТРОЙСТВО -> ПАМЯТЬ )
#define DMA_CH_INPUT  7 //НОМЕР DMA ПЕРЕДАТЧИКА ( ПАМЯТЬ -> УСТРОЙСТВО )
#define INPUTFNAME  "w:/MILANDR/MSO/CURRENT_FOLDER/DDCDR_PING_PONG/DDCDR_PING_PONG/iddc.bin"
#define OUTPUTFNAME "w:/MILANDR/MSO/CURRENT_FOLDER/DDCDR_PING_PONG/DDCDR_PING_PONG/oddc.bin"

static int32_t HAL_DMA_INIT_MEM_TYPE( const void* addr );// ОПРЕДЕЛЕНИЯ ТИПА ПАМЯТИ И ТИПА ПЕРЕДАЧИ
static int32_t num_bank( const void* addr );//ОПРЕДЕЛЕНИЯ НОМЕРА ИСПОЛЬЗУЕМОГО БАНКА ДЛЯ "INTERNAL BANK"

__attribute__((aligned(4))) uint32_t otcb_ping[4];//PING
__attribute__((aligned(4))) uint32_t otcb_pong[4];//PONG
__attribute__((aligned(4))) uint32_t itcb[4];     //INPUT_DMA

__attribute__((aligned(4))) uint16_t samples[N];
__attribute__((section("datasectionping"))) uint32_t samples_ping[N_PI_PO];// max 65536 по 32(размер одного банка)
__attribute__((section("datasectionpong"))) uint32_t samples_pong[N_PI_PO];// max 65536‬ по 32(размер одного банка)

static FILE* ofid;
static uint32_t flag;
//Функция обработчика прерывания DMA PING PONG
__attribute((interrupt))
static void dma_handler()
{
  flag = flag ^ 0x00000001;
  if(flag)       {fwrite((void*)samples_ping,sizeof(uint32_t),N_PI_PO,ofid);}
  else if(!flag) {fwrite((void*)samples_pong,sizeof(uint32_t),N_PI_PO,ofid);}
}

//#define CFG_FILE
int main(void)
{
  FILE* ifid; 
  int DMA_CNT=1,iqcnt,ErrFlag;
  uint32_t TCB_TYPE_MEMORY;
  uint32_t status=0;
  
//Разрешение прерывания от канла DMA 8, передача указателя на функцию обработчика прерывания
    HAL_Interrupt_GlobalEnable();
    HAL_Interrupt_Enable( intDMA8 , dma_handler );

/*INPUT DMA*/ 
  HAL_DMA_RqstClr(DMA_CH_INPUT);//ОЧИСТКА ИСТОЧНИКА ЗАПРОСА DMA
  HAL_DMA_RqstSet(DMA_CH_INPUT, dmaUPDOWN0);//УСТАНОВКА НОВОГО ИСТОЧНИКА ОПРОСА ДЛЯ КАНАЛА DMA_CH_INPUT
  TCB_TYPE_MEMORY = HAL_DMA_INIT_MEM_TYPE(&samples);//ОПРЕДЕЛЕНИЕ ТИПА ИСПОЛЬЗУЕМОЙ ПАМЯТИ !!!!!!!!!!должна быть перед заполнением структуры!!!!! 
  if(TCB_TYPE_MEMORY == -1){ puts("ERROR TYPE MEMORY - RX");}
  else num_bank(&samples);
  
  itcb[0] = ((void*)samples);  					             //DI
  itcb[1] = ((N/2) << 16) | 4; 					             //DX
  itcb[2] = 0;               					               //DY
  itcb[3] = TCB_TYPE_MEMORY | DP_DMA_LENGTH_DATA_128;//DP

  HAL_DMA_RqstClr(DMA_CH_OUTPUT);//ОЧИСТКА ИСТОЧНИКА ЗАПРОСА DMA
  HAL_DMA_RqstSet(DMA_CH_OUTPUT, dmaUPDOWN0);//УСТАНОВКА НОВОГО ИСТОЧНИКА ОПРОСА ДЛЯ КАНАЛА DMA_CH_OUTPUT
  TCB_TYPE_MEMORY = HAL_DMA_INIT_MEM_TYPE(&samples_ping);//ОПРЕДЕЛЕНИЕ ТИПА ИСПОЛЬЗУЕМОЙ ПАМЯТИ !!!!!!!!!!должна быть перед заполнением структуры!!!!! 
  if(TCB_TYPE_MEMORY == -1){ puts("ERROR TYPE MEMORY - TX");}
  
/*TCB_PING*/
  otcb_ping[0] = (void*)samples_ping;     //DI
  //otcb_ping[1] = 0xFFFC0000 | 4;        //DX
  otcb_ping[1] = ((N_PI_PO<<16) | 4);     //DX
  otcb_ping[2] = 0;                       //DY
  otcb_ping[3] |= TCB_TYPE_MEMORY        |
                  DP_DMA_LENGTH_DATA_128 |
                  DP_DMA_INT_EN          |
                  DP_DMA_CHANEL_8        ;//DP

/*TCB_PONG*/
  otcb_pong[0] = (void*)samples_pong;      //DI
  //otcb_ping[1] = 0xFFFC0000 | 4;         //DX
  otcb_pong[1] = ((N_PI_PO<<16) | 4);      //DX
  otcb_pong[2] = 0;                        //DY
  otcb_pong[3] |= TCB_TYPE_MEMORY         |
                  DP_DMA_LENGTH_DATA_128  |
                  DP_DMA_INT_EN           |
                  DP_DMA_CHANEL_8         ;//DP

  HAL_DMA_Stop( DMA_CH_OUTPUT );
  HAL_DMA_GetChannelStatusClear( DMA_CH_OUTPUT );
  
 //Создание цепочки PING PONG
  HAL_DMA_CreateChannelDest(DMA_CH_OUTPUT,&otcb_ping,&otcb_pong);
  HAL_DMA_CreateChannelDest(DMA_CH_OUTPUT,&otcb_pong,&otcb_ping);

  ifid = fopen(INPUTFNAME,"rb") ;   if(ifid == 0){ puts("Unable open iddc.bin.") ; return -1;}
  ofid = fopen(OUTPUTFNAME,"a+b") ; if(ofid == 0){ puts("Unable open oddc.bin.");}

/*UP_DOWN*/
#ifndef CFG_FILE
  fread((void *)samples,sizeof(uint32_t),2,ifid);
  DDUC_STEP = 23593;            //REG_STEP
  DDUC_CR   = SR_DDUC_ENABLE    |
              SR_FIR_TAP*15     |
              SR_SRC_DR         |
              SR_ROUND_OFF      |
              SR_SATURATION_OFF |
              SR_ROUNDM_OFF     |
              SR_FIR_ORDER3     |
              SR_INPUT_BE       |
              SR_OUTPUT_LE      |
              SR_MODE_DDC       |
              SR_SHIFT*14;
#else
  fread((void *)samples,sizeof(uint32_t),2,ifid) ;
  DDUC_STEP = (uint32_t)(samples[1]<<16) + samples[0];
  DDUC_CR   = (uint32_t)(samples[3]<<16) + samples[2];
#endif

/*Включение PING PONG*/
  HAL_DMA_WriteDC(DMA_CH_OUTPUT, &otcb_ping);

  while(fread((void *)samples,sizeof(uint16_t),N,ifid))
  { 
    status = HAL_DMA_GetChannelStatus( 8 );
    HAL_DMA_Stop( DMA_CH_INPUT );
    HAL_DMA_GetChannelStatusClear( DMA_CH_INPUT );
    HAL_DMA_WriteDC(DMA_CH_INPUT,  &itcb);
    
    ErrFlag = HAL_DMA_WaitForChannel(DMA_CH_INPUT);
    if(ErrFlag == 0)	 { printf("OK TX DMA - %i\n",DMA_CNT++);}
    else if(ErrFlag == 1){ puts("ERROR TX DMA") ; return(-1) ;}
    else if(ErrFlag == 2){ puts("TX DMA OFF")   ; return(-1) ;}
  }
  fclose(ifid);
  fclose(ofid);
  puts("END");
  return(0);
}
