#include "DMA.h"

#ifndef sdram_H_
#define sdram_H_

#define SDRAM_BASE

/*____________________________________REGISTER_SYSCON____________________________________*/
/*Регистр SYSCON является регистром конфигурации контроллера внешнего
интерфейса и должен быть запрограммирован до того, как начнется передача
данных на шине (если значения по умолчанию должны изменяться). В режиме
пользователя доступ к SYSCON невозможен.*/

/*(0) "BNK0IDLE" - Вставка пустого цикла между операциями доступа к банку памяти MS0.*/ 
#define SDRAM_BNK0IDLE_NOINSERT  0x0‬ // нет
#define SDRAM_BNK0IDLE_INSERT    0x1‬ // вставить цикл

/*(2:1) "BNK0WAIT" - Количество внутренних циклов ожидания при обращении к банку MS0.*/
#define SDRAM_BNK0WAIT_0 0x0 // 0 циклов
#define SDRAM_BNK0WAIT_1 0x2 // 1 цикла
#define SDRAM_BNK0WAIT_2 0x4 // 2 цикла
#define SDRAM_BNK0WAIT_3 0x6 // 3 цикла

/*(4:3) "BNK0PIPE" - Глубина конвейера банка памяти MS0.*/
#define SDRAM_BNK0PIPE_1 0x00 // 1 цикл
#define SDRAM_BNK0PIPE_2 0x08 // 2 цикла
#define SDRAM_BNK0PIPE_3 0x10 // 3 цикла
#define SDRAM_BNK0PIPE_4 0x18 // 4 цикла

/*(5) "BNK0SLOW" - Тип протокола обмена для банка MS0.*/
#define SDRAM_BNK0SLOWE_SLOW 0x20 // медленный (асинхронный)
#define SDRAM_BNK0SLOW_FAST  0x00 // синхронный (конвейерный)

/*(6) "BNK1IDLE" - Вставка пустого цикла между операциями доступа к банку памяти MS1.*/
#define SDRAM_BNK1IDLE_INSERT    0x40 // вставить цикл
#define SDRAM_BNK1IDLE_NOINSERT  0x00 // нет

/*(8:7) "BNK1WAIT" - Количество внутренних циклов ожидания при обращении к банку MS1.*/
#define SDRAM_BNK1WAIT_0 0x000 // 0 циклов
#define SDRAM_BNK1WAIT_1 0x080 // 1 цикл
#define SDRAM_BNK1WAIT_2 0x100 // 2 цикла
#define SDRAM_BNK1WAIT_3 0x180 // 3 цикла

/*(10:9) "BNK1PIPE" - Глубина конвейера банка памяти MS1.*/
#define SDRAM_BNK1PIPE_1 0x000 // 1 цикл
#define SDRAM_BNK1PIPE_2 0x200 // 2 цикла
#define SDRAM_BNK1PIPE_3 0x400 // 3 цикла
#define SDRAM_BNK1PIPE_4 0x600 // 4 цикла

/*(11) "BNK1SLOW" - Тип протокола обмена для банка MS1.*/
#define SDRAM_BNK1SLOW_SLOW 0x800 // медленный (асинхронный)
#define SDRAM_BNK1SLOW_FAST 0x000 // синхронный (конвейерный)

/*(17:12) - Зарезервировано.*/

/*(18) - Всегда 0*/

/*(19) "MEMWIDTH" - Ширина шины данных при обращении к внешней памяти.*/
#define SDRAM_MEMWIDTH_16 0x80000 // 16 бит
#define SDRAM_MEMWIDTH_32 0x00000 // 32 бита

/*(21:20) - Зарезервировано.*/

/*(23:22) - Всегда 0*/

/*(24) "-" - Режим работы входного FIFO внешнего интерфейса.*/
/*Бит можно только установить. Сброс только аппаратно.*/
#define SDRAM_WORK_FIFO_NORM 0x0000000 // нормальный режим
#define SDRAM_WORK_FIFO_FAST 0x1000000 // ускорение передачи данных

/*(25) "-" - Режим работы выходного буфера внешнего интерфейса.*/
/*Бит можно только установить. Сброс только аппаратно.*/
#define SDRAM_WORK_OUT_BUFFER_NORM 0x0000000 // нормальный режим
#define SDRAM_WORK_OUT_BUFFER_FAST 0x2000000 // ускорение передачи данных

/*(26) - Всегда 0*/

