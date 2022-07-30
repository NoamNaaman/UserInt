
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "string.h"

#include "setup.h"

u32  TMR_5mS_flags, TMR_50mS_flags;
u32 tick_count;
u32  TMR_1mS_flags,   TMR_1mS_count,  TMR_1mS_cnt;
u32  TMR_10mS_flags,  TMR_10mS_count, TMR_10mS_cnt;
u32  TMR_100mS_flags, TMR_100mS_count,TMR_100mS_cnt;
u32  TMR_50mS_flags,  TMR_50mS_count, TMR_50mS_cnt;
u32  TMR_1Sec_flags,  TMR_1Sec_count, TMR_1Sec_cnt;

/* Private variables ---------------------------------------------------------*/

/**
  * @brief  Sets a single I/O pin to any operational mode.
  * @param  GPIOx:  Port A, B, C etc.
  * @Param  Pin:    Pin on port 0..15
  * @Param  Mode:   One of the modes in GPIOModeFunc_TypeDef
  * @retval None
  */
void SetPinMode(GPIO_TypeDef* GPIOx, u16 Pin, GPIOModeFunc_TypeDef PortMode)
  {
  u32 mask1, mask3;
  u32 Mode, Out, Speed, Pull;

  mask1 = ~((u32)1 << Pin);
  mask3 = ~((u32)3 << (Pin * 2));

  Mode = PortMode & 3;           Mode  <<= Pin * 2;
  Out = (PortMode >> 4) & 1;     Out   <<= Pin;
  Speed = (PortMode >> 8) & 3;   Speed <<= Pin * 2;
  Pull = (PortMode >> 12) & 3;   Pull  <<= Pin * 2;

  GPIOx->MODER   = (GPIOx->MODER   & mask3) | Mode;
  GPIOx->OTYPER  = (GPIOx->OTYPER  & mask1) | Out;
  GPIOx->OSPEEDR = (GPIOx->OSPEEDR & mask3) | Speed;
  GPIOx->PUPDR   = (GPIOx->PUPDR   & mask3) | Pull;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void SetPin(u32 pin, u32 value)
  {
  pin += 0x80; // POINT TO OUTPUT REGISTER
  *(u32 *)pin = value & 1;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 GetPin(u32 pin)
  {
  u32 in;
  in = *(u32 *)pin & 1;
  return in;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void TogglePin(u32 pin)
  {
  if (GetPin(pin))
    {
    SetPin(pin, 0);
    }
  else
    {
    SetPin(pin, 1);
    }
  }

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
void SysTickConfig(void)
  {
  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
    {
    /* Capture error */
    while (1);
    }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
  }

