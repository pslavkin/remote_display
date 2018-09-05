#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "welcome_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "pass_pic.h"
#include "keyb_pic.h"
#include "debug.h"
#include "schedule.h"
#include "tpanel.h"

//--------------Pics Data----------------------------------
uint16_t Welcome_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
//#ifdef PICS_ENABLED_TEST
   #include "pointer_logo.raw"
#endif
};
uint16_t *Welcome_Data[] RODATA=
{
   Welcome_Data_Raw,
};
//---------------Functions---------------------------------------
void Welcome_Constr(void)
{
   Layer_Clr_Lcd();
}
void Welcome_Destr(void)
{
   Add_Keyb();
   Layer_Clr_Lcd();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Welcome_Events[] RODATA=
{
   { { 0 ,0   ,0 ,0}   ,0          ,0 ,{Welcome_Constr ,Del_Welcome ,Welcome_Destr} } ,// On_Create
   { { 0 ,239 ,0 ,319} ,Any_Button ,1 ,{Rien           ,Rien        ,Del_Welcome}   } ,
};
//--------------Pics Info----------------------------------
struct Struct_Pic Welcome_Pic RODATA=
{
   {16,16+207,100,100+125},4,0,2,Welcome_Events,1,Welcome_Data
};
void Add_Welcome               ( void ) { Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome               ( void ) { Del_Pic(&Welcome_Pic)          ;}
//------------------------------------------------------
