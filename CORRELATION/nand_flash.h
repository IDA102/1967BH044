#include "DMA.h"

#ifndef nand_flash_H_
#define nand_flash_H_

/*____________________________________IO_CFG____________________________________*/
/*Данный регистр позволяет определить основные временные параметры протокола обмена. 
Длительность стробов выражается количеством тактов частоты синхронизации, на которой работает контроллер. 
В качестве базовой частоты используется частота периферийной шины.
*/

/*(2:0) "CSCA" - Число периодов(+1) CLK с момента активизации NF_CS [2:0] до момента активизации 
        сигналов nRE или nWE.*/ 
//reset default - 111(0x7)
#define NAND_CSCA_1 0x0‬ // 
#define NAND_CSCA_2 0x1‬ // 
#define NAND_CSCA_3 0x2 // 
#define NAND_CSCA_4 0x3 // 
#define NAND_CSCA_5 0x4 // 
#define NAND_CSCA_6 0x5‬ // 
#define NAND_CSCA_7 0x6 // 
#define NAND_CSCA_8 0x7‬ // 

/*(7:3) "CA" - Число периодов (+1) активности сигнала nRE во время чтения памяти.*/ 
//reset default - 11111(0x8F)
#define NAND_CA_1  0x00‬  // 
#define NAND_CA_2  0x08  // 
#define NAND_CA_3  0x10 // 
#define NAND_CA_4  0x18 // 
#define NAND_CA_5  0x20 // 
#define NAND_CA_6  0x28 // 
#define NAND_CA_7  0x30 // 
#define NAND_CA_8  0x‬38 // 
#define NAND_CA_9  0x‬40 // 
#define NAND_CA_10 0x‬48 // 
#define NAND_CA_11 0x50‬ // 
#define NAND_CA_12 0x‬58 // 
#define NAND_CA_13 0x60‬ // 
#define NAND_CA_14 0x‬68 // 
#define NAND_CA_15 0x70‬ // 
#define NAND_CA_16 0x‬78 // 
#define NAND_CA_17 0x80‬ // 
#define NAND_CA_18 0x‬88 // 
#define NAND_CA_19 0x‬90 // 
#define NAND_CA_20 0x‬98 // 
#define NAND_CA_21 0x‬A0 // 
#define NAND_CA_22 0x‬A8 // 
#define NAND_CA_23 0x‬B0 // 
#define NAND_CA_24 0x‬B8 // 
#define NAND_CA_25 0x‬C0 // 
#define NAND_CA_26 0x‬C8 // 
#define NAND_CA_27 0xD0‬ // 
#define NAND_CA_28 0x‬D8 // 
#define NAND_CA_29 0x‬E0 // 
#define NAND_CA_30 0xE8‬ // 
#define NAND_CA_31 0x‬F0 // 
#define NAND_CA_32 0x‬F8 //

/*(10:8) "BWD" - Число периодов (+1) активности nWE во время записи.*/ 
//reset default - 111(0x700)
#define NAND_BWD_1 0x0‬00 // 
#define NAND_BWD_2 0x100‬ // 
#define NAND_BWD_3 0x200 // 
#define NAND_BWD_4 0x300 // 
#define NAND_BWD_5 0x400 // 
#define NAND_BWD_6 0x5‬00 // 
#define NAND_BWD_7 0x600 // 
#define NAND_BWD_8 0x7‬00 // 

/*(14:11) "BRT" - Число периодов (+1) от момента снятия одного NF_CSx до момента активизации 
           NF_CSx другого банка, с целью избежать конфликт на шине данных.*/ 
//reset default - 1111(0x7800)
#define NAND_BRT_1  0x0800 // 
#define NAND_BRT_2  0x0100 // 
#define NAND_BRT_3  0x1800 // 
#define NAND_BRT_4  0x2000 // 
#define NAND_BRT_5  0x2800 // 
#define NAND_BRT_6  0x3000 // 
#define NAND_BRT_7  0x3800‬ // 
#define NAND_BRT_8  0x4000‬ // 
#define NAND_BRT_9  0x4800‬ // 
#define NAND_BRT_10 0x5000 // 
#define NAND_BRT_11 0x5800 // 
#define NAND_BRT_12 0x6000 // 
#define NAND_BRT_13 0x6800 // 
#define NAND_BRT_14 0x7000 // 
#define NAND_BRT_15 0x7800 // 

