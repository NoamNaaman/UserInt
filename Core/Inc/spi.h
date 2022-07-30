///////////////////////////////////////////////////////////////////////////////
// Header file spi.h
//
//
// Version : 11.01.02.XX
// Added: lptm, adc, loader, adpd1, adpd2, 6d, ble, uart
///////////////////////////////////////////////////////////////////////////////

#ifndef __SPI_H__
#define __SPI_H__

#include "main.h"

#define _SPI1_DMA_     1
#define _SPI2_DMA_     1
#define _SPI3_DMA_     0

#define SPI_DUMMY_BYTE            (0xEA)

#define NBR_SGN_BUFF                (4)// @#$ (8) 

#define SPI_SERVICE_INFO          (1)

#define SPI_OFFSET_ADDR           (0)
#define SPI_OFFSET_DATA           (1)

#define SPI_SET_MB                (0x40)
#define SPI_SET_RD                (0x80)
#define SPI_MASTER_RD             (0x80)
#define SPI_MAX_RD                (0x01)
#define SPI_ADPD_WR               (0x01)

#define SPI_BUFF_SIZE             (0x220)
#define SPI_THR_BUFF_SIZE         (0x40)

#define SPI_THR_TX_OUT_LEN        (20)
#define SPI_THR_RX_MAX_LEN        (20)
#define SPI_THR_SZ_HEADER         (2)

#define SPT_SENSOR_ADD            (0x50)

#define OS_SPT_CMD                (0x00)
#define OS_SPT_LEN                (0x01)

#define _SPI_ONE_INT_             (1)

void delay_us(uint32_t);
#define DelayCycles delay_us
typedef enum
{
  eSpiIdle,
  eSpiReadHeader,
  eSpiWrite,
  eSpiRead,
  eSpiStopRead,
  eSpiBuffAuth
}SPI_SPL_STATE;

typedef enum
{
  eBleRxIdle,
  eBleRxAuth
}BLE_RX_STATE;

typedef enum
{
  eWadpdCs,
  eFadpdCs,
  e6dCs,
}SPI_TWO_CS;

typedef enum
{
  eDisplayCs,
  eFlashCs
}SPI_ONE_CS;

typedef enum
{
  e3dIdle,
  e3dGotInterr,
  e3dStndInfo,
  e3dRcvdStndInfo,
  e3dGotStndInfo,
  e3dUpdStndInfo,
  e3dMax
}_3D_STATE;

typedef enum
{
  e6dIdle,
  e6dGotInterr,
  e6dStartStndInfo,
  e6dStndInfo,
  e6dRcvdStndInfo,
  e6dGotStndInfo,
  e6dUpdStndInfo,
  e6dFifoStatus,
  e6dRcvdFifoStatus,
  e6dGotFifoStatus,
  e6dFifoData,
  e6dRcvdFifoData,
  e6dGotFifoData,
  e6dMax
}_6D_STATE;

//-- adpd module state machine --//
typedef enum 
{
	eAdpdIdle = 0,	 // idle state		
	eAdpdGotInterr,	 // was got interrupt
	eAdpdStartCleanI,// start clean adpd interrupt
	eAdpdCleanInt,   // clean adpd interrupt
	eAdpdStartReadB, // start reading adpd buffers
	eAdpdReadBuff,	 // reading buffer process
	eAdpdEndReadBuff,   // adpd buffer has been received
	eAdpdReadFifo,	 // reading fifo process
	eAdpdEndReadFifo,   // adpd fifo has been received
	eAdpdDataCalc    // start data calculattion
}ADPD_STATE;

#pragma pack(2)
typedef struct
{
  int16_t   AccX;
  int16_t   AccY;
  int16_t   AccZ;
  int16_t   Roll;
  int16_t   Pitch;
  int16_t   Yaw;//!Qaz12345
}_6D_DATA;

//-- display module state machine --//
typedef enum 
{
	eDspIdle,	     // idle state		
	eDspBlockStart,// start block data
	eDspBlockStop, // stop block data
  eDspUpdate
}DISPLAY_STATE;

//-- correction module state machine --//
typedef enum 
{
  eCorrIdle,
  eCorrPntA,
  eCorrPntC,
  eCorrWait
}CORR_STATE;

