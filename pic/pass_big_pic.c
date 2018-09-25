#include <string.h>
#include "display_pics.h"
#include "type_conversion.h"
#include "events.h"
#include "display_layers.h"
#include "pass_pic.h"
#include "pass_big_pic.h"
#include "str.h"
#include "bkgd_pic.h"
#include "font_80x88.h"
#include "tpanel.h"
#include "debug.h"
//------------------------------------------------------
uint16_t *Pass_Big_Data[10]; // 8 digitos + arrow + null
                             // 1 2 3
                             // 4 5 6
                             // 7 8 <

void Pass_Big_Destr(void)
{
}
void Pass_Big_Constr(void)
{
   Pass_Big_String2Pic();
}
void Chec2Del_Pass_Big(void)
{
   if(Read_Drag_Counter()>0) //un toque de demora en drag para evitar retornos accidentales
      Del_Pass_Big();
}
struct Struct_Pic_Events Pass_Big_Events[] =
{
 {{0   ,0   ,0   ,0}   ,Invalid_Button ,0 ,{Pass_Big_Constr ,Rien ,Pass_Big_Destr}}    ,// On_Create
 {{160 ,239 ,221 ,319} ,Invalid_Button ,1 ,{Rien            ,Rien ,Chec2Del_Pass_Big}} ,
};

struct Struct_Pic Pass_Big_Pic[]  =
{
{ {0    ,0+79 ,10  ,10+87}  ,0 ,0 ,2 ,Pass_Big_Events ,3 ,&Pass_Big_Data[0] },
{ {0    ,0+79 ,115 ,115+87} ,0 ,0 ,1 ,Rien_Events     ,3 ,&Pass_Big_Data[3] },
{ {0    ,0+79 ,221 ,221+87} ,0 ,0 ,1 ,Rien_Events     ,3 ,&Pass_Big_Data[6] },
{ { 0   ,239  ,0   ,0+1}    ,0 ,1 ,1 ,Rien_Events     ,1 ,Line_Blue_Data    },
{ { 0   ,239  ,107 ,107+1}  ,0 ,1 ,1 ,Rien_Events     ,1 ,Line_Blue_Data    },
{ { 0   ,239  ,212 ,212+1}  ,0 ,1 ,1 ,Rien_Events     ,1 ,Line_Blue_Data    },
{ { 0   ,239  ,318 ,319}    ,0 ,1 ,1 ,Rien_Events     ,1 ,Line_Blue_Data    },
{ { 0   ,1    ,0   ,319}    ,0 ,1 ,1 ,Rien_Events     ,1 ,Line_Blue_Data    },
{ { 238 ,239  ,0   ,319}    ,0 ,1 ,1 ,Rien_Events     ,1 ,Line_Blue_Data    },
};

void Pass_Big_String2Pic(void)
{
   char Buff[10], Aux;
   String_Padd ( Read_Pass_String() ,Buff      ,10,' ');

   Aux     = Buff[3];
   Buff[3] = '\0';
   String2Pic  ( Buff+0 ,&Pass_Big_Pic[0] ,&Font_80x88_Pic );

   Buff[3] = Aux;
   Aux     = Buff[6];
   Buff[6] = '\0';
   String2Pic  ( Buff+3 ,&Pass_Big_Pic[1] ,&Font_80x88_Pic );

   Buff[6]=Aux;
   Buff[8]='+';   //el + es el arrow
   String2Pic  ( Buff+6 ,&Pass_Big_Pic[2] ,&Font_80x88_Pic );
}

void Add_Pass_Big ( void ) {
   uint8_t i;
   Add_Bkgd_Black();
   for(i=0;i<sizeof(Pass_Big_Pic)/sizeof(Pass_Big_Pic[0]);i++)
      Add_Pic_On_Top ( &Pass_Big_Pic[i] );
}
void Del_Pass_Big ( void ) {
   uint8_t i;
   for(i=0;i<sizeof(Pass_Big_Pic)/sizeof(Pass_Big_Pic[0]);i++)
      Del_Pic ( &Pass_Big_Pic[i] );
  Del_Bkgd_Black ( );
  Layer_Clr_Lcd  ( );
}

