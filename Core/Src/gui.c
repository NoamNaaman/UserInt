#include "stm32f0xx_hal.h"

#include "setup.h"
#include "stdlib.h"
#include "SYS_DEFS.h"
#include "ssd1306.h"
//#include "firmwareLoader.h"
//#include "misc.h"



/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void clear_screen(void)
  {
  ssd1306_Fill(Black);
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void put_string(u32 x, u32 y, u32 font_size, char *string)
  {
//  return;
  switch (font_size)
    {
    case 18:
      ssd1306_SetCursor(x, y);
      ssd1306_WriteString(string, Font_11x18, White);
      break;  
      
    case 10:
      ssd1306_SetCursor(x, y);
      ssd1306_WriteString(string, Font_7x10, White);
      break;
  
    case 8:
      ssd1306_SetCursor(x, y);
      ssd1306_WriteString(string, Font_6x8, White);
      break;
    }
  ssd1306_UpdateScreen();
  }

/////////////////////////////////////////////////////////////////////
// Name:        
// Description: 
// Parameters:  
// Returns:     NONE
/////////////////////////////////////////////////////////////////////
void status_screen(void)
  {
  }
            
            
            
            
            
            