#ifndef DDUC_H_
#define DDUC_H_

#define DDUC_BASE0 0x80000280
#define DDUC_BASE1 0x80000260
#define DDUC_BASE2 0x800002A0
#define DDUC_BASE3 0x800002C0

#define DDUC_CR_OFFSET         0 //R/W-0         Регистр управления
#define DDUC_SR_OFFSET         1 //SR R-0        Регистр состояния
#define DDUC_STEP_OFFSET       2 //STEP R/W-0    Регистр шага по таблице коэффициентов
#define DDUC_DR_OFFSET         4 //DR R/W -      Регистр данных
#define DDUC_RCNT_OFFSET       8 //RCNT R/W-0    Счетчик отсчетов
#define DDUC_XCR_OFFSET        9 //XCR R/W-0 	 Регистр управления инициализацией от внешних выводов
#define DDUC_RCNT_STEP_OFFSET 10 //RCNT_STEP R-0 Составной регистр формата {RCNT_buf,0x0000, STEP}

#ifndef DDUC_BASE
#define DDUC_BASE DDUC_BASE0
#endif

#define DDUC_STEP (*(uint32_t*)(DDUC_BASE+DDUC_STEP_OFFSET))
#define DDUC_CR   (*(uint32_t*)(DDUC_BASE+DDUC_CR_OFFSET))

/*____________________________________CR BIT FIELDS____________________________________*/
/*(0) "EN" - Бит разрешение работы */
#define SR_DDUC_ENABLE	  0x00000001 //1 – работа разрешена
#define SR_DDUC_DISABLE	  0x00000000 //0 – работа запрещена

/*(1)"LINK" - Источник данных в режиме DOWN*/
#define SR_SRC_DR         0x00000000 //0 – Прием данных с АРВ шины (тестовый режим)
#define SR_SRC_LINK       0x00000002 //1 – Прием данных с ЛИНК порта

/*(2) "ROUND" */
#define SR_ROUND_ON       0x00000004 //1 – Округление выходного результата
#define SR_ROUND_OFF      0x00000000 //0 – Нет округления

/*(3) "SAT" */
#define SR_SATURATION_ON  0x00000008 //1 – Насыщение для выходного результата
#define SR_SATURATION_OFF 0x00000000 //0 – Нет насыщения

/*(4) "ROUNDM" */
#define SR_ROUNDM_ON      0x00000010 //1 – Округление после перемножения
#define SR_ROUNDM_OFF     0x00000000 //0 – Нет округления

/*(5) "TBD" */
#define SR_INPUT_BE       0x00000000 //0 – Первый 16-бит отсчет находится в младших битах 128-бит слова
#define SR_INPUT_LE       0x00000020 //1 – Первый 16-бит отсчет находится в старших битах 128-бит слова

/*(6) "IQ_QI" - Выдача результата из двух 16-разрядных половин*/
#define SR_OUTPUT_BE      0x00000040 //1 – формат выдачи данных {I[15:0]}, Q[15:0]}
#define SR_OUTPUT_LE      0x00000000 //0 – формат выдачи данных {Q[15:0]}, I[15:0]}

/*(7) "DAM" - Режим DOWN либо UP:*/
#define SR_MODE_DDC       0x00000000 //0 – DOWN – данные принимаются из линк порта
#define SR_MODE_DUC       0x00000080 //1 – UP – данные выдаются в линк порт

/*(16-8) "Kdelay" -
  Коэффициент передачи между фильтрами.
  Определяет коэфф. интерполяции/децимации который равен  Kdelay+1)*2.
  Допустимые значения Kdelay > 0*/
#define SR_FIR_TAP        0x00000100 //1 < SR_FIR_TA < 511

/* (23:17) "SHFR"
  Сдвиг вправо выходного 86-разрядного данного для получения 16-разрядного результата.
  SHFR = FLEN*log2((Kdelay+1)*2)*/
#define SR_SHIFT          0x00020000 // 0 < SR_SHIFT < 127

/*(24) "INT_BLK" - Блокировка прерывания при обнулении RCNT*/
#define SR_INT_DISABLE    0x01000000 //1 – прерывание запрещено
#define SR_INT_ENABLE     0x00000000 //0 – прерывание разрешено

/*(29:28) "FLEN" - Длина фильтров*/
#define SR_FIR_ORDER3     0x00000000 //00 – 3 ступени
#define SR_FIR_ORDER5     0x10000000 //01 – 5 ступеней
#define SR_FIR_ORDER7     0x20000000 //10 – 7 ступеней

/*(30) "RCNT_ON" - Разрешение работы счетчика*/
#define SR_RCNT_ENABLE    0x40000000 //1 – работа разрешена
#define SR_RCNT_DISABLE   0x00000000 //0 – работа запрещена

/*(31) "LNKUSE" - Номер Link-порта для приема (DOWN) или передачи данных (UP)*/
#define SR_LINK0          0x00000000
#define SR_LINK1          0x80000000

/*____________________________________SR BIT FIELDS____________________________________*/
/*(0) "T_req" - Флаг готовности модуля к приему данных*/
#define SR_INPUT_READY    0x00000001 //1 – модуль готов принять новые данные / 0 – модуль не готов принять новые данные

/*(1) "R_req" - Флаг готовности модуля выдать данные*/
#define SR_OUTPUT_READY   0x00000002 //1 – модуль готов выдать данные / 0 – модуль не готов выдать данные

/*(2) "Ferr" - Флаг состояния выходного FIFO*/
#define SR_OUTPUT_OVERFLOW 0x00000004 //1 – переполнение / 0 – нет переполнения

/*(3) "overf" - Флаг состояния входного FIFO:*/
#define SR_INPUT_OVERFLOW  0x00000008 //1 – перепполнение



#endif
