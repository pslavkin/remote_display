#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "schedule.h"
#include "mask_pic.h"
#include "numbers_pic.h"
#include "accept_reject_pic.h"

//------------------------------------------------------
uint16_t Mask_Raw[100*100] =
{
   0, 
};
uint16_t *Mask[] RODATA=
{
   Mask_Raw,
};
//---------------Functions---------------------------------------
struct Struct_Pic Mask_Pic=
{
 0,0,0,0,0,0,1,Rien_Events,1,Mask
};

void Update_Mask_Pic(void)
{
   Lcd2Pic_Inverted(&Mask_Pic);
}
struct Struct_Pic* Read_Mask_Pic(void)
{
   return &Mask_Pic;
}
void Resize_Mask_Pic(uint16_t Start_X,uint16_t Width,uint16_t Start_Y,uint16_t Height)
{
   Mask_Pic.Start_X=Start_X;
   Mask_Pic.Width  =Width;
   Mask_Pic.Start_Y=Start_Y;
   Mask_Pic.Height =Height;
   Layer_Structure_Modified();
   Update_Or_New_None_Periodic_Func_Schedule(10,Del_Mask);
}
void Add_Mask_Delete ( void ) { Resize_Mask_Pic(  4    ,73 ,261 ,53);Del_Digit();}
void Add_Mask0       ( void ) { Resize_Mask_Pic( 83    ,73 ,261 ,53)                           ;Add_Digit0();}
void Add_Mask_Enter  ( void ) { /* Resize_Mask_Pic(161 ,73 ,261 ,53);*/Add_Accept_Or_Reject();}
void Add_Mask1       ( void ) { Resize_Mask_Pic(  4    ,73 ,83  ,53)                           ;Add_Digit1();}
void Add_Mask2       ( void ) { Resize_Mask_Pic( 83    ,73 ,83  ,53)                           ;Add_Digit2();}
void Add_Mask3       ( void ) { Resize_Mask_Pic(161    ,73 ,83  ,53)                           ;Add_Digit3();}
void Add_Mask4       ( void ) { Resize_Mask_Pic(  4    ,73 ,143 ,53)                           ;Add_Digit4();}
void Add_Mask5       ( void ) { Resize_Mask_Pic( 83    ,73 ,143 ,53)                           ;Add_Digit5();}
void Add_Mask6       ( void ) { Resize_Mask_Pic(161    ,73 ,143 ,53)                           ;Add_Digit6();}
void Add_Mask7       ( void ) { Resize_Mask_Pic(  4    ,73 ,202 ,53)                           ;Add_Digit7();}
void Add_Mask8       ( void ) { Resize_Mask_Pic( 83    ,73 ,202 ,53)                           ;Add_Digit8();}
void Add_Mask9       ( void ) { Resize_Mask_Pic(161    ,73 ,202 ,53)                           ;Add_Digit9();}
void Del_Mask        ( void ) { Resize_Mask_Pic(  0    ,0  ,0   ,0)                           ;}

