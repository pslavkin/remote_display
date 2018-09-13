#ifndef DISPLAY_PICS
#define DISPLAY_PICS

#include <stdint.h>
#include <stdbool.h>
#include "display_pics.h"
//---------------------------------------------------------
struct Struct_Pic_Pos
{
   uint16_t XL;
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
   uint16_t Blink_Period           ;
   unsigned char Mod               ; // waf?? es un modulo de avance de los datos de la pic en el DMA.. se es cero, recorre toda la pic. si es 1, solo 1 byte, 2, 4 bytes, y asi.. puedo hacer un bkgd de color solido o tramado con 1 a 10 bytes de rom...! magico!!
   unsigned char ECount            ; // numero de eventos
   struct Struct_Pic_Events* Events; //
   unsigned char PCount            ; // Pic count, numero de pics.... usualmente 1, pero mas de uno para strings.
   uint16_t**    Data              ; //
};

//---------------------------------------------------------
extern struct Struct_Pic_Events Rien_Events[]                                                        ;
extern bool       Pic_Pos_Compare   ( struct Struct_Pic_Pos *A, struct Struct_Pic_Pos *B            );
extern uint16_t   Pic_Width         ( struct Struct_Pic *Pic                                        );
extern uint16_t   Pic_Hight         ( struct Struct_Pic *Pic                                        );
extern uint32_t   Pic_Area          ( struct Struct_Pic *Pic                                        );
extern void       Init_Display_Pics ( void                                                          );
extern uint8_t    String2Pic_Data   ( char *String ,uint16_t** Data ,struct Struct_Pic *Font        );
extern void       String2Pic        ( char *String ,struct Struct_Pic *Pic ,struct Struct_Pic *Font );
//---------------------------------------------------------
#endif