extern volatile  _6D_DATA      g_s6D, g_sInt6D;
extern  volatile _6D_STATE     g_e6dState;
extern  volatile ADPD_STATE		g_eWadpdState;
extern  volatile ADPD_STATE		g_eFadpdState;

extern  volatile DISPLAY_STATE	g_eDspRenewState;
extern  volatile DISPLAY_STATE	g_eDspUpdateState;
extern  volatile CORR_STATE 	  g_eCorrState;
extern  volatile CORR_STATE 	  g_eSpo2CorrState;

extern  bool g_bSem6dOn, g_bSemWadpdOn, g_bSemFadpdOn, g_bSemNorFlashOn;

#define SZ_6D         sizeof(_6D_DATA)

extern SPI_ONE_CS g_eSpiOneCs;
extern SPI_SPL_STATE g_eSpiOneState;
extern u8         g_u8SpiOneRxBuff[SPI_BUFF_SIZE];
extern u8         g_u8SpiOneTxBuff[SPI_BUFF_SIZE];
extern volatile u32        g_u32SpiOneTxLen;
extern volatile u32        g_u32SpiOneRxLen;
extern volatile u32        g_u32SpiOneRxCount;
extern volatile u32        g_u32SpiOneTxCount;
extern volatile bool       g_bSemSpiOneBusy;

extern volatile SPI_TWO_CS g_eSpiTwoCs;
extern volatile SPI_SPL_STATE g_eSpiTwoState;
extern volatile u8         g_u8SpiTwoBuff[SPI_BUFF_SIZE];
extern volatile u32        g_u32SpiTwoTxLen;
extern volatile u32        g_u32SpiTwoRxLen;
extern volatile u32        g_u32SpiTwoCount;
extern volatile bool       g_bSemSpiTwoBusy;

extern SPI_SPL_STATE g_eSpiThrState;
extern u8         g_u8SpiThrRxBuff[SPI_THR_BUFF_SIZE];
extern u8         g_u8SpiThrTxBuff[SPI_THR_BUFF_SIZE];
extern u32        g_u32SpiThrTxLen;
extern u32        g_u32SpiThrRxLen;
extern u32        g_u32SpiThrRxCount;
extern u32        g_u32SpiThrTxCount;

void  SpiOneOutBuffPrepare(void);

void  WriteToSpiOne(u32 u32Add, u32 u32Len, u8* pBuff);
void  ReadFromSpiOne(u32 u32Add, u32 u32Len);
void  StartSpiOneTx(void);
void  SpiOneInit(void);
void  CbSpiOneTx(void);

void  WriteToSpiTwo(u32 u32Add, u32 u32Len, u8* pBuff);
void  ReadFromSpiTwo(u32 u32Add, u32 u32Len);
void  StartSpiTwoTx(void);
void  SpiTwoInit(void);

void  InitLis3mdl(void);
void  StartShift3DInfo(void);
void  Get3DInfo(void);
void	 WriteTo3dRegSpi(u8 u8Add, u8 u8Data);
u8	   Get3dRegInfo(u8 u8Add);

//void  InitLsm6dsl(bool bSemOn);
//void  StartShift6DStndInfo(void);
//void  StartShift6DFifoState(void);
//void  StartShift6DFifoState2(void);
//void  StartShift6DFifoData(void);
//void  Get6DStndInfo(void);
//void  Get6DFifoStatus(void);
//void  Get6DFifoData(void);
//u8	   Get6dRegInfo(u8 u8Add);
//void	 WriteTo6dRegSpi(u8 u8Add, u8 u8Data);

void  InitBle(void);
void  InitMax30001(void);
void  StartShiftEcgStatus(void);
void  StartShiftEcgOneSmp(void);
void  StartShiftEcgFifoB(void);
void  GetEcgFifoBurst(void);
void  GetEcgStatus(void);
void  GetEcgOneSmp(void);
void  StartShiftBiozOneSmp(void);
void  GetBiozOneSmp(void);
void	 WriteToEcgRegSpi(u8 u8Add, u32 u32Data);
bool  GetEcgChipInfo(u8 u8Add);

void  StartSpiThrTx(void);
void  SpiThrInit(void);

#endif
