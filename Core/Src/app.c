
#include "stm32f0xx_hal.h"
#include "setup.h"
#include "misc.h"
#include "stdlib.h"
#include "SYS_DEFS.h"
#include "ssd1306.h"

u8 disp_buf[32];

char command[128];
u32  comm_ptr;
u32  MsgPtr;

u32 button1_cnt;
u32 button2_cnt;
u32 button3_cnt;
bool button_flag[3];

enum {
  HOST_MSG_START,
  HOST_MSG_CONT,
} HostMsgState;

void clear_screen(void);
void put_string(u32 x, u32 y, u32 font_size, char *string);
void init_i2c(void);
void HandleHostcommands(void);
void HandleButtons(void);
void ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);
void  EnableUartInts(void);


/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void gui_app(void)
  {
  char buf[22];
  init_i2c();
  
  ssd1306_Init();
  
  EnableUartInts();

  clear_screen();
  put_string(0, 0, 18, "Hello");
  
  delay_ms(2000);
  
  
  clear_screen();
  
  while (1)
    {
    HandleHostcommands();
    HandleButtons();
    for (u32 btn = 0; btn < 3; btn++)
      {
      if (button_flag[btn])
        {
        button_flag[btn] = 0;
        clear_screen();
//        sprintf(buf, "Button %d", btn+1);
//        put_string(0, 0, 10, buf);
        if (btn == 0)
          {
          put_string(0, 0, 18, "Button 1");
          }
        else if (btn == 1)
          {
          put_string(0, 0, 18, "Button 2");
          }
        else
          {
          put_string(0, 0, 18, "Button 3");
          }
        }
      }
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 get_char(void)
  {
  return command[comm_ptr++];
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 peek_char(void)
  {
  return command[comm_ptr];
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void skip_spc(void)
  {
  while (command[comm_ptr] && (command[comm_ptr] == ',' || command[comm_ptr] == 9 || command[comm_ptr] == ' '))
    {
    comm_ptr++;
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 get_string(char *str, char delim)
  {
  u32 len = 0;
  skip_spc();
  while (peek_char() && peek_char() != delim && peek_char() != '\r')
    {
    *str++ = get_char();
    len++;
    }
  *str = 0;
  return len;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 get_hex(void)
  {
  u32 num = 0;
  char chr;
  skip_spc();
  if (peek_char())
    {
    num = 0;
    while (isxdigit(peek_char()))
      {
      chr = get_char();
      chr = toupper(chr);
      if (chr <= '9')
        {
        chr -= '0';
        }
      else
        {
        chr = chr - ('A' - 10);
        }
      num = num * 16 + (u32)chr;
      }
    }
  return num;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
u32 get_bin(void)
  {
  u32 num = 0;
  char chr;
  skip_spc();
  if (peek_char())
    {
    num = 0;
    while (isdigit(peek_char()))
      {
      chr = get_char();
      if (chr > '1')
        {
        break;
        }
      chr -= '0';
      num = num * 2 + (u32)chr;
      }
    }
  return num;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
s32 get_int(void)
  {
  s32 num = 0, sign = 1;
  skip_spc();
  if (command[comm_ptr])
    {
    num = 0;
    if (toupper(peek_char()) == 'X') // hexadecimal number ?
      {
      get_char(); // skip 'x' char
      num = get_hex();
      return num;
      }
    else if (toupper(peek_char()) == 'B') // binary number ?
      {
      get_char(); // skip 'b' char
      num = get_bin();
      return num;
      }
    
    if (peek_char() == '-')
      {
      sign = -1;
      get_char(); // skip '-' char
      }
    while (isdigit(command[comm_ptr]))
      num = (num * 10) + (command[comm_ptr++] - '0');
    }
//  skip_spc();
  return num * sign;
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void PutText(void)
  {
  u32 x,y,font, str_len;
  x = get_int();
  y = get_int();
  font = get_int();
  skip_spc();
  if (peek_char() == '"')
    {
    comm_ptr++;
    for (u32 qtp = comm_ptr; command[qtp] != 0; qtp++)
      {
      if (command[qtp] == '"')
        {
        command[qtp] = 0;
        break;
        }
      }
    put_string(x, y, font, &command[comm_ptr]);
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void DrawRect(void)  
  {
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void DrawArc(void)   
  {
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void DrawCircle(void)
  {
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void ProcessHostcommand(void)
  {
  char chr;
  MsgPtr = 0;
  chr = get_char();
  switch (chr)
    {
    case 'E': clear_screen(); break;
    case 'T': PutText();      break;
    case 'R': DrawRect();     break;
    case 'A': DrawArc();      break;
    case 'C': DrawCircle();   break;
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void HandleHostcommands(void)
  {
  char chr;
  while (HostChars())
    {
    chr = HostGetChar();
    switch (HostMsgState)
      {
      case HOST_MSG_START: 
        if (chr == '$')
          {
          HostMsgState = HOST_MSG_CONT;
          }
        break;
      case HOST_MSG_CONT: 
        command[comm_ptr++] = chr;
        if (chr == '\r')
          {
          command[comm_ptr] = 0;
          ProcessHostcommand();
          comm_ptr = 0;
          HostMsgState = HOST_MSG_START;
          }
        break;
      }
    }
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void HandleButtons(void)
  {
  if ((TMR_1mS_flags & 1) != 0)
    {
    TMR_1mS_flags &= ~1;
    if (input(BUTTON1) == 0)
      {
      if (++button1_cnt == 100)
        {
        USART1->TDR = '1';
        button_flag[0] = 1;
        }
      }
    else
      {
      button1_cnt = 0;
      }
    if (input(BUTTON2) == 0)
      {
      if (++button2_cnt == 100)
        {
        USART1->TDR = '2';
        button_flag[1] = 1;
        }
      }
    else
      {
      button2_cnt = 0;
      }
    if (input(BUTTON3) == 0)
      {
      if (++button3_cnt == 100)
        {
        USART1->TDR = '3';
        button_flag[2] = 1;
        }
      }
    else
      {
      button3_cnt = 0;
      }
    }
  }


