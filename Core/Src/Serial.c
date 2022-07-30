///////////////////////////////////////////////////////////////////////////////
// Source file serial.c
//
// Written by: Noam Naaman
// email:   noam@cardiacsense.com
///////////////////////////////////////////////////////////////////////////////

#include "stm32F0xx_hal.h"
#include "string.h"
#include "setup.h"
//#include "misc.h"
//#include "taskPriorities.h"
//#include "cmsis_os.h"
#include "SYS_DEFS.H"
//#include "protocol.h"
#include "stdio.h"
//#include "firmwareLoader.h"






/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void SendAck(void)
  {
  }

////////////////////////////////////////////////////////////////////////////////////////////////
// serial comm port input processing functions
////////////////////////////////////////////////////////////////////////////////////////////////
#define HOST_RX_LEN 64
u8  HostRxBuffer[HOST_RX_LEN];
u32 HostRxBufferIx, HostRxBufferC, HostRxBufferOx;

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void USART1_IRQHandler(void)
  {
  u8 byte;
  u32 stat = USART1->ISR;
  if ((stat & 0x380) != 0)
    {
    __HAL_UART_CLEAR_FEFLAG(&huart1);
    __HAL_UART_CLEAR_NEFLAG(&huart1); 
    __HAL_UART_CLEAR_OREFLAG(&huart1);
    }
  byte = USART1->RDR;
  __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
  HostRxBuffer[HostRxBufferIx] = byte;
  if (++HostRxBufferIx >= HOST_RX_LEN)
    {
    HostRxBufferIx = 0;
    }
  if (++HostRxBufferC > HOST_RX_LEN)
    {
    HostRxBufferC = HOST_RX_LEN;
    HostRxBufferOx = HostRxBufferIx;
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 HostChars(void)
  {
  return HostRxBufferC;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u8 HostPeek(void)
  {
  return HostRxBuffer[HostRxBufferOx];
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u8 HostGetChar(void)
  {
  u8 chr = HostRxBuffer[HostRxBufferOx];
  if (++HostRxBufferOx >= HOST_RX_LEN)
    {
    HostRxBufferOx = 0;
    }
  if (HostRxBufferC)
    {
    HostRxBufferC--;
    }
  return chr;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void  EnableUartInts(void)
  {
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  }