/*(27) "XSDM" - Модификация адресного пространства.*/
#define SDRAM_MEMWIDTH_16 0x0000000 // стандартное адресное пространство.
#define SDRAM_MEMWIDTH_32 0x8000000 // модифицированное адресное пространство 
                                    // в котором адреса c 0x10000000 по 0x2FFFFFFF 
                                    // соответствуют адресному пространству динамической 
                                    // памяти. Используется для поддержки байтовой адресации 
                                    // внешней памяти

/*(31:28) - Всегда 0*/

/*____________________________________REGISTER_SDRCON____________________________________*/
/*Микросхемы SDRAM могут быть получены от разных поставщиков и в зависимости от поставщика, 
микросхемы могут иметь различные требования к последовательности включения и временным 
параметрам. С целью возможности работы с большим количеством поставщиков, в процессоре 
предусмотрен регистр SDRCON в котором можно запрограммировать некоторые параметры SDRAM.*/

/*(0) "SDREN" - Включение контроллера SDRAM.*/
#define SDRAM_SDREN_DISABLE 0x0 // выключен
#define SDRAM_SDREN_ENABLE  0x1 // включен

/*(2:1) "CAS" - Задержка CAS.*/
#define SDRAM_CAS_1 0x0 // 1 цикл
#define SDRAM_CAS_2 0x2 // 2 цикла
#define SDRAM_CAS_3 0x4 // 3 цикла
#define SDRAM_CAS_R 0x6 // резерв

/*(3) "PIPE" - Дополнительный конвейер.*/
#define SDRAM_PIPE_DISABLE 0x0 // выключен
#define SDRAM_PIPE_ENABLE  0x8 // включен

/*(5:4) "PAGE" - Размер страницы.*/
#define SDRAM_PAGE_256  0x00 // 256 слов
#define SDRAM_PAGE_512  0x10 // 512 слов
#define SDRAM_PAGE_1024 0x20 // 1024 слов
#define SDRAM_PAGE_R    0x30 // резерв

/*(6) - Зарезервировано.*/

/*(8:7) "REF" - Период регенерации памяти (в тактах SCLK).*/
#define SDRAM_REF_1110  0x000 // 1100 циклов
#define SDRAM_REF_1850  0x080 // 1850 циклов
#define SDRAM_REF_2200  0x100 // 2200 циклов
#define SDRAM_REF_3700  0x180 // 3700 циклов

/*(10:9) "PRC2RAS" - Задержка от подзаряда до строба RAS (такты SCLK).*/
#define SDRAM_PRC2RAS_2  0x000 // 2
#define SDRAM_PRC2RAS_3  0x200 // 3
#define SDRAM_PRC2RAS_4  0x400 // 4
#define SDRAM_PRC2RAS_5  0x600 // 5

/*(13:11) "RAS2PRC" - Задержка от строба RAS до команды подзаряда.*/
#define SDRAM_RAS2PRC_2 0x0000 // 2
#define SDRAM_RAS2PRC_3 0x0800 // 3
#define SDRAM_RAS2PRC_4 0x1000 // 4
#define SDRAM_RAS2PRC_5 0x1800 // 5
#define SDRAM_RAS2PRC_6 0x2000 // 6
#define SDRAM_RAS2PRC_7 0x2800 // 7
#define SDRAM_RAS2PRC_8 0x3000 // 8

/*(14) "INIT" - Выбор последовательности инициализации.*/
#define SDRAM_INIT_FRONT 0x0000 // команда MRS перед регенерацией памяти
#define SDRAM_INIT_AFTER 0x4000 // команда MRS после регенерации памяти

/*(15) "EMREN" - Использование дополнительного регистра режима.*/
#define SDRAM_EMREN_ENABLE  0x8000 // разрешено
#define SDRAM_EMREN_DISABLE 0x0000 // не используется

/*(31:16) - Всегда 0*/

/*____________________________________REGISTER_SYSTAT____________________________________*/
/*Регистр SYSTAT предназначен только для чтения и указывает на состояние некоторых параметров системы.*/

typedef union
{
  uint32_t REGISTER;
  struct
  {
    unsigned ZERO_0       :10;
    unsigned ONE          :1;
    unsigned ZERO_1       :1;
    unsigned ZERO_CFG1    :1;
    unsigned MRSCOMP      :1;
    unsigned BUSLKACT     :1;
    unsigned ZERO_2       :1;
    unsigned BROADREADERR :1;
    unsigned AUTODMAERR   :1;
    unsigned SDRAMERR     :1;
    unsigned MPREADERR    :1;
    unsigned ZERO_3       :12;
  } BITE;
} REGISTER_SYSTAT;

#endif