#include "stm32f0xx_hal.h"

#include "setup.h"
#include "stdlib.h"
#include "SYS_DEFS.h"


#define NOP10 { __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); }
#define NOP30 { NOP10; NOP10; NOP10; }


/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void delay_us(u32 delay)
  {
  while (delay--)
    {
    NOP30;
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void delay_ms(u32 delay)
  {
  while (delay)
    {
    delay_us(1000);
    delay--;
    }
  }


/////////////////////////////////////////////////////////////////////
// Name:        SYSTICK_timers
// Description: called by SysTick IRQ for setting timed flags and 
//              incrementing time counters
// Parameters:  none
// Returns:     NONE
/////////////////////////////////////////////////////////////////////

void SYSTICK_timers(void)
  {
  TMR_1mS_flags = 0xFFFFFFFF;
  TMR_1mS_count++;
  if (!TMR_1mS_cnt || TMR_1mS_cnt == 5)
    {
    TMR_5mS_flags = 0xFFFFFFFF;
    }
  if (++TMR_1mS_cnt >= 10)
    {
    TMR_1mS_cnt = 0;
    TMR_10mS_count++;
    TMR_10mS_flags = 0xFFFFFFFF;
    if (++TMR_10mS_cnt >= 10)
      {
      TMR_10mS_cnt = 0;
      TMR_100mS_flags = 0xFFFFFFFF;
      if (++TMR_100mS_cnt >= 10)
        {
        TMR_100mS_cnt = 0;
        TMR_1Sec_flags = 0xFFFFFFFF;
        TMR_1Sec_count++;
        }
      }
    }
  }


