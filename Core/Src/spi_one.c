/**
******************************************************************************
* File Name          : spi_1.c
* Date               : 01/9/2019 
* Company            : Cardiacsense
* Description        : SPI 1 driver
* Author	     : Lev Zoosmanovskiy
******************************************************************************

******************************************************************************
*/

#include "main.h"
#include "setup.h"
//#include "custom_gpio.h"
//#include "spi_1.h"
#include "spi_one.h"
//#include "nor_flash.h"
#include "setup.h"
//#include "projdefs.h"
//#include "interrupts_priority.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "no_init.h"
//#include "misc.h"

uint8_t g_u8SpiOneRxBuff[SPI_1_RX_BUFF_SIZE];
uint8_t g_u8SpiOneTxBuff[SPI_1_TX_BUFF_SIZE];
void RestartMCU(u32);

u8 *g_pSpi1DataDest;

extern bool g_bRtosRunning;



SPI_ONE_CS g_eSpiOneCs;
volatile bool g_bFlashPageReadFinished, g_bFlashPageWriteFinished;

volatile u32        g_u32SpiOneTxLen;
volatile u32        g_u32SpiOneRxLen;
volatile u32        g_u32SpiOneRxCount;
volatile u32        g_u32SpiOneTxCount;
volatile bool       g_bSemSpiOneBusy;
SPI_SPL_STATE g_eSpiOneState;

SPI_HandleTypeDef hspi1;

void Error_Handler(void);



/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
/* SPI1 init function */
void init_spi_1(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Peripheral clock enable */
  __HAL_RCC_SPI1_CLK_ENABLE();
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /**SPI1 GPIO Configuration    
  PA5     ------> SPI1_SCK
  PA6     ------> SPI1_MISO
  PA7     ------> SPI1_MOSI 
  */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
    Error_Handler();
    }
  SPI1->CR1 |= SPI_CR1_SPE;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void deinit_spi_1(void)
  {
  HAL_SPI_DeInit(&hspi1);
  
  input_analog(SPI1_PIN_SCK );
  input_analog(SPI1_PIN_MISO);
  input_analog(SPI1_PIN_MOSI);
  }


/**************************************************
* Function name	: Configure_SPI1_DMA
* Returns	:	
* Arg		: 
* Created by	: Lev Zoosmanovskiy
* Date created	: 10/12/2018
* Description	: SPI1 DMA channel config
* Notes		: Channel 2 for RX, channel 3 for TX
**************************************************/
void Configure_SPI1_DMA(void)
  {
  
  /*  Enable the clock of DMA1 and DMA1 */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  
    /* SPI1 DMA Init */
  
  /* SPI1_RX Init */
#if 0  
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_SPI1_RX);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);

  /* SPI1_TX Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_SPI1_TX);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_BYTE);

  
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_EnableNSSPulseMgt(SPI1);
#endif
  
  /* Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DMA1_Channel1_IRQn, _SPI1_DMA_RX_IRQ_PRIORITY);
  NVIC_EnableIRQ  (DMA1_Channel1_IRQn);
  NVIC_SetPriority(DMA1_Channel2_IRQn, _SPI1_DMA_RX_IRQ_PRIORITY);
  NVIC_EnableIRQ  (DMA1_Channel2_IRQn);
  
  LL_SPI_SetDMAParity_RX(SPI1, LL_SPI_DMA_PARITY_EVEN);
  
  /* Configure the DMA1_Channel1 functional parameters */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_1,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                        LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, LL_SPI_DMA_GetRegAddr(SPI1), (uint32_t)g_u8SpiOneRxBuff,
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
  
  
  /* Configure the DMA1_Channel2 functional parameters */
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_2,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                        LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2, (uint32_t)g_u8SpiOneTxBuff, LL_SPI_DMA_GetRegAddr(SPI1),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2));
  
  /* Enable DMA interrupts complete/error */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
  }

/**************************************************
* Function name	: StartSpiOneTx
* Returns	:	
* Arg		: 
* Created by	: Lev Zoosmanovskiy
* Date created	: 10/11/2018
* Description	: Initiates transfer 
* Notes		: This function initiates SPI TX and RX
* Notes		: The RX executes from the TX complete interrupt
* Notes		: If g_u32SpiOneRxLen > 0
**************************************************/
void StartSpiOneTx(void)
  {
    u32 count_SPI_Busy =0;
  // Set SPI1 busy flag  
//  g_bSemSpiOneBusy = true;
    g_bFlashPageReadFinished = false;
    g_bFlashPageWriteFinished = false;
    FLASH_CS_ENB();
    delay_us(5);
  
#if _SPI1_DMA_
  
  while (SPI1->SR & SPI_SR_BSY)
  {
    osDelay(pdMS_TO_TICKS(5));
    if(count_SPI_Busy ++ > 5)
    {
      RestartMCU(RESTART_REASON_SPI_BUSY);
    }
  }    
  
  SPI1_StartFlashTransfer();

#else //Interrupt mode
  g_u32SpiOneTxCount = 0;
  LL_SPI_TransmitData8(SPI1, *(g_u8SpiOneTxBuff+g_u32SpiOneTxCount++));
  LL_SPI_EnableIT_TXE(SPI1); 
#endif
  
  }

