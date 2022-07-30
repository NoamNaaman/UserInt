#ifndef __SETUP__
#define __SETUP__

#define MAIN_VERSION "1.00"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define s8  int8_t
#define s16 int16_t
#define s32 int32_t
#define bool u8

#define true 1
#define false 0
#define TRUE 1
#define FALSE 0

#define SYSTEM_TYPE_DMI		0
#define SYSTEM_TYPE_FILTER	1
#define SYSTEM_TYPE			SYSTEM_TYPE_DMI
//#define SYSTEM_TYPE			SYSTEM_TYPE_FILTER

//========== STM32 housekeeping functions ===================================
// I/O pin functionality definitions for use with SetPinMode()

// port modes
#define PORTMODE_INPUT         0
#define PORTMODE_OUTPUT        1
#define PORTMODE_ALTERNATE     2
#define PORTMODE_ANALOG        3

// output modes
#define OUTMODE_PP    0x00
#define OUTMODE_OD    0x10

// output speed
#define OUTSPEED_SLOW        0x000
#define OUTSPEED_MODERATE          0x100
#define OUTSPEED_FAST         0x200
#define OUTSPEED_VERY_FAST         0x300

// pullup/pulldown
#define PULL_NONE              0x0000
#define PULL_UP                0x1000
#define PULL_DOWN              0x2000

typedef enum
{ GPIO_FMode_AIN         = ( PORTMODE_ANALOG  ),       // analog input or DAC output
  GPIO_FMode_IN_FLOATING = ( PORTMODE_INPUT  ),       // digital input, no pull-up/down resistor
  GPIO_FMode_IPD         = ( PORTMODE_INPUT | PULL_DOWN  ),       // digital input, pull-up resistor
  GPIO_FMode_IPU         = ( PORTMODE_INPUT | PULL_UP    ),       // digital input, pull-down resistor

  GPIO_FMode_p4_Out_PP   = ( PORTMODE_OUTPUT    | OUTSPEED_SLOW | OUTMODE_PP  ),       // 400KHz digital output, push-pull output
  GPIO_FMode_p4_Out_OD   = ( PORTMODE_OUTPUT    | OUTSPEED_SLOW | OUTMODE_OD  ),       // 400KHz digital output, open drain output
  GPIO_FMode_p4_AF_PP    = ( PORTMODE_ALTERNATE | OUTSPEED_SLOW | OUTMODE_PP  ),       // 400KHz digital output, push-pull alternate function output
  GPIO_FMode_p4_AF_OD    = ( PORTMODE_ALTERNATE | OUTSPEED_SLOW | OUTMODE_OD  ),       // 400KHz digital output, open drain alternate function output

  GPIO_FMode_2_Out_PP    = ( PORTMODE_OUTPUT    | OUTSPEED_MODERATE   | OUTMODE_PP  ),       // 2MHz digital output, push-pull output
  GPIO_FMode_2_Out_OD    = ( PORTMODE_OUTPUT    | OUTSPEED_MODERATE   | OUTMODE_OD  ),       // 2MHz digital output, open drain output
  GPIO_FMode_2_AF_PP     = ( PORTMODE_ALTERNATE | OUTSPEED_MODERATE   | OUTMODE_PP  ),       // 2MHz digital output, push-pull alternate function output
  GPIO_FMode_2_AF_OD     = ( PORTMODE_ALTERNATE | OUTSPEED_MODERATE   | OUTMODE_OD  ),       // 2MHz digital output, open drain alternate function output

  GPIO_FMode_10_Out_PP   = ( PORTMODE_OUTPUT    | OUTSPEED_FAST  | OUTMODE_PP  ),       // 10MHz digital output, push-pull output
  GPIO_FMode_10_Out_OD   = ( PORTMODE_OUTPUT    | OUTSPEED_FAST  | OUTMODE_OD  ),       // 10MHz digital output, open drain output
  GPIO_FMode_10_AF_PP    = ( PORTMODE_ALTERNATE | OUTSPEED_FAST  | OUTMODE_PP  ),       // 10MHz digital output, push-pull alternate function output
  GPIO_FMode_10_AF_OD    = ( PORTMODE_ALTERNATE | OUTSPEED_FAST  | OUTMODE_OD  ),       // 10MHz digital output, open drain alternate function output

  GPIO_FMode_40_Out_PP   = ( PORTMODE_OUTPUT    | OUTSPEED_VERY_FAST  | OUTMODE_PP  ),       // 50MHz digital output, push-pull output
  GPIO_FMode_40_Out_OD   = ( PORTMODE_OUTPUT    | OUTSPEED_VERY_FAST  | OUTMODE_OD  ),       // 50MHz digital output, open drain output
  GPIO_FMode_40_AF_PP    = ( PORTMODE_ALTERNATE | OUTSPEED_VERY_FAST  | OUTMODE_PP  ),       // 50MHz digital output, push-pull alternate function output
  GPIO_FMode_40_AF_OD    = ( PORTMODE_ALTERNATE | OUTSPEED_VERY_FAST  | OUTMODE_OD  )        // 50MHz digital output, open drain alternate function output
} GPIOModeFunc_TypeDef;