/*(17:15) "BHT" - Число периодов (+1) CLK в течение которых NF_CSx находится в неактивном 
          состоянии между последовательными выборками. А также число периодов (+1) после 
          снятия nRE или nWE до следующего активного сигнала.*/ 
//reset default - 111(0x78000)
#define NAND_BHT_1 0x00‬000 // 
#define NAND_BHT_2 0x08000‬ // 
#define NAND_BHT_3 0x10000 // 
#define NAND_BHT_4 0x18000 // 
#define NAND_BHT_5 0x20000 // 
#define NAND_BHT_6 0x28000 // 
#define NAND_BHT_7 0x30000 // 
#define NAND_BHT_8 0x78000 //

/*(18) "MDT" -Тип интерфейса. Используется совместно с VGA битом.*/ 
//reset default - 0(0x0)
#define NAND_MDT_0 0x00‬000 // 
#define NAND_MDT_1 0x40000‬ // 

//reset default - 00(0x0)
/*(20:19) - Не используется. Значение безразлично.*/

/*(21) "VGA" - Тип интерфейса. Имеет значение при установленном MDT.*/ 
//reset default - 1(0x200000‬)
#define NAND_VGA_0 0x00‬0000 // 
#define NAND_VGA_1 0x200000‬ //

/*(22) - Не используется. Значение безразлично.*/

/*(26:23) "CSKPL" - Управление линиями NF_CSx между доступами к флэш-памяти.*/ 
//reset default - 0001(0x800000)
#define NAND_CSKPL_0 0x00‬0000 // линии NF_CSх становятся неактивными между выборками
#define NAND_CSKPL_1 0x800000 // линии NF_CSx активны (=0) между выборками

/*(31:27) - Не используется. Значение безразлично.*/

/*____________________________________WCT_CFG____________________________________*/
/*Регистр позволяет управлять анализом сигнала готовности внешней памяти и 
программировать максимальное время ожидания.*/

/*(3:0) "ENWT" - Разрешение анализа входа RnB во время обмена с одним из банков NF_CS[3:0].*/ 
//reset default - 1111(0xF)
#define NAND_ENWT_0 0x0‬ // RnB не анализируется
#define NAND_ENWT_1 0xF‬ // RnB используется для анализа во время цикла обмена

/*(7:4) "CCSE" - Разрешение анализа входа RnB во время обмена с одним из банков NF_CS[3:0].*/ 
//reset default - 1111(0xF0)
#define NAND_CCSE_0 0x00‬ // установить неактивный уровень если RnB =0
#define NAND_CCSE_1 0xF‬0 // разрешить активный уровень на NF_CSx во время RnB=0

/*(30:21) "WTOC" - Длительность периода “time-out”. Эти биты определяют длительность периода, 
          используемого для контроля ситуации “time-out”. Превышение длительности RnB=0 над 
          длительностью периода WTOC•1024 приводит к ситуации “time-out” и установке бита TOEX.*/ 
//reset default - 0(0x0)

/*(31) "TOE" - Разрешение контроля ситуации “time-out”.*/ 
//reset default - 0(0x0)
#define NAND_TOE_0 0x00‬000000 // запрещено
#define NAND_TOE_1 0x80000000 // разрешено

/*____________________________________NAND_CFG____________________________________*/
/*Позволяет задать основные параметры флэш-памяти и режимы работы.*/




/*____________________________________OTHER____________________________________*/
/*Перечесление содержит имена моделей памяти Samsung для которых создан драйвер.*/
typedef enum { K9F8G08UXM, K9F2G08X0B } SDRAM_TYPE;

#endif
