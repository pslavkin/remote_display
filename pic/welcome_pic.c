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
#include "ftm.h"

//--------------Pics Data----------------------------------
uint16_t Welcome_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "pointer_logo_mini.raw"
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
//   {16,16+207,100,100+125},3,0,2,Welcome_Events,1,Welcome_Data  //big
   {16,16+204,142,142+ 34},3,0,2,Welcome_Events,1,Welcome_Data //mini
};
void Add_Welcome               ( void ) { Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome               ( void ) { Del_Pic(&Welcome_Pic)          ;}
//------------------------------------------------------
void Off_Constr(void)
{
   Layer_Clr_Lcd();
}
void Off_Destr(void)
{
   Add_Keyb();
   Layer_Clr_Lcd();
}
struct Struct_Pic_Events Off_Events[] RODATA=
{
   { { 0 ,0   ,0 ,0}   ,0          ,0 ,{Off_Constr ,Rien ,Off_Destr} } ,// On_Create
   { { 0 ,239 ,0 ,319} ,Any_Button ,1 ,{Rien       ,Rien ,Del_Off}   } ,
};
struct Struct_Pic Off_Pic RODATA=
{
///   {16,16+207,100,100+125},0,0,2,Off_Events,1,Welcome_Data //big
   {16,16+204,142,142+ 34},0,0,2,Off_Events,1,Welcome_Data //mini
};
void Add_Off               ( void ) { Add_Pic_On_Bottom(&Off_Pic);}
void Del_Off               ( void ) { Del_Pic(&Off_Pic)          ;}
//