#define SetOutputHigh(PORTx, Pin) PORTx->BSRR = (uint32_t)1L << Pin
#define SetOutputLow(PORTx, Pin) PORTx->BSRR = (uint32_t)1L << (Pin+16)

#define SetOutputPin(PORTx, Pin, value) \
  if (value)                            \
    SetOutputHigh(PORTx, Pin);          \
  else                                  \
    SetOutputLow(PORTx, Pin)

#define InputPin(PORTx, Pin) ((PORTx->IDR & (1 << Pin)) != 0)

#define OutputToggle(PORTx, Pin) \
    if (InputPin(PORTx, Pin))    \
      SetOutputLow(PORTx, Pin);  \
    else                         \
      SetOutputHigh(PORTx, Pin);

//#define output_drive(x)     SetPinMode(x, GPIO_FMode_10_Out_PP)
//#define output_float(x)     SetPinMode(x, GPIO_FMode_IN_FLOATING);
//#define input_pullup(x)     SetPinMode(x, GPIO_FMode_IPU);
//
//#define output_low(x)   SetOutputLow(x)
//#define output_high(x)  SetOutputHigh(x)
//#define output_toggle(x) OutputToggle(x)
//#define input(x)        InputPin(x)

#define ClearTflag(w, b) w &= ~(1 << b)
#define SetTflag(w, b) w |= (1 << b)
#define GetTflag(w, b) ((w & (1 << b)) != 0)
      
      
#define EnableEXTI(Pin) EXTI->IMR |= 1 << Pin
#define DisableEXTI(Pin) EXTI->IMR &= ~(1 << Pin)


#define GetTmrFlag(x) GetTflag(x)
#define ClearTmrFlag(x) ClearTflag(x)
#define SetTmrFlag(x) SetTflag(x)

#define output_drive(x) SetPinMode(x, GPIO_FMode_10_Out_PP)
#define output_float(x) SetPinMode(x, GPIO_FMode_IN_FLOATING);
#define output_od(x) SetPinMode(x, GPIO_FMode_10_Out_OD)
#define input_pullup(x) SetPinMode(x, GPIO_FMode_IPU);
#define input_pulldown(x) SetPinMode(x, GPIO_FMode_IPD);

#define output_low(x)   SetOutputLow(x)
#define output_high(x)  SetOutputHigh(x)
#define output_pin(x, onoff) SetOutputPin(x, onoff)
#define output_toggle(x) OutputToggle(x)
#define input(x)        InputPin(x)
#define output_toggle(x) OutputToggle(x)
#define enable_ext_interrupt(x) EnableEXTI(x)
#define disable_ext_interrupt(x) DisableEXTI(x)
#define clear_tim_interrupt(htim) __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE)
      
      
#define make32(x3,x2,x1,x0) (((u32)x3 << 24) | ((u32)x2 << 16) | ((u32)x1 << 8) | (u32)x0)
#define make16(x1,x0) (((u32)x1 << 8) | (u32)x0)
#define make8(data, byte) ((data >> (byte * 8)) & 0xFF)
      
      
void SetPinMode(GPIO_TypeDef* GPIOx, u16 Pin, GPIOModeFunc_TypeDef PortMode);

void NVIC_Configuration(void);
void InitBoard();
void SetPin(u32 pin, u32 value);
u32  GetPin(u32 pin);
void TogglePin(u32 pin);
void init_outputs(void);
void delay_us(u32 delay);
void delay_ms(u32 delay);

extern u32  TMR_5mS_flags, TMR_50mS_flags;
extern u32 tick_count;
extern u32  TMR_1mS_flags,   TMR_1mS_count,  TMR_1mS_cnt;
extern u32  TMR_10mS_flags,  TMR_10mS_count, TMR_10mS_cnt;
extern u32  TMR_100mS_flags, TMR_100mS_count,TMR_100mS_cnt;
extern u32  TMR_50mS_flags,  TMR_50mS_count, TMR_50mS_cnt;
extern u32  TMR_1Sec_flags,  TMR_1Sec_count, TMR_1Sec_cnt;




#define FLASH_TABLE_ELEMENTS  4096
#define FLASH_TABLE_SIZE      (FLASH_TABLE_ELEMENTS * 512)


#define TMR_1mS_FLASH_ERASE TMR_1mS_flags,1
#define TMR_1mS_PC_TIMEOUT  TMR_1mS_flags,2

#define TMR_10mS_COMM_TIMEOUT TMR_10mS_flags,1
#define TMR_10MS_COMM_TIMER   TMR_10mS_flags,2 
#define TMR_10MS_WATCH_ACK    TMR_10mS_flags,3
#define TMR_10MS_CMD_TIMEOUT  TMR_10mS_flags,4

#define TMR_100MS_UPDT        TMR_100mS_flags,1  
#define TMR_100MS_LED         TMR_100mS_flags,2  
#define TMR_100MS_FAST_BLINK  TMR_100mS_flags,3  
#define TMR_100MS_CMD_TIMEOUT TMR_100mS_flags,4


#define TMR_1SEC_WATCH_ID     TMR_1Sec_flags,1
#define TMR_1SEC_SLOW_BLINK   TMR_1Sec_flags,2




#endif