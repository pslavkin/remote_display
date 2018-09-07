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
   Set_Bligth ( 80 );
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
   Update_Off();
}
uint8_t Off_Tout=0;
void Count2Off(void)
{
   if(Off_Tout<10)
      Off_Tout++;
   else
      Dec10_Bligth();
}
void Free_Count2OFf(void)
{
   Update_Off();
   if(Off_Tout>=10)
      Del_Keyb();
   Off_Tout=0;
}
void Update_Off(void)
{
   Update_Blink_Pic(Read_Keyb_Pic());
}
//--------------Events----------------------------------
struct Struct_Pic_Events Keyb_Events[] RODATA=
{
{ {  0 ,0       ,0   ,0       } ,Invalid_Button ,0 ,{Keyb_Constr     ,Del_Keyb       ,Keyb_Destr}                 }         ,// On_Create
{ {  4 ,4 +73   ,71  ,71 +53 }  ,Invalid_Button ,1 ,{Add_Pass_Digit1 ,Count2Off      ,Free_Count2OFf}             }         ,
{ { 83 ,83 +73  ,71  ,71 +53 }  ,Invalid_Button ,1 ,{Add_Pass_Digit2 ,Rien           ,Update_Off}                         } ,
{ {161 ,161 +73 ,71  ,71 +53 }  ,Invalid_Button ,1 ,{Add_Pass_Digit3 ,Rien           ,Update_Off}                         } ,
{ {  4 ,4 +73   ,131 ,131 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit4 ,Rien           ,Update_Off}                 }         ,
{ { 83 ,83 +73  ,131 ,131 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit5 ,Rien           ,Update_Off}                 }         ,
{ {161 ,161 +73 ,131 ,131 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit6 ,Rien           ,Update_Off}                 }         ,
{ {  4 ,4 +73   ,189 ,189 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit7 ,Rien           ,Update_Off}                 }         ,
{ { 83 ,83 +73  ,189 ,189 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit8 ,Rien           ,Update_Off}                 }         ,
{ {161 ,161 +73 ,189 ,189 +53 } ,Invalid_Button ,1 ,{Add_Pass_Digit9 ,Rien           ,Update_Off}                 }         ,
{ { 83 ,83 +73  ,248 ,319 }     ,Invalid_Button ,1 ,{Add_Pass_Digit0 ,Rien           ,Update_Off}                 }         ,
{ {  4 ,4 +73   ,248 ,319 }     ,Invalid_Button ,1 ,{Del_Pass_Digit  ,Del_Pass_Digit ,Update_Off}                 }         ,
{ {161 ,161 +73 ,248 ,319 }     ,Invalid_Button ,1 ,{Rien            ,Rien           ,Test_Pass_Length2Add_Clock} }         ,
                                                                                                                  };
//--------------Pics Info----------------------------------
struct Struct_Pic Keyb_Pic RODATA=
{
   {0,239,70,319},120,0,13,Keyb_Events,1,Keyb_Data
};
void Add_Keyb ( void ) { Add_Pic_On_Top(&Keyb_Pic);}
void Del_Keyb ( void ) { Del_Pic(&Keyb_Pic)          ;}
struct Struct_Pic* Read_Keyb_Pic ( void ) { return &Keyb_Pic;}
//------------------------------------------------------
