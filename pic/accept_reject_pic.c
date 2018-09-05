#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "accept_reject_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "pass_pic.h"
#include "debug.h"
#include "tpanel.h"
#include "bkgd_pic.h"
#include "log_pic.h"

//--------------Pics Data----------------------------------
uint16_t Accept_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "accepted.raw"
#endif
};
uint16_t *Accept_Data[] RODATA=
{
   Accept_Data_Raw,
};

//---------------Functions---------------------------------------
void Accept_Constr(void)
{
}
void Accept_Destr(void)
{
  Init_Pass();
  Del_Bkgd_Black();
  Layer_Clr_Lcd();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Accept_Events[] RODATA=
{
{ { 0  ,0   ,0  ,0 }   ,0          ,0 ,{Accept_Constr ,Del_Accept ,Accept_Destr} }       ,// On_Create
{ { 26 ,212 ,66 ,252 } ,Any_Button ,1 ,{Rien          ,Rien       ,Del_Accept}         } ,
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Accept_Pic RODATA=
{
   { 26 ,212 ,66 ,252 },5 ,0 ,2 ,Accept_Events ,1 ,Accept_Data
};
void Add_Accept ( void ) { Add_Pic_On_Top(&Accept_Pic);}
void Del_Accept ( void ) { Del_Pic(&Accept_Pic)       ;}
//------------------------------------------------------
//--------------Pics Data----------------------------------
uint16_t Reject_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "rejected.raw"
#endif
};
uint16_t *Reject_Data[] RODATA=
{
   Reject_Data_Raw,
};

//---------------Functions---------------------------------------
void Reject_Constr(void)
{
}
void Reject_Destr(void)
{
   Del_Bkgd_Black();
   Layer_Clr_Lcd();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Reject_Events[] RODATA=
{
   { { 0  ,0   ,0  ,0 }   ,0          ,0 ,{Reject_Constr ,Del_Reject ,Reject_Destr} } ,// On_Create
   { { 26 ,212 ,66 ,252 } ,Any_Button ,1 ,{Rien          ,Rien       ,Del_Reject }}   ,
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Reject_Pic RODATA=
{
   { 26 ,212 ,66 ,252 },5,0,2,Reject_Events,1,Reject_Data
};
void Add_Reject            (void)   {Add_Pic_On_Top(&Reject_Pic);}
void Del_Reject            (void)   {Del_Pic(&Reject_Pic);}
//------------------------------------------------------
void Add_Accept_Or_Reject(void)
{
   Add_Bkgd_Black();
   if(Psw_Compare())  {
      Add_Accept();
      Copy_Pass2Log(Read_Pass_String(),true);
   }
   else {
      Add_Reject();
      Copy_Pass2Log(Read_Pass_String(),false);
   }
}

