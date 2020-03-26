/*
 * link044.cpp
 *
 *  Created on: 25 марта 2020 г.
 *      Author: IDA_102
 */
#include "H.h"

#define LINK_NUM_1	1 //RX
#define LINK_NUM_2	0//TX

#define DATA_SIZE	1024
int data_tx32[DATA_SIZE];
int data_rx32[DATA_SIZE];

void ERROR(){printf("ERROR\n");};

int main()
{
	int i = 0;
	int err_RX = 0,err_TX = 0,err = 0;
	int LInitSt;
	for(i = 0 ; i < DATA_SIZE ; ++i){data_tx32[i] = 0;}
	for(i = 0 ; i < DATA_SIZE ; ++i){data_rx32[i] = 0;}
	data_tx32[0] = -1;
	int *ptrDataTx  = data_tx32;
	int *ptrDataRx = data_rx32;

	//Инициализация LINK порта TX
	LinkTx_Init_type LTx;
	LTx.CheckBCMPO = Link_CheckBCMP_Dis;
	LTx.CheckSum = Link_CheckSum_Dis;
	LTx.TOIntEn = Link_TOIT_Dis;
	LTx.ClkSource = Link_TxClk_Master;
	LTx.DataSize = Link_DataSize_1bit;
	err_TX = HAL_LinkTx_Enable(LINK_NUM_2, &LTx, 0);
	if(!err_TX){printf("initiation LINK port TX %i = true\n",LINK_NUM_2,err_TX);}
	else {printf("initiation LINK port TX %i = false\n",LINK_NUM_2,err_TX);}


	//Инициализация LINK порта RX
	LinkRx_Init_type LRx;
	LRx.CheckSum = Link_CheckSum_Dis;
	LRx.CheckBCMPI = Link_CheckBCMP_Dis;
	LRx.OVRIntEn = Link_OvrIT_Dis;
	LRx.TOIntEn = Link_TOIT_Dis;
	LRx.DataSize = Link_DataSize_1bit;
	err_RX = HAL_LinkRx_Enable(LINK_NUM_1, &LRx, 0);
	if(!err_RX){printf("initiation LINK port RX %i = true\n",LINK_NUM_1,err_RX);}
	else {printf("initiation LINK port RX %i = false\n",LINK_NUM_1,err_RX);}

	//Передача данных

	err_TX = HAL_LinkCore_Transmit(LINK_NUM_2, ptrDataTx, 4);
	err_RX = HAL_LinkCore_Receive(LINK_NUM_1, ptrDataRx, 4);

	//Выключение порта приёмника и передатчика
	HAL_LinkTx_Disable(LINK_NUM_2);
	HAL_LinkRx_Disable(LINK_NUM_1);
	return 0;
}



