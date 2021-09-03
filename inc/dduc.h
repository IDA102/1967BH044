#ifndef DDUC_H
#define DDUC_H

#define DDUC_BASE0 0x80000280
#define DDUC_BASE1 0x80000260
#define DDUC_BASE2 0x800002A0
#define DDUC_BASE3 0x800002C0

#define DDUC_CR_OFFSET   0 //R/W 0 Регистр управления
#define DDUC_SR_OFFSET   1 //SR R 0 Регистр состояния
#define DDUC_STEP_OFFSET 2 //STEP R/W 0 Регистр шага по таблице коэффициентов
#define DDUC_DR_OFFSET   4 //DR R/W - Регистр данных
#define DDUC_RCNT_OFFSET 8 //RCNT R/W 0 Счетчик отсчетов
#define DDUC_XCR_OFFSET  9 //XCR R/W 0  Регистр управления инициализацией от внешних выводов
#define DDUC_CR_OFFSET  10 //RCNT_STEP R 0 Составной регистр формата {RCNT_buf,0x0000, STEP}

#ifndef DDUC_BASE
#define DDUC_BASE DDUC_BASE0
#endif

#define DDUC_STEP (*(uint32_t*)(DDUC_BASE+DDUC_STEP_OFFSET))
#define DDUC_CR   (*(uint32_t*)(DDUC_BASE+DDUC_CR_OFFSET))

//CR bit fields 
#define SR_DDUC_ENABLE		0x00000001 //EN 0 Разрешение работы //1 – работа разрешена/0 – работа запрещена
                                           //1 LINK 0 Источник данных в режиме DOWN
#define SR_SRC_DR		0x00000000 //0 – прием данных с АРВ шины (тестовый режим)
#define SR_SRC_LINK		0x00000002 //1 – прием данных с ЛИНК порта
                                           //2 ROUND 0 
#define SR_ROUND_ON		0x00000004 //1 – округление выходного результата
#define SR_ROUND_OFF		0x00000000 //0 – нет округления
//3 SAT 0
#define SR_SATURATION_ON	0x00000008 //1 – насыщение для выходного результата
#define SR_SATURATION_OFF	0x00000000 //0 – нет насыщения
//4 ROUNDM 0
#define SR_ROUNDM_ON		0x00000010 //1 – округление после перемножения
#define SR_ROUNDM_OFF		0x00000000 //0 – нет округления
//5 TBD 0
#define SR_INPUT_BE		0x00000000 //0 – первый 16-бит отсчет находится в младших битах 128-бит слова
#define SR_INPUT_LE		0x00000020 //1 – в старших
                                           //6 IQ_QI 0 Выдача результата из двух 16-разрядных половин
#define SR_OUTPUT_BE		0x00000040 //1 – формат выдачи данных {I[15:0]}, Q[15:0]}
#define SR_OUTPUT_LE		0x00000000 //0 – формат выдачи данных {Q[15:0]}, I[15:0]}
                                           //7 DAM 0 Режим DOWN либо UP:
#define SR_MODE_DDC		0x00000000 //0 – DOWN – данные принимаются из линк порта
#define SR_MODE_DUC		0x00000080 //1 – UP – данные выдаются в линк порт
#define SR_FIR_TAP		0x00000100 //16:8 Kdelay 0 Коэффициент передачи между фильтрами. Определяет коэффициент интерполяции/децимации, который равен (Kdelay+1)*2. Допустимые значения Kdelay > 0
#define SR_SHIFT		0x00020000 //23:17 SHFR 0 Сдвиг вправо выходного 86-разрядного данного для получения 16-разрядного результата SHFR = FLEN*log2((Kdelay+1)*2)
                                           //24 INT_BLK 0 Блокировка прерывания при обнулении RCNT
#define SR_INT_DISABLE		0x01000000 //1 – прерывание запрещено
#define SR_INT_ENABLE		0x00000000 //0 – прерывание разрешено
                                           //27:25 Резерв 0 –
                                           //29:28 FLEN 0 Длина фильтров 
#define SR_FIR_ORDER3		0x00000000 //00 – 3 ступени
#define SR_FIR_ORDER5		0x10000080 //01 – 5 ступеней
#define SR_FIR_ORDER7		0x20000000 //10 – 7 ступеней
                                           //30 RCNT_ON 0 Разрешение работы счетчика
#define SR_RCNT_ENABLE		0x40000000 //1 – работа разрешена
#define SR_RCNT_DISABLE		0x00000000 //0 – работа запрещена
                                           //31 LNKUSE 0 Номер Link-порта для приема (DOWN) или передачи данных (UP)
#define SR_LINK0		0x00000000 
#define SR_LINK1		0x80000000 

//SR bit fields 
#define SR_INPUT_READY     0x00000001 //T_req Флаг готовности модуля к приему данных
                                      //1 – модуль готов принять новые данные
                                      //0 – модуль не готов принять новые данные
#define SR_OUTPUT_READY    0x00000002 //R_req Флаг готовности модуля выдать данные
                                      //1 – модуль готов выдать данные
                                      //0 – модуль не готов выдать данные
#define SR_OUTPUT_OVERFLOW 0x00000004 //2 Ferr Флаг состояния выходного FIFO
                                      //1 – переполнение 
                                      //0 – нет переполнения
#define SR_INPUT_OVERFLOW  0x00000008 //3 overf Флаг состояния входного FIFO:
                                      //1 – перепполнение
                                      //0 – нет переполнения
#endif DDUC_H












