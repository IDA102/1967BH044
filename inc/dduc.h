#ifndef DDUC_H
#define DDUC_H

#define DDUC_BASE0 0x80000280
#define DDUC_BASE1 0x80000260
#define DDUC_BASE2 0x800002A0
#define DDUC_BASE3 0x800002C0

#define DDUC_CR_OFFSET   0 //R/W 0 ������� ����������
#define DDUC_SR_OFFSET   1 //SR R 0 ������� ���������
#define DDUC_STEP_OFFSET 2 //STEP R/W 0 ������� ���� �� ������� �������������
#define DDUC_DR_OFFSET   4 //DR R/W - ������� ������
#define DDUC_RCNT_OFFSET 8 //RCNT R/W 0 ������� ��������
#define DDUC_XCR_OFFSET  9 //XCR R/W 0  ������� ���������� �������������� �� ������� �������
#define DDUC_CR_OFFSET  10 //RCNT_STEP R 0 ��������� ������� ������� {RCNT_buf,0x0000, STEP}

#ifndef DDUC_BASE
#define DDUC_BASE DDUC_BASE0
#endif

#define DDUC_STEP (*(uint32_t*)(DDUC_BASE+DDUC_STEP_OFFSET))
#define DDUC_CR   (*(uint32_t*)(DDUC_BASE+DDUC_CR_OFFSET))

//CR bit fields 
#define SR_DDUC_ENABLE		0x00000001 //EN 0 ���������� ������ //1 � ������ ���������/0 � ������ ���������
                                           //1 LINK 0 �������� ������ � ������ DOWN
#define SR_SRC_DR		0x00000000 //0 � ����� ������ � ��� ���� (�������� �����)
#define SR_SRC_LINK		0x00000002 //1 � ����� ������ � ���� �����
                                           //2 ROUND 0 
#define SR_ROUND_ON		0x00000004 //1 � ���������� ��������� ����������
#define SR_ROUND_OFF		0x00000000 //0 � ��� ����������
//3 SAT 0
#define SR_SATURATION_ON	0x00000008 //1 � ��������� ��� ��������� ����������
#define SR_SATURATION_OFF	0x00000000 //0 � ��� ���������
//4 ROUNDM 0
#define SR_ROUNDM_ON		0x00000010 //1 � ���������� ����� ������������
#define SR_ROUNDM_OFF		0x00000000 //0 � ��� ����������
//5 TBD 0
#define SR_INPUT_BE		0x00000000 //0 � ������ 16-��� ������ ��������� � ������� ����� 128-��� �����
#define SR_INPUT_LE		0x00000020 //1 � � �������
                                           //6 IQ_QI 0 ������ ���������� �� ���� 16-��������� �������
#define SR_OUTPUT_BE		0x00000040 //1 � ������ ������ ������ {I[15:0]}, Q[15:0]}
#define SR_OUTPUT_LE		0x00000000 //0 � ������ ������ ������ {Q[15:0]}, I[15:0]}
                                           //7 DAM 0 ����� DOWN ���� UP:
#define SR_MODE_DDC		0x00000000 //0 � DOWN � ������ ����������� �� ���� �����
#define SR_MODE_DUC		0x00000080 //1 � UP � ������ �������� � ���� ����
#define SR_FIR_TAP		0x00000100 //16:8 Kdelay 0 ����������� �������� ����� ���������. ���������� ����������� ������������/���������, ������� ����� (Kdelay+1)*2. ���������� �������� Kdelay > 0
#define SR_SHIFT		0x00020000 //23:17 SHFR 0 ����� ������ ��������� 86-���������� ������� ��� ��������� 16-���������� ���������� SHFR = FLEN*log2((Kdelay+1)*2)
                                           //24 INT_BLK 0 ���������� ���������� ��� ��������� RCNT
#define SR_INT_DISABLE		0x01000000 //1 � ���������� ���������
#define SR_INT_ENABLE		0x00000000 //0 � ���������� ���������
                                           //27:25 ������ 0 �
                                           //29:28 FLEN 0 ����� �������� 
#define SR_FIR_ORDER3		0x00000000 //00 � 3 �������
#define SR_FIR_ORDER5		0x10000080 //01 � 5 ��������
#define SR_FIR_ORDER7		0x20000000 //10 � 7 ��������
                                           //30 RCNT_ON 0 ���������� ������ ��������
#define SR_RCNT_ENABLE		0x40000000 //1 � ������ ���������
#define SR_RCNT_DISABLE		0x00000000 //0 � ������ ���������
                                           //31 LNKUSE 0 ����� Link-����� ��� ������ (DOWN) ��� �������� ������ (UP)
#define SR_LINK0		0x00000000 
#define SR_LINK1		0x80000000 

//SR bit fields 
#define SR_INPUT_READY     0x00000001 //T_req ���� ���������� ������ � ������ ������
                                      //1 � ������ ����� ������� ����� ������
                                      //0 � ������ �� ����� ������� ����� ������
#define SR_OUTPUT_READY    0x00000002 //R_req ���� ���������� ������ ������ ������
                                      //1 � ������ ����� ������ ������
                                      //0 � ������ �� ����� ������ ������
#define SR_OUTPUT_OVERFLOW 0x00000004 //2 Ferr ���� ��������� ��������� FIFO
                                      //1 � ������������ 
                                      //0 � ��� ������������
#define SR_INPUT_OVERFLOW  0x00000008 //3 overf ���� ��������� �������� FIFO:
                                      //1 � �������������
                                      //0 � ��� ������������
#endif DDUC_H












