#ifndef DISPLAY_PHISICAL
#define DISPLAY_PHISICAL

#include <stdint.h>
#include <stdbool.h>
#include "display_pics.h"

//---------------------------------------------------
extern void       Init_Display_Phisical_9341(void);
extern void       Init_Display_Phisical_7789(void);
// ---------------------------------------------------
extern void       Init_Lcd_Pins         ( void );
//---------------------------------------------------
extern void       Clear_Lcd            ( void                   );
extern void       Red_Lcd              ( void                   );
extern void       Green_Lcd            ( void                   );
extern void       Blue_Lcd             ( void                   );
extern void       Pic2Lcd              ( struct Struct_Pic *Pic );
extern void       Sub_Pic2Lcd          ( void                   );
extern void       Lcd2Pic_Inverted     ( struct Struct_Pic *Pic );
extern void       Set_Frame_Address    ( struct Struct_Pic *Pic );
extern uint16_t   Read_Disp_Data       ( void                   );
extern void       Write_Disp_16b_Param ( uint16_t Data          );
extern void       Write_Disp_8b_Param  ( uint8_t Data           );
extern void       Write_Disp_16b_Data  ( uint16_t Data          );
extern void       Write_Disp_Instr     ( uint8_t Instr          );
// ---------------------------------------------------;
extern void       Disp_CS_Set          ( void                   );
extern void       Disp_CS_Clr          ( void                   );
extern void       Disp_DI_Set          ( void                   );
extern void       Disp_DI_Clr          ( void                   );
extern void       Disp_Rst_Set         ( void                   );
extern void       Disp_Rst_Clr         ( void                   );
extern void       Disp_WR_Set          ( void                   );
extern void       Disp_WR_Clr          ( void                   );
extern void       Disp_RD_Set          ( void                   );
extern void       Disp_RD_Clr          ( void                   );
extern void       Disp_Imo_Set         ( void                   );
extern void       Disp_Imo_Clr         ( void                   );
#endif