/**************************************************
* Function name	: SPI1_ReceiveComplete_Callback
* Returns	:	
* Arg		: 
* Created by	: Lev Zoosmanovskiy
* Date created	: 10/11/2018
* Description	: SPI1 Receive complete callback
* Notes		: 
**************************************************/
void SPI1_ReceiveComplete_Callback(void)
  {
  /* DMA Rx transfer completed */
  
  //Wait until SPI will finish the transmition
  while (SPI1->SR & SPI_SR_BSY);  
  
  //Disable the channel
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  
  //Relase ChipSelect
  delay_us(5);
  FLASH_CS_DSB();
  g_bFlashPageReadFinished = true;
  if (g_pSpi1DataDest)
    {
    memcpy(g_pSpi1DataDest, g_u8SpiOneRxBuff, g_u32SpiOneRxLen);
    g_pSpi1DataDest = NULL;
    }

  //Reset the RX request length
  g_u32SpiOneRxLen = 0;
  
  
//  if (nor_struct.eNorFlashState == eNorStartRead)
//    {
//    //NOR Flash state flag
//    nor_struct.eNorFlashState = eNorStopRead;
//    }

  //Release busy flag
  g_bSemSpiOneBusy = false;
  //Set state flag
  g_eSpiOneState = eSpiStopRead; 
  }



/**************************************************
* Function name	: SPI1_TransmitComplete_Callback
* Returns	:	
* Arg		: 
* Created by	: Lev Zoosmanovskiy
* Date created	: 10/11/2018
* Description	: SPI1 Transmit complete callback
* Notes		: 
**************************************************/
void SPI1_TransmitComplete_Callback(void)
  {
  //Wait until SPI will finish the transmition
  while (SPI1->SR & SPI_SR_BSY);    
  //Disable the DMA channel
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  
  //If SPI1 state was "write"
  if (g_eSpiOneState == eSpiWrite)
  {
  // Release the Chip select
    FLASH_CS_DSB();
    g_bFlashPageWriteFinished = true;
    
    //Update the NOR flash state
//    switch (nor_struct.eNorFlashState)
//      {
//      case eNorStartErase:   nor_struct.eNorFlashState = eNorStopErase; break;
//      case eNorStartWrite:   nor_struct.eNorFlashState = eNorStopWrite; break;
//      }
  }
  
  //If upcoming receive
  if (g_u32SpiOneRxLen)
    {
    //Initiate receive after transmit 

    //Wait until SPI will finish the transmition
    while (SPI1->SR & SPI_SR_BSY);    

    //Set output dummy buffer
    memset((u8*)g_u8SpiOneTxBuff, SPI_DUMMY_BYTE, sizeof(g_u8SpiOneTxBuff));

    //Empty RX fifo (which contains junk from previous TX
    while (LL_SPI_GetRxFIFOLevel(SPI1)) 
      {
      LL_SPI_ReceiveData8(SPI1);
      }
    
    //Set RX and TX length
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, g_u32SpiOneRxLen);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, g_u32SpiOneRxLen);
    //Enable channels
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
    //Set receive request
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_SPI1_RX);
    //LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_1);
    
    }
  else
    {    
    //Release the SPI busy flag if no read request
    g_bSemSpiOneBusy = false; 
    }
  }

/**************************************************
* Function name	: SPI1_TransferError_Callback
* Returns	:	
* Arg		: 
* Created by	: Lev Zoosmanovskiy
* Date created	: 10/11/2018
* Description	: SPI1 Transfer error callback
* Notes		: 
**************************************************/
void SPI1_TransferError_Callback(void)
  {
  /* Disable DMA1 Rx Channel */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
  
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  }

void SPI1_StartFlashTransfer(void)
  {
  
  //Disable the DMA channels
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, g_u32SpiOneTxLen);
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_SPI1_TX);//$$$
  
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2); // start DMA transfer
  }



/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
/**
* @brief  This function handles DMA1 interrupt request.
* @param  None
* @retval None
*/
void DMA1_Channel1_IRQHandler(void)
  {
  
  if (LL_DMA_IsActiveFlag_TC1(DMA1))
    {
    LL_DMA_ClearFlag_GI1(DMA1);
    /* Call function Reception complete Callback */
    SPI1_ReceiveComplete_Callback();
    }
  else if (LL_DMA_IsActiveFlag_TE1(DMA1))
    {
    /* Call Error function */
    SPI1_TransferError_Callback();
    }
  
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
/**
* @brief  This function handles DMA1 interrupt request.
* @param  None
* @retval None
*/
void DMA1_Channel2_IRQHandler(void)
  {
  if (LL_DMA_IsActiveFlag_TC2(DMA1))
    {
    LL_DMA_ClearFlag_GI2(DMA1);
    /* Call function Transmission complete Callback */
    SPI1_TransmitComplete_Callback();
    }
  else if (LL_DMA_IsActiveFlag_TE2(DMA1))
    {
    /* Call Error function */
    SPI1_TransferError_Callback();
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void WaitForSpiOneReady(void)
  {
  while (g_bSemSpiOneBusy)
    {
    if (g_bRtosRunning)
      {
      osDelay(pdMS_TO_TICKS(1));
      }
    }
  }