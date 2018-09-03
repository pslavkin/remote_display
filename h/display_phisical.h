#ifndef DISPLAY_PHISICAL
#define DISPLAY_PHISICAL

#include <stdint.h>
#include <stdbool.h>

struct Struct_Pic_Pos
{
   uint16_t XL ;
   uint16_t XR;
   uint16_t YU;
   uint16_t YL;
};
struct Struct_Pic_Events
{
 struct     Struct_Pic_Pos  Pos;
 uint8_t    Button               ;
 bool       Draw_Box             ; // si es 1 se dibuja un box alrededor de la zona delimitada por el touch
 void ( * Handler[3] )(void)     ;
};

struct Struct_Pic
{
   struct Struct_Pic_Pos Pos       ;
   uint8_t Blink_Period            ;
   unsigned char Alpha             ;
   unsigned char ECount            ; // numero de eventos
   struct Struct_Pic_Events* Events; //
   unsigned char PCount            ; // Pic count, numero de pics.... usualmente 1, pero mas de uno para strings.
   uint16_t**    Data              ; //
};

//---------------------------------------------------
extern void       Init_Display_Phisical_9341(void);
extern void       Init_Display_Phisical_7789(void);
// ---------------------------------------------------
extern void       Init_Lcd_Pins         ( void );
//---------------------------------------------------
extern uint16_t   Pic_Width            ( struct Struct_Pic *Pic );
extern uint16_t   Pic_Hight            ( struct Struct_Pic *Pic );
extern uint32_t   Pic_Area             ( struct Struct_Pic *Pic );
extern void       Init_Dump_Lcd        ( void                   );
extern void       End_Dump_Lcd         ( void                   );
extern void       Dump_Lcd             ( void                   );
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

