/**
******************************************************************************
* File Name          : spi_one.h
* Date               : 01/9/2019 
* Description        : SPI 1 driver
* Author	     : Lev Zoosmanovskiy
******************************************************************************

******************************************************************************
*/

#ifndef __SPI_1_H
#define __SPI_1_H
#include "spi.h"

#define _SPI1_DMA_ 1
#define SPI_1_TX_BUFF_SIZE             (0x220)
#define SPI_1_RX_BUFF_SIZE             (0x220)

extern uint8_t g_u8SpiOneRxBuff[];
extern uint8_t g_u8SpiOneTxBuff[];
extern SPI_ONE_CS g_eSpiOneCs;
extern      volatile bool g_bFlashPageReadFinished, g_bFlashPageWriteFinished;

extern volatile u32        g_u32SpiOneTxLen;
extern volatile u32        g_u32SpiOneRxLen;
extern volatile u32        g_u32SpiOneRxCount;
extern volatile u32        g_u32SpiOneTxCount;
extern volatile bool       g_bSemSpiOneBusy;


void Configure_SPI1_DMA(void);
void SPI1_StartFlashTransfer(void);
void init_spi_1(void);
void WaitForSpiOneReady(void);

#endif //__SPI_1_H