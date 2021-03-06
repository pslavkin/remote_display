#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include <string.h>
#include "keyb_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "pass_pic.h"
#include "welcome_pic.h"
#include "debug.h"
#include "tpanel.h"
#include "schedule.h"
#include "ftm.h"
#include "clock_pic.h"
#include "pass_big_pic.h"
#include "accept_reject_pic.h"

//--------------Pics Data----------------------------------
uint16_t Keyb_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "keyb.raw"
#endif
};
uint16_t *Keyb_Data[] RODATA=
{
   Keyb_Data_Raw,
};

//---------------Functions---------------------------------------
void Keyb_Constr(void)
{
   Add_Pass   (    );
}
void Keyb_Destr(void)
{
   Del_Pass ( );
   Add_Off  ( );
}
void Test_Pass_Length2Add_Clock(void)
{
   if(strlen(Read_Pass_String())>0)
     Add_Clock();
}

void Check2Add_Big_Pass(void)
{
   if(Read_Drag_Counter()>10)
      Add_Pass_Big();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Keyb_Events[] RODATA=
{
{ {  0 ,0       ,0   ,0       } ,Invalid_Button ,0 ,{Keyb_Constr     ,Rien           ,Keyb_Destr}                 },// On_Create
{ {  4 ,4 +73   ,71  ,71 +53 }  ,Invalid_Button ,1 ,{Add_Pass_Digit1 ,Rien           ,Check2Add_Big_Pass}         },
{ { 83 ,83 +73  ,71  ,71 +53 }  ,Invalid_Button ,1 ,{Add_Pass_Digit2 ,Rien           ,Rien}                       },
{ {161 ,161 +73 ,71  ,71 +53 }  ,Invalid_Button ,1 ,{Add_Pass_Digit3 ,Rien           ,Rien}                       },
{ {  4 ,4 +73   ,131 ,131 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit4 ,Rien           ,Rien}                       },
{ { 83 ,83 +73  ,131 ,131 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit5 ,Rien           ,Rien}                       },
{ {161 ,161 +73 ,131 ,131 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit6 ,Rien           ,Rien}                       },
{ {  4 ,4 +73   ,189 ,189 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit7 ,Rien           ,Rien}                       },
{ { 83 ,83 +73  ,189 ,189 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit8 ,Rien           ,Rien}                       },
{ {161 ,161 +73 ,189 ,189 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit9 ,Rien           ,Rien}                       },
{ { 83 ,83 +73  ,248 ,319 }     ,Invalid_Button ,1 ,{Add_Pass_Digit0 ,Rien           ,Rien}                       },
{ {  4 ,4 +73   ,248 ,319 }     ,Invalid_Button ,1 ,{Del_Pass_Digit  ,Del_Pass_Digit ,Rien}                       },
{ {161 ,161 +73 ,248 ,319 }     ,Invalid_Button ,1 ,{Rien            ,Rien           ,Test_Pass_Length2Add_Clock} },
};
//--------------Pics Info----------------------------------
struct Struct_Pic Keyb_Pic RODATA=
{
   {0,239,70,319},  0,0,13,Keyb_Events,1,Keyb_Data
};
void Add_Keyb ( void ) { Add_Pic_On_Top(&Keyb_Pic);}
void Del_Keyb ( void ) { Del_Pic(&Keyb_Pic)          ;}
struct Struct_Pic* Read_Keyb_Pic ( void ) { return &Keyb_Pic;}
//------------------------------------------------------
